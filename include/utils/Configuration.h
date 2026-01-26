#ifndef AWSFORWARDER_CONFIGURATION_H
#define AWSFORWARDER_CONFIGURATION_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QIODevice>
#include <QDir>

// Awsmock includes
#include <utils/JsonUtils.h>

//#include "ForwarderConfig.h"

#ifdef _WIN32
#define DEFAULT_CONFIGURATION_FILE_PATH QString("C:\\Program Files\\awsloginserver\\awsloginserver.json")
#else
#define DEFAULT_CONFIGURATION_FILE_PATH QString("/usr/local/awsforwarder/etc/awsforwarder.json")
#endif

struct AccountId {

    QString intId;
    QString prodId;

    void FromJsonObject(const QJsonObject &jsonObject) {
        intId = jsonObject["int"].toString();
        prodId = jsonObject["prod"].toString();
    }

    [[nodiscard]] QJsonObject ToJsonObject() const {
        QJsonObject jsonObject;
        jsonObject["int"] = intId;
        jsonObject["prod"] = prodId;
        return jsonObject;
    }
};

struct AppIds {

    int intId;
    int prodId;

    void FromJsonObject(const QJsonObject &jsonObject) {
        intId = jsonObject["int"].toInt();
        prodId = jsonObject["prod"].toInt();
    }

    [[nodiscard]] QJsonObject ToJsonObject() const {
        QJsonObject jsonObject;
        jsonObject["int"] = intId;
        jsonObject["prod"] = prodId;
        return jsonObject;
    }
};

struct AwsConfiguration {

    QString region;

    int sessionPeriod{};

    int secretPeriod{};

    int validationPeriod{};

    QString credentials;

    QString exec;

    AccountId accountId;

    void FromJsonObject(const QJsonObject &jsonObject) {
        region = jsonObject["region"].toString();
        sessionPeriod = jsonObject["session-period"].toInt();
        secretPeriod = jsonObject["secret-period"].toInt();
        validationPeriod = jsonObject["validation-period"].toInt();
        credentials = jsonObject["credentials"].toString();
        exec = jsonObject["exec"].toString();
        if (jsonObject.contains("account-id")) {
            accountId.FromJsonObject(jsonObject["account-id"].toObject());
        }
    }

    [[nodiscard]] QJsonObject ToJsonObject() const {
        QJsonObject jsonObject;
        jsonObject["region"] = region;
        jsonObject["session-period"] = sessionPeriod;
        jsonObject["secret-period"] = secretPeriod;
        jsonObject["validation-period"] = validationPeriod;
        jsonObject["credentials"] = credentials;
        jsonObject["exec"] = exec;
        jsonObject["account-id"] = accountId.ToJsonObject();
        return jsonObject;
    }
};

struct OneLoginConfiguration {

    QString subDomain;

    QString user;

    QString password;

    QString clientId;

    QString clientSecret;

    AppIds appIds{};

    void FromJsonObject(const QJsonObject &jsonObject) {
        subDomain = jsonObject["subDomain"].toString();
        user = jsonObject["user"].toString();
        password = jsonObject["password"].toString();
        clientId = jsonObject["clientId"].toString();
        clientSecret = jsonObject["clientSecret"].toString();
        if (jsonObject.contains("app-id")) {
            appIds.FromJsonObject(jsonObject["app-id"].toObject());
        }
    }

    [[nodiscard]] QJsonObject ToJsonObject() const {
        QJsonObject jsonObject;
        jsonObject["subDomain"] = subDomain;
        jsonObject["user"] = user;
        jsonObject["password"] = password;
        jsonObject["clientId"] = clientId;
        jsonObject["clientSecret"] = clientSecret;
        jsonObject["app-id"] = appIds.ToJsonObject();
        return jsonObject;
    }
};

struct Otp {

    QString key;

    void FromJsonObject(const QJsonObject &jsonObject) {
        key = jsonObject["key"].toString();
    }

    [[nodiscard]] QJsonObject ToJsonObject() const {
        QJsonObject jsonObject;
        jsonObject["key"] = key;
        return jsonObject;
    }
};

struct OneLogin {

    QString subDomain;

    QString user;

    QString password;

    QString clientId;

    QList<int> appIds;

    void FromJsonObject(const QJsonObject &jsonObject) {
        subDomain = jsonObject["subDomain"].toString();
        user = jsonObject["user"].toString();
        password = jsonObject["password"].toString();
        clientId = jsonObject["clientId"].toString();
        if (jsonObject.contains("appId")) {
            for (const auto &appId: jsonObject["appIds"].toArray()) {
                appIds.append(appId.toInt());
            }
        }
    }

