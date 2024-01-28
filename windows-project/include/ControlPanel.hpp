#pragma once
#include <ConsoleBox.hpp>
#include <DynamicChart.hpp>
#include <QMainWindow>
#include <QWidget>
#include <QtCharts>
#include <SerialConfigDialog.hpp>
#include <qchartview.h>
#include <qserialport.h>
#include <ui_ControlPanelWidget.h>

enum CHART_MOTOR {
	A_CHART, // Angle
	V_CHART, // Velocity
	I_CHART, // Current
	CHART_COUNT
};
class ControlPanel : public QWidget {
	Q_OBJECT
  signals:
	void showStatusMessage(const QString &, int);

  private:
	QString m_recvBuffer;
	Ui::ControlPanelWidgetUI *m_ui;
	ConsoleBox *m_ConsoleBox;
	SerialConfigDialog *m_SerialConfigdialog;
	QSerialPort *m_SerialPort;
	QValidator *m_InputValidator;
	DynamicChart *m_dynamicChartView[CHART_COUNT];
	float m_actualAngle = 0.0f, m_targetAngle = 0.0f;
	float m_actualVelocity = 0.0f, m_targetVelocity = 0.0f;
	int m_actualCurrent = 0, m_targetCurrent = 0;

  public:
	ControlPanel();
	~ControlPanel();
	void SerialConnect();
	void SerialDisconnect();
	bool isSerialOpen();
  public slots:
	void WriteData(const QByteArray &);
	void ReadData();
  private slots:
	void updateCOMLabel();
};