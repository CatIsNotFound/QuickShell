#include "GUI/mainwindow.h"
#include "Libs/settings.h"
#include "Libs/Version.h"
#include <QApplication>
#include <QLocale>
#include <QStyleFactory>
#include <QFile>
#include <QTranslator>
#include <QMessageBox>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString tmp = "";
    #define arg a.arguments()
    if (a.arguments().size() > 1) {
        if (arg.at(1) == "--help") {
            qDebug() << "Command Help:";
            qDebug() << "Usage: [<OPTIONS> <VALUE>]";
            qDebug() << "--config <File>     Use <File> as config file";
            qDebug() << "--help              Display help info  ";
            qDebug() << "--version           Display current version";
            return 0;
        } else if (arg.at(1) == "--version") {
            qDebug() << APP_NAME << "version" << APP_VERSION << "-" << APP_VERSION_TYPE;
            qDebug() << "Author:" << APP_AUTHOR;
            qDebug() << "Repo:" << REMOTE_REPO;
            return 0;
        } else if (arg.at(1) == "--config") {
            if (arg.size() < 3) {
                qDebug() << "Error: Please input the config file path when use \'--config\' option.";
                return 1;
            } else {
                tmp = arg.at(2);
                qDebug() << "Try to Load file:" << tmp;
            }
        } else {
            qDebug() << "Error: Unknown option! Try \'--help\' option to see usage.";
            return 1;
        }
    }
    Settings *settings;
    if (!tmp.isEmpty()) {
        settings = new Settings(tmp);
    } else {
        settings = new Settings("conf/config.conf");
        settings->loadConfigFile("conf/config.conf");
    }
    if (!tmp.isEmpty() && settings->loadConfigFile(tmp, true)) {
        return 1;
    }

    QTranslator translator;
    QString language = settings->getValue("Interface", "language");

    if (!QString::compare(language, "System")) {
        // System Auto select language
        const QStringList uiLanguages = QLocale::system().uiLanguages();
        for (const QString &locale : uiLanguages) {
            const QString baseName = "QuickShell_" + QLocale(locale).name();
            if (translator.load(":/i18n/" + baseName)) {
                a.installTranslator(&translator);
                break;
            }
        }
    } else {
        if (translator.load(":/i18n/QuickShell_" + language)) {
            a.installTranslator(&translator);
        } else {
            qDebug() << "Error: Unable to load unsupported language! Please try to change the interface language in [Preference].";
            // QMessageBox::critical(nullptr, QObject::tr("Error"), "Something has problem while the programme is running.\n" +
            //             "- Unable to load language because this interface language is not supported.\n" +
            //             "  Maybe you can try changing the interface language in [Preference].");
            settings->setValue("Interface", "language", "System");
            settings->saveAllSettings();
        }
    }
    QString theme = settings->getValue("Interface", "theme");
    if (QString::compare(theme, "System")) {
        QFile qssFile(theme);
        if (qssFile.open(QFile::ReadOnly)) {
            a.setStyleSheet(qssFile.readAll());
            qssFile.close();
        } else {
            qDebug() << "Error: Can not load theme from " + theme;
            QMessageBox::critical(nullptr, QObject::tr("Error"), QObject::tr("Can not load theme. Because File ") + theme +
                                                            QObject::tr(" is not found!"));
            settings->setValue("Interface", "theme", "System");
            settings->saveAllSettings();
        }
    }

    QString window = settings->getValue("Interface", "window");
    if (!window.isEmpty() && QString::compare(window, "System")) {
        a.setStyle(QStyleFactory::create(window));
    }
    MainWindow mainWindow;
    mainWindow.show();
    QDir cDir(".");
    QString collection_path = settings->getValue("Collection", "name");
    if (!QString::compare(collection_path, "Default")) {
        collection_path = cDir.absolutePath() + "/scripts/Collection.conf";
    }
    mainWindow.initConfig(collection_path, *settings);
    delete settings;
    return a.exec();
}
