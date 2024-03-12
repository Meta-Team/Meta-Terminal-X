#pragma once
#include <QPlainTextEdit>
#include <iostream>
class ConsoleBox: public QPlainTextEdit{
    Q_OBJECT
    signals:
        void getData(const QByteArray &);
    public:
        ConsoleBox(QWidget*);
        ~ConsoleBox();
    protected:
        void keyPressEvent(QKeyEvent*) override;
        void mousePressEvent(QMouseEvent*) override;
        void mouseDoubleClickEvent(QMouseEvent*) override;
};