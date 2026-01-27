//
// Created by vogje01 on 10/03/2025.
//

#include <utils/AwsUtils.h>

void AwsUtils::GetAwsSessionToken(const QString &awsAccount, const QString &nameSpace) {

    const auto region = Configuration::instance().GetValue<QString>("aws.region");
    const int timeout = Configuration::instance().GetValue<int>("aws.validation-period");

    // Get the OneLogin SAML assertion XML
    _oneloginUtils->GetAccessToken(awsAccount);

    connect(_oneloginUtils, &OneLoginUtils::GetAccessTokenSignal, this, [this, region, nameSpace, timeout](const QString &account, const QString &accessToken) {

        // Get the AWS role ARN
        if (const QVector<QString> roleArn = GetRoleArn(accessToken); roleArn.isEmpty()) {
            log_error("Get role ARN failed, account: " + account);
        } else {
            log_debug("Got role ARN, account: " + account + ", roleArn: " + roleArn[0] + ", principleArn: " + roleArn[1]);

            // Configure STS client
            Aws::Client::ClientConfiguration clientConfig;
            clientConfig.region = region.toStdString();
            const Aws::STS::STSClient stsClient(clientConfig);

            // Create STS request
            Aws::STS::Model::AssumeRoleWithSAMLRequest stsRequest;
            stsRequest.SetRoleArn(roleArn.at(0).toStdString());
            stsRequest.SetPrincipalArn(roleArn.at(1).toStdString());
            stsRequest.SetSAMLAssertion(accessToken.toStdString());
            stsRequest.SetDurationSeconds(timeout);

            if (const Aws::STS::Model::AssumeRoleWithSAMLOutcome outcome = stsClient.AssumeRoleWithSAML(stsRequest); !outcome.IsSuccess()) {
                log_error("Login to AWS failed, account: " + account);
            } else {
                log_info("Login to AWS succeeded, account: " + account);
                WriteAwsCredentialsFile(account, outcome);
                WriteKubernetesConfig(account, outcome);
                _credentials[account] = outcome.GetResult().GetCredentials();
                emit GetAwsCredentialsSignal(account, nameSpace, outcome.GetResult().GetCredentials());
            }
        }
    });
}

QVector<QString> AwsUtils::GetRoleArn(const QString &encodedSamlAssertion) {

    // Decode the provided SAML assertion
    const QString samlResponse = QByteArray::fromBase64(encodedSamlAssertion.toUtf8());

    QXmlStreamReader reader(samlResponse);

    while (!reader.atEnd()) {
        reader.readNext();

        // Only look at start elements
        if (reader.isStartElement()) {
            // Check for "order" tag
            if (reader.name() == "Attribute") {
                if (QXmlStreamAttributes attrs = reader.attributes(); attrs.hasAttribute("Name") && attrs.value("Name") == "https://aws.amazon.com/SAML/Attributes/Role") {

                    while (!reader.atEnd()) {
                        reader.readNext();
                        if (reader.isStartElement() && reader.name() == "AttributeValue") {
                            return reader.readElementText().split(",");
                        }
                    }
                }
            }
        }
    }
    return {};
}

void AwsUtils::WriteAwsCredentialsFile(const QString &awsAccount, const Aws::STS::Model::AssumeRoleWithSAMLOutcome &outcome) {

    const auto awsCredentialsFilename = Configuration::instance().GetValue<QString>("aws.credentials");

    QSettings settings(awsCredentialsFilename, QSettings::IniFormat);

    if (awsAccount == "pim-int") {

        settings.setValue("integration/aws_access_key_id", QVariant(QString::fromStdString(outcome.GetResult().GetCredentials().GetAccessKeyId())));
        settings.setValue("integration/aws_secret_access_key", QVariant(QString::fromStdString(outcome.GetResult().GetCredentials().GetSecretAccessKey())));
        settings.setValue("integration/aws_session_token", QVariant(QString::fromStdString(outcome.GetResult().GetCredentials().GetSessionToken())));

    } else if (awsAccount == "pim-prod") {

        settings.setValue("production/aws_access_key_id", QVariant(QString::fromStdString(outcome.GetResult().GetCredentials().GetAccessKeyId())));
        settings.setValue("production/aws_secret_access_key", QVariant(QString::fromStdString(outcome.GetResult().GetCredentials().GetSecretAccessKey())));
        settings.setValue("production/aws_session_token", QVariant(QString::fromStdString(outcome.GetResult().GetCredentials().GetSessionToken())));
    }
    settings.sync();
    log_debug("AWS credentials written, account: " + awsAccount);
}

QString AwsUtils::GetClusterArn(const QString &awsAccount) {

    const auto region = Configuration::instance().GetValue<QString>("aws.region");
    const auto accountIdInt = Configuration::instance().GetValue<QString>("aws.accountId.int");
    const auto accountIdProd = Configuration::instance().GetValue<QString>("aws.accountId.prod");
    const auto accountId = awsAccount == "pim-int" ? accountIdInt : accountIdProd;

    return "arn:aws:eks:" + region + ":" + accountId + ":cluster/" + awsAccount + "-eks";
}

void AwsUtils::WriteKubernetesConfig(const QString &awsAccount, const Aws::STS::Model::AssumeRoleWithSAMLOutcome &outcome) const {

    const QString clusterArn = Configuration::instance().GetClusterArn(awsAccount);
    const QString clusterName = Configuration::instance().GetClusterName(awsAccount);

    // Credentials
    Aws::Auth::AWSCredentials credentials;
    credentials.SetAWSAccessKeyId(outcome.GetResult().GetCredentials().GetAccessKeyId());
    credentials.SetAWSSecretKey(outcome.GetResult().GetCredentials().GetSecretAccessKey());
    credentials.SetSessionToken(outcome.GetResult().GetCredentials().GetSessionToken());

    // Configure STS client
    Aws::Client::ClientConfiguration clientConfig;
    clientConfig.region = _region.toStdString();

    const Aws::EKS::EKSClient eksClient(credentials, clientConfig);

    Aws::EKS::Model::DescribeClusterRequest request;
    request.SetName(clusterName.toStdString());

    if (auto describeClusterOutcome = eksClient.DescribeCluster(request); describeClusterOutcome.IsSuccess()) {
        const auto cluster = describeClusterOutcome.GetResult().GetCluster();
        const QString endpoint = cluster.GetEndpoint().c_str();
        const QString caData = cluster.GetCertificateAuthority().GetData().c_str();
        KubernetesUtils::UpdateKubernetesConfig(clusterArn, endpoint, caData);
    }
}
