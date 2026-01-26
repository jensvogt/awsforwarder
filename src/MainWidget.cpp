//
// Created by vogje01 on 1/18/26.
//

#include "MainWidget.h"
#include "ui_MainWidget.h"

MainWidget::MainWidget(QWidget *parent) : QWidget(parent), _ui(new Ui::MainWidget) {

    // Setup UI components
    _ui->setupUi(this);

    _ui->devRefreshButton->setText(nullptr);
    _ui->devRefreshButton->setIcon(IconUtils::GetIcon("refresh"));

    _ui->intRefreshButton->setText(nullptr);
    _ui->intRefreshButton->setIcon(IconUtils::GetIcon("refresh"));

    _ui->prodRefreshButton->setText(nullptr);
    _ui->prodRefreshButton->setIcon(IconUtils::GetIcon("refresh"));

    SetupDevTable();
    SetupIntTable();
    SetupProdTable();

    // Reset tab index
    _ui->tabWidget->setCurrentIndex(0);
    connect(_ui->tabWidget, &QTabWidget::currentChanged, this, [this]() {
        LoadContent();
    });

    // Connect logging list
    _logDataModel = new QStandardItemModel(this);
    _ui->logListView->setModel(_logDataModel);
    connect(&EventBus::instance(), &EventBus::LoggingSignal, this, &MainWidget::LogMessage);

    // Start secrets thread
    StartSecretsThread("pim-int", "pim-dev");
    StartSecretsThread("pim-int", "pim-int");
    StartSecretsThread("pim-prod", "pim-prod");

    MainWidget::LoadContent();

    // Set logging message
    log_debug("MainWindow initialized");
}

MainWidget::~MainWidget() {

    emit EventBus::instance().ShutdownSignal();

    for (const auto &thread: _threads) {

        // Quit event loop
        thread->quit();

        // Wait for completion
        thread->wait();

        // Safe to delete thread now
        thread->deleteLater();
    }
    delete _ui;
}

void MainWidget::SetupDevTable() {

    const QStringList headers = QStringList() = {
                                    tr("Service"), tr("Cluster"), tr("Last Started"), tr("Expired")
                                };

    _devDataModel = new QStandardItemModel(this);
    _devDataModel->setHorizontalHeaderLabels(headers);
    _devDataModel->setColumnCount(static_cast<int>(headers.count()));

    // Proxy model for prefix filtering
    _ui->devTableView->setModel(_devDataModel);
    _ui->devTableView->setShowGrid(true);
    _ui->devTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->devTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->devTableView->setSortingEnabled(true);
    _ui->devTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->devTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    _ui->devTableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    _ui->devTableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    _ui->devTableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    _ui->devTableView->addAction(GetRefreshAction(this));

    // Connect double-click
    connect(_ui->devTableView, &QTableView::doubleClicked, this, [this](const QModelIndex &index) {

        // Extract ARN
        const auto service = _devDataModel->item(index.row(), 0)->text();

        ServiceDetailsDialog dialog("pim-dev", service);
        dialog.exec();
    });

    // Add context menu
    _ui->devTableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_ui->devTableView, &QTableView::customContextMenuRequested, this, &MainWidget::ShowContextMenu);
}

void MainWidget::SetupIntTable() {

    const QStringList headers = QStringList() = {
                                    tr("Service"), tr("Cluster"), tr("Last Started"), tr("Expired")
                                };

    _intDataModel = new QStandardItemModel(this);
    _intDataModel->setHorizontalHeaderLabels(headers);
    _intDataModel->setColumnCount(static_cast<int>(headers.count()));

    // Proxy model for prefix filtering
    _ui->intTableView->setModel(_intDataModel);
    _ui->intTableView->setShowGrid(true);
    _ui->intTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->intTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->intTableView->setSortingEnabled(true);
    _ui->intTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->intTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    _ui->intTableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    _ui->intTableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    _ui->intTableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    _ui->intTableView->addAction(GetRefreshAction(this));

    // Connect double-click
    connect(_ui->intTableView, &QTableView::doubleClicked, this, [this](const QModelIndex &index) {

        // Extract ARN
        const auto service = _intDataModel->item(index.row(), 0)->text();

        ServiceDetailsDialog dialog("pim-int", service);
        dialog.exec();
    });

    // Add context menu
    _ui->intTableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_ui->intTableView, &QTableView::customContextMenuRequested, this, &MainWidget::ShowContextMenu);
}

