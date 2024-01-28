#include "ui_ControlPanelWidget.h"
#include <ConsoleBox.hpp>
#include <ControlPanel.hpp>
#include <SerialConfigDialog.hpp>
#include <TerminalWindow.hpp>
#include <iostream>
#include <ostream>
#include <qdialog.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qserialport.h>
#include <qserialportinfo.h>
#include <qvalidator.h>
#include <qwindowdefs.h>
ControlPanel::ControlPanel()
	: m_recvBuffer(""), m_ui(new Ui::ControlPanelWidgetUI()), m_ConsoleBox(new ConsoleBox(this)),
	  m_SerialConfigdialog(new SerialConfigDialog(this)), m_SerialPort(new QSerialPort(this)) {
	m_ui->setupUi(this);
	m_ui->verticalLayout_Console->addWidget(m_ConsoleBox);
	m_dynamicChartView[A_CHART] = new DynamicChart(this, "Angle");
	m_dynamicChartView[V_CHART] = new DynamicChart(this, "Velocity");
	m_dynamicChartView[I_CHART] = new DynamicChart(this, "Current");

	m_ui->hL_Charts->addWidget(m_dynamicChartView[A_CHART]);
	m_ui->hL_Charts->addWidget(m_dynamicChartView[V_CHART]);
	m_ui->hL_Charts->addWidget(m_dynamicChartView[I_CHART]);

	// disable input of consolebox

	m_InputValidator = new QDoubleValidator(0.0, 1.0, 2, this);

	connect(m_ui->pushButton_SerialConfig, &QPushButton::clicked, m_SerialConfigdialog, &QDialog::show);
	connect(m_ui->pushButton_SerialConnect, &QPushButton::clicked, this, &ControlPanel::SerialConnect);
	connect(m_SerialPort, &QSerialPort::readyRead, this, &ControlPanel::ReadData);
	connect(m_ConsoleBox, &ConsoleBox::getData, this, &ControlPanel::WriteData);
	// update UI: COM status
	connect(m_SerialConfigdialog, &SerialConfigDialog::updateCOMLabel, this, &ControlPanel::updateCOMLabel);
	// disable consolebox
	m_ConsoleBox->setEnabled(false);
	m_ui->horizontalLayout_8->setEnabled(false);
	// update COMStatus Label
	updateCOMLabel();
}
ControlPanel::~ControlPanel() {
	delete m_ui;
	delete m_ConsoleBox;
	delete m_SerialConfigdialog;
	delete m_dynamicChartView[A_CHART];
	delete m_dynamicChartView[V_CHART];
	delete m_dynamicChartView[I_CHART];
}
void ControlPanel::SerialConnect() {
	const auto p = m_SerialConfigdialog->settings();
	m_SerialPort->setPortName(p.name);
	m_SerialPort->setBaudRate(p.baudRate);
	m_SerialPort->setDataBits(p.dataBits);
	m_SerialPort->setParity(p.parity);
	m_SerialPort->setStopBits(p.stopBits);
	m_SerialPort->setFlowControl(p.flowControl);
	std::cout << "connect" << p.name.toStdString() << std::endl;
	if (m_SerialPort->open(QIODevice::ReadWrite)) {

		// reuse the original `connect` button as `disconnect`
		connect(m_ui->pushButton_SerialConnect, &QPushButton::clicked, this, &ControlPanel::SerialDisconnect);
		disconnect(m_ui->pushButton_SerialConnect, &QPushButton::clicked, this, &ControlPanel::SerialConnect);
		m_ui->pushButton_SerialConnect->setText("Disconnect");
		m_ui->pushButton_SerialConfig->setEnabled(false);
		m_ConsoleBox->setEnabled(true);
		for (auto &&child : m_ui->gL_PID->findChildren<QSlider *>()) {
			child->setEnabled(true);
		}
		// m_console->setLocalEchoEnabled(p.localEchoEnabled);
		// m_ui->actionConnect->setEnabled(false);
		// m_ui->actionDisconnect->setEnabled(true);
		// m_ui->actionConfigure->setEnabled(false);

		this->showStatusMessage(tr("Connected successfully%1").arg(p.name), 1000);
	} else {
		QMessageBox::critical(this, "Error", m_SerialPort->errorString());
		emit showStatusMessage(tr("Open error"), 1000);
	}
}
void ControlPanel::SerialDisconnect() {
	if (m_SerialPort->isOpen()) {
		m_SerialPort->close();
		// reuse the original `connect` button as `connect`
		disconnect(m_ui->pushButton_SerialConnect, &QPushButton::clicked, this, &ControlPanel::SerialDisconnect);
		connect(m_ui->pushButton_SerialConnect, &QPushButton::clicked, this, &ControlPanel::SerialConnect);
		m_ui->pushButton_SerialConnect->setText("Connect");
		m_ui->pushButton_SerialConfig->setEnabled(true);
		m_ConsoleBox->setEnabled(false);
		for (auto &&child : m_ui->gL_PID->findChildren<QSlider *>()) {
			child->setEnabled(false);
		}
		this->showStatusMessage("Serial disconnected", 1000);
	} else {
		this->showStatusMessage("Already disconnected", 1000);
	}
}
void ControlPanel::ReadData() {
	// directly get data from console

	// template
	// !fb,%u,%u,%.2f,%.2f,%.2f,%.2f,%d,%d
	// example data
	// !fb,12353,0,23.67,41.78,340.01,-14.28,1422,3546
	// angle: actual,target
	// velocity: actual,target
	// torque_current: actual,target
	m_recvBuffer += m_SerialPort->readAll();
	unsigned long long strBegin, strEnd;
	// printf("Begin>>>\n%s<<<End\n", m_recvBuffer.toStdString().c_str());
	if ((strBegin = m_recvBuffer.indexOf("!fb,")) != -1 && (strEnd = m_recvBuffer.indexOf("\r\n", strBegin)) != -1) {
		// exclude `!fb,` and `\r\n`
		QString feedbackStr = m_recvBuffer.mid(strBegin + 4, strEnd - strBegin);
		unsigned int timestamp = 0, motorId = 0;
		sscanf_s(feedbackStr.toStdString().c_str(), "%u,%u,%f,%f,%f,%f,%d,%d", &timestamp, &motorId, &m_actualAngle,
				 &m_targetAngle, &m_actualVelocity, &m_targetVelocity, &m_actualCurrent, &m_targetCurrent);
		m_ConsoleBox->insertPlainText("FeedBack:" + feedbackStr + "\n");
		m_recvBuffer = m_recvBuffer.mid(strEnd + 2);
		// printf("Dropped!\nNow>>>\n%s<<<End\n", m_recvBuffer.toStdString().c_str());
		// printf("Time:%u,MotorId=%u\nactual,target\nangle(%.2f,%.2f)\nvelocity(%.2f,%.2f)\ncurrent(%d,%d)\n", timestamp, motorId,
		// 	   m_actualAngle, m_targetAngle, m_actualVelocity, m_targetVelocity, m_actualCurrent, m_targetCurrent);
	}
}
void ControlPanel::WriteData(const QByteArray &data) {
	m_dynamicChartView[A_CHART]->popBack();
	m_SerialPort->write(data);
}
bool ControlPanel::isSerialOpen() { return this->m_SerialPort->isOpen(); }
void ControlPanel::updateCOMLabel() {
	const auto p = m_SerialConfigdialog->settings();
	QString label("%1@%2bps");
	m_ui->labelCOMstatus->setText(label.arg(p.name).arg(p.baudRate));
}
