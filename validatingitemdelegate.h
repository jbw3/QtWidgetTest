#ifndef VALIDATINGITEMDELEGATE_H
#define VALIDATINGITEMDELEGATE_H

#include <QItemDelegate>

class ValidatingItemDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    explicit ValidatingItemDelegate(QObject* parent = 0);

    void setValidator(const QValidator* v);

    virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;

protected:
    const QValidator* validator;
};

#endif // VALIDATINGITEMDELEGATE_H
