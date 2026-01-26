//
// Created by vogje01 on 1/23/26.
//

#include "ServiceDetailsDialog.h"
#include "ui_ServiceDetailsDialog.h"

ServiceDetailsDialog::ServiceDetailsDialog(const QString &nameSpace, const QString &service, QWidget *parent) : QDialog(parent), _ui(new Ui::ServiceDetailsDialog), _namespace(nameSpace), _service(service) {

    _ui->setupUi(this);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &ServiceDetailsDialog::HandleAccept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &ServiceDetailsDialog::HandleReject);

    _awsAccount = _namespace == "pim-prod" ? "pim-prod" : "pim-int";

    const ForwarderConfig forwarder = Configuration::instance().GetForwarderConfigsByService(_namespace, service);

    // Text fields
    _ui->awsAccountEdit->setText(_awsAccount);
    _ui->namespaceEdit->setText(_namespace);
    _ui->serviceEdit->setText(_service);
    _ui->usernameEdit->setText(forwarder.username);
    _ui->passwordEdit->setText(forwarder.password);
    _ui->localPortEdit->setText(QString::number(forwarder.localPort));
    _ui->remotePortEdit->setText(QString::number(forwarder.remotePort));
    _ui->clusterPortEdit->setText(QString::number(forwarder.clusterPort));
    _ui->lastStartedEdit->setText(forwarder.lastStarted.toString("yyyy-MM-dd hh:mm::ss"));
    _ui->expiresEdit->setText(forwarder.expires.toString("yyyy-MM-dd hh:mm::ss"));
    _ui->workbenchGroupEdit->setText(forwarder.workbenchGroup);
    if (forwarder.type == "postgres") {
        connect(_ui->workbenchGroupEdit, &QLineEdit::textChanged, this, [this]() {
            _changed = true;
        });
        _ui->workbenchProfileEdit->setText(forwarder.workbenchName);
        connect(_ui->workbenchProfileEdit, &QLineEdit::textChanged, this, [this]() {
            _changed = true;
        });
    } else {
        _ui->gridLayout->removeWidget(_ui->workbenchGroupEdit);
        _ui->gridLayout->removeWidget(_ui->workbenchProfileEdit);
        _ui->gridLayout->removeWidget(_ui->workbenchGroupLabel);
        _ui->gridLayout->removeWidget(_ui->workbenchProfileLabel);
    }

    // Refresh
    _ui->refreshButton->setText(nullptr);
    _ui->refreshButton->setIcon(IconUtils::GetIcon("refresh"));

    // Password
    _ui->passwordButton->setText(nullptr);
    _ui->passwordButton->setIcon(IconUtils::GetIcon("copy"));
    connect(_ui->passwordButton, &QPushButton::clicked, this, [this]() {
        QGuiApplication::clipboard()->setText(_ui->passwordEdit->text());
    });

    // Set title
    setWindowTitle("Forwarder Details: " + nameSpace + ":" + service);
}

ServiceDetailsDialog::~ServiceDetailsDialog() {
    delete _ui;
}


void ServiceDetailsDialog::HandleAccept() {
    if (_changed) {
        Configuration::instance().SetForwarderConfigByWorkbenchName(_namespace, _service, _ui->workbenchGroupEdit->text(), _ui->workbenchProfileEdit->text());
    }
    accept();
}

void ServiceDetailsDialog::HandleReject() {
    accept();
}
