#pragma once
#include <QtCharts>
#include <iostream>
#include <qlineseries.h>

#define LineType QSplineSeries
class DynamicChart : public QChartView {
  private:
	QString m_chartName;
	LineType *m_seriesActual, *m_seriesTarget;
	QChart *m_chart;
	QChart *createLineChart();

  public:
	DynamicChart(QWidget *, const QString &);
	void popBack();
	void appendData();
};
