#ifndef AWSFORWARDER_REST_MANAGER_H
#define AWSFORWARDER_REST_MANAGER_H

#include <QMap>
#include <QObject>
#include <QPointer>
#include <QUrl>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <functional>

class RestManager final : public QObject {
    Q_OBJECT

public:
    explicit RestManager(QObject *parent = nullptr);

    ~RestManager() override;

    std::function<void()> post(const QUrl &url,
                               const QByteArray &body,
                               const QMap<QString, QString> &headers,
                               std::function<void(bool success,
                                                  QByteArray response,
                                                  int status,
                                                  QString error)> callback) const;

    std::function<void()> get(const QUrl &url,
                              const QMap<QString, QString> &headers,
                              std::function<void(bool success,
                                                 QByteArray response,
                                                 int status,
                                                 QString error)> callback) const;

    std::function<void()> del(const QUrl &url,
                              const QMap<QString, QString> &headers,
                              std::function<void(bool success,
                                                 QByteArray response,
                                                 int status,
                                                 QString error)> callback) const;

    std::function<void()> post(const QUrl &url, const QByteArray &body, const QMap<QString, QString> &headers, const QSslConfiguration &sslConfig, std::function<void(bool, QByteArray, int, QString)> callback) const;

    std::function<void()> get(const QUrl &url, const QMap<QString, QString> &headers, const QSslConfiguration &sslConfig, std::function<void(bool, QByteArray, int, QString)> callback) const;

    std::function<void()> del(const QUrl &url, const QMap<QString, QString> &headers, const QSslConfiguration &sslConfig, std::function<void(bool, QByteArray, int, QString)> callback) const;

private:
    QNetworkAccessManager *_manager;
};

#endif // AWSFORWARDER_REST_MANAGER_H
