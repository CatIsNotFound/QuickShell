#include "settings.h"

Settings::Settings() {
    path = "";
    yourSettings = nullptr;
}

Settings::Settings(QString loadSettingsFromFile) {
    path = loadSettingsFromFile;
    if (path.isEmpty()) {
        qDebug() << "Warning: Nothing config file to load. It will open config.conf file.";
        path = "config.conf";
        yourSettings = new QSettings(path, QSettings::IniFormat);
    } else {
        yourSettings = new QSettings(path, QSettings::IniFormat);
    }
}

Settings::~Settings() {
    if (yourSettings != nullptr) {    
        delete yourSettings;
        yourSettings = nullptr;
    }
}

QString Settings::getValue(QString group, QString key) {
    QString getKey = group;
    getKey.append('/');
    getKey.push_back(key);
    return yourSettings->value(getKey).toString();
}

void Settings::setValue(QString group, QString key, QString value) {
    QString modifyKey = group;
    modifyKey.append('/');
    modifyKey.push_back(key);
    yourSettings->setValue(modifyKey, value);
}

void Settings::initAllSettings() {
    yourSettings->beginGroup("Collection");
    yourSettings->setValue("name", "Default");
    yourSettings->setValue("open_as", "Default");
    yourSettings->setValue("recently", "");
    yourSettings->endGroup();
    yourSettings->beginGroup("Config");
    yourSettings->setValue("version", APP_VERSION);
    yourSettings->setValue("version_type", APP_VERSION_TYPE);
    yourSettings->endGroup();
    yourSettings->beginGroup("Interface");
    yourSettings->setValue("language", "System");
    yourSettings->setValue("theme", "System");
    yourSettings->setValue("window", "System");
    yourSettings->endGroup();
    yourSettings->sync();
}

void Settings::saveAllSettings() {
    yourSettings->sync();
}

int Settings::loadConfigFile(QString configFile, bool fromCommand) {
    if (configFile.isEmpty()) {
        qDebug() << "Error: Can not load file! the file name has not valid."
                 << "It will be opened config.conf.";
        configFile = "config.conf";
    }

    QFileInfo isExist(configFile);
    if (isExist.isDir()) {
        qDebug() << "Error: File" << configFile
                 << "is a directory, not a config file!";
        return 1;
    }

    if (yourSettings == nullptr) {
        yourSettings = new QSettings(configFile, QSettings::IniFormat);
    } else {
        yourSettings->setPath(QSettings::IniFormat, QSettings::UserScope, configFile);
    }

    if (!isExist.isFile()) {
        if (fromCommand) {
            qDebug() << "Error: File" << configFile
                     << "has not exist!";
            return 1;
        } else {
            qDebug() << "Error: File" << configFile
                     << "has not exist! It will be create a new config file.";
            initAllSettings();
        }
    }

    bool is_exist = false;
    for (auto i : yourSettings->allKeys()) {
        if (!QString::compare(i, "Config/version")) {
            is_exist = true;
            break;
        }
    }

    if (!is_exist && !fromCommand) {
        qDebug() << "Warning:" << configFile
                 << "is not valid."
                 << "It will be set as default settings and override.";
        initAllSettings();
    }

    if (!is_exist && fromCommand) {
        qDebug() << "Error: File" << configFile
                 << "is not a valid config file!";
        return 1;
    }

    qDebug() << "Info:" << configFile
             << "is loaded successfully!";
    path = configFile;
    return 0;
}

QString Settings::getConfigPath() {
    return path;
}
