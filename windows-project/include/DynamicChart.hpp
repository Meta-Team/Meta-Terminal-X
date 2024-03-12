#pragma once
#include <QtCharts>
#include <iostream>
#include <qlineseries.h>

//#define LineType QSplineSeries
#define LineType QLineSeries
class DynamicChart : public QChartView {
  private:
	QString m_chartName;
	LineType *m_seriesActual, *m_seriesTarget;
	QChart *m_chart;
	QChart *createLineChart();
	float lastTimestamp;


  public:
	DynamicChart(QWidget *, const QString &);
	void updateData(unsigned timestamp, float actualData, float targetData);
	void setYRange(float min, float max);
	void setXRange(float min, float max);
};