    [[nodiscard]] QJsonObject ToJsonObject() const {
        QJsonObject jsonObject;
        jsonObject["subDomain"] = subDomain;
        jsonObject["user"] = user;
        jsonObject["password"] = password;
        jsonObject["clientId"] = clientId;
        if (!appIds.isEmpty()) {
            QJsonArray appIdsArray;
            for (const auto &appId: appIds) {
                appIdsArray.append(appId);
            }
            jsonObject["appIds"] = appIdsArray;
        }
        return jsonObject;
    }
};

struct ForwarderConfig {

    QString name;

    QString nameSpace;

    QString password;

    QString username;

    QString type;

    QString cluster;

    QString secret;

    QString awsAccount;

    QString workbenchGroup;

    QString workbenchName;

    bool active{};

    int localPort{};

    int remotePort{};

    int clusterPort{};

    QDateTime lastStarted;

    QDateTime expires;

    void FromJsonObject(const QJsonObject &jsonObject) {
        name = jsonObject["name"].toString();
        nameSpace = jsonObject["nameSpace"].toString();
        password = jsonObject["password"].toString();
        username = jsonObject["username"].toString();
        type = jsonObject["type"].toString();
        cluster = jsonObject["cluster"].toString();
        secret = jsonObject["secret"].toString();
        awsAccount = jsonObject["aws-account"].toString();
        workbenchGroup = jsonObject["workbench-group"].toString();
        workbenchName = jsonObject["workbench-name"].toString();
        active = jsonObject["active"].toBool();
        localPort = jsonObject["localPort"].toInt();
        remotePort = jsonObject["remotePort"].toInt();
        clusterPort = jsonObject["clusterPort"].toInt();
        lastStarted = QDateTime::fromString(jsonObject["last-started"].toString(), Qt::ISODate);
        expires = QDateTime::fromString(jsonObject["expires"].toString(), Qt::ISODate);
    }

    [[nodiscard]] QJsonObject ToJsonObject() const {
        QJsonObject jsonObject;
        jsonObject["name"] = name;
        jsonObject["nameSpace"] = nameSpace;
        jsonObject["password"] = password;
        jsonObject["username"] = username;
        jsonObject["type"] = type;
        jsonObject["cluster"] = cluster;
        jsonObject["secret"] = secret;
        jsonObject["awsAccount"] = awsAccount;
        jsonObject["workbench-group"] = workbenchGroup;
        jsonObject["workbench-name"] = workbenchName;
        jsonObject["active"] = active;
        jsonObject["localPort"] = localPort;
        jsonObject["remotePort"] = remotePort;
        jsonObject["clusterPort"] = clusterPort;
        jsonObject["last-started"] = lastStarted.toString(Qt::ISODate);
        jsonObject["expires"] = expires.toString(Qt::ISODate);
        return jsonObject;
    }
};

struct Cluster {
    QString name;
    QString awsAccount;
    QString startCron;
    QString endCron;
    QString clusterArn;

    void FromJsonObject(const QJsonObject &jsonObject) {
        name = jsonObject["name"].toString();
        awsAccount = jsonObject["aws-account"].toString();
        startCron = jsonObject["startCron"].toString();
        endCron = jsonObject["endCron"].toString();
        clusterArn = jsonObject["cluster-arn"].toString();
    }

    [[nodiscard]] QJsonObject ToJsonObject() const {
        QJsonObject jsonObject;
        jsonObject["name"] = name;
        jsonObject["aws-account"] = awsAccount;
        jsonObject["startCron"] = startCron;
        jsonObject["endCron"] = endCron;
        jsonObject["cluster-arn"] = clusterArn;
        return jsonObject;
    }
};

struct Kubernetes {

    QString configFile;

    QString exec;

    QString suffix;

    QString forwarderImageName;

    QList<Cluster> clusters;

    QList<ForwarderConfig> forwarders;

    void FromJsonObject(const QJsonObject &object) {
        if (object.contains("config-file")) {
            configFile = object["config-file"].toString();
        }
        if (object.contains("exec")) {
            exec = object["exec"].toString();
        }
        if (object.contains("suffix")) {
            suffix = object["suffix"].toString();
        }
        if (object.contains("forwarder-image")) {
            forwarderImageName = object["forwarder-image"].toString();
        }
        for (const auto &jsonCluster: object.value("clusters").toArray()) {
            Cluster cluster;
            cluster.FromJsonObject(jsonCluster.toObject());
            clusters.push_back(cluster);
        }
        for (const auto &forwarder: object.value("forwarders").toArray()) {
            ForwarderConfig forwarderConfig;
            forwarderConfig.FromJsonObject(forwarder.toObject());
            forwarders.push_back(forwarderConfig);
        }
    }

