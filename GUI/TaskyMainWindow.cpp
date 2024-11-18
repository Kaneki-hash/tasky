#include "TaskyMainWindow.h"
#include "TaskOperations.h"
#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDir>
#include <QFile>
#include <QHeaderView>
#include <QMessageBox>
#include "json-library/single_include/nlohmann/json.hpp"
#include <fstream>

using json = nlohmann::json;

TaskyMainWindow::TaskyMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    create_config_paths();
    setupConfig();         
    setupUI();            
    loadTasks();         
}

void TaskyMainWindow::setupUI()
{
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    
    taskTable = new QTableWidget(this);
    taskTable->setColumnCount(4);
    taskTable->setHorizontalHeaderLabels({"ID", "Created At", "Status", "Description"});
    taskTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    
    QHBoxLayout* inputLayout = new QHBoxLayout();
    taskDescriptionInput = new QLineEdit(this);
    addButton = new QPushButton("Add Task", this);
    inputLayout->addWidget(taskDescriptionInput);
    inputLayout->addWidget(addButton);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    deleteButton = new QPushButton("Delete", this);
    editButton = new QPushButton("Edit", this);
    markDoneButton = new QPushButton("Mark Done", this);
    markInProgressButton = new QPushButton("Mark In Progress", this);
    
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(markDoneButton);
    buttonLayout->addWidget(markInProgressButton);
    
    mainLayout->addWidget(taskTable);
    mainLayout->addLayout(inputLayout);
    mainLayout->addLayout(buttonLayout);
    
    setCentralWidget(centralWidget);
    
    connect(addButton, &QPushButton::clicked, this, &TaskyMainWindow::addTask);
    connect(deleteButton, &QPushButton::clicked, this, &TaskyMainWindow::deleteTask);
    connect(editButton, &QPushButton::clicked, this, &TaskyMainWindow::editTask);
    connect(markDoneButton, &QPushButton::clicked, this, &TaskyMainWindow::markDone);
    connect(markInProgressButton, &QPushButton::clicked, this, &TaskyMainWindow::markInProgress);
    
    resize(800, 600);
    setWindowTitle("Tasky GUI");
}

void TaskyMainWindow::loadTasks()
{
    std::string data_json_path = std::string(getenv("HOME")) + "/.config/tasky/data.json";
    std::ifstream file(data_json_path);
    
    if (!file.is_open()) {
        QMessageBox::warning(this, "Error", "Could not open data file");
        return;
    }
    
    file.seekg(0, std::ios::end);
    if (file.tellg() == 0) {
        file.close();
        std::ofstream output(data_json_path);
        output << "{\"tasks\":[]}" << std::endl;
        output.close();
        
        file.open(data_json_path);
    }
    
    file.seekg(0);
    
    json data;
    file >> data;
    
    taskTable->setRowCount(0);
    
    if (data.contains("tasks")) {
        for (const auto& task : data["tasks"]) {
            int row = taskTable->rowCount();
            taskTable->insertRow(row);
            
            taskTable->setItem(row, 0, new QTableWidgetItem(QString::number(task["id"].get<int>())));
            taskTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(task["created-at"])));
            taskTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(task["status"])));
            taskTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(task["description"])));
        }
    }
}

void TaskyMainWindow::addTask()
{
    QString description = taskDescriptionInput->text();
    if (description.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter a task description");
        return;
    }
    
    std::string stdDescription = description.toStdString();
    add_task(stdDescription.c_str());
    
    refreshTaskList();
    taskDescriptionInput->clear();
}

void TaskyMainWindow::deleteTask()
{
    auto selected = taskTable->selectedItems();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select a task to delete");
        return;
    }
    
    QString taskId = taskTable->item(selected.first()->row(), 0)->text();
    
    refreshTaskList();
}

void TaskyMainWindow::editTask()
{
    auto selected = taskTable->selectedItems();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select a task to edit");
        return;
    }
    
    QString taskId = taskTable->item(selected.first()->row(), 0)->text();
    QString newDescription = QInputDialog::getText(this, "Edit Task", "Enter new description:");
    
    if (!newDescription.isEmpty()) {
        refreshTaskList();
    }
}

void TaskyMainWindow::markDone()
{
    auto selected = taskTable->selectedItems();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select a task");
        return;
    }
    
    QString taskId = taskTable->item(selected.first()->row(), 0)->text();
    
    refreshTaskList();
}

void TaskyMainWindow::markInProgress()
{
    auto selected = taskTable->selectedItems();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select a task");
        return;
    }
    
    QString taskId = taskTable->item(selected.first()->row(), 0)->text();
    
    refreshTaskList();
}

void TaskyMainWindow::refreshTaskList()
{
    loadTasks();
}

void TaskyMainWindow::setupConfig() {
    QString configPath = QDir::homePath() + "/.config/tasky";
    QDir dir(configPath);
    
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    
    QString dataFile = configPath + "/data.json";
    if (!QFile::exists(dataFile)) {
        QFile file(dataFile);
        if (file.open(QIODevice::WriteOnly)) {
            file.write("{\"tasks\":[]}");
            file.close();
        }
    }
    
    QString idFile = configPath + "/issued_ID.txt";
    if (!QFile::exists(idFile)) {
        QFile file(idFile);
        if (file.open(QIODevice::WriteOnly)) {
            file.write("0");
            file.close();
        }
    }
} 