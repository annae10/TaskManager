#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QListWidgetItem>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QInputDialog>
#include <QFile>
#include <QTextStream>
#include <QDate>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connect the add button to the slot
    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::addItem);

    // Load data from file
    loadData();
    loadCompletedTasks();
    updateCompletedTasksDisplay();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addItem()
{
    QString text = ui->lineEdit->text();
    if (!text.isEmpty()) {
        QListWidgetItem *item = new QListWidgetItem(text, ui->listWidget);

        // Add a delete button to the item
        QPushButton *deleteButton = new QPushButton("Удалить");
        connect(deleteButton, &QPushButton::clicked, this, [this, item]() {
            delete ui->listWidget->takeItem(ui->listWidget->row(item));
        });

        // Add an edit button to the item
        QPushButton *editButton = new QPushButton("Редактировать");
        connect(editButton, &QPushButton::clicked, this, [this, item]() {
            editItem(item);
        });

        // Add a complete button to the item
        QPushButton *completeButton = new QPushButton("Готово");
        connect(completeButton, &QPushButton::clicked, this, [this, item]() {
            completeItem(item);
        });

        // Create a layout for the item
        QWidget *widget = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout(widget);
        layout->addWidget(editButton);
        layout->addWidget(deleteButton);
        layout->addWidget(completeButton);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setAlignment(Qt::AlignRight);
        widget->setLayout(layout);

        // Set the custom widget for the item
        ui->listWidget->setItemWidget(item, widget);

        // Clear the line edit
        ui->lineEdit->clear();
    } else {
        QMessageBox::warning(this, "Предупреждение", "Введите текст для добавления.");
    }
}

void MainWindow::editItem(QListWidgetItem *item)
{
    // Create a dialog for editing the item
    bool ok;
    QString newText = QInputDialog::getText(this, "Редактировать элемент", "Введите новый текст:", QLineEdit::Normal, item->text(), &ok);
    if (ok && !newText.isEmpty()) {
        item->setText(newText);
    }
}

void MainWindow::completeItem(QListWidgetItem *item)
{
    // Remove the item from the list
    delete ui->listWidget->takeItem(ui->listWidget->row(item));

    // Update the completed tasks count for today
    QDate currentDate = QDate::currentDate();
    completedTasks[currentDate.toString("yyyy-MM-dd")]++;

    // Update the display
    updateCompletedTasksDisplay();

    // Save the completed tasks data
    saveCompletedTasks();
}

void MainWindow::loadData()
{
    QFile file("data.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QListWidgetItem *item = new QListWidgetItem(line, ui->listWidget);
// Add a delete button to the item
            QPushButton *deleteButton = new QPushButton("Удалить");
            connect(deleteButton, &QPushButton::clicked, this, [this, item]() {
                delete ui->listWidget->takeItem(ui->listWidget->row(item));
            });

            // Add an edit button to the item
            QPushButton *editButton = new QPushButton("Редактировать");
            connect(editButton, &QPushButton::clicked, this, [this, item]() {
                editItem(item);
            });

            // Add a complete button to the item
            QPushButton *completeButton = new QPushButton("Готово");
            connect(completeButton, &QPushButton::clicked, this, [this, item]() {
                completeItem(item);
            });

            // Create a layout for the item
            QWidget *widget = new QWidget();
            QHBoxLayout *layout = new QHBoxLayout(widget);
            layout->addWidget(editButton);
            layout->addWidget(deleteButton);
            layout->addWidget(completeButton);
            layout->setContentsMargins(0, 0, 0, 0);
            layout->setAlignment(Qt::AlignRight);
            widget->setLayout(layout);

            // Set the custom widget for the item
            ui->listWidget->setItemWidget(item, widget);
        }
        file.close();
    }
}

void MainWindow::saveData()
{
    QFile file("data.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (int i = 0; i < ui->listWidget->count(); ++i) {
            QListWidgetItem *item = ui->listWidget->item(i);
            out << item->text() << "\n";
        }
        file.close();
    }
}

void MainWindow::loadCompletedTasks()
{
    QFile file("completed_tasks.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(":");
            if (parts.size() == 2) {
                QString date = parts[0];
                int count = parts[1].toInt();
                completedTasks[date] = count;
            }
        }
        file.close();
    }

    QDate currentDate = QDate::currentDate();
    for(auto it = completedTasks.begin(); it != completedTasks.end();){
        QDate date = QDate::fromString(it.key(), "yyyy-MM-dd");
        if(date<currentDate.addDays(-4)){
            it = completedTasks.erase(it);
        } else {
            ++it;
        }
    }
}

void MainWindow::saveCompletedTasks()
{
    QFile file("completed_tasks.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (auto it = completedTasks.begin(); it != completedTasks.end(); ++it) {
            out << it.key() << ":" << it.value() << "\n";
        }
        file.close();
    }
}

void MainWindow::updateCompletedTasksDisplay()
{
    QDate currentDate = QDate::currentDate();
    QStringList labels;
    for (int i = 0; i < 5; ++i) {
        QDate date = currentDate.addDays(-i);
        QString dateString = date.toString("yyyy-MM-dd");
        int count = completedTasks.value(dateString, 0);
        labels.prepend(QString::number(count));
        //labels << QString::number(count);
    }
    ui->completedTasksLabel->setText(labels.join(" "));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveData();
    saveCompletedTasks();
    event->accept();
}

