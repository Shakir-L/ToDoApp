#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTreeWidgetItem>
#include <QListWidgetItem>


void MainWindow::on_pushButton_clicked()
{
    QString task = ui->lineEdit->text();
    if(!task.isEmpty()) {
        ui->listWidget->addItem(task);
        ui->lineEdit->clear();
    }
}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    ui->listWidget->takeItem(ui->listWidget->row(item));
    delete item;
}
