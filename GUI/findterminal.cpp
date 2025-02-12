#include "findterminal.h"
#include "ui_findterminal.h"

FindTerminal::FindTerminal(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FindTerminal)
{
    ui->setupUi(this);

    // Add Items
    TermsList terms("../conf/terminal.inf");
    QStringList apps = terms.getList(true);
    // ui->listWidget->addItems(apps);
    int k = 0;
    for (auto i : apps) {
        ui->listWidget->addItem(i);
        QIcon temp_icon(":/images/assets/icons/terminal-app.png");
        ui->listWidget->item(k++)->setIcon(temp_icon);
    }
    // Connect events
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, [=] {
        int i = ui->listWidget->currentRow();
        if (i > -1) {
            emit getTerminal(ui->listWidget->item(i)->text());
        }
    });

    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, [=] {
        int i = ui->listWidget->currentRow();
        emit getTerminal(ui->listWidget->item(i)->text());
        this->close();
    });
}

FindTerminal::~FindTerminal()
{
    delete ui;
}
