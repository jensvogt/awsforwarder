//
// Created by vogje01 on 1/24/26.
//

#include <utils/KubernetesUtils.h>

KubernetesUtils::KubernetesUtils(RestManager *restManager) : _kubernetesService(nullptr) {
    _systemUtils = new SystemUtils();
    _suffix = Configuration::instance().GetForwarderSuffix();
    _kubernetesService = new KubernetesService(restManager);
}

KubernetesUtils::~KubernetesUtils() {
    delete _kubernetesService;
    delete _systemUtils;
}

void KubernetesUtils::StopForwarder() const {
    _systemUtils->StopProcesses();
}

bool KubernetesUtils::PodExists(const QString &awsAccount, const QString &nameSpace, const QString &podName) const {
    const QString kubeCtlCmd = Configuration::instance().GetKubeCtlExecutable();
    const QString context = Configuration::instance().GetClusterArn(awsAccount);

    const QStringList args{"--context", context, "get", "pod", podName, "--output", "json", "-n", nameSpace};

    const RunCommandResult result = _systemUtils->RunCommandSync(kubeCtlCmd, args);

    const QJsonDocument doc = QJsonDocument::fromJson(result.stdOut.toUtf8());
    return doc["status"]["containerStatuses"][0]["started"].toBool();
}

void KubernetesUtils::StartForwarder(const QString &awsAccount, const QString &nameSpace, const QVector<QString> &ports) const {
    const QString kubeCtlCmd = Configuration::instance().GetKubeCtlExecutable();
    const QString kubernetesConfig = Configuration::instance().GetKubernetesConfigFile();
    const QString context = Configuration::instance().GetClusterArn(awsAccount);
    const QString suffix = Configuration::instance().GetForwarderSuffix();

    const QString podName = "pod/" + nameSpace + suffix;
    QStringList args{"port-forward", podName, "--namespace", nameSpace, "--kubeconfig", kubernetesConfig, "--context", context};
    for (const auto &port: ports) {
        args << port;
    }
    _systemUtils->RunCommandAsync(nameSpace, kubeCtlCmd, args);
}

void KubernetesUtils::StartForwarderPod(const QString &awsAccount, const QString &nameSpace, const QString &fileName) const {
    const QString kubeCtlCmd = Configuration::instance().GetKubeCtlExecutable();
    const QString kubernetesConfig = Configuration::instance().GetKubernetesConfigFile();
    const QString context = Configuration::instance().GetClusterArn(awsAccount);

    QStringList args{"--context", context, "create", "-f", fileName, "--kubeconfig", kubernetesConfig, "--namespace", nameSpace};
    const RunCommandResult createResult = _systemUtils->RunCommandSync(kubeCtlCmd, args);
    log_info("PortForwarder started, waiting to be ready, nameSpace: " + nameSpace + ", exitCode: " + QString::number(createResult.exitCode));
    args = QStringList{"--context", context, "wait", "pod/" + nameSpace + _suffix, "--kubeconfig", kubernetesConfig, "--namespace", nameSpace};
    const RunCommandResult waitResult = _systemUtils->RunCommandSync(kubeCtlCmd, args);
    log_info("PortForwarder ready, nameSpace: " + nameSpace + ", exitCode: " + QString::number(createResult.exitCode));
}

Pod KubernetesUtils::CreatePodDto(const QString &nameSpace, const QString &podName, const QString &imageName) {
    // Create Pod
    Pod pod;
    pod.apiVersion = "v1";
    pod.kind = "Pod";
    pod.objectMeta.name = podName;
    pod.objectMeta.nameSpace = nameSpace;

    // Create metadata
    QMap<QString, QString> labels;
    labels["port-forwarder"] = nameSpace;
    pod.objectMeta.labels = labels;

    // Create Container
    Container container;
    container.name = podName;
    container.imageName = imageName;
    container.pullPolicy = PullIfNotPresent;

    for (QVector<ForwarderConfig> forwarders = Configuration::instance().GetForwarderConfigs(nameSpace); const auto &forwarder: forwarders) {
        Environment envVar;
        envVar.name = QString("PORT_%1").arg(forwarder.remotePort);
        envVar.value = QString("%1:%2-%3.%4:%5").arg(forwarder.localPort).arg(nameSpace, forwarder.name, forwarder.cluster).arg(forwarder.clusterPort);
        container.envVars.emplace_back(envVar);
    }

    // Create PodSpec
    PodSpec podSpec;
    podSpec.containers.emplace_back(container);
    pod.podSpec = podSpec;

    return pod;
}
