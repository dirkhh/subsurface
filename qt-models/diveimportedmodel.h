#ifndef DIVEIMPORTEDMODEL_H
#define DIVEIMPORTEDMODEL_H

#include <QAbstractTableModel>
#include <vector>
#include "core/divesite.h"
#include "core/downloadfromdcthread.h"

class DiveImportedModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	enum roleTypes { DateTime = Qt::UserRole + 1, Duration, Depth, Selected};

	DiveImportedModel(QObject *parent = 0);
	int columnCount(const QModelIndex& index = QModelIndex()) const;
	int rowCount(const QModelIndex& index = QModelIndex()) const;
	QVariant data(const QModelIndex& index, int role) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	void setImportedDivesIndexes(int first, int last);
	Qt::ItemFlags flags(const QModelIndex &index) const;
	Q_INVOKABLE void clearTable();
	QHash<int, QByteArray> roleNames() const;
	void deleteDeselected();
	Q_INVOKABLE void recordDives();
	Q_INVOKABLE void startDownload();

	DownloadThread thread;
public
slots:
	void changeSelected(QModelIndex clickedIndex);
	void selectRow(int row);
	void selectAll();
	void selectNone();

private
slots:
	void downloadThreadFinished();

signals:
	void downloadFinished();

private:
	void repopulate(dive_table_t *table, dive_site_table_t *sites);
	int firstIndex;
	int lastIndex;
	std::vector<char> checkStates; // char instead of bool to avoid silly pessimization of std::vector.
	struct dive_table *diveTable;
	struct dive_site_table *sitesTable;
};

#endif
