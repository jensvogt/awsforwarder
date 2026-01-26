//
// Created by vogje01 on 1/24/26.
//

#ifndef AWSFORWARDER_MAIN_WINDOW_H
#define AWSFORWARDER_MAIN_WINDOW_H

// Qt includes
#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>

// Forwarder includes
#include <MainWidget.h>
#include <utils/IconUtils.h>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow();

private:
    void SetupMenuBar();

    void SetupToolBar();

    static void Exit();
};

#endif //AWSFORWARDER_MAIN_WINDOW_H
