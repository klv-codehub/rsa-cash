#include "debug.h"

bool dprint(std::string text)
{
    if( !DebugPrintQTE ) return false;

    DebugPrintQTE->moveCursor(QTextCursor::End);
    DebugPrintQTE->insertPlainText(QString::fromStdString(text));
    DebugPrintQTE->moveCursor(QTextCursor::End);
    QApplication::processEvents();
    return true;
}
