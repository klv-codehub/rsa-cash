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
      N a ("42673456789546785646780793245678996553472546578676954747534367869706958473672635789607695847256378697-876545");
     QByteArray b = a.to_bytearray();
      N c = N::from_bytearray(b);
      qDebug() << a.to_str();
      qDebug() << c.to_str();

//    QString t;
//    for(int i = 0; b[i] != '\0'; i++)
//    {
//        qDebug() << b[i];
//        if(i) t+=' ';
//        for(int k = 7; k >=0; k--)
//            t+= (b[i]>>k & 1) + '0';
//    }


    //  qDebug() << b.toHex();
//    qDebug() << QCryptographicHash::hash(b, QCryptographicHash::Md5).toHex();


    QApplication app(argc, argv);
    MainWindow w;
    w.show();
//---------------------------------------------------------

//---------------------------------------------------------
    return app.exec();
}
