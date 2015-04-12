// Qt includes
#include <QLineEdit>

// project includes
#include "validatingitemdelegate.h"

ValidatingItemDelegate::ValidatingItemDelegate(QObject* parent) :
    QItemDelegate(parent),
    validator(NULL)
{
}

const QValidator* ValidatingItemDelegate::getValidator() const
{
    return validator;
}

void ValidatingItemDelegate::setValidator(const QValidator* v)
{
    validator = v;
}

QWidget* ValidatingItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /*option*/, const QModelIndex& /*index*/) const
{
    QLineEdit* lineEdit = new QLineEdit(parent);
    lineEdit->setValidator(validator);

    return lineEdit;
}
