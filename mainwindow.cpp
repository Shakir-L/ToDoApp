#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTreeWidgetItem>
#include <QListWidgetItem>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create some example categories
    categories << Category{"Personal", QColor(Qt::green)};
    categories << Category{"Work", QColor(Qt::red)};
    categories << Category{"Shopping", QColor(Qt::blue)};

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

}


MainWindow::~MainWindow()
{
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

        ui->listWidget->addItem(item);
        ui->lineEdit->clear();
    }
}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    ui->listWidget->takeItem(ui->listWidget->row(item));
    delete item;
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    bool ok;
    QString editedText = QInputDialog::getText(this, tr("Edit Task"), tr("Task:"), QLineEdit::Normal, item->text(), &ok);

    if (ok && !editedText.isEmpty()) {
        item->setText(editedText);
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











