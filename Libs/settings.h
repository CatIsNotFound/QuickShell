#ifndef SETTINGS_H
#define SETTINGS_H
#include <QSettings>
#include <QFileInfo>
#include <QString>

#include "../Libs/Version.h"

class Settings
{
public:
    Settings();
    Settings(QString loadSettingsFromFile);
    ~Settings();
    QString getValue(QString group, QString key);
    void setValue(QString group, QString key, QString value);
    void initAllSettings();
    void saveAllSettings();
    int loadConfigFile(QString configFile, bool fromCommand = false);
    QString getConfigPath();
    
private:
    QSettings *yourSettings;
    QString path;
};

#endif // SETTINGS_H
