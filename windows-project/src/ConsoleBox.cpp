#include <ConsoleBox.hpp>
ConsoleBox::ConsoleBox(QWidget* parent):
QPlainTextEdit(parent)
{
    document()->setMaximumBlockCount(100);
    QPalette p = palette();
    p.setColor(QPalette::Base,Qt::black);
    p.setColor(QPalette::Text, Qt::green);
    setPalette(p);
}
ConsoleBox::~ConsoleBox(){
    
}
void ConsoleBox::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Backspace:
    case Qt::Key_Left:
    case Qt::Key_Right:
    case Qt::Key_Up:
    case Qt::Key_Down:
        break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        QPlainTextEdit::keyPressEvent(e);
        // std::cout<<"Pressed Enter:datalength="<<e->text().toStdString().length()<<std::endl;
        emit getData(tr("\r\n").toLocal8Bit());
        break;
    default:
        // std::cout<<"Pressed Plain:datalength="<<e->text().toStdString().length()<<std::endl;
        QPlainTextEdit::keyPressEvent(e);
        emit getData(e->text().toLocal8Bit());
    }
}