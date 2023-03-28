#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTreeWidgetItem>
#include <QListWidgetItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create some example categories
    categories << Category{"Personal", QColor(Qt::green)};
    categories << Category{"Work", QColor(Qt::red)};
    categories << Category{"Shopping", QColor(Qt::blue)};

    // Add the categories to the combo box
    for (const auto &category : categories) {
        ui->categoryComboBox->addItem(category.name);
    }

    // Connect the signal/slot for adding tasks
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_pushButton_clicked);

    // Connect the signal/slot for removing tasks
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &MainWindow::on_listWidget_itemDoubleClicked);
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
