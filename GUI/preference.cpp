#include "preference.h"
#include "ui_preference.h"
#include "../Libs/scriptscollection.h"

Preference::Preference(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Preference)
{
    ui->setupUi(this);
    // this->setWindowFlags(Qt::WindowCloseButtonHint);
    connect(ui->optionBox, &QDialogButtonBox::clicked, this, [=](QAbstractButton *btn) {
        QDialogButtonBox::StandardButton std_btn = ui->optionBox->standardButton(btn);
        switch (std_btn) {
        case QDialogButtonBox::Ok:
            applyAllSettings();
            this->close();
            break;
        case QDialogButtonBox::Cancel:
            this->close();
            break;
        case QDialogButtonBox::Apply:
            applyAllSettings();
            break;
        default:
            break;
        }
    });

    connect(ui->pushButton_ScriptBrowser, &QPushButton::clicked, this, [=] {
        QString path = QFileDialog::getOpenFileName(this,
                                                    tr("Open Script Collection File"), ".",
                                                    tr("Script Collection Files (*.conf)"));
        if (!path.isEmpty()) {
            ui->comboBox_ScriptPath->addItem(path);
            ui->comboBox_ScriptPath->setCurrentIndex(ui->comboBox_ScriptPath->count() - 1);
        }
    });

    connect(ui->comboBox_ScriptPath, &QComboBox::currentIndexChanged, this, [=]{
        QString path = ui->comboBox_ScriptPath->currentText();
        if (ui->comboBox_ScriptPath->currentIndex() == 0) {
            ui->label_valid->setText("");
            return;
        }
        ScriptsCollection sc;
        int n = sc.loadCollection(path);
        // qDebug() << "Error Code:" << n;
        if (!n) {
            // qDebug() << "is valid!";
            ui->label_valid->setStyleSheet("color: rgb(16, 128, 16);");
            ui->label_valid->setText(tr("Script Name: ") + sc.getValue("Collection/name"));
        } else {
            // qDebug() << "is not valid!";
            ui->label_valid->setStyleSheet("color: rgb(192, 16, 16);");
            ui->label_valid->setText(tr("Current file is not valid!"));

        }
    });

    connect(ui->pushButton_AppBrowser, &QPushButton::clicked, this, [=] {
        FindTerminal *win = new FindTerminal(this);
        connect(win, &FindTerminal::getTerminal, this, &Preference::getTerminal);
        win->exec();
    });

    connect(ui->comboBox_InterfaceThemes, &QComboBox::currentIndexChanged, this, [=] {
        if (ui->comboBox_InterfaceThemes->currentIndex() == 1) {
            QString path = QFileDialog::getOpenFileName(this,
                                                        tr("Open Theme File"), "../themes",
                                                        tr("Theme Files (*.qss)"));
            if (!path.isEmpty()) {
                ui->comboBox_InterfaceThemes->addItem(path);
                ui->comboBox_InterfaceThemes->setCurrentIndex(ui->comboBox_InterfaceThemes->count() - 1);
            } else {
                ui->comboBox_InterfaceThemes->setCurrentIndex(0);
            }
        }
    });

    // add items to listWidgets
    // Get supported languages
    QDir dir(":i18n/");
    QStringList languageList, supportedLang, fileName = dir.entryList(languageList, QDir::Files);
    languageList << "*.qm";
    for (auto i : fileName) {
        QString code = i.split(".")[0].section("_", 1, 2);
        QLocale locale(code);
        QString tmp = locale.nativeLanguageName() + "(" + locale.name() + ")";
        supportedLang.push_back(tmp);
    }
    ui->comboBox_InterfaceLanguage->addItems(supportedLang);
    // Get Window widgets
    ui->comboBox_InterfaceWindow->addItems(QStyleFactory::keys());
}

Preference::~Preference()
{
    delete ui;
}

