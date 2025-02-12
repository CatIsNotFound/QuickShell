#ifndef PREFERENCE_H
#define PREFERENCE_H

#include <QDialog>
#include <QLocale>
#include <QDir>
#include <QStyleFactory>
#include <QFileDialog>
#include <QMessageBox>
#include "../Libs/settings.h"
#include "mainwindow.h"
#include "findterminal.h"

namespace Ui {
class Preference;
}

class Preference : public QDialog
{
    Q_OBJECT

public:
    explicit Preference(QWidget *parent = nullptr);
    ~Preference();
    void applyAllSettings();
    void init(Settings *config);
    void getAllSettings();
    void getTerminal(const QString &index);

signals:
    void scriptsCollectionChanged(const QString &path);


private:
    Ui::Preference *ui;
    Settings *settings;
};

#endif // PREFERENCE_H
