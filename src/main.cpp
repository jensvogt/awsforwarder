#include <QApplication>
#include <QScreen>
#include <QPushButton>
#include <QStyleFactory>
#include <QCommandLineParser>

// AWS SDK
#include <aws/core/Aws.h>

// Forwarder includes
#include <MainWidget.h>
#include <ui_MainWidget.h>
#include <utils/IconUtils.h>
#include <secrets/SecretsWorker.h>

#include "MainWindow.h"

int main(int argc, char *argv[]) {

    // Initialize application
    const QApplication app(argc, argv);

    // Set icon
    QApplication::setWindowIcon(IconUtils::GetCommonIcon("awsmock"));

    // Command line options
    QCommandLineParser parser;
    parser.setApplicationDescription("AWS Forwarder");
    parser.addHelpOption();
    parser.addVersionOption();
    const QCommandLineOption configOption(QStringList() << "c" << "config", "Path to config file.", "file");
    parser.addOption(configOption);

    // Process the actual command-line arguments
    parser.process(app);

    // Retrieve values
    if (const QString configPath = parser.value(configOption); !configPath.isEmpty()) {
        Configuration::instance().SetFilePath(configPath);
    } else {
        Configuration::instance().SetFilePath(DEFAULT_CONFIGURATION_FILE_PATH);
    }

    // Set style
    const auto style = Configuration::instance().GetValue<QString>("style", "");
    const auto styleType = Configuration::instance().GetValue<QString>("style-type", "");
    qApp->setStyle(style);
    if (!styleType.isEmpty() && styleType == "dark") {
        if (QFile f(":/styles/styles/dark.qss"); f.open(QFile::ReadOnly)) {
            qApp->setStyleSheet(f.readAll());
        }
    } else {
        if (QFile f(":/styles/styles/light.qss"); f.open(QFile::ReadOnly)) {
            qApp->setStyleSheet(f.readAll());
        }
    }

    const Aws::SDKOptions options;
    InitAPI(options);

    MainWindow w;
    constexpr int screenIndex = 0;
    if (const QList<QScreen *> screens = QGuiApplication::screens(); screenIndex < screens.count()) {
        const QScreen *targetScreen = screens.at(screenIndex);
        const QRect screenGeometry = targetScreen->geometry();

        // Center the window on that screen in global coordinates
        const int x = screenGeometry.x() + (screenGeometry.width() - w.width()) / 2;
        const int y = screenGeometry.y() + (screenGeometry.height() - w.height()) / 2;

        w.move(x, y);
    }
    w.resize(2000, 1000);
    w.show();
    return QApplication::exec();
}
