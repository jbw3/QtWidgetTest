#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui
{
class Dialog;
}
class QFileSystemWatcher;
class QListWidgetItem;
class QShortcut;

class Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialog(QWidget* parent = 0);
    ~Dialog();
    
private:
    Ui::Dialog* ui;
    QShortcut* shortcut;
    QFileSystemWatcher* watcher;

private slots:
    void loadStyle();
    void UpdateValues(int value);
    void FillLineEdits();
    void filterFocus();
    void updateFileList();
};

#endif // DIALOG_H
