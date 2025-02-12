#ifndef TERMSLIST_H
#define TERMSLIST_H

#include <QSettings>
#include <QProcess>
#include <QFileInfo>

class TermsList
{
public:
    TermsList();
    TermsList(QString configPath);
    ~TermsList();
    QStringList getList(bool find_available = false);
    QString getConfigPath();
    int runCommand(QString name, QString scriptPath, QString args);

private:
    QString configPath;
    QSettings *termsList;
};

#endif // TERMSLIST_H
