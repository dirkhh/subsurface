// SPDX-License-Identifier: GPL-2.0
#ifndef STATS_VIEW_H
#define STATS_VIEW_H

#include <memory>
#include <QQuickWidget>

struct dive;
struct StatsType;
struct StatsBinner;
struct StatsBin;
struct StatsState;

namespace QtCharts {
	class QAbstractAxis;
	class QAbstractSeries;
	class QBarCategoryAxis;
	class QCategoryAxis;
	class QChart;
	class QLegend;
	class QValueAxis;
}
class QGraphicsLineItem;
class QGraphicsPixmapItem;
class QGraphicsSimpleTextItem;
class BarSeries;
class ScatterSeries;

enum class ChartSubType : int;
enum class StatsOperation : int;

class StatsView : public QQuickWidget {
	Q_OBJECT
public:
	StatsView(QWidget *parent = NULL);
	~StatsView();

	void plot(const StatsState &state);
private slots:
	void plotAreaChanged(const QRectF &plotArea);
private:
	void reset(); // clears all series and axes
	void addAxes(QtCharts::QAbstractAxis *x, QtCharts::QAbstractAxis *y); // Add new x- and y-axis
	void plotBarChart(const std::vector<dive *> &dives,
			  ChartSubType subType,
			  const StatsType *categoryType, const StatsBinner *categoryBinner,
			  const StatsType *valueType, const StatsBinner *valueBinner);
	void plotValueChart(const std::vector<dive *> &dives,
			    ChartSubType subType,
			    const StatsType *categoryType, const StatsBinner *categoryBinner,
			    const StatsType *valueType, StatsOperation valueAxisOperation, bool labels);
	void plotDiscreteCountChart(const std::vector<dive *> &dives,
				    ChartSubType subType,
				    const StatsType *categoryType, const StatsBinner *categoryBinner, bool labels);
	void plotPieChart(const std::vector<dive *> &dives,
			  const StatsType *categoryType, const StatsBinner *categoryBinner);
	void plotDiscreteBoxChart(const std::vector<dive *> &dives,
				  const StatsType *categoryType, const StatsBinner *categoryBinner, const StatsType *valueType);
	void plotDiscreteScatter(const std::vector<dive *> &dives,
				 const StatsType *categoryType, const StatsBinner *categoryBinner,
				 const StatsType *valueType);
	void plotHistogramCountChart(const std::vector<dive *> &dives,
				     ChartSubType subType,
				     const StatsType *categoryType, const StatsBinner *categoryBinner,
				     bool labels, bool showMedian, bool showMean);
	void plotHistogramBarChart(const std::vector<dive *> &dives,
				   ChartSubType subType,
				   const StatsType *categoryType, const StatsBinner *categoryBinner,
				   const StatsType *valueType, StatsOperation valueAxisOperation, bool labels);
	void plotScatter(const std::vector<dive *> &dives, const StatsType *categoryType, const StatsType *valueType);
	void setTitle(const QString &);
	void showLegend();
	void hideLegend();

	template <typename T>
	T *makeAxis();

	template <typename T>
	T *addSeries(const QString &name);
	ScatterSeries *addScatterSeries(const QString &name, const StatsType &typeX, const StatsType &typeY);
	BarSeries *addBarSeries(const QString &name, bool horizontal);
	void initSeries(QtCharts::QAbstractSeries *series, const QString &name);

	template<typename T>
	QtCharts::QBarCategoryAxis *createCategoryAxis(const StatsBinner &binner, const std::vector<T> &bins);
	template<typename T>
	QtCharts::QCategoryAxis *createHistogramAxis(const StatsBinner &binner,
						     const std::vector<T> &bins,
						     bool isHorizontal);
	QtCharts::QValueAxis *createValueAxis(double min, double max, int decimals, bool isHorizontal);
	QtCharts::QValueAxis *createCountAxis(int count, bool isHorizontal);

	// Helper functions to add feature to the chart
	void addLineMarker(double pos, double low, double high, const QPen &pen, bool isHorizontal);

	// A short line used to mark quartiles
	struct QuartileMarker {
		std::unique_ptr<QGraphicsLineItem> item;
		QtCharts::QAbstractSeries *series; // In case we ever support charts with multiple axes
		double pos, value;
		QuartileMarker(double pos, double value, QtCharts::QAbstractSeries *series);
		void updatePosition();
	};

	QtCharts::QChart *chart;
	std::vector<std::unique_ptr<QtCharts::QAbstractAxis>> axes;
	std::vector<std::unique_ptr<ScatterSeries>> scatterSeries;
	std::vector<std::unique_ptr<BarSeries>> barSeries;
	std::vector<QuartileMarker> quartileMarkers;
	ScatterSeries *highlightedScatterSeries;	// scatter series with highlighted element
	BarSeries *highlightedBarSeries;		// bar series with highlighted element

	// This is unfortunate: we can't derive from QChart, because the chart is allocated by QML.
	// Therefore, we have to listen to hover events using an events-filter.
	// Probably we should try to get rid of the QML ChartView.
	struct EventFilter : public QObject {
		StatsView *view;
		EventFilter(StatsView *view) : view(view) {}
	private:
		bool eventFilter(QObject *o, QEvent *event);
	} eventFilter;
	friend EventFilter;
	void hover(QPointF pos);
};

#endif
