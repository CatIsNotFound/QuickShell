#include "aboutapp.h"
#include "ui_aboutapp.h"

AboutApp::AboutApp(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AboutApp)
{
    ui->setupUi(this);
    this->setFixedSize(QSize(this->width(), this->height()));
    //this->setWindowFlags(Qt::WindowCloseButtonHint);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &AboutApp::close);
    connect(ui->pushButton_LookForUpdate, &QPushButton::clicked, this, [=] {
        QDesktopServices::openUrl(QUrl(RELEASE_URL));
    });
    connect(ui->pushButton_VisitRepo, &QPushButton::clicked, this, [=] {
        QDesktopServices::openUrl(QUrl(REMOTE_REPO));
    });
    ui->label_appName->setText(APP_NAME);
    QString s = "Version ";
    s += APP_VERSION; s += "-"; s += APP_VERSION_TYPE;
    ui->label_appVersion->setText(s);
    QStringList ls;
    ls << "Author: " << APP_AUTHOR <<
        "<br>Homepage: " << HOMEPAGE <<
        "<br>Reposity: " << REMOTE_REPO;
    QString info;
    for (auto i : ls) {
        info += i;
    }
    ui->textBrowser->setHtml(info);
}

AboutApp::~AboutApp()
{
    delete ui;
}
