#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTreeWidgetItem>
#include <QListWidgetItem>
#include <QInputDialog>
#include <QPainter>
#include <QSettings>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSettings settings("IUT", "ToDoApp", this);

    // Load the saved categories and their colors from the settings file
    int categorySize = settings.beginReadArray("categories");
    for (int i = 0; i < categorySize; i++) {
        settings.setArrayIndex(i);
        QString name = settings.value("name").toString();
        QString colorStr = settings.value("color").toString();
        QColor color = QColor(colorStr); // Parse the color string into a QColor
        categories << Category{name, color};
    }
    settings.endArray();

    // Load the saved tasks from the settings file
    int taskSize = settings.beginReadArray("tasks");
    for (int i = 0; i < taskSize; i++) {
        settings.setArrayIndex(i);
        QString text = settings.value("text").toString();
        QString categoryName = settings.value("category").toString();
        QString colorStr = settings.value("categoryColor").toString();
        QColor categoryColor = Qt::gray; // Default color if category not found
        for (const auto &category : this->categories) {
            if (category.name == categoryName) {
                categoryColor = category.color;
                break;
            }
        }
        // If the color string is not empty, parse it into a QColor
        if (!colorStr.isEmpty()) {
            categoryColor = QColor(colorStr);
        }
        TodoItem *item = new TodoItem();
        item->setText(text);
        item->category.name = categoryName;
        item->category.color = categoryColor;
        item->setBackground(categoryColor);
        item->isHighPriority = settings.value("isHighPriority").toBool();
        if (item->isHighPriority) {
            QPixmap pixmap(16, 16);
            pixmap.fill(Qt::transparent);
            QPainter painter(&pixmap);
            painter.setPen(Qt::red);
            painter.setBrush(Qt::red);
            painter.drawEllipse(0, 0, 16, 16);
            item->setIcon(QIcon(pixmap));
        }
        ui->listWidget->addItem(item);
    }
    settings.endArray();







    // Create some example categories
    categories << Category{"Personal", QColor(Qt::green)};
    categories << Category{"Work", QColor(Qt::red)};
    categories << Category{"Shopping", QColor(Qt::blue)};

    // Add the priority levels to the combo box

    // Add the categories to the combo boxes
    for (const auto &category : categories) {
        ui->categoryComboBox->addItem(category.name);
        ui->categoryFilterComboBox->addItem(category.name);
    }

    // Add "All" category to the filter combo box
    ui->categoryFilterComboBox->insertItem(0, "All");
    ui->categoryFilterComboBox->setCurrentIndex(0);

    // Connect the signal/slot for adding tasks
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_pushButton_clicked);

    // Connect the signal/slot for removing tasks
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &MainWindow::on_listWidget_itemDoubleClicked);

    // Connect the signal/slot for editing tasks
    connect(ui->listWidget, &QListWidget::itemClicked, this, &MainWindow::on_listWidget_itemClicked);

    // Connect the signal/slot for filtering tasks
    connect(ui->categoryFilterComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_categoryFilterComboBox_currentIndexChanged);

    connect(ui->lineEdit, &QLineEdit::returnPressed, ui->pushButton, &QPushButton::click);

    connect(ui->highPriorityCheckBox, &QCheckBox::stateChanged, this, &MainWindow::on_highPriorityCheckBox_stateChanged);

}


MainWindow::~MainWindow()
{
    QSettings settings("IUT", "ToDoApp", this);
    settings.beginWriteArray("categories");
    for (int i = 0; i < categories.size(); i++) {
        settings.setArrayIndex(i);
        settings.setValue("name", categories[i].name);
        settings.setValue("color", categories[i].color.name()); // Save the color as a string
    }
    settings.endArray();

    settings.beginWriteArray("tasks");
    for (int i = 0; i < ui->listWidget->count(); i++) {
        QListWidgetItem *item = ui->listWidget->item(i);
        TodoItem *todoItem = dynamic_cast<TodoItem *>(item);
        settings.setArrayIndex(i);
        settings.setValue("text", item->text());
        settings.setValue("category", todoItem->category.name);
        settings.setValue("categoryColor", todoItem->category.color.name()); // Save the color as a string
        settings.setValue("isHighPriority", todoItem->isHighPriority);
    }
    settings.endArray();


    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString task = ui->lineEdit->text();

    if (!task.isEmpty()) {
        // Get the selected category
        QString categoryName = ui->categoryComboBox->currentText();
        QColor categoryColor = Qt::gray; // Default color if category not found

        for (const auto &category : this->categories) {
            if (category.name == categoryName) {
                categoryColor = category.color;
                break;
            }
        }

        // Create a new item with the task text and category color
        TodoItem *item = new TodoItem();
        item->setText(task);
        item->category.name = categoryName;
        item->category.color = categoryColor;
        item->setBackground(categoryColor);

        // Set the high-priority flag based on the state of the check box
        item->isHighPriority = ui->checkBox->isChecked();

        // Set the icon of the item to a red circle if it is a high-priority task
        if (item->isHighPriority) {
            QPixmap pixmap(16, 16);
            pixmap.fill(Qt::transparent);
            QPainter painter(&pixmap);
            painter.setPen(Qt::red);
            painter.setBrush(Qt::red);
            painter.drawEllipse(0, 0, 16, 16);
            item->setIcon(QIcon(pixmap));
        }

        ui->listWidget->addItem(item);
        ui->lineEdit->clear();

        QSettings settings("IUT", "ToDoApp", this);
        settings.beginWriteArray("tasks");
        settings.setArrayIndex(ui->listWidget->count() - 1); // set the array index to the last added item
        settings.setValue("text", task);
        settings.setValue("category", categoryName);
        settings.setValue("categoryColor", categoryColor.name()); // store the color as a string
        settings.setValue("isHighPriority", ui->checkBox->isChecked());
        settings.endArray();

    }
}



void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    delete item;

    // Check if there are any remaining items
    if (ui->listWidget->count() == 0) {
        // If there are no remaining items, close the application
        QApplication::exit();
    }
}



void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    TodoItem *todoItem = dynamic_cast<TodoItem *>(item);
    if (todoItem) {
        // Update the check box state to reflect the priority level
        ui->checkBox->setChecked(todoItem->isHighPriority);
    }
}




void MainWindow::on_categoryFilterComboBox_currentIndexChanged(int index)
{
    for (int i = 0; i < ui->listWidget->count(); i++) {
        QListWidgetItem *item = ui->listWidget->item(i);
        TodoItem *todoItem = dynamic_cast<TodoItem *>(item);

        if (index == 0) { // "All" is selected
            item->setHidden(false);
        } else {
            if (todoItem && todoItem->category.name == ui->categoryFilterComboBox->currentText()) {
                item->setHidden(false);
            } else {
                item->setHidden(true);
            }
        }
    }
}

void MainWindow::on_highPriorityCheckBox_stateChanged(int state)
{
    QString selectedCategory = ui->categoryFilterComboBox->currentText();

    for (int i = 0; i < ui->listWidget->count(); i++) {
        QListWidgetItem *item = ui->listWidget->item(i);
        TodoItem *todoItem = dynamic_cast<TodoItem *>(item);

        if (selectedCategory == "All") {
            if (state == Qt::Unchecked || todoItem->isHighPriority) {
                item->setHidden(false);
            } else {
                item->setHidden(true);
            }
        } else if (todoItem && todoItem->category.name == selectedCategory) {
            if (state == Qt::Unchecked || todoItem->isHighPriority) {
                item->setHidden(false);
            } else {
                item->setHidden(true);
            }
        }
    }
}













