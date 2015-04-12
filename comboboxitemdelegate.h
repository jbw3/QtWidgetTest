#ifndef COMBOBOXITEMDELEGATE_H
#define COMBOBOXITEMDELEGATE_H

#include <QItemDelegate>

class ComboBoxItemDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    explicit ComboBoxItemDelegate(QObject *parent = 0);

    virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;

    void setItems(const QStringList& texts);

protected:
    QStringList itemTexts;
};

#endif // COMBOBOXITEMDELEGATE_H
