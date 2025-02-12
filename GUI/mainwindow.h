#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QDesktopServices>
#include "preference.h"
#include "aboutapp.h"
#include "findterminal.h"
#include "../Libs/scriptscollection.h"
#include "../Libs/settings.h"
#include "../Libs/Version.h"
#include "../Libs/termslist.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void resizeEvent(QResizeEvent *e) override;
    void showPreference();
    void showVersion();
    void initConfig(QString fileName, Settings &import);
    void refresh();
    int loadScriptCollection(QString path);
    void importScriptCollection();
    void openTerminal(bool include_args = false);

private slots:
    void getNewScriptCollection(const QString &path);

private:
    Ui::MainWindow *ui;
    ScriptsCollection *collection;
    Settings *settings;
    QString curruntPath;
};
#endif // MAINWINDOW_H