void MainWidget::SetupProdTable() {

    const QStringList headers = QStringList() = {
                                    tr("Service"), tr("Cluster"), tr("Last Started"), tr("Expired")
                                };

    _prodDataModel = new QStandardItemModel(this);
    _prodDataModel->setHorizontalHeaderLabels(headers);
    _prodDataModel->setColumnCount(static_cast<int>(headers.count()));

    // Proxy model for prefix filtering
    _ui->prodTableView->setModel(_prodDataModel);
    _ui->prodTableView->setShowGrid(true);
    _ui->prodTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->prodTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->prodTableView->setSortingEnabled(true);
    _ui->prodTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->prodTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    _ui->prodTableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    _ui->prodTableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    _ui->prodTableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    _ui->prodTableView->addAction(GetRefreshAction(this));

    // Connect double-click
    connect(_ui->prodTableView, &QTableView::doubleClicked, this, [this](const QModelIndex &index) {

        // Extract ARN
        const auto service = _prodDataModel->item(index.row(), 0)->text();

        ServiceDetailsDialog dialog("pim-prod", service);
        dialog.exec();
    });

    // Add context menu
    _ui->prodTableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_ui->prodTableView, &QTableView::customContextMenuRequested, this, &MainWidget::ShowContextMenu);
}

void MainWidget::LoadContent() {

    if (const QString nameSpace = GetNamespaceFromTab(); nameSpace == "pim-dev") {
        const int selectedRow = _ui->devTableView->selectionModel()->currentIndex().row();
        _ui->devTableView->setSortingEnabled(false);
        _devDataModel->removeRows(0, _devDataModel->rowCount());
        int r = 0;
        for (const auto &config: Configuration::instance().GetForwarderConfigs("pim-dev")) {
            SetColumn(_devDataModel, r, 0, config.name);
            SetColumn(_devDataModel, r, 1, config.cluster);
            SetColumn(_devDataModel, r, 2, config.lastStarted.toString("yyyy-MM-dd HH:mm:ss"));
            SetColumn(_devDataModel, r, 3, config.expires.toString("yyyy-MM-dd HH:mm:ss"));
            r++;
        }
        _ui->devTableView->setSortingEnabled(true);
        _ui->devTableView->selectRow(selectedRow);
    } else if (nameSpace == "pim-int") {
        const int selectedRow = _ui->intTableView->selectionModel()->currentIndex().row();
        _ui->intTableView->setSortingEnabled(false);
        _intDataModel->removeRows(0, _intDataModel->rowCount());
        int r = 0;
        for (const auto &config: Configuration::instance().GetForwarderConfigs("pim-int")) {
            SetColumn(_intDataModel, r, 0, config.name);
            SetColumn(_intDataModel, r, 1, config.cluster);
            SetColumn(_intDataModel, r, 2, config.lastStarted.toString("yyyy-MM-dd HH:mm:ss"));
            SetColumn(_intDataModel, r, 3, config.expires.toString("yyyy-MM-dd HH:mm:ss"));
            r++;
        }
        _ui->intTableView->setSortingEnabled(true);
        _ui->intTableView->selectRow(selectedRow);
    } else if (nameSpace == "pim-prod") {
        const int selectedRow = _ui->prodTableView->selectionModel()->currentIndex().row();
        _ui->prodTableView->setSortingEnabled(false);
        _prodDataModel->removeRows(0, _prodDataModel->rowCount());
        int r = 0;
        for (const auto &config: Configuration::instance().GetForwarderConfigs("pim-prod")) {
            SetColumn(_prodDataModel, r, 0, config.name);
            SetColumn(_prodDataModel, r, 1, config.cluster);
            SetColumn(_prodDataModel, r, 2, config.lastStarted.toString("yyyy-MM-dd HH:mm:ss"));
            SetColumn(_prodDataModel, r, 3, config.expires.toString("yyyy-MM-dd HH:mm:ss"));
            r++;
        }
        _ui->prodTableView->setSortingEnabled(true);
        _ui->prodTableView->selectRow(selectedRow);
    }

    // Log to status bar
    _ui->statusLabel->setText("Last update: " + QDateTime::currentDateTime().toString("HH:mm:ss"));
}

void MainWidget::SetColumn(QStandardItemModel *dataModel, const int row, const int col, const QString &value, const Qt::Alignment alignment) {
    const auto item = new QStandardItem(value);
    item->setTextAlignment(alignment);
    const QModelIndex index = dataModel->index(row, col);
    dataModel->setData(index, QVariant(alignment));
    dataModel->setData(index, value, Qt::EditRole);
    dataModel->setData(index, value, Qt::DisplayRole);
    dataModel->setItem(row, col, item);
}


QString MainWidget::GetNamespaceFromTab() const {
    if (_ui->tabWidget->currentIndex() == 0) {
        return "pim-dev";
    }
    if (_ui->tabWidget->currentIndex() == 1) {
        return "pim-int";
    }
    if (_ui->tabWidget->currentIndex() == 2) {
        return "pim-prod";
    }
    return {};
}