    QJsonObject ToJsonObject() {
        QJsonObject jsonObject;
        jsonObject["config-file"] = configFile;
        jsonObject["exec"] = exec;
        jsonObject["suffix"] = suffix;
        jsonObject["forwarder-image"] = forwarderImageName;
        if (!forwarders.isEmpty()) {
            QJsonArray jsonArray;
            for (const auto &forwarder: forwarders) {
                jsonArray.append(forwarder.ToJsonObject());
            }
            jsonObject["forwarders"] = jsonArray;
        }
        if (!clusters.isEmpty()) {
            QJsonArray jsonArray;
            for (const auto &cluster: clusters) {
                jsonArray.append(cluster.ToJsonObject());
            }
            jsonObject["clusters"] = jsonArray;
        }
        return jsonObject;
    }
};

struct ConfigurationObject {

    QString style;

    QString styleType;

    QString workbenchConfigFilename;

    Otp otp;

    AwsConfiguration aws;

    OneLoginConfiguration oneLogin;

    Kubernetes kubernetes;


    void FromJsonObject(const QJsonObject &jsonObject) {
        style = jsonObject["style"].toString();
        styleType = jsonObject["style-type"].toString();
        workbenchConfigFilename = jsonObject["workbench-configuration"].toString();
        if (jsonObject.contains("otp")) {
            otp.FromJsonObject(jsonObject["otp"].toObject());
        }
        if (jsonObject.contains("aws")) {
            aws.FromJsonObject(jsonObject["aws"].toObject());
        }
        if (jsonObject.contains("onelogin")) {
            oneLogin.FromJsonObject(jsonObject["onelogin"].toObject());
        }
        if (jsonObject.contains("kubernetes")) {
            kubernetes.FromJsonObject(jsonObject["kubernetes"].toObject());
        }
    }

    QJsonObject ToJsonObject() {
        QJsonObject jsonObject;
        jsonObject.insert("style", style);
        jsonObject.insert("style-type", styleType);
        jsonObject.insert("workbench-configuration", workbenchConfigFilename);
        jsonObject["otp"] = otp.ToJsonObject();
        jsonObject["aws"] = aws.ToJsonObject();
        jsonObject["onelogin"] = oneLogin.ToJsonObject();
        jsonObject["kubernetes"] = kubernetes.ToJsonObject();
        return jsonObject;
    }
};

