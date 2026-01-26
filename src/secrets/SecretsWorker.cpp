//
// Created by vogje01 on 1/21/26.
//

#include <secrets/SecretsWorker.h>

#include "kubernetes/KubernetesWorker.h"

SecretsWorker::SecretsWorker(QString nameSpace) : _nameSpace(std::move(nameSpace)), _awsUtils(nullptr), _kubernetesUtils(nullptr), _restManager(nullptr) {

    if (_nameSpace == "pim-prod") {
        _awsAccount = "pim-prod";
    } else {
        _awsAccount = "pim-int";
    }
    _region = Configuration::instance().GetValue<QString>("aws.region");
}

void SecretsWorker::Shutdown() const {
    delete _restManager;
    delete _awsUtils;
    delete _kubernetesUtils;
}

void SecretsWorker::DoWork() {

    if (!_restManager)
        _restManager = new RestManager(this);
    if (!_awsUtils)
        _awsUtils = new AwsUtils(_restManager, this);
    if (!_kubernetesUtils)
        _kubernetesUtils = new KubernetesWorker();

    // Get AWS session token
    _awsUtils->GetAwsSessionToken(_awsAccount);

    connect(_awsUtils, &AwsUtils::GetAwsCredentialsSignal, this, &SecretsWorker::GetPasswords);
    connect(_awsUtils, &AwsUtils::GetAwsCredentialsSignal, _kubernetesUtils, &KubernetesWorker::DoWork);
}

void SecretsWorker::GetPasswords(const QString &awsAccount, const Aws::STS::Model::Credentials &credentials) const {

    const auto ssmCredentials = Aws::Auth::AWSCredentials(credentials.GetAccessKeyId(), credentials.GetSecretAccessKey(), credentials.GetSessionToken());

    Aws::Client::ClientConfiguration clientConfig;
    clientConfig.region = _region.toStdString();
    clientConfig.profileName = awsAccount == "pim-prod" ? "production" : "integration";
    log_info("Secretsmanager configuration initialized, region: " + QString(clientConfig.region.c_str()) + ", profile: " + QString(clientConfig.profileName.c_str()));

    const Aws::SecretsManager::SecretsManagerClient ssmClient(ssmCredentials, clientConfig);
    log_info("Secretsmanager initialized, region: " + _region);

    for (QVector forwarders = Configuration::instance().GetForwarderConfigs(_nameSpace); const auto &forwarder: forwarders) {

        // We do not have passwords for opensearch
        if (forwarder.type == "opensearch") {
            continue;
        }

        // Get username/password
        QString secret = GetSecretFromAws(ssmClient, forwarder.secret);
        Configuration::instance().SetKubernetesPassword(_nameSpace, forwarder.name, GetPassword(secret));
        Configuration::instance().SetKubernetesUsername(_nameSpace, forwarder.name, GetUsername(secret));
        log_info(_nameSpace + ":" + forwarder.name + ", password: " + GetPassword(secret));
    }
    Configuration::instance().WriteConfigurationFile();
    WriteWorkbenchConfig();
}

QString SecretsWorker::GetSecretFromAws(const Aws::SecretsManager::SecretsManagerClient &smClient, const QString &secretId) {

    // Create request
    Aws::SecretsManager::Model::GetSecretValueRequest smRequest;
    smRequest.SetSecretId(secretId.toStdString());

    // Get the secret value
    if (Aws::SecretsManager::Model::GetSecretValueOutcome outcome = smClient.GetSecretValue(smRequest); outcome.IsSuccess()) {
        return {outcome.GetResult().GetSecretString().c_str()};
    }
    log_error("Could not get secrets from AWS account, secretId: " + secretId);
    return {};
}

void SecretsWorker::WriteWorkbenchConfig() {

    const auto workbenchFilename = Configuration::instance().GetWorkbenchConfiguration();

    QSettings settings(workbenchFilename, QSettings::IniFormat);
    for (const auto &line: settings.allKeys()) {
        if (line.split(".").at(2) == "name") {
            QString nameValue = settings.value(line).toString();
            if (ForwarderConfig config = Configuration::instance().GetForwarderConfigByWorkbenchName(nameValue); !config.name.isEmpty()) {
                QString passwordKey = QString(line.toUtf8()).replace("name", "password");
                settings.setValue(passwordKey, config.password);
            }
        }
    }
    settings.sync();
    if (settings.status() != QSettings::NoError) {
        log_warning("Failed to write settings, filename: " + workbenchFilename);
    }
}

QString SecretsWorker::GetUsername(const QString &secret) {

    SecretRecord secretRecord;
    secretRecord.FromJson(QJsonDocument::fromJson(secret.toUtf8()).object());
    return secretRecord.username;
}

QString SecretsWorker::GetPassword(const QString &secret) {

    SecretRecord secretRecord;
    secretRecord.FromJson(QJsonDocument::fromJson(secret.toUtf8()).object());
    return secretRecord.password;
}
