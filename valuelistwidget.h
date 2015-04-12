#ifndef VALUELISTWIDGET_H
#define VALUELISTWIDGET_H

#include <QListWidget>

class ValueListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit ValueListWidget(QWidget* parent = 0, const QString& defaultValue = QStringLiteral(""));

    virtual ~ValueListWidget();

    virtual void dragEnterEvent(QDragEnterEvent* event);

    virtual void dropEvent(QDropEvent* event);

    virtual void setDefaultValue(const QString& defaultValue);

    virtual void setValidator(const QValidator* v);

public slots:
    void addAndEditItem();
    void removeSelectedItems();

protected:
    QString defaultVal;
    const QValidator* validator;

    virtual void keyPressEvent(QKeyEvent* event);
};

#endif // VALUELISTWIDGET_H
