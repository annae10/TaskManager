#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;



    //w.setCentralWidget(openGLWidget);
    //window.resize(800, 600);

    w.show();
    return a.exec();
}
