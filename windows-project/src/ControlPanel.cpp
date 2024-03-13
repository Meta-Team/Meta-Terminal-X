#include "ui_ControlPanelWidget.h"
#include <ConsoleBox.hpp>
#include <ControlPanel.hpp>
#include <QtWidgets/qcombobox.h>
#include <SerialConfigDialog.hpp>
#include <TerminalWindow.hpp>
#include <iostream>
#include <ostream>
#include <qdialog.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qserialport.h>
#include <qserialportinfo.h>
#include <qslider.h>
#include <qvalidator.h>
#include <qwindowdefs.h>
ControlPanel::ControlPanel()
	: m_recvBuffer(""), m_ui(new Ui::ControlPanelWidgetUI()), m_ConsoleBox(new ConsoleBox(this)),
	  m_SerialConfigdialog(new SerialConfigDialog(this)), m_SerialPort(new QSerialPort(this)) {
	m_ui->setupUi(this);

	// add charts
	m_ui->verticalLayout_Console->addWidget(m_ConsoleBox);
	m_dynamicChartView[A_CHART] = new DynamicChart(this, "Angle");
	m_dynamicChartView[A_CHART]->setYRange(-190.0f, 190.0f);
	m_dynamicChartView[V_CHART] = new DynamicChart(this, "Velocity");
	m_dynamicChartView[V_CHART]->setYRange(-500.0f, 500.0f);
	m_dynamicChartView[I_CHART] = new DynamicChart(this, "Current");
	m_dynamicChartView[I_CHART]->setYRange(-500.f, 500.f);
	m_ui->hL_Charts->addWidget(m_dynamicChartView[A_CHART]);
	m_ui->hL_Charts->addWidget(m_dynamicChartView[V_CHART]);
	m_ui->hL_Charts->addWidget(m_dynamicChartView[I_CHART]);
	// add Motors Conbobox
	for (int i = 0; i < 6; i++) {
		m_ui->comboBox_CurrentMotor->addItem(tr("%1").arg(i), 0);
	}
	m_ui->comboBox_CurrentMotor->setCurrentIndex(5);
	this->currentMotor = 5;
	connect(m_ui->comboBox_CurrentMotor, SIGNAL(currentIndexChanged(int)), this, SLOT(updateCurrentMotor(int)));
	// add PIDType Combobox
	m_ui->comboBox_PIDType->addItem("A2V", A2V);
	m_ui->comboBox_PIDType->addItem("V2I", V2I);
	m_ui->comboBox_PIDType->addItem("AutoStraightening", AUTOSTRAIGHTENING);
	m_ui->comboBox_PIDType->setCurrentIndex(V2I);
	updatePIDType(this->m_pidType);
	connect(m_ui->comboBox_PIDType, SIGNAL(currentIndexChanged(int)), this, SLOT(updatePIDType(int)));
	// command buttons bindings
	connect(m_ui->pushButton_EnableA2V, &QPushButton::clicked, this, &ControlPanel::enableA2V);
	connect(m_ui->pushButton_EnableV2I, &QPushButton::clicked, this, &ControlPanel::enableV2I);
	connect(m_ui->pushButton_DisableA2V, &QPushButton::clicked, this, &ControlPanel::disableA2V);
	connect(m_ui->pushButton_DisableV2I, &QPushButton::clicked, this, &ControlPanel::disableV2I);
	connect(m_ui->pushButton_EnableFeedback, &QPushButton::clicked, this, &ControlPanel::enableFB);
	connect(m_ui->pushButton_DisableFeedback, &QPushButton::clicked, this, &ControlPanel::disableFB);
	connect(m_ui->pushButton_FetchParams, &QPushButton::clicked, this, &ControlPanel::fetchParams);
	connect(m_ui->pushButton_SendParams, &QPushButton::clicked, this, &ControlPanel::sendParams);
	connect(m_ui->pushButton_SendAngle, &QPushButton::clicked, this, &ControlPanel::sendAngle);
	connect(m_ui->pushButton_SendVelocity, &QPushButton::clicked, this, &ControlPanel::sendVelocity);

	m_InputValidator = new QDoubleValidator(0.0, 1.0, 2, this);

	connect(m_ui->pushButton_SerialConfig, &QPushButton::clicked, m_SerialConfigdialog, &QDialog::show);
	connect(m_ui->pushButton_SerialConnect, &QPushButton::clicked, this, &ControlPanel::SerialConnect);
	connect(m_SerialPort, &QSerialPort::readyRead, this, &ControlPanel::ReadData);
	connect(m_ConsoleBox, &ConsoleBox::getData, this, &ControlPanel::WriteData);

	// connect signals from PID UI
	// update UI: COM status
	connect(m_SerialConfigdialog, &SerialConfigDialog::updateCOMLabel, this, &ControlPanel::updateCOMLabel);
	// disable consolebox
	m_ConsoleBox->setEnabled(false);
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
		// enable all command buttons
		m_ui->pushButton_EnableA2V->setEnabled(true);
		m_ui->pushButton_EnableV2I->setEnabled(true);
		m_ui->pushButton_DisableA2V->setEnabled(true);
		m_ui->pushButton_DisableV2I->setEnabled(true);
		m_ui->pushButton_EnableFeedback->setEnabled(true);
		m_ui->pushButton_DisableFeedback->setEnabled(true);
		m_ui->pushButton_FetchParams->setEnabled(true);
		m_ui->pushButton_SendParams->setEnabled(true);
		m_ui->pushButton_SendAngle->setEnabled(true);
		m_ui->pushButton_SendVelocity->setEnabled(true);
		// to print hello info and clear garbage data in the Receiver's buffer
		m_SerialPort->write("hello\r\n");
		// get PID params on startup
		for(PID_TYPE pidType = A2V; pidType < PID_TYPE_COUNT; pidType = (PID_TYPE)(pidType + 1)){
			fetchParamsSpecified(pidType);
		}
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
		// disable all command buttons
		m_ui->pushButton_EnableA2V->setEnabled(false);
		m_ui->pushButton_EnableV2I->setEnabled(false);
		m_ui->pushButton_DisableA2V->setEnabled(false);
		m_ui->pushButton_DisableV2I->setEnabled(false);
		m_ui->pushButton_EnableFeedback->setEnabled(false);
		m_ui->pushButton_DisableFeedback->setEnabled(false);
		m_ui->pushButton_FetchParams->setEnabled(false);
		m_ui->pushButton_SendParams->setEnabled(false);
		m_ui->pushButton_SendAngle->setEnabled(false);
		m_ui->pushButton_SendVelocity->setEnabled(false);
		this->showStatusMessage("Serial disconnected", 1000);
	} else {
		this->showStatusMessage("Already disconnected", 1000);
	}
}
void ControlPanel::ReadData() {
	// directly get data from console

	m_recvBuffer += m_SerialPort->readAll();
	unsigned long long strBegin, strEnd;
	// printf("Begin>>>\n%s<<<End\n", m_recvBuffer.toStdString().c_str());
	// printf("BufferSize=%d\n", m_recvBuffer.length());

	// digest a big group of lines using loop
	while ((strBegin = m_recvBuffer.indexOf("!fb,")) != -1 && (strEnd = m_recvBuffer.indexOf("\r\n", strBegin)) != -1) {
		// template
		// !fb,%u,%u,%.2f,%.2f,%.2f,%.2f,%d,%d
		// example data
		// !fb,1187120,5,8.43,0.00,0.00,0.00,52,0
		// systime, motorId
		// angle: actual,target
		// velocity: actual,target
		// torque_current: actual,target
		// exclude `!fb,` and `\r\n`
		QString feedbackStr = m_recvBuffer.mid(strBegin + 4, strEnd - strBegin);
		unsigned timestamp = 0, motorId = 0;
		sscanf(feedbackStr.toStdString().c_str(), "%u,%u,%f,%f,%f,%f,%d,%d", &timestamp, &motorId, &m_actualAngle, &m_targetAngle,
			   &m_actualVelocity, &m_targetVelocity, &m_actualCurrent, &m_targetCurrent);
		// m_ConsoleBox->insertPlainText("FeedBack:" + feedbackStr + "\n");
		m_recvBuffer = m_recvBuffer.mid(0, strBegin) + m_recvBuffer.mid(strEnd + 2);
		m_dynamicChartView[A_CHART]->updateData(timestamp, m_actualAngle, m_targetAngle);
		m_dynamicChartView[V_CHART]->updateData(timestamp, m_actualVelocity, m_targetVelocity);
		m_dynamicChartView[I_CHART]->updateData(timestamp, m_actualCurrent, m_targetCurrent);
		// printf("Dropped!\nNow>>>\n%s<<<End\n", m_recvBuffer.toStdString().c_str());
		// printf("Time:%u,MotorId=%u\nactual,target\nangle(%.2f,%.2f)\nvelocity(%.2f,%.2f)\ncurrent(%d,%d)\n", timestamp,
		// motorId,
		//    m_actualAngle, m_targetAngle, m_actualVelocity, m_targetVelocity, m_actualCurrent, m_targetCurrent);
		// printf("ShrinkedBufferSize=%d\n", m_recvBuffer.length());
	}
	while ((strBegin = m_recvBuffer.indexOf("!kp,")) != -1 && (strEnd = m_recvBuffer.indexOf("\r\n", strBegin)) != -1) {
		// !kp,%u,%u,%.2f,ki:%.2f,kd:%.2f,il:%.2f,ol:%.2f
		// example data
		QString feedbackStr = m_recvBuffer.mid(strBegin + 4, strEnd - strBegin);
		unsigned tmp_pidType = 0, motorId = 0;
		float tmp_P = 0.0f, tmp_I = 0.0f, tmp_D = 0.0f, tmp_I_Limit = 0.0f, tmp_Out_Limit = 0.0f;
		sscanf(feedbackStr.toStdString().c_str(), "%u,%u,%f,%f,%f,%f,%f", &motorId, &tmp_pidType, &tmp_P, &tmp_I, &tmp_D,
			   &tmp_I_Limit, &tmp_Out_Limit);

		m_P[tmp_pidType] = tmp_P;
		m_I[tmp_pidType] = tmp_I;
		m_D[tmp_pidType] = tmp_D;
		m_I_Limit[tmp_pidType] = tmp_I_Limit;
		m_Out_Limit[tmp_pidType] = tmp_Out_Limit;

		if ((PID_TYPE)tmp_pidType == m_pidType) {
			// update UI
			m_ui->lineEdit_Saved_P->setText(QString::number(tmp_P));
			m_ui->lineEdit_Saved_I->setText(QString::number(tmp_I));
			m_ui->lineEdit_Saved_D->setText(QString::number(tmp_D));
			m_ui->lineEdit_Saved_I_Limit->setText(QString::number(tmp_I_Limit));
			m_ui->lineEdit_Saved_Out_Limit->setText(QString::number(tmp_Out_Limit));
			// also exact_UI
			m_ui->label_ExactP->setText(QString::number(tmp_P));
			m_ui->label_ExactI->setText(QString::number(tmp_I));
			m_ui->label_ExactD->setText(QString::number(tmp_D));
			m_ui->label_ExactI_Limit->setText(QString::number(tmp_I_Limit));
			m_ui->label_ExactOut_Limit->setText(QString::number(tmp_Out_Limit));
		}
		m_ConsoleBox->insertPlainText("FeedBack:" + feedbackStr + "\n");
		m_recvBuffer = m_recvBuffer.mid(0, strBegin) + m_recvBuffer.mid(strEnd + 2);
	}
	while ((strEnd = m_recvBuffer.indexOf("\r\n")) != -1) {
		// Now all commands are resolved, then the rest is normal message
		m_ConsoleBox->insertPlainText(m_recvBuffer.mid(0, strEnd) + "\n");
		m_recvBuffer = m_recvBuffer.mid(strEnd + 2);
	}
}
void ControlPanel::WriteData(const QByteArray &data) { m_SerialPort->write(data); }
bool ControlPanel::isSerialOpen() { return this->m_SerialPort->isOpen(); }
void ControlPanel::updateCOMLabel() {
	const auto p = m_SerialConfigdialog->settings();
	QString label("%1@%2bps");
	m_ui->labelCOMstatus->setText(label.arg(p.name).arg(p.baudRate));
}
void ControlPanel::focusP() { m_ui->lineEdit_Saved_P->setFocus(); }
void ControlPanel::focusI() { m_ui->lineEdit_Saved_I->setFocus(); }
void ControlPanel::focusD() { m_ui->lineEdit_Saved_D->setFocus(); }
void ControlPanel::increaseScale() { this->showStatusMessage("FocusP", 2000); }
void ControlPanel::decreaseScale() { this->showStatusMessage("FocusP", 2000); }
void ControlPanel::increaseAmount() { this->showStatusMessage("FocusP", 2000); }
void ControlPanel::decreaseAmount() { this->showStatusMessage("FocusP", 2000); }
void ControlPanel::updateCurrentMotor(int index) {
	this->currentMotor = index;
	showStatusMessage(tr("Current Motor: %1").arg(index), 1000);
}
void ControlPanel::enableA2V() { m_SerialPort->write(tr("set_enable_a %1\r\n").arg(currentMotor).toLocal8Bit()); }
void ControlPanel::enableV2I() { m_SerialPort->write(tr("set_enable_v %1\r\n").arg(currentMotor).toLocal8Bit()); }
void ControlPanel::disableA2V() { m_SerialPort->write(tr("set_disable_a %1\r\n").arg(currentMotor).toLocal8Bit()); }
void ControlPanel::disableV2I() { m_SerialPort->write(tr("set_disable_v %1\r\n").arg(currentMotor).toLocal8Bit()); }
void ControlPanel::enableFB() { m_SerialPort->write(tr("fb_enable %1\r\n").arg(currentMotor).toLocal8Bit()); }
void ControlPanel::disableFB() { m_SerialPort->write(tr("fb_disable %1\r\n").arg(currentMotor).toLocal8Bit()); }
void ControlPanel::fetchParams() { m_SerialPort->write(tr("echo_pid %1 %2\r\n").arg(currentMotor).arg(m_pidType).toLocal8Bit()); }
void ControlPanel::fetchParamsSpecified(int pidType) {
	m_SerialPort->write(tr("echo_pid %1 %2\r\n").arg(currentMotor).arg(pidType).toLocal8Bit());
}
void ControlPanel::sendParams() {
	m_P[m_pidType] = m_ui->lineEdit_Saved_P->text().toFloat();
	m_I[m_pidType] = m_ui->lineEdit_Saved_I->text().toFloat();
	m_D[m_pidType] = m_ui->lineEdit_Saved_D->text().toFloat();
	m_I_Limit[m_pidType] = m_ui->lineEdit_Saved_I_Limit->text().toFloat();
	m_Out_Limit[m_pidType] = m_ui->lineEdit_Saved_Out_Limit->text().toFloat();

	m_SerialPort->write(tr("set_pid %1 %2 %3 %4 %5 %6 %7\r\n")
							.arg(currentMotor)
							.arg(m_pidType)
							.arg(m_P[m_pidType])
							.arg(m_I[m_pidType])
							.arg(m_D[m_pidType])
							.arg(m_I_Limit[m_pidType])
							.arg(m_Out_Limit[m_pidType])
							.toLocal8Bit());
}
void ControlPanel::updatePIDType(int type) {
	m_pidType = (PID_TYPE)type;
	m_ui->lineEdit_Saved_P->setText(QString::number(m_P[m_pidType]));
	m_ui->lineEdit_Saved_I->setText(QString::number(m_I[m_pidType]));
	m_ui->lineEdit_Saved_D->setText(QString::number(m_D[m_pidType]));
	m_ui->lineEdit_Saved_I_Limit->setText(QString::number(m_I_Limit[m_pidType]));
	m_ui->lineEdit_Saved_Out_Limit->setText(QString::number(m_Out_Limit[m_pidType]));
	m_ui->label_ExactP->setText(QString::number(m_P[m_pidType]));
	m_ui->label_ExactI->setText(QString::number(m_I[m_pidType]));
	m_ui->label_ExactD->setText(QString::number(m_D[m_pidType]));
	m_ui->label_ExactI_Limit->setText(QString::number(m_I_Limit[m_pidType]));
	m_ui->label_ExactOut_Limit->setText(QString::number(m_Out_Limit[m_pidType]));
}
void ControlPanel::sendAngle() {
	m_SerialPort->write(
		tr("set_target_angle %1 %2\r\n").arg(currentMotor).arg(m_ui->lineEdit_Saved_TargetA->text().toFloat()).toLocal8Bit());
}
void ControlPanel::sendVelocity() {
	m_SerialPort->write(
		tr("set_target_vel %1 %2\r\n").arg(currentMotor).arg(m_ui->lineEdit_Saved_TargetV->text().toFloat()).toLocal8Bit());
}