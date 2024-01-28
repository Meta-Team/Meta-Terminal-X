#pragma once
#include <QMainWindow>
#include <ui_TerminalWindow.h>
#include <ControlPanel.hpp>
#include <ConsoleBox.hpp>
class TerminalWindow:public QMainWindow{
    Q_OBJECT
    private:
    Ui::TerminalWindow* m_ui;
    ControlPanel* m_ControlPanel;
    public:
        TerminalWindow();
        ~TerminalWindow();
    protected:
        void closeEvent(QCloseEvent*) override;
    public slots:
        void showStatusMessage(const QString&, int);
};