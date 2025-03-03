#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QCloseEvent>
#include <QMap>
#include <QDate>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void addItem();
    void editItem(QListWidgetItem *item);
    void completeItem(QListWidgetItem *item);

private:
    void loadData();
    void saveData();
    void loadCompletedTasks();
    void saveCompletedTasks();
    void updateCompletedTasksDisplay();

    Ui::MainWindow *ui;
    QMap<QString, int> completedTasks;
};
#endif // MAINWINDOW_H
