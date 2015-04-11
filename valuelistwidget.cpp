#include <QDebug>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFileInfo>
#include <QMimeData>
#include <QTime>

#include "validatingitemdelegate.h"
#include "valuelistwidget.h"

ValueListWidget::ValueListWidget(QWidget* parent) :
    QListWidget(parent)
{
    validator = new QIntValidator(0, 1000000000);
    ValidatingItemDelegate* delegate = new ValidatingItemDelegate(this);
    delegate->setValidator(validator);

    setItemDelegate(delegate);

    setAcceptDrops(true);
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    connect(this, &ValueListWidget::itemChanged, this, &ValueListWidget::doneEditing);
}

ValueListWidget::~ValueListWidget()
{
    delete validator;
}

void ValueListWidget::dragEnterEvent(QDragEnterEvent* event)
{
    bool canDrop = false;

    if (event->source() == this)
    {
        canDrop = true;
    }
    else
    {
        const QList<QUrl>& urls = event->mimeData()->urls();

        if (urls.length() > 0)
        {
            canDrop = true;
            foreach (QUrl url, urls)
            {
                QFileInfo fileInfo(url.toLocalFile());
                if (!fileInfo.exists() || fileInfo.suffix() != QStringLiteral("csv"))
                {
                    canDrop = false;
                    break;
                }
            }
        }
        else
        {
            canDrop = false;
        }
    }

    if (canDrop)
    {
        qDebug() << "You can drop!";

        event->acceptProposedAction();
    }
    else
    {
        qDebug() << "You cannot drop :(";
    }
}

void ValueListWidget::dropEvent(QDropEvent* event)
{
    qDebug() << __FUNCTION__;

    if  (event->source() == this)
    {
        QListWidget::dropEvent(event);
    }
    else
    {
        foreach (QUrl url, event->mimeData()->urls())
        {
            QFile inFile(url.toLocalFile());
            bool ok = inFile.open(QIODevice::ReadOnly);
            if (ok)
            {
                QTextStream inStream(&inFile);

                foreach (QString val, inStream.readAll().split(QStringLiteral(",")))
                {
                    QString str = val;

                    bool isValid = false;
                    int pos = 0;
                    if (validator->validate(str, pos) == QValidator::Acceptable)
                    {
                        isValid = true;
                    }
                    else
                    {
                        // try stripping whitespace to see if that makes the string valid
                        str = str.trimmed();
                        if (validator->validate(str, pos) == QValidator::Acceptable)
                        {
                            isValid = true;
                        }
                        else
                        {
                            isValid = false;
                        }
                    }

                    if (isValid)
                    {
                        QListWidgetItem* item = new QListWidgetItem(str, this);
                        item->setFlags(item->flags() | Qt::ItemIsEditable);
                    }
                    else
                    {
                        qDebug() << str << "is not valid";
                    }
                }

                inFile.close();
            }
            else
            {
                qWarning() << "Could not open file:" << inFile.errorString();
            }
        }
    }
}

void ValueListWidget::addAndEditItem()
{
    QListWidgetItem* item = new QListWidgetItem("", this);
    item->setFlags(item->flags() | Qt::ItemIsEditable);

    editItem(item);
}

void ValueListWidget::removeSelectedItems()
{
    qDeleteAll(selectedItems());
}

void ValueListWidget::doneEditing(QListWidgetItem* item)
{
    if (item->text().isEmpty())
    {
        if (0 >= validator->bottom() && 0 <= validator->top())
            item->setText("0");
        else
            item->setText(QString::number(validator->bottom()));
    }
}
