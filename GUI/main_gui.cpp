#include <QtWidgets/QApplication>
#include "TaskyMainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    TaskyMainWindow window;
    window.show();
    return app.exec();
} 