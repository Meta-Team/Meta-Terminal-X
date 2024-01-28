#include <QtCharts/QChartGlobal>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE
class QComboBox;
class QCheckBox;
class Ui_ThemeWidgetForm;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QChart;
QT_CHARTS_END_NAMESPACE

typedef QPair<QPointF, QString> Data;
typedef QList<Data> DataList;
typedef QList<DataList> DataTable;
QT_CHARTS_USE_NAMESPACE

class ThemeWidget : public QWidget {
	Q_OBJECT
  public:
	explicit ThemeWidget(QWidget *parent = 0);
	~ThemeWidget();

  private Q_SLOTS:
	void updateUI();

  private:
	DataTable generateRandomData(int listCount, int valueMax, int valueCount) const;
	void populateThemeBox();
	void populateAnimationBox();
	void populateLegendBox();
	void connectSignals();
	QChart *createAreaChart() const;
	QChart *createBarChart(int valueCount) const;
	QChart *createPieChart() const;
	QChart *createLineChart() const;
	QChart *createSplineChart() const;
	QChart *createScatterChart() const;

  private:
	int m_listCount;
	int m_valueMax;
	int m_valueCount;
	QList<QChartView *> m_charts;
	DataTable m_dataTable;

	Ui_ThemeWidgetForm *m_ui;
};