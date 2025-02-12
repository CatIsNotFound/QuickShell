#include "termslist.h"

TermsList::TermsList() {
    termsList = nullptr;
}

TermsList::~TermsList() {
    if (termsList != nullptr) {
        delete termsList;
    }
}

TermsList::TermsList(QString configPath) {
    termsList = new QSettings(configPath, QSettings::IniFormat);
    this->configPath = configPath;
}

QStringList TermsList::getList(bool find_available) {
    if (find_available) {
        QStringList available_terms;
        for (auto i : termsList->childGroups()) {
            QFileInfo isExist(termsList->value(i + "/path").toString());
            if (isExist.isFile()) {
                available_terms.push_back(i);
            }
        }
        return available_terms;
    }
    return termsList->childGroups();
}

QString TermsList::getConfigPath() {
    return configPath;
}

int TermsList::runCommand(QString name, QString scriptPath, QString args) {
    // Is terminal can not open?
    QString path = termsList->value(name + "/path").toString();
    QFileInfo is_terminal_not_found(path);
    if (path.isEmpty()) {
        return 1;
    } else if (!is_terminal_not_found.isFile()) {
        return 2;
    }
    // Is script path is not exist?
    QFileInfo isExist(scriptPath);
    if (!isExist.isFile()) {
        return 3;
    }

    // Is arguments valid?
    QStringList arguments;

    arguments << termsList->value(name + "/options").toString() << scriptPath;
    if (!args.isEmpty()) {
        arguments.push_back(args);
    }

    // Debug Command Line
    QString t_a = "";
    for (auto i : arguments) {
        t_a.append(i);
        t_a.append(' ');
    }
    qDebug() << "Running Command Line:" << path << t_a;

    // Start Running
    QProcess *tmp_process = new QProcess();
    tmp_process->startDetached(path, arguments);
    tmp_process->waitForStarted();
    int err = tmp_process->exitCode();
    tmp_process->close();
    // qDebug() << "Error code:" << err;
    return err;
}
