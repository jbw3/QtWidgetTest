#include <QDebug>
#include <QDir>
#include <QFileSystemWatcher>
#include <QShortcut>
#include <QTime>

#include "comboboxitemdelegate.h"
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

const QStringList STYLES = QStringList() << "<default>" << "blue" << "dark" << "roundButtons" << "listWidget";

#ifdef Q_OS_DARWIN
const QString WATCH_PATH = QStringLiteral("../../../test");
#else
const QString WATCH_PATH = QStringLiteral("test");
#endif

Dialog::Dialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    Q_INIT_RESOURCE(resources);

    Q_ASSERT(ui != nullptr);

    ui->setupUi(this);

    watcher = new QFileSystemWatcher;
    watcher->addPath(WATCH_PATH);

    ui->styleComboBox->addItems(STYLES);
    ui->styleComboBox->setCurrentText("<default>");
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

    // int values
//    validator = new QIntValidator(-10, 1000000000);
//    ValidatingItemDelegate* valDel = new ValidatingItemDelegate(ui->listWidget2);
//    valDel->setValidator(validator);
//    ui->listWidget2->setItemDelegate(valDel);
//    ui->listWidget2->setDefaultValue(QStringLiteral("0"));
//    ui->listWidget2->setValidator(validator);

    // enum values
    QStringList values = QStringList() << "one" << "two" << "three";
    QRegularExpression regEx("one|two|three");
    validator = new QRegularExpressionValidator(regEx);
    ComboBoxItemDelegate* itemDel = new ComboBoxItemDelegate(ui->listWidget2);
    itemDel->setItems(values);
    ui->listWidget2->setItemDelegate(itemDel);
    ui->listWidget2->setDefaultValue(values.at(0));
    ui->listWidget2->setValidator(validator);

    updateFileList();

    // ------ Connections ------

    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(UpdateValues(int)));
    connect(ui->dial, SIGNAL(valueChanged(int)), this, SLOT(UpdateValues(int)));
    connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(UpdateValues(int)));

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(FillLineEdits()));

    connect(shortcut, SIGNAL(activated()), this, SLOT(filterFocus()));

    connect(ui->addPushButton, &QPushButton::clicked, ui->listWidget2, &ValueListWidget::addAndEditItem);
    connect(ui->removePushButton, &QPushButton::clicked, ui->listWidget2, &ValueListWidget::removeSelectedItems);
    connect(ui->listWidget2->itemDelegate(), SIGNAL(commitData(QWidget*)), ui->addPushButton, SLOT(setFocus()));

    connect(ui->styleComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(loadStyle()));

    connect(watcher, &QFileSystemWatcher::directoryChanged, this, &Dialog::updateFileList);
}

Dialog::~Dialog()
{
    delete ui;
    delete watcher;
    delete validator;

    qDebug() << __FUNCTION__;
}

void Dialog::loadStyle()
{

//    const QString STYLE_SHEET_NAME = ":/qss/dark.qss";
//    const QString STYLE_SHEET_NAME = ":/qss/roundButtons.qss";
//    const QString STYLE_SHEET_NAME = ":qss/listWidget.qss";
    QString styleText = ui->styleComboBox->currentText();
    if (styleText == "<default>")
    {
        setStyleSheet("");
    }
    else
    {
        QString styleName = QString(":/qss/%1.qss").arg(styleText);

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
}

void Dialog::filterFocus()
{
    qDebug() << __FUNCTION__;

    ui->filterLineEdit->setFocus();
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

void Dialog::updateFileList()
{
    QDir dirPath(WATCH_PATH);

    ui->listWidget3->clear();
    ui->listWidget3->addItems(dirPath.entryList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files));
}
