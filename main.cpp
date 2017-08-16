#include "gui_mainwindow.h"
#include <QApplication>

#include <QCryptographicHash>
#include <QDebug>
#include <string>
#include "integers.h"

#include "print.h"
QTextEdit *DebugPrintQTE = NULL;
QTextEdit *ProtocolPrintQTE = NULL;

N hash(const N& X)
{
    return N::from_bytearray(
                QCryptographicHash::hash(
                    X.to_bytearray(),
                    QCryptographicHash::Md5));
}

int main(int argc, char *argv[])
{
      qDebug() << hash(65).to_str();
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