void Preference::applyAllSettings() {
    // General
    QString value = ui->comboBox_ScriptPath->currentText();
    QString path = value;
    int index = ui->comboBox_ScriptPath->currentIndex();
    if (!index) {
        QDir d(".");
        path = d.absolutePath() + "/Collection.conf";
        settings->setValue("Collection", "name", "Default");
    } else {
        settings->setValue("Collection", "name", value);
    }

    value = ui->comboBox_DefaultTerminalApp->currentText();
    index = ui->comboBox_DefaultTerminalApp->currentIndex();
    if (!index) {
        settings->setValue("Collection", "open_as", "Default");
    } else {
        settings->setValue("Collection", "open_as", value);
    }

    // Interface
    value = ui->comboBox_InterfaceLanguage->currentText().section("(", 1).split(")")[0];
    index = ui->comboBox_InterfaceLanguage->currentIndex();
    if (!index) {
        settings->setValue("Interface", "language", "System");
    } else {
        settings->setValue("Interface", "language", value);
    }

    value = ui->comboBox_InterfaceThemes->currentText();
    index = ui->comboBox_InterfaceThemes->currentIndex();
    if (!index) {
        settings->setValue("Interface", "theme", "System");
    } else {
        settings->setValue("Interface", "theme", value);
    }

    value = ui->comboBox_InterfaceWindow->currentText();
    index = ui->comboBox_InterfaceWindow->currentIndex();
    if (!index) {
        settings->setValue("Interface", "window", "System");
    } else {
        settings->setValue("Interface", "window", value);
    }

    settings->saveAllSettings();
    QMessageBox::information(this, tr("Success"),
                             tr("All settings have been saved! Some settings need to take effect on the next startup."),
                             QMessageBox::Ok);
    emit scriptsCollectionChanged(path);
}

void Preference::init(Settings *config) {
    settings = config;
    getAllSettings();
}

void Preference::getAllSettings() {
    // General
    QString name = settings->getValue("Collection", "name");
    if (QString::compare(name, "Default")) {
        ui->comboBox_ScriptPath->addItem(name);
        ui->comboBox_ScriptPath->setCurrentIndex(1);
        ScriptsCollection sc;
        int n = sc.loadCollection(name);
        if (!n) {
            // qDebug() << "is valid!";
            ui->label_valid->setStyleSheet("color: rgb(16, 128, 16);");
            ui->label_valid->setText("Collection Name: " + sc.getValue("Collection/name"));
        } else {
            // qDebug() << "is not valid!";
            ui->label_valid->setStyleSheet("color: rgb(192, 16, 16);");
            ui->label_valid->setText(tr("Current file is not valid!"));
        }
    }
    name = settings->getValue("Collection", "open_as");
    if (QString::compare(name, "Default")) {
        ui->comboBox_DefaultTerminalApp->addItem(name);
        ui->comboBox_DefaultTerminalApp->setCurrentIndex(1);
    }

    // Interface
    name = settings->getValue("Interface", "language");
    if (QString::compare(name, "System")) {
        for (int i = 1; i < ui->comboBox_InterfaceLanguage->count(); i++) {
            QString code = ui->comboBox_InterfaceLanguage->itemText(i).section("(", 1).split(")")[0];
            if (!QString::compare(name, code)) {
                ui->comboBox_InterfaceLanguage->setCurrentIndex(i);
                break;
            }
        }
    }

    name = settings->getValue("Interface", "theme");
    if (QString::compare(name, "System")) {
        ui->comboBox_InterfaceThemes->addItem(name);
        int k = ui->comboBox_InterfaceThemes->count() - 1;
        ui->comboBox_InterfaceThemes->setCurrentIndex(k);
    }

    name = settings->getValue("Interface", "window");
    if (QString::compare(name, "System")) {
        int k = ui->comboBox_InterfaceWindow->findText(name);
        ui->comboBox_InterfaceWindow->setCurrentIndex(k);
    }
}

void Preference::getTerminal(const QString &index) {
    int c = ui->comboBox_DefaultTerminalApp->count();
    if (c > 1) {
        for (int i = 0; i < c; i++) {
            if (!QString::compare(index, ui->comboBox_DefaultTerminalApp->itemText(i))) {
                ui->comboBox_DefaultTerminalApp->setCurrentIndex(i);
                return;
            }
        }
    }
    ui->comboBox_DefaultTerminalApp->addItem(index);
    ui->comboBox_DefaultTerminalApp->setCurrentIndex(c);
}