QAction *MainWidget::GetRefreshAction(QWidget *parent) const {
    // Function key F5 refresh action
    const auto refreshAction = new QAction(parent);
    refreshAction->setShortcut(QKeySequence(Qt::Key_F5));
    refreshAction->setShortcutContext(Qt::ApplicationShortcut);
    connect(refreshAction, &QAction::triggered, this, &MainWidget::LoadContent);
    refreshAction->deleteLater();
    return refreshAction;
}

void MainWidget::LogMessage(const QString &level, const QString &file, const int line, const QString &message, const QDateTime &timestamp) const {
    const QString shortFile = file.mid(file.lastIndexOf("/") + 1);
    const QString logMessage = timestamp.toString("yyyy-MM-dd HH:mm:ss") + " [" + level + "] " + shortFile + ":" + QString::number(line) + " " + message;
    const auto item = new QStandardItem(logMessage);
    if (level == "WARN") {
        item->setForeground(QBrush(Qt::darkYellow));
    } else if (level == "ERROR") {
        item->setForeground(QBrush(Qt::darkRed));
    } else if (level == "DEBUG") {
        item->setForeground(QBrush(Qt::darkGreen));
    }
    _logDataModel->insertRow(0, item);
}

void MainWidget::StartSecretsThread(const QString &awsAccount, const QString &nameSpace) {

    const int period = Configuration::instance().GetValue<int>("aws.secret-period");

    auto *thread = new QThread(this);
    auto *worker = new SessionWorker(nameSpace);
    worker->moveToThread(thread);

    // When thread starts, start the timer
    auto *timer = new QTimer;
    timer->setInterval(period * 1000);
    timer->moveToThread(thread);

    connect(thread, &QThread::started, timer, QOverload<>::of(&QTimer::start));
    connect(timer, &QTimer::timeout, worker, [worker]() {
        worker->DoWork("pim-prod", "pim-prod");
    });
    if (nameSpace == "pim-prod") {
        connect(_ui->prodRefreshButton, &QPushButton::clicked, worker, [worker,awsAccount, nameSpace]() {
            worker->DoWork(awsAccount, nameSpace);
        });
    } else if (nameSpace == "pim-int") {
        connect(_ui->intRefreshButton, &QPushButton::clicked, worker, [worker,awsAccount, nameSpace]() {
            worker->DoWork(awsAccount, nameSpace);
        });
    } else if (nameSpace == "pim-dev") {
        connect(_ui->devRefreshButton, &QPushButton::clicked, worker, [worker,awsAccount, nameSpace]() {
            worker->DoWork(awsAccount, nameSpace);
        });
    }

    // Cleanup
    connect(thread, &QThread::finished, timer, &QObject::deleteLater);
    connect(thread, &QThread::finished, worker, &QObject::deleteLater);

    thread->start();
    _threads.append(thread);
}

void MainWidget::ShowContextMenu(const QPoint &pos) const {

    const QString nameSpace = GetNamespaceFromTab();

    // Cell index
    QString name;
    QModelIndex index;
    if (nameSpace == "pim-prod") {
        index = _ui->prodTableView->indexAt(pos);
        name = _prodDataModel->item(index.row(), 0)->text();
    } else if (nameSpace == "pim-int") {
        index = _ui->intTableView->indexAt(pos);
        name = _intDataModel->item(index.row(), 0)->text();
    } else if (nameSpace == "pim-dev") {
        index = _ui->devTableView->indexAt(pos);
        name = _devDataModel->item(index.row(), 0)->text();
    }
    if (!index.isValid()) return;

    QMenu menu;
    QAction *startSQLWAction = menu.addAction(IconUtils::GetIcon("connect"), "Start SQL Workbench");
    startSQLWAction->setToolTip("Start the SQL Workbench.");

    QAction *editAction = menu.addAction(IconUtils::GetIcon("edit"), "Edit Service");
    editAction->setToolTip("Edit the forwarder service.");

    if (const QAction *selectedAction = menu.exec(_ui->prodTableView->viewport()->mapToGlobal(pos)); selectedAction == startSQLWAction) {
        const auto systemUtils = new SystemUtils();
        if (const ForwarderConfig forwarderConfig = Configuration::instance().GetForwarderConfigsByService(nameSpace, name); forwarderConfig.type == "postgres") {
            systemUtils->RunCommandAsync(nameSpace, "sqlworkbench", {"postgres@PROD", forwarderConfig.workbenchName});
        }
    } else if (selectedAction == editAction) {
        ServiceDetailsDialog dialog(nameSpace, name);
        dialog.exec();
    }
}
