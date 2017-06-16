#ifndef DEBUG_H
#define DEBUG_H

#include <QApplication>
#include <QTextEdit>

extern QTextEdit *DebugPrintQTE;
extern QTextEdit *ProtocolPrintQTE;

bool dprint(QString);
bool say(QString text, QString color);

#endif // DEBUG_H
