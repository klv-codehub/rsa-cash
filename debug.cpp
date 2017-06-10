#include "debug.h"

bool dprint(QString text)
{
    if( !DebugPrintQTE ) return false;

    DebugPrintQTE->moveCursor(QTextCursor::End);
    DebugPrintQTE->insertPlainText(text);
    DebugPrintQTE->moveCursor(QTextCursor::End);
    QApplication::processEvents();
    return true;
}
