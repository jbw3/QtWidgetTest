#include <QComboBox>
#include <QDebug>

#include "comboboxitemdelegate.h"

ComboBoxItemDelegate::ComboBoxItemDelegate(QObject* parent /* = 0 */) :
    QItemDelegate(parent)
{
}

void ComboBoxItemDelegate::setItems(const QStringList& texts)
{
    itemTexts = texts;
}

QWidget* ComboBoxItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /*option*/, const QModelIndex& /*index*/) const
{
    qDebug() << __FUNCTION__;

    QComboBox* comboBox = new QComboBox(parent);
    comboBox->addItems(itemTexts);

    return comboBox;
}
