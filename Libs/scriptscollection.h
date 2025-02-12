#ifndef SCRIPTSCOLLECTION_H
#define SCRIPTSCOLLECTION_H

#include <QSettings>
#include <QFileInfo>
#include <QDir>
#include <QString>
#include <QObject>
#include <QIcon>

class ScriptsCollection
{
public:
    ScriptsCollection();
    ScriptsCollection(QString fileName);
    ~ScriptsCollection();
    int loadCollection(QString fileName);
    void saveCollection();
    QStringList getGroups();
    QStringList getItems(QString group);
    bool findGroup(QString group);
    bool findItem(QString group, QString item);
    bool findGroupOrItem(QString key);
    void setValue(QString key, QString value);
    QString getValue(QString key);
    void createCollection(QString id, QString name);
    void createGroup(QString name, QString fullName, QString path);
    void addItem(QString name, QString group, QString fullName, QString path, QString descriptionFile = "");
    bool removeItem(QString name, QString group);
    QString getGroup(QString name);
    void setGroup(QString name, QString new_group);
    void setFullName(QString key, QString fullName);
    QString getFullName(QString key);
    void setPath(QString key, QString path);
    QString getPath(QString key);
    int setIcon(QString group, QString iconPath);
    QString getIcon(QString group);
    void setNeedArgs(QString item, QString group, bool is_need_args = false);
    bool getNeedArgs(QString item, QString group);
    void setArgs(QString item, QString group, QString args = "");
    QString getArgs(QString item, QString group);
    int setDescriptionFile(QString item, QString group, QString fileName);
    QString getDescriptionFile(QString item, QString group);

private:
    QSettings *collection;
    QString path;
};

#endif // SCRIPTSCOLLECTION_H
