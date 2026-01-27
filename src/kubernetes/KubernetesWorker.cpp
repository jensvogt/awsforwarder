//
// Created by vogje01 on 1/21/26.
//

#include <kubernetes/KubernetesWorker.h>

KubernetesWorker::KubernetesWorker() : _kubernetesUtils(nullptr), _restManager(nullptr) {

    _suffix = Configuration::instance().GetForwarderSuffix();
    _kubernetesUtils = new KubernetesUtils(_restManager);
}

void KubernetesWorker::Shutdown() const {
    _kubernetesUtils->StopForwarder();
}

void KubernetesWorker::DoWork(const QString &awsAccount, const QString &nameSpace, const Aws::STS::Model::Credentials &credentials) const {

    if (!_kubernetesUtils->PodExists(awsAccount, nameSpace, nameSpace + _suffix)) {
        log_warning("Forwarder POD not found, nameSpace: " + nameSpace);
        StartForwarderPod(awsAccount, nameSpace);
    } else {
        log_info("Forwarder POD found, nameSpace: " + nameSpace);
    }
    _kubernetesUtils->StartForwarder(awsAccount, nameSpace, GetPortList(nameSpace));
}

QStringList KubernetesWorker::GetPortList(const QString &nameSpace) {
    QStringList ports;
    for (const auto &config: Configuration::instance().GetForwarderConfigs(nameSpace)) {
        ports << QString::number(config.localPort) + ":" + QString::number(config.remotePort);
    }
    return ports;
}

void KubernetesWorker::StartForwarderPod(const QString &awsAccount, const QString &nameSpace) const {

    const QString kubernetesCmd = Configuration::instance().GetKubeCtlExecutable();
    const QString kubernetesConfig = Configuration::instance().GetKubernetesConfigFile();
    const QString forwarderImageName = Configuration::instance().GetForwarderImageName();

    // CreatePod
    const QString podName = nameSpace + _suffix;
    const Pod pod = KubernetesUtils::CreatePodDto(nameSpace, podName, forwarderImageName);

    // Save to a temporary file
    QTemporaryFile tempFile;
    if (tempFile.open()) {
        tempFile.write(pod.ToJsonString().toUtf8());
        tempFile.flush();
        tempFile.close();
    }

    _kubernetesUtils->StartForwarderPod(awsAccount, nameSpace, tempFile.fileName());
}
