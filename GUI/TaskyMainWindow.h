#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QInputDialog>
#include "TaskyConfig.h"

class TaskyMainWindow : public QMainWindow {
    Q_OBJECT

public:
    TaskyMainWindow(QWidget *parent = nullptr);

private Q_SLOTS:
    void addTask();
    void deleteTask();
    void editTask();
    void markDone();
    void markInProgress();
    void refreshTaskList();

private:
    QTableWidget* taskTable;
    QLineEdit* taskDescriptionInput;
    QPushButton* addButton;
    QPushButton* deleteButton;
    QPushButton* editButton;
    QPushButton* markDoneButton;
    QPushButton* markInProgressButton;
    
    void setupUI();
    void loadTasks();
    void setupConfig();
    friend void add_task(const char* task_description);
}; 