#include "mainwindow.h"
#include <QApplication>

#include "common.h"
QTextEdit *DebugPrintQTE = NULL;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
//---------------------------------------------------------
    w.logWrite("Yea! It Works!\n");
//---------------------------------------------------------
    return app.exec();
}
