#include <QDebug>
#include <QLineEdit>

#include "validatingitemdelegate.h"

#define CREATE_LINE_EDIT 0

ValidatingItemDelegate::ValidatingItemDelegate(QObject* parent) :
    QItemDelegate(parent),
    validator(NULL)
{
}

void ValidatingItemDelegate::setValidator(const QValidator* v)
{
    validator = v;
}

QWidget* ValidatingItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    qDebug() << __FUNCTION__;

#if CREATE_LINE_EDIT
    QLineEdit* lineEdit = new QLineEdit(parent);
    lineEdit->setValidator(validator);

    return lineEdit;
#else
    QWidget* widget = QItemDelegate::createEditor(parent, option, index);

    QLineEdit* lineEdit = reinterpret_cast<QLineEdit*>(widget);
    if (lineEdit != NULL)
    {
        lineEdit->setValidator(validator);
    }

    return widget;
#endif
}
