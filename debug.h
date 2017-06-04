#ifndef DEBUG_H
#define DEBUG_H

#include <QApplication>
#include <QTextEdit>
#include <string>

extern QTextEdit *DebugPrintQTE;

bool dprint(std::string text);

#endif // DEBUG_H