class Configuration final : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     */
    Configuration() = default;

    /**
     * @brief Singleton instance
     *
     * @return
     */
    static Configuration &instance() {
        static Configuration instance;
        return instance;
    }

    template<class T>
    T GetValue(const QString &path) {
        const QJsonValue v = JsonUtils::JsonValueByPath(_configurationRoot, path);
        if constexpr (std::is_same_v<T, int>) {
            return static_cast<T>(v.toInt());
        } else if constexpr (std::is_same_v<T, long>) {
            return static_cast<T>(v.toInteger());
        } else if constexpr (std::is_same_v<T, double>) {
            return static_cast<T>(v.toDouble());
        } else if constexpr (std::is_same_v<T, QString>) {
            return static_cast<T>(v.toString());
        } else if constexpr (std::is_same_v<T, bool>) {
            return static_cast<T>(v.toBool());
        } else if constexpr (std::is_same_v<T, QJsonObject>) {
            return static_cast<T>(v.toObject());
        } else if constexpr (std::is_same_v<T, QJsonArray>) {
            return static_cast<T>(v.toArray());
        } else {
            return {};
        }
    }

    template<class T>
    T GetValue(const QString &path, T defaultValue) {
        const QJsonValue v = JsonUtils::JsonValueByPath(_configurationRoot, path);
        if constexpr (std::is_same_v<T, int>) {
            return v.isDouble() ? static_cast<T>(v.toInt()) : defaultValue;
        } else if constexpr (std::is_same_v<T, long>) {
            return v.isDouble() ? static_cast<T>(v.toInteger()) : defaultValue;
        } else if constexpr (std::is_same_v<T, double>) {
            return v.isDouble() ? static_cast<T>(v.toDouble()) : defaultValue;
        } else if constexpr (std::is_same_v<T, QString>) {
            return v.isString() ? static_cast<T>(v.toString()) : defaultValue;
        } else if constexpr (std::is_same_v<T, bool>) {
            return v.isBool() ? static_cast<T>(v.toBool()) : defaultValue;
        } else if constexpr (std::is_same_v<T, QJsonObject>) {
            return v.isObject() ? static_cast<T>(v.toObject()) : defaultValue;
        } else if constexpr (std::is_same_v<T, QJsonArray>) {
            return v.isArray() ? static_cast<T>(v.toArray()) : defaultValue;
        } else {
            return defaultValue;
        }
    }

    template<class T>
    void SetValue(const QString &path, T value) {
        if constexpr (std::is_same_v<T, long>) {
            JsonUtils::setByPath(_configurationRoot, path, static_cast<qint64>(value));
            WriteConfigurationFile(_filePath);
            emit ConfigurationChanged(path, QString::number(value));
        } else {
            JsonUtils::setByPath(_configurationRoot, path, static_cast<T>(value));
            WriteConfigurationFile(_filePath);
            emit ConfigurationChanged(path, value);
        }
    }

    void SetKubernetesPassword(const QString &nameSpace, const QString &name, const QString &password);

    void SetKubernetesUsername(const QString &nameSpace, const QString &name, const QString &username);

    /**
     * @brief Write a JSON configuration file
     *
     * @param filePath absolute file path of the configuration file
     */
    void WriteConfigurationFile(const QString &filePath);

    void WriteConfigurationFile();

    /**
     * @brief Write a JSON configuration file
     *
     * @param filePath absolute file path of the configuration file
     */
    void ReadConfigurationFile(const QString &filePath);

    /**
     * @brief SetFilePath
     *
     * @param filePath absolute path to configuration file
     */
    void SetFilePath(const QString &filePath);

    QVector<ForwarderConfig> GetForwarderConfigs();

    QVector<ForwarderConfig> GetForwarderConfigs(const QString &nameSpace);

    ForwarderConfig GetForwarderConfigsByService(const QString &nameSpace, const QString &service);

    ForwarderConfig GetForwarderConfigByWorkbenchName(const QString &workbenchName);

    [[nodiscard]] QString GetForwarderImageName() const {
        return _configurationObject.kubernetes.forwarderImageName;
    }

    void SetForwarderConfigByWorkbenchName(const QString &nameSpace, const QString &name, const QString &workbenchGroup, const QString &workbenchName);

    [[nodiscard]] QString GetOtpKey() const {
        return _configurationObject.otp.key;
    }

    [[nodiscard]] QString GetAwsRegion() const {
        return _configurationObject.aws.region;
    }

    QString GetClusterArn(const QString &awsAccount) {
        for (const auto &config: _configurationObject.kubernetes.clusters) {
            if (config.awsAccount == awsAccount) {
                return config.clusterArn;
            }
        }
        return {};
    }

    QString GetClusterName(const QString &awsAccount) {
        for (const auto &config: _configurationObject.kubernetes.clusters) {
            if (config.awsAccount == awsAccount) {
                return config.name;
            }
        }
        return {};
    }

    [[nodiscard]] QString GetAwsExecutable() const {
        return _configurationObject.aws.exec;
    }

    [[nodiscard]] QString GetKubernetesConfigFile() const {
        return _configurationObject.kubernetes.configFile;
    }

    [[nodiscard]] QString GetWorkbenchConfiguration() const {
        return _configurationObject.workbenchConfigFilename;
    }

    [[nodiscard]] QString GetKubeCtlExecutable() const {
        return _configurationObject.kubernetes.exec;
    }

    [[nodiscard]] QString GetForwarderSuffix() const {
        return _configurationObject.kubernetes.suffix;
    }

    void SetLastStarted(const QString &awsAccount, const QDateTime &lastStarted) {
        for (auto &config: _configurationObject.kubernetes.forwarders) {
            if (config.awsAccount == awsAccount) {
                config.lastStarted = lastStarted;
            }
        }
    }

    void SetExpires(const QString &awsAccount, const QDateTime &expires) {
        for (auto &config: _configurationObject.kubernetes.forwarders) {
            if (config.awsAccount == awsAccount) {
                config.expires = expires;
            }
        }
    }

signals:
    /**
     * @brief Send when a preferences changed
     *
     * @param key preference key
     * @param value preference value
     */
    void ConfigurationChanged(const QString &key, const QString &value);

private:
    /**
     * @brief Read a single forwarder config
     *
     * @param jsonConfig JSON configuration
     * @return ForwarderConfig
     */
    static ForwarderConfig GetForwarderConfig(const QJsonObject &jsonConfig);

    /**
     * @brief Configuration root
     */
    QJsonObject _configurationRoot{};

    /**
     * @brief File path
     */
    QString _filePath = DEFAULT_CONFIGURATION_FILE_PATH;

    /**
     * @brief Connection flag
     */
    bool _connected = true;

    ConfigurationObject _configurationObject;
};

#endif // AWSFORWARDER_CONFIGURATION_H
