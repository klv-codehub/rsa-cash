#include "gui_mainwindow.h"
#include <QApplication>

#include <QCryptographicHash>
#include <QDebug>
#include <string>
#include "integers.h"

#include "print.h"
QTextEdit *DebugPrintQTE = NULL;
QTextEdit *ProtocolPrintQTE = NULL;

int main(int argc, char *argv[])
{
    N a ("4276545");
    QByteArray b = a.to_bytearray();
//    int i = b.length() - 1;
//    while(i >= 0)
//    {
//        for(int k = 0; k < 8; k++)
//            t.insert(0, (b[i]>>k & 1) + '0');
//        i--;
//    }

    QString t;
    for(int i = 0; b[i] != '\0'; i++)
    {
        qDebug() << b[i];
        if(i) t+=' ';
        for(int k = 7; k >=0; k--)
            t+= (b[i]>>k & 1) + '0';
    }
//    b.clear();
//    b+='a';
//    b+='b';
//    b+='c';
//    QString c = "(abc)";

    qDebug() << t;

    qDebug() << QString::fromStdString(QCryptographicHash::hash(b, QCryptographicHash::Md5).toStdString());


    QApplication app(argc, argv);
    MainWindow w;
    w.show();
//---------------------------------------------------------

//---------------------------------------------------------
    return app.exec();
}
