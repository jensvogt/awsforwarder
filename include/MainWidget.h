//
// Created by vogje01 on 1/18/26.
//

#ifndef AWSFORWARDER_MAIN_WIDGET_H
#define AWSFORWARDER_MAIN_WIDGET_H
// Qt includes
#include <QMainWindow>
#include <QMenuBar>
#include <QStandardItemModel>

// Forwarder includes
#include <QTableView>

// Forwarder includes
#include <ServiceDetailsDialog.h>
#include <utils/Configuration.h>
#include <utils/AwsUtils.h>
#include <utils/EventBus.h>
#include <utils/IconUtils.h>
#include <kubernetes/KubernetesWorker.h>
#include <session/SessionWorker.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class MainWidget;
}

QT_END_NAMESPACE

class MainWidget : public QWidget {
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);

    ~MainWidget() override;

    void SetupDevTable();

    void SetupIntTable();

    void SetupProdTable();

    QAction *GetRefreshAction(QWidget *parent) const;

public
slots:
    virtual void LoadContent();

    void LogMessage(const QString &level, const QString &file, int line, const QString &message, const QDateTime &timestamp) const;

    void StartSecretsThread(const QString &awsAccount, const QString &nameSpace);

    void ShowContextMenu(const QPoint &pos) const;

private:
    static void SetColumn(QStandardItemModel *dataModel, int row, int col, const QString &value, Qt::Alignment alignment = Qt::AlignLeft | Qt::AlignVCenter);

    [[nodiscard]] QString GetNamespaceFromTab() const;

    /**
     * @brief UI components
     */
    Ui::MainWidget *_ui;

    /**
     * @brief Sort column index
     */
    int _sortColumn = 0;

    /**
     * @brief Sort order
     */
    Qt::SortOrder _sortOrder = Qt::AscendingOrder;

    /**
     * @brief Forwarder table view
     */
    QTableView *_devTableView{};

    /**
     * @brief Forwarder data model
     */
    QStandardItemModel *_devDataModel{};

    /**
     * @brief Forwarder table view
     */
    QTableView *_intTableView{};

    /**
     * @brief Forwarder data model
     */
    QStandardItemModel *_intDataModel{};

    /**
     * @brief Forwarder table view
     */
    QTableView *_prodTableView{};

    /**
     * @brief Forwarder data model
     */
    QStandardItemModel *_prodDataModel{};

    /**
     * @brief Logging data model
     */
    QStandardItemModel *_logDataModel{};

    /**
     * @brief Worker threads
     */
    QVector<QThread *> _threads;
};


#endif //AWSFORWARDER_MAIN_WIDGET_H
