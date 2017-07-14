#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QQuickWidget>

#include "core/divesite.h"

class QResizeEvent;
class QQuickItem;
struct dive_site;

class MapWidget : public QQuickWidget {

	Q_OBJECT

public:
	MapWidget(QWidget *parent = NULL);
	~MapWidget();

	static MapWidget *instance();

public slots:
	void centerOnDiveSite(struct dive_site *);

private:
	static MapWidget *m_instance;
	QQuickItem *m_rootItem;

};

#endif // MAPWIDGET_H
