#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui
{
class Dialog;
}
class QIntValidator;
class QListWidgetItem;
class QShortcut;

class Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    
private:
    Ui::Dialog *ui;
    QShortcut* shortcut;
    QIntValidator* validator;

private slots:
    void loadStyle();
    void UpdateValues(int value);
    void FillLineEdits();
    void filterFocus();
    void addItem();
    void removeItem();
    void doneEditing(QListWidgetItem* item);
};

#endif // DIALOG_H
