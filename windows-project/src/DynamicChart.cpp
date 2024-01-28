#include <DynamicChart.hpp>
#include <iostream>
#include <qrandom.h>
#include <stdlib.h>

QChart *DynamicChart::createLineChart() {

	m_chart = new QChart();

	m_seriesActual = new LineType(m_chart);
	m_seriesTarget = new LineType(m_chart);
	m_seriesActual->setName("actual");
	m_seriesTarget->setName("target");
	for (int i = 0; i < 10; i++) {
		m_seriesActual->append(i, i * i);
		m_seriesTarget->append(i, (i + 1) * (i + 1));
	}

	m_chart->addSeries(m_seriesActual);
	m_chart->addSeries(m_seriesTarget);

	// set axis auto range

	m_chart->createDefaultAxes();
	m_chart->axes(Qt::Horizontal).first()->setRange(0, 10);
	m_chart->axes(Qt::Vertical).first()->setRange(0, 10);

	// QValueAxis *axisY = qobject_cast<QValueAxis *>(chart->axes(Qt::Vertical).first());
	// Q_ASSERT(axisY);
	((QValueAxis *)(m_chart->axes(Qt::Vertical).first()))->setLabelFormat("%.1f  ");
	return m_chart;
}
DynamicChart::DynamicChart(QWidget *parent, const QString &name) : QChartView(createLineChart()), m_chartName(name) {
	m_chart->setTitle(m_chartName);
	setParent(parent);
	setRenderHint(QPainter::Antialiasing, true);
}
void DynamicChart::popBack() {
	m_seriesActual->remove(0);
	m_seriesActual->append(rand() % 10, rand() % 10);
}