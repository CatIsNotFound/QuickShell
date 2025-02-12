#ifndef FINDTERMINAL_H
#define FINDTERMINAL_H

#include <QDialog>
#include <QFileInfo>
#include <QSettings>

#include "../Libs/termslist.h"

namespace Ui {
class FindTerminal;
}

class FindTerminal : public QDialog
{
    Q_OBJECT

public:
    explicit FindTerminal(QWidget *parent = nullptr);
    ~FindTerminal();

signals:
    void getTerminal(const QString &index);

private:
    Ui::FindTerminal *ui;
    QSettings *configs;
};

#endif // FINDTERMINAL_H
