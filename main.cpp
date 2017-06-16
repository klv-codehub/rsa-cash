#include "mainwindow.h"
#include <QApplication>

#include "common.h"
QTextEdit *DebugPrintQTE = NULL;
QTextEdit *ProtocolPrintQTE = NULL;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
//---------------------------------------------------------

//---------------------------------------------------------
    return app.exec();
}
