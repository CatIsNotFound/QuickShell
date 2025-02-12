#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{   
    // Setup
    collection = nullptr;
    settings = nullptr;
    ui->setupUi(this);
    // Run button
    connect(ui->btnRunScript, &QPushButton::clicked, this, [=]{openTerminal(true);});
    // Menu Bar
    // - File
    connect(ui->action_Import, &QAction::triggered, this, &MainWindow::importScriptCollection);
    connect(ui->action_Options, &QAction::triggered, this, &MainWindow::showPreference);
    connect(ui->action_Exit, &QAction::triggered, this, &MainWindow::close);
    // - View
    connect(ui->action_Refresh, &QAction::triggered, this, &MainWindow::refresh);
    connect(ui->action_ReloadCollection, &QAction::triggered, this, [=] {
        if (!loadScriptCollection(curruntPath)) refresh();
    });
    // - Help
    connect(ui->action_README, &QAction::triggered, this, [=] {
        QDesktopServices::openUrl(QUrl(REMOTE_REPO));
    });
    connect(ui->action_About, &QAction::triggered, this, &MainWindow::showVersion);
    connect(ui->action_Qt, &QAction::triggered, this, &QApplication::aboutQt);

    // List
    // - Set events
    connect(ui->actionRunSelectedScript, &QAction::triggered, this, [=]{openTerminal(true);});
    connect(ui->action_RunTheSelectedScriptsNotIncludeArguments, &QAction::triggered, this, &MainWindow::openTerminal);
    connect(ui->listScripts, &QListWidget::itemDoubleClicked, this, [=]{openTerminal(true);});
    connect(ui->action_EditScriptFile, &QAction::triggered, this, [=] {
        int k = ui->listScripts->currentRow();
        QString s_name = ui->listScripts->item(k)->whatsThis();
        QString s_group = collection->getGroup(s_name);
        QString s_path = collection->getPath(s_group);
        QString script_name = s_path + "/" + collection->getPath(s_name);
        QFileInfo isExist(script_name);
        if (isExist.isFile()) {
            QDesktopServices::openUrl(QUrl::fromLocalFile(script_name));
        } else {
            QMessageBox::critical(this, tr("Error"), tr("Can not open script file: ") + script_name);
        }
    });
    // - Add PopupMenu
    connect(ui->listScripts, &QListWidget::customContextMenuRequested, this, [=]{
        if (ui->listScripts->currentRow() > -1) {
            QMenu popupMenu;
            popupMenu.addAction(ui->actionRunSelectedScript);
            popupMenu.addAction(ui->action_RunTheSelectedScriptsNotIncludeArguments);
            popupMenu.addAction(ui->action_EditScriptFile);
            popupMenu.exec(QCursor::pos());
        }
    });
    // - Connect events
    connect(ui->listCategory, &QListWidget::currentRowChanged, this, [=] {
        if (ui->listCategory->currentRow() < 0) {
            return;
        }
        QString group = ui->listCategory->item(ui->listCategory->currentRow())->whatsThis();
        if (collection->findGroup(group)) {
            ui->listScripts->clear();
            QStringList items = collection->getItems(group);
            int k = 0;
            for (auto i : items) {
                ui->listScripts->addItem(collection->getFullName(i));
                ui->listScripts->item(k)->setWhatsThis(i);
                // QIcon s_icon(QIcon::fromTheme(QIcon::ThemeIcon::ViewRefresh));
                QIcon s_icon(":/images/logo.png");
                ui->listScripts->item(k++)->setIcon(s_icon);
            }
            // ui->listScripts->addItems(collection->getItems(group));
        }
    });
    connect(ui->listScripts, &QListWidget::currentRowChanged, this, [=] {
        if (ui->listScripts->currentRow() < 0) {
            ui->btnRunScript->setEnabled(false);
            return;
        }
        QString item = ui->listScripts->item(ui->listScripts->currentRow())->whatsThis();
        QString group = ui->listCategory->item(ui->listCategory->currentRow())->whatsThis();
        QString fileName = collection->getPath(group) + "/" + collection->getDescriptionFile(item, group);
        if (fileName.isEmpty()) {
            ui->plainTextDetails->clear();
            ui->plainTextDetails->setPlainText(QObject::tr("Unable to display script detailed information."));
            return;
        }
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream text(&file);
            ui->plainTextDetails->clear();
            // ui->plainTextDetails->setPlainText(text.readAll());
            ui->plainTextDetails->setMarkdown(text.readAll());
            file.close();
        } else {
            ui->plainTextDetails->clear();
            ui->plainTextDetails->setPlainText(QObject::tr("Unable to display script detailed information."));
        }
        ui->btnRunScript->setEnabled(true);
    });

}

MainWindow::~MainWindow()
{
    if (collection != nullptr) {
        delete collection;
        collection = nullptr;
    }
    if (settings != nullptr) {
        delete settings;
        settings = nullptr;
    }
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *e) {
    ui->horizontalLayoutWidget->setGeometry(QRect(0, 0, ui->centralwidget->geometry().width(), ui->centralwidget->geometry().height()));
}

