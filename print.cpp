#include "print.h"

bool dprint(QString text)
{
    if( !DebugPrintQTE ) return false;

    DebugPrintQTE->moveCursor(QTextCursor::End);
    DebugPrintQTE->insertPlainText(text);
    DebugPrintQTE->moveCursor(QTextCursor::End);
    QApplication::processEvents();
    return true;
}

bool say(QString text, QString color)
{
    if( !ProtocolPrintQTE ) return false;


    ProtocolPrintQTE->moveCursor(QTextCursor::End);
    QTextCursor cursor( ProtocolPrintQTE->textCursor() );

    QTextCharFormat format;
   // format.setFontWeight( QFont::DemiBold );
    format.setForeground( QBrush( QColor( color ) ) );
    cursor.setCharFormat( format );
    cursor.insertText( text );

    ProtocolPrintQTE->moveCursor(QTextCursor::End);
    QApplication::processEvents();
    return true;
}
