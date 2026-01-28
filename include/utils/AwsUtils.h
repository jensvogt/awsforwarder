//
// Created by vogje01 on 10/03/2025.
//

#ifndef AWSFORWARDER_AWSUTILS_H
#define AWSFORWARDER_AWSUTILS_H

// C++ includes
#include <fstream>

// Qt includes
#include <QString>
#include <QSettings>
#include <QXmlStreamReader>

#ifdef _WIN32
//#define USE_IMPORT_EXPORT
//#define USE_WINDOWS_DLL_SEMANTICS
#define YAML_CPP_API
#endif

// AWS includes
#include <aws/sts/STSClient.h>
#include <aws/sts/STSServiceClientModel.h>
#include <aws/sts/model/AssumeRoleWithSAMLRequest.h>
#include <aws/eks/EKSClient.h>
#include <aws/eks/model/DescribeClusterRequest.h>
#include <aws/core/auth/AWSCredentials.h>

// AwsLogin includes
#include <utils/Configuration.h>
#include <utils/OneLoginUtils.h>
#include <utils/KubernetesUtils.h>

class AwsUtils : public QObject {
    Q_OBJECT

public:
    explicit AwsUtils(RestManager *restManager, QObject *parent = nullptr) : _parent(parent), _restManager(restManager) {
        _oneloginUtils = new OneLoginUtils(_restManager, parent);
        _region = Configuration::instance().GetAwsRegion();
    }

    /**
     * @brief Get the AWS credentials
     *
     * @param awsAccount AWS account
     * @param nameSpace
     * @return AWS credentials
     */
    void GetAwsSessionToken(const QString &awsAccount, const QString &nameSpace);

signals:
    /**
     * @brief Signaled when an access token is available
     *
     * @param awsAccount AWS account, either pim-int or pim-prod
     * @param nameSpace AWS name space
     * @param credentials AWS credentials
     */
    void GetAwsCredentialsSignal(const QString &awsAccount, const QString &nameSpace, const Aws::STS::Model::Credentials &credentials);

private:
    /**
     * @brief Returns the AWS role ARN
     *
     * @param encodedSamlAssertion Base64 encoded SAML assertion
     * @return
     */
    static QVector<QString> GetRoleArn(const QString &encodedSamlAssertion);

    /**
     * @brief write the AWS credentials to a file
     *
     * @param awsAccount AWS account
     * @param outcome AWS SAML assume role outcome
     */
    static void WriteAwsCredentialsFile(const QString &awsAccount, const Aws::STS::Model::AssumeRoleWithSAMLOutcome &outcome);

    /**
     * @brief Create cluster ARN
     */
    static QString GetClusterArn(const QString &awsAccount);

    void WriteKubernetesConfig(const QString &awsAccount, const Aws::STS::Model::AssumeRoleWithSAMLOutcome &outcome) const;

    /**
     * @brief AWS credentials
     */
    QMap<QString, Aws::STS::Model::Credentials> _credentials;

    /**
     * @brief AWS connection
     */
    QMetaObject::Connection _sessionTokenConnection;

    /**
     * @brief OneLogin connection
     */
    OneLoginUtils *_oneloginUtils;

    /**
     * @brief Parent widget
     */
    QObject _parent;

    /**
     * @brief REST manager
     */
    RestManager *_restManager;

    /**
     * @brief AWS region
     */
    QString _region;
};

#endif// AWSFORWARDER_AWSUTILS_H
