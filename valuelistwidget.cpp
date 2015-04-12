// Qt includes
#include <QDebug>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFileInfo>
#include <QKeyEvent>
#include <QMimeData>

// project includes
#include "validatingitemdelegate.h"
#include "valuelistwidget.h"

ValueListWidget::ValueListWidget(QWidget* parent /* = 0 */, const QString& defaultValue /* = QStringLiteral("") */) :
    QListWidget(parent),
    defaultVal(defaultValue),
    validator(NULL)
{
    setAcceptDrops(true);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
}

ValueListWidget::~ValueListWidget()
{
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
        event->acceptProposedAction();
    }
}

void ValueListWidget::dropEvent(QDropEvent* event)
{
    qDebug() << __FUNCTION__;

    // if the event came from this widget, the user is dragging the
    // items around; let the base class handle this
    if (event->source() == this)
    {
        QListWidget::dropEvent(event);
    }
    else // the user is dragging something from outside this widget
    {
        foreach (QUrl url, event->mimeData()->urls())
        {
            QFile inFile(url.toLocalFile());
            bool ok = inFile.open(QIODevice::ReadOnly);
            if (ok)
            {
                QTextStream inStream(&inFile);

                const QStringList& values = inStream.readAll().split(QStringLiteral(","));
                foreach (QString val, values)
                {
                    QString str = val;

                    bool isValid = false;
                    if (validator == NULL)
                    {
                        isValid = true;
                    }
                    else
                    {
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

void ValueListWidget::setDefaultValue(const QString& defaultValue)
{
    defaultVal = defaultValue;
}

void ValueListWidget::setValidator(const QValidator* v)
{
    validator = v;
}

void ValueListWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Delete)
    {
        removeSelectedItems();
    }
    else
    {
        QListWidget::keyPressEvent(event);
    }
}

void ValueListWidget::addAndEditItem()
{
    QListWidgetItem* item = new QListWidgetItem(defaultVal, this);
    item->setFlags(item->flags() | Qt::ItemIsEditable);

    editItem(item);
}

void ValueListWidget::removeSelectedItems()
{
    qDeleteAll(selectedItems());
}
