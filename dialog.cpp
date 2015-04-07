#include <QDebug>
#include <QShortcut>

#include "dialog.h"
#include "ui_dialog.h"
#include "qfile.h"
#include "validatingitemdelegate.h"

const QString TAB_STYLE =
"import QtQuick 2.0\n"
"QTabBar\n"
"{\n"
"color: rgb(255, 0, 0);\n"
"transform: Rotation { origin.x: 0; origin.y: 0; angle: 45 };\n"
"}";

const QStringList STYLES = QStringList() << "blue" << "dark" << "roundButtons" << "listWidget";

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    Q_INIT_RESOURCE(resources);

    ui->setupUi(this);

    ui->styleComboBox->addItems(STYLES);
    ui->styleComboBox->setCurrentText("dark");
    loadStyle();

    ui->listWidget1->setTextElideMode(Qt::ElideRight);
    for (int i = 0; i < 10; ++i)
    {
        ui->listWidget1->addItem(QString("Item %1").arg(i+1));
    }
    ui->listWidget1->addItem("This item has a very, very long label");

    qDebug() << "width:" << width();
    int count = ui->splitter->count();
    qDebug() << "count:" << count;
    QList<int> sizes;
    sizes.push_back(90);
    int otherSize = width() / (count - 1);
    for (int i = 0; i < count - 1; ++i)
        sizes.push_back(otherSize);
    ui->splitter->setSizes(sizes);

    // ------ Table Widget ------

    QStringList labels = QStringList() << "Kiwi" << "Banana" << "Grape" << "Apple" << "Orange";

    const int NUM_ROWS = 5;
    const int NUM_COLS = 2;
    ui->tableWidget->setRowCount(NUM_ROWS);
    ui->tableWidget->setColumnCount(NUM_COLS);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSortingEnabled(true);
    ui->tableWidget->verticalHeader()->setHidden(true);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(NUM_COLS-1, QHeaderView::Stretch);
    for (int r = 0; r < NUM_ROWS; ++r)
    {
        QTableWidgetItem* item = new QTableWidgetItem(labels.at(r));
        ui->tableWidget->setItem(r, 0, item);
    }

    // ------ Tab Widget ------

//    ui->tabWidget->setStyleSheet(TAB_STYLE);

    // ------ Shorcuts ------

    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_F), ui->tab5);

    // ------ List Widgets ------

    validator = new QIntValidator(1, 20);
    ValidatingItemDelegate* delegate = new ValidatingItemDelegate(this);
    delegate->setValidator(validator);

    ui->listWidget2->setItemDelegate(delegate);

    // ------ Connections ------

    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(UpdateValues(int)));
    connect(ui->dial, SIGNAL(valueChanged(int)), this, SLOT(UpdateValues(int)));
    connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(UpdateValues(int)));

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(FillLineEdits()));

    connect(shortcut, SIGNAL(activated()), this, SLOT(filterFocus()));

    connect(ui->addPushButton, SIGNAL(clicked()), this, SLOT(addItem()));
    connect(ui->removePushButton, SIGNAL(clicked()), this, SLOT(removeItem()));

    connect(ui->listWidget2, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(doneEditing(QListWidgetItem*)));
//    connect(ui->listWidget2, SIGNAL(activated
//    connect(ui->listWidget2, SIGNAL(itemChanged(QListWidgetItem*)), ui->addPushButton, SLOT(setFocus()));

    connect(ui->styleComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(loadStyle()));
}

Dialog::~Dialog()
{
    delete ui;
    delete validator;
}

void Dialog::loadStyle()
{

    const QString STYLE_SHEET_NAME = ":/qss/dark.qss";
//    const QString STYLE_SHEET_NAME = ":/qss/roundButtons.qss";
//    const QString STYLE_SHEET_NAME = ":qss/listWidget.qss";
    QString styleName = QString(":/qss/%1.qss").arg(ui->styleComboBox->currentText());

    QFile file(styleName);
    bool ok = file.open(QFile::ReadOnly);
    if (ok)
    {
        QString styleSheet = QLatin1String(file.readAll());
        file.close();
        setStyleSheet(styleSheet);
    }
    else
    {
        qWarning() << "Could not load style sheet:" << file.errorString();
    }
}

void Dialog::filterFocus()
{
    qDebug() << __FUNCTION__;

    ui->filterLineEdit->setFocus();
}

void Dialog::addItem()
{
    qDebug() << __FUNCTION__;

    QListWidgetItem* item = new QListWidgetItem("", ui->listWidget2);
    item->setFlags(item->flags() | Qt::ItemIsEditable);

//    ui->listWidget2->addItem(item);

    ui->listWidget2->editItem(item);
}

void Dialog::removeItem()
{
    qDebug() << __FUNCTION__;

    QList<QListWidgetItem*> selected = ui->listWidget2->selectedItems();

    qDebug() << selected;

    foreach (QListWidgetItem* item, selected)
    {
        int row = ui->listWidget2->row(item);
        item = ui->listWidget2->takeItem(row);
        delete item;
    }
}

void Dialog::doneEditing(QListWidgetItem* item)
{
    qDebug() << __FUNCTION__;

    if (item->text().isEmpty())
    {
        if (0 >= validator->bottom() && 0 <= validator->top())
            item->setText("0");
        else
            item->setText(QString::number(validator->bottom()));
    }

    ui->addPushButton->setFocus();
}

void Dialog::UpdateValues(int value)
{
    ui->sliderLabel->setText(QString::number(value));
    ui->horizontalSlider->setValue(value);
    ui->dial->setValue(value);
    ui->spinBox->setValue(value);
}

void Dialog::FillLineEdits()
{
    ui->lineEdit->setText("This is a test.");
    ui->lineEdit2->setText("Line edit 2");
}
