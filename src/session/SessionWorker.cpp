//
// Created by vogje01 on 1/21/26.
//

#include <session/SessionWorker.h>

SessionWorker::SessionWorker(QString nameSpace) : _nameSpace(std::move(nameSpace)), _awsUtils(nullptr), _secretsWorker(nullptr), _kubernetesWorker(nullptr), _restManager(nullptr) {

    if (_nameSpace == "pim-prod") {
        _awsAccount = "pim-prod";
    } else {
        _awsAccount = "pim-int";
    }
    _region = Configuration::instance().GetValue<QString>("aws.region");
}

SessionWorker::~SessionWorker() {
    delete _restManager;
    delete _awsUtils;
    delete _secretsWorker;
    delete _kubernetesWorker;
}

void SessionWorker::DoWork(const QString &awsAccount, const QString &nameSpace) {

    if (!_restManager)
        _restManager = new RestManager();
    if (!_awsUtils)
        _awsUtils = new AwsUtils(_restManager);
    if (!_secretsWorker)
        _secretsWorker = new SecretsWorker(_awsAccount);
    if (!_kubernetesWorker)
        _kubernetesWorker = new KubernetesWorker();

    // Get AWS session token
    _awsUtils->GetAwsSessionToken(awsAccount, nameSpace);

    connect(_awsUtils, &AwsUtils::GetAwsCredentialsSignal, _secretsWorker, &SecretsWorker::GetPasswords);
    connect(_awsUtils, &AwsUtils::GetAwsCredentialsSignal, _kubernetesWorker, &KubernetesWorker::DoWork);
}
