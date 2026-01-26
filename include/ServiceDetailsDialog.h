//
// Created by vogje01 on 1/23/26.
//

#ifndef AWSFORWARDER_SERVICE_DETAILS_DIALOG_H
#define AWSFORWARDER_SERVICE_DETAILS_DIALOG_H

// Qt includes
#include <QDialog>
#include <QGuiApplication>
#include <QClipboard>

// Forwarder includes
#include <utils/Configuration.h>
//#include <utils/ForwarderConfig.h>
#include <utils/IconUtils.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class ServiceDetailsDialog;
}

QT_END_NAMESPACE

class ServiceDetailsDialog : public QDialog {
    Q_OBJECT

public:
    explicit ServiceDetailsDialog(const QString &nameSpace, const QString &service, QWidget *parent = nullptr);

    ~ServiceDetailsDialog() override;

    void HandleAccept();

    void HandleReject();

private:
    /**
     * @brief UI components
     */
    Ui::ServiceDetailsDialog *_ui;

    /**
     * @brief AWS namespace
     */
    QString _namespace;

    /**
     * @brief AWS service name
     */
    QString _service;

    /**
     * @brief AWS account
     */
    QString _awsAccount;

    /**
     * @brief changed flag
     */
    bool _changed = false;
};


#endif //AWSFORWARDER_SERVICE_DETAILS_DIALOG_H