void MainWindow::showPreference() {
    Preference *preference = new Preference(this);
    connect(preference, &Preference::scriptsCollectionChanged, this, &MainWindow::getNewScriptCollection);
    preference->init(settings);
    preference->exec();
}

void MainWindow::showVersion() {
    AboutApp about(this);
    about.exec();
}

void MainWindow::initConfig(QString fileName, Settings &import) {
    collection = new ScriptsCollection();
    settings = new Settings(import.getConfigPath());
    curruntPath = fileName;
    int ret = collection->loadCollection(fileName);
    switch (ret) {
    case 1:
        qDebug() << "Error: " << fileName << "is a directory, not a file!";
        QMessageBox::critical(this, tr("Load failed"),
            tr("Error: ") + fileName + tr(" is a directory, not a file!"));
        break;
    case 2:
        qDebug() << "Error: File" << fileName << "is not exist! It will be created.";
        QMessageBox::critical(this, tr("Load failed"),
                              tr("Error: File ") + fileName +
                              tr(" is not exist! It will be created."));
        collection->createCollection("collection", "Script Collection Example");
        collection->saveCollection();
        break;
    case 3:
        qDebug() << "Error: File" << fileName << "is not a valid script collection file.";
        QMessageBox::critical(this, tr("Load failed"), tr("Error: File ") + fileName + tr(" is not a valid script collection file."));
        break;
    }
    if (ret == 0 || ret == 2) {
        refresh();
    }
}

void MainWindow::refresh() {
    QStringList groups = collection->getGroups();
    ui->listCategory->clear();
    ui->listScripts->clear();
    ui->plainTextDetails->setPlainText(tr("Please select any script to display detailed information."));
    int k = 0;
    for (auto i : groups) {
        ui->listCategory->addItem(collection->getFullName(i));
        // ui->listCategory[k++].setObjectName(i);
        // ui->listCategory->item(k)->setData(1, i);
        QIcon t(":/images/assets/icons/folder.svg");
        ui->listCategory->item(k)->setIcon(t);
        ui->listCategory->item(k++)->setWhatsThis(i);
    }
    ui->listCategory->setCurrentRow(0);
}

int MainWindow::loadScriptCollection(QString path) {
    if (!path.isEmpty()) {
        int ret = collection->loadCollection(path);
        switch (ret) {
        case 0:
            curruntPath = path;
            refresh();
            return 0;
            break;
        case 1:
            qDebug() << "Error: " << path << "is a directory, not a file!";
            QMessageBox::critical(this, tr("Load failed"),
                                  tr("Error: ") + path + tr(" is a directory, not a file!"));
            break;
        case 3:
            qDebug() << "Error: File" << path << "is not a valid script collection file.";
            QMessageBox::critical(this, tr("Load failed"), tr("Error: File ") + path + tr(" is not a valid script collection file."));
            break;
        }
    }
    refresh();
    // qDebug() << "Error: path is not valid!";
    return 1;
}

void MainWindow::importScriptCollection() {
    QString path = QFileDialog::getOpenFileName(this,
                                                tr("Import Script Collection File"), ".",
                                                tr("Script Collection Files (*.conf)"));
    if (!loadScriptCollection(path)) {
        refresh();
    }
}

void MainWindow::getNewScriptCollection(const QString &path) {
    qDebug() << "Info:" << path << "has loaded.";
    curruntPath = path;
    if (!loadScriptCollection(curruntPath)) refresh();
}

void MainWindow::openTerminal(bool include_args) {
    QDir d("..");
    TermsList t(d.absolutePath() + "/conf/terminal.inf");
    QString term_app = settings->getValue("Collection", "open_as");
    int k = ui->listScripts->currentRow();
    QString s_name = ui->listScripts->item(k)->whatsThis();
    QString s_group = collection->getGroup(s_name);
    QString s_path = collection->getPath(s_group);
    QString script_name = s_path + "/" + collection->getPath(s_name);
    // for (auto i : t.getList()) {
    //     qDebug() << i;
    // }
    QStringList terms = t.getList(true);
    if (terms.isEmpty()) {
        QMessageBox::critical(this, tr("Error"), tr("No available terminals found!"));
        return;
    }
    QString default_term = terms.at(0);
    int ret;
    QString t_args = "";
    if (include_args) {
        t_args = ui->lineEdit_ExecutionArguments->text();
    }
    qDebug() << "Get Args:" << t_args;
    if (comparesEqual(term_app, "Default")) {
        ret = t.runCommand(default_term, script_name, t_args);
    } else {
        ret = t.runCommand(term_app, script_name, t_args);
    }
    switch (ret) {
    case 1:
    case 2:
        QMessageBox::critical(this, tr("Error"), tr("Unable to start terminal: ") + default_term);
        break;
    case 3:
        QMessageBox::critical(this, tr("Error"), tr("Can not open script file: ") + script_name);
        break;
    case 0:
        break;
    }
}
