#include "mainwindow.h"
#include <QFile>
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    // Open Stylesheet file
    QFile styleSheetFile("./Combinear.qss");
    styleSheetFile.open(QFile::ReadOnly);
    // Store all the contents of the file in a string
    QString styleSheet = QLatin1String(styleSheetFile.readAll());
    // Load the stylesheet
    QApplication a(argc, argv);
    MainWindow w;
    w.setStyleSheet(styleSheet);
    w.show();

    return a.exec();
}
