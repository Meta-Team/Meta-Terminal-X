
#include <ControlPanel.hpp>
#include <QApplication>
#include <QMessageBox>
#include <TerminalWindow.hpp>

TerminalWindow::TerminalWindow() : m_ui(new Ui::TerminalWindow()), m_ControlPanel(new ControlPanel()) {
	m_ui->setupUi(this);
	// m_ui->statusbar->showMessage("Hello",2000);
	connect(m_ControlPanel, &ControlPanel::showStatusMessage, this, &TerminalWindow::showStatusMessage);
	setCentralWidget(m_ControlPanel);
}
TerminalWindow::~TerminalWindow() {
	delete m_ui;
	delete m_ControlPanel;
}
void TerminalWindow::closeEvent(QCloseEvent *event) {
	if (!this->m_ControlPanel->isSerialOpen()) {
		// QMessageBox::StandardButton resBtn =
		// 	QMessageBox::question(this, "Confirm", tr("Are you sure?\n"), QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);
		// if (resBtn != QMessageBox::Yes) {
		// 	event->ignore();
		// } else {
		// 	event->accept();
		// }
	} else {
		showStatusMessage("please disconnect serialport first", 2000);
		event->ignore();
	}
}
void TerminalWindow::showStatusMessage(const QString &msg, int timeout) { m_ui->statusbar->showMessage(msg, timeout); }