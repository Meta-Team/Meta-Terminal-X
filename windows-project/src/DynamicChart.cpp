#include <DynamicChart.hpp>
#include <iostream>
#include <qglobal.h>
#include <qnamespace.h>
#include <qrandom.h>
#include <stdlib.h>

QChart *DynamicChart::createLineChart() {

	m_chart = new QChart();

	m_seriesActual = new LineType(m_chart);
	m_seriesTarget = new LineType(m_chart);
	m_seriesActual->setName("actual");
	m_seriesTarget->setName("target");
	// for (int i = 0; i < 10; i++) {
	// 	m_seriesActual->append(i, i * i);
	// 	m_seriesTarget->append(i, (i + 1) * (i + 1));
	// }

	m_chart->addSeries(m_seriesActual);
	m_chart->addSeries(m_seriesTarget);

	// set axis auto range

	m_chart->createDefaultAxes();
	// x-axis range
	// default 5.0s
	setXRange(0.0f, 5.0f);
	setYRange(-190.0f, 190.0f);
	// auto range adaption

	// QValueAxis *axisY = qobject_cast<QValueAxis
	// *>(chart->axes(Qt::Vertical).first()); Q_ASSERT(axisY);
	((QValueAxis *)(m_chart->axes(Qt::Vertical).first()))->setLabelFormat("%.1f  ");
	return m_chart;
}
DynamicChart::DynamicChart(QWidget *parent, const QString &name)
	: QChartView(createLineChart()), m_chartName(name) {
	m_chart->setTitle(m_chartName);
	setParent(parent);
	setRenderHint(QPainter::Antialiasing, true);
}
/**
 * @brief updateData update data of the chart, and remove potential out-of-range data
 * @param timestamp in ms
 * @param actualData float
 * @param targetData float
 */
void DynamicChart::updateData(unsigned timestamp, float actualData, float targetData) {
	lastTimestamp = timestamp / 1000.f;
	setXRange(lastTimestamp - 5.0f, lastTimestamp);
	m_seriesActual->append((float)timestamp / 1000.f, actualData);
	m_seriesTarget->append((float)timestamp / 1000.f, targetData);

	float minY = 0.0f;
	float maxY = 0.0f;
	int shownId = 0;
	while (1) {
		if (m_seriesActual->at(0).x() < lastTimestamp - 5.0f) {
			// delete potential out-of-range data
			m_seriesActual->remove(0);
			m_seriesTarget->remove(0);
		} else {
			if (shownId >= m_seriesActual->count()) {
				// then all data are calculated min and max
				break;
			}
			m_seriesActual->at(shownId).y() < minY ? minY = m_seriesActual->at(shownId).y() : minY;
			m_seriesTarget->at(shownId).y() < minY ? minY = m_seriesTarget->at(shownId).y() : minY;
			m_seriesActual->at(shownId).y() > maxY ? maxY = m_seriesActual->at(shownId).y() : maxY;
			m_seriesTarget->at(shownId).y() > maxY ? maxY = m_seriesTarget->at(shownId).y() : maxY;
			shownId++;
		}
	}
	// set chart to adapt y axis range automatically
	setYRange(minY - (maxY - minY) * 0.1, maxY + (maxY - minY) * 0.1);
}
// tool function for setting y range
void DynamicChart::setYRange(float min, float max) {
	m_chart->axes(Qt::Vertical).first()->setRange(min, max);
}
// tool function for setting x range
void DynamicChart::setXRange(float min, float max) {
	m_chart->axes(Qt::Horizontal).first()->setRange(min, max);
}