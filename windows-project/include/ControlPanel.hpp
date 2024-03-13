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
enum PID_TYPE{
	A2V,
	V2I,
	AUTOSTRAIGHTENING,
	PID_TYPE_COUNT
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
	int currentMotor = 0;
	// feedback motor data
	float m_actualAngle = 0.0f, m_targetAngle = 0.0f;
	float m_actualVelocity = 0.0f, m_targetVelocity = 0.0f;
	int m_actualCurrent = 0, m_targetCurrent = 0;
	// PID Params
	PID_TYPE m_pidType = V2I;
	float m_A2V_P = 0.0f, m_A2V_I = 0.0f, m_A2V_D = 0.0f, m_A2V_I_Limit = 0.0f, m_A2V_Out_Limit = 0.0f;
	float m_V2I_P = 0.0f, m_V2I_I = 0.0f, m_V2I_D = 0.0f, m_V2I_I_Limit = 0.0f, m_V2I_Out_Limit = 0.0f;
	float m_AutoStraightening_P = 0.0f, m_AutoStraightening_I = 0.0f, m_AutoStraightening_D = 0.0f, m_AutoStraightening_I_Limit = 0.0f, m_AutoStraightening_Out_Limit = 0.0f;

  public:
	ControlPanel();
	~ControlPanel();
	void SerialConnect();
	void SerialDisconnect();
	bool isSerialOpen();
  public slots:
	void WriteData(const QByteArray &);
	void ReadData();
	// functions for external window or widget to bind to
	void focusP();
	void focusI();
	void focusD();
	void increaseScale();
	void decreaseScale();
	void increaseAmount();
	void decreaseAmount();
  private slots:
	void updateCOMLabel();
	void updatePIDType(int);
	void updateCurrentMotor(int);
	void enableA2V();
	void enableV2I();
	void disableA2V();
	void disableV2I();
	void enableFB();
	void disableFB();
	void fetchParams();
	void sendParams();
	void sendAngle();
	void sendVelocity();
};