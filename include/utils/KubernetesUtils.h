//
// Created by vogje01 on 1/24/26.
//

#ifndef AWSFORWARDER_KUBERNETES_UTILS_H
#define AWSFORWARDER_KUBERNETES_UTILS_H

// C++ includes
#include <fstream>

// YAML-cpp include
#include <yaml-cpp/yaml.h>
#include <aws/sts/model/Credentials.h>

// Forwarder includes
#include <utils/Configuration.h>
#include <utils/KubernetesService.h>
#include <utils/SystemUtils.h>
#include <dto/kubernetes/Pod.h>

class KubernetesUtils {

public:
    explicit KubernetesUtils(RestManager *restManager);

    ~KubernetesUtils();

    static void UpdateKubernetesConfig(const QString &clusterArn, const QString &endpoint, const QString &caData) {

        try {
            YAML::Node kubernetesConfig = YAML::LoadFile(Configuration::instance().GetKubernetesConfigFile().toStdString());

            // Extract the Server URL and CA Data from the clusters list
            for (auto cluster: kubernetesConfig["clusters"]) {
                if (cluster["name"].as<std::string>() == clusterArn.toStdString()) {
                    cluster["cluster"]["server"] = endpoint.toStdString();
                    cluster["cluster"]["certificate-authority-data"] = caData.toStdString();
                    break;
                }
            }

            std::ofstream fout(Configuration::instance().GetKubernetesConfigFile().toStdString());
            fout << kubernetesConfig;
            fout.close();

        } catch (const std::exception &e) {
            qDebug() << "Error reading kubeconfig:" << e.what();
        }
    }

    static QString GetAuthorityDataFromConfig(const QString &clusterArn) {
        try {

            // Extract the Server URL and CA Data from the clusters list
            for (YAML::Node kubernetesConfig = YAML::LoadFile(Configuration::instance().GetKubernetesConfigFile().toStdString()); auto cluster: kubernetesConfig["clusters"]) {
                if (cluster["name"].as<std::string>() == clusterArn.toStdString()) {
                    return QString::fromStdString(cluster["cluster"]["certificate-authority-data"].as<std::string>());
                }
            }
        } catch (const std::exception &e) {
            qDebug() << "Error reading kubeconfig:" << e.what();
        }
        return {};
    }

    static QSslConfiguration GetSslConfiguration(const QString &clusterArn) {
        const QByteArray caPem = QByteArray::fromBase64(GetAuthorityDataFromConfig(clusterArn).toUtf8());
        const QSslCertificate caCert(caPem, QSsl::Pem);
        QSslConfiguration sslConfig = QSslConfiguration::defaultConfiguration();
        QList<QSslCertificate> caCerts = sslConfig.caCertificates();
        caCerts.append(caCert);
        sslConfig.setCaCertificates(caCerts);
        return sslConfig;
    }

    static QString GetEndpoint(const QString &clusterArn) {
        try {

            // Extract the Server URL and CA Data from the clusters list
            for (YAML::Node kubernetesConfig = YAML::LoadFile(Configuration::instance().GetKubernetesConfigFile().toStdString()); auto cluster: kubernetesConfig["clusters"]) {
                if (cluster["name"].as<std::string>() == clusterArn.toStdString()) {
                    return QString::fromStdString(cluster["cluster"]["server"].as<std::string>());
                }
            }
        } catch (const std::exception &e) {
            qDebug() << "Error reading kubeconfig:" << e.what();
        }
        return {};
    }

    static QString SetCurrentContext(const QString &awsAccount) {
        try {

            // Extract the Server URL and CA Data from the clusters list
            YAML::Node kubernetesConfig = YAML::LoadFile(Configuration::instance().GetKubernetesConfigFile().toStdString());
            for (auto cluster: kubernetesConfig["clusters"]) {
                if (QString::fromStdString(cluster["name"].as<std::string>()).contains(awsAccount)) {
                    kubernetesConfig["current-context"] = cluster["name"].as<std::string>();
                }
            }

            std::ofstream fout(Configuration::instance().GetKubernetesConfigFile().toStdString());
            fout << kubernetesConfig;
            fout.close();

        } catch (const std::exception &e) {
            qDebug() << "Error reading kubeconfig:" << e.what();
        }
        return {};
    }

    [[nodiscard]] bool PodExists(const QString &awsAccount, const QString &nameSpace, const QString &podName) const;

    static Pod CreatePodDto(const QString &nameSpace, const QString &podName, const QString &imageName);

    void StartForwarder(const QString &awsAccount, const QString &nameSpace, const QVector<QString> &ports) const;

    void StartForwarderPod(const QString &awsAccount, const QString &nameSpace, const QString &fileName) const;

    void StopForwarder() const;

    static QString GetEksToken(const QString &clusterName) {

        const QString awsCmd = Configuration::instance().GetAwsExecutable();
        const QString cmd = awsCmd + " --region eu-central-1 eks get-token --cluster-name " + clusterName + " --output json";
#ifndef _WIN32
        FILE *pipe = popen(cmd.toUtf8().constData(), "r");
        if (!pipe) {
            log_error("Failed to run aws eks get-token, clusterName: " + clusterName + " failed.");
        }

        QByteArray output;
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), pipe))
            output += buffer;

        pclose(pipe);

        const QJsonDocument doc = QJsonDocument::fromJson(output);
        return doc["status"]["token"].toString();
#else
        return {};
#endif
    }

private:
    /**
     * @brief Kubernetes service
     */
    KubernetesService *_kubernetesService;

    /**
     * @brief System utils
     */
    SystemUtils *_systemUtils;
};
#endif //AWSFORWARDER_KUBERNETES_UTILS_H
