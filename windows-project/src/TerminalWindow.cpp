
#include <ControlPanel.hpp>
#include <QApplication>
#include <QMessageBox>
#include <TerminalWindow.hpp>

TerminalWindow::TerminalWindow() : m_ui(new Ui::TerminalWindow()), m_ControlPanel(new ControlPanel()) {
	m_ui->setupUi(this);
	// m_ui->statusbar->showMessage("Hello",2000);
	connect(m_ControlPanel, &ControlPanel::showStatusMessage, this, &TerminalWindow::showStatusMessage);
	bindControlPanel();
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
void TerminalWindow::bindControlPanel() {
	connect(m_ui->actionAdjustP, SIGNAL(triggered()), m_ControlPanel, SLOT(focusP()));
	connect(m_ui->actionAdjustI, SIGNAL(triggered()), m_ControlPanel, SLOT(focusI()));
	connect(m_ui->actionAdjustD, SIGNAL(triggered()), m_ControlPanel, SLOT(focusD()));
	connect(m_ui->actionDecreaseAmount, SIGNAL(triggered()), m_ControlPanel, SLOT(decreaseAmount()));
	connect(m_ui->actionDecreaseScale, SIGNAL(triggered()), m_ControlPanel, SLOT(decreaseScale()));
	connect(m_ui->actionIncreaseAmount, SIGNAL(triggered()), m_ControlPanel, SLOT(increaseAmount()));
	connect(m_ui->actionIncreaseScale, SIGNAL(triggered()), m_ControlPanel, SLOT(increaseScale()));
}
void TerminalWindow::unbindControlPanel() {
	disconnect(m_ui->actionAdjustP, SIGNAL(triggered()), m_ControlPanel, SLOT(focusP()));
	disconnect(m_ui->actionAdjustI, SIGNAL(triggered()), m_ControlPanel, SLOT(focusI()));
	disconnect(m_ui->actionAdjustD, SIGNAL(triggered()), m_ControlPanel, SLOT(focusD()));
	disconnect(m_ui->actionDecreaseAmount, SIGNAL(triggered()), m_ControlPanel, SLOT(decreaseAmount()));
	disconnect(m_ui->actionDecreaseScale, SIGNAL(triggered()), m_ControlPanel, SLOT(decreaseScale()));
	disconnect(m_ui->actionIncreaseAmount, SIGNAL(triggered()), m_ControlPanel, SLOT(increaseAmount()));
	disconnect(m_ui->actionIncreaseScale, SIGNAL(triggered()), m_ControlPanel, SLOT(increaseScale()));
}