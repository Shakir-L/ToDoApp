#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QDate>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct Category {
    QString name;
    QColor color; // Optional: to give each category a different color
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QList<Category> categories;
    void addCategoriesToComboboxes();

private slots:
    void on_pushButton_clicked();
    void on_categoryFilterComboBox_currentIndexChanged(int index);
    void on_highPriorityCheckBox_stateChanged(int state);
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_deleteButton_clicked();

protected:
    void saveList();

};

class TodoItem : public QListWidgetItem {
public:
    Category category;
    bool isHighPriority = false;
};



#endif // MAINWINDOW_H
