//
// Created by vogje01 on 1/24/26.
//

#include <MainWindow.h>
#include <QSpacerItem>

MainWindow::MainWindow() {

    // Setup menu bar and toolbar
    SetupMenuBar();
    SetupToolBar();

    const auto widget = new MainWidget(this);
    setCentralWidget(widget);
}

void MainWindow::SetupMenuBar() {
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addSeparator();

    const auto exitAction = new QAction(IconUtils::GetIcon("exit"), tr("E&xit"), this);
    connect(exitAction, &QAction::triggered, this, &MainWindow::Exit);
    fileMenu->addAction(exitAction);
}

void MainWindow::SetupToolBar() {
    const auto exitAction = new QAction(IconUtils::GetIcon("exit"), tr("E&xit"), this);
    connect(exitAction, &QAction::triggered, this, &MainWindow::Exit);

    // Create/Get the Toolbar and add the action
    QToolBar *mainToolBar = addToolBar("Main Toolbar");
    addAction(exitAction);

    const auto spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    mainToolBar->addWidget(spacer);
    mainToolBar->addAction(exitAction);
}

void MainWindow::Exit() {
    QApplication::quit();
}
