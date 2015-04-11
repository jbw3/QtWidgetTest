#ifndef VALUELISTWIDGET_H
#define VALUELISTWIDGET_H

#include <QListWidget>

class ValueListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit ValueListWidget(QWidget* parent = 0);

    virtual ~ValueListWidget();

    virtual void dragEnterEvent(QDragEnterEvent* event);

    virtual void dropEvent(QDropEvent* event);

public slots:
    void addAndEditItem();
    void removeSelectedItems();

protected:
    QIntValidator* validator;

protected slots:
    void doneEditing(QListWidgetItem* item);
};

#endif // VALUELISTWIDGET_H
