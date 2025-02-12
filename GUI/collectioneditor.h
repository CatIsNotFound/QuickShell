#ifndef COLLECTIONEDITOR_H
#define COLLECTIONEDITOR_H

#include <QMainWindow>

namespace Ui {
class CollectionEditor;
}

class CollectionEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit CollectionEditor(QWidget *parent = nullptr);
    ~CollectionEditor();

private:
    Ui::CollectionEditor *ui;
};

#endif // COLLECTIONEDITOR_H
