#include <aws/secretsmanager/model/FilterNameStringType.h>
#include <utils/Configuration.h>

void Configuration::SetFilePath(const QString &filePath) {
    this->_filePath = filePath;
    ReadConfigurationFile(filePath);
}

void Configuration::ReadConfigurationFile(const QString &filePath) {

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open config file:" << file.errorString();
        return;
    }

    const QByteArray jsonData = file.readAll();
    file.close();

    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "JSON parse error:" << parseError.errorString();
        return;
    }

    if (!doc.isObject()) {
        qWarning() << "Invalid JSON format: root is not an object";
        return;
    }

    // Root
    _configurationRoot = doc.object();
    _configurationObject.FromJsonObject(_configurationRoot);
}

void Configuration::WriteConfigurationFile(const QString &filePath) {

    if (!filePath.isEmpty()) {
        this->_filePath = filePath;
    }

    // Wrap it in a QJsonDocument
    const QJsonDocument doc(_configurationObject.ToJsonObject());

    // Open the file for writing
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Couldn't open file for writing:" << file.errorString();
        return;
    }

    // Write formatted (pretty-printed) JSON
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
}

void Configuration::WriteConfigurationFile() {
    WriteConfigurationFile(this->_filePath);
}

ForwarderConfig Configuration::GetForwarderConfig(const QJsonObject &jsonConfig) {
    //JsonUtils::WriteJsonString(jsonConfig);
    ForwarderConfig config;
    config.name = jsonConfig["name"].toString();
    config.nameSpace = jsonConfig["nameSpace"].toString();
    config.password = jsonConfig["password"].toString();
    config.username = jsonConfig["username"].toString();
    config.type = jsonConfig["type"].toString();
    config.cluster = jsonConfig["cluster"].toString();
    config.secret = jsonConfig["secret"].toString();
    config.awsAccount = jsonConfig["aws-account"].toString();
    config.active = jsonConfig["active"].toBool();
    config.localPort = jsonConfig["localPort"].toInt();
    config.remotePort = jsonConfig["remotePort"].toInt();
    config.clusterPort = jsonConfig["clusterPort"].toInt();
    return config;
}

QVector<ForwarderConfig> Configuration::GetForwarderConfigs() {
    return _configurationObject.kubernetes.forwarders;
}

QVector<ForwarderConfig> Configuration::GetForwarderConfigs(const QString &nameSpace) {
    QVector<ForwarderConfig> forwarderConfigs;
    for (const auto &forwarderConfig: _configurationObject.kubernetes.forwarders) {
        if (forwarderConfig.nameSpace == nameSpace) {
            forwarderConfigs.append(forwarderConfig);
        }
    }
    return forwarderConfigs;
}

ForwarderConfig Configuration::GetForwarderConfigsByService(const QString &nameSpace, const QString &service) {
    for (const auto &forwarderConfig: _configurationObject.kubernetes.forwarders) {
        if (forwarderConfig.nameSpace == nameSpace && forwarderConfig.name == service) {
            return forwarderConfig;
        }
    }
    return {};
}

ForwarderConfig Configuration::GetForwarderConfigByWorkbenchName(const QString &workbenchName) {
    for (const auto &forwarderConfig: _configurationObject.kubernetes.forwarders) {
        if (forwarderConfig.workbenchName == workbenchName) {
            return forwarderConfig;
        }
    }
    return {};
}

void Configuration::SetForwarderConfigByWorkbenchName(const QString &nameSpace, const QString &name, const QString &workbenchGroup, const QString &workbenchName) {
    for (auto &forwarderConfig: _configurationObject.kubernetes.forwarders) {
        if (forwarderConfig.nameSpace == nameSpace && forwarderConfig.name == name) {
            forwarderConfig.workbenchGroup = workbenchGroup;
            forwarderConfig.workbenchName = workbenchName;
        }
    }
    WriteConfigurationFile();
}

void Configuration::SetKubernetesPassword(const QString &nameSpace, const QString &name, const QString &password) {
    for (auto &forwarder: _configurationObject.kubernetes.forwarders) {
        if (forwarder.nameSpace == nameSpace && forwarder.name == name) {
            forwarder.password = password;
        }
    }
}

void Configuration::SetKubernetesUsername(const QString &nameSpace, const QString &name, const QString &username) {
    for (auto &forwarder: _configurationObject.kubernetes.forwarders) {
        if (forwarder.nameSpace == nameSpace && forwarder.name == name) {
            forwarder.username = username;
        }
    }
}
