// SPDX-License-Identifier: GPL-2.0
#include "maplocationmodel.h"
#include "divelocationmodel.h"
#include "core/divesite.h"
#ifndef SUBSURFACE_MOBILE
#include "qt-models/filtermodels.h"
#include "desktop-widgets/mapwidget.h"
#endif

#include <QDebug>
#include <algorithm>

#define MIN_DISTANCE_BETWEEN_DIVE_SITES_M 50.0

MapLocation::MapLocation() : m_ds(nullptr), m_selected(false)
{
}

MapLocation::MapLocation(struct dive_site *ds, QGeoCoordinate coord, QString name, bool selected) :
    m_ds(ds), m_coordinate(coord), m_name(name), m_selected(selected)
{
}

// Check whether we are in divesite-edit mode. This doesn't
// exist on mobile. And on desktop we have to access the MapWidget.
// Simplify this!
static bool inEditMode()
{
#ifdef SUBSURFACE_MOBILE
	return false;
#else
	return MapWidget::instance()->editMode();
#endif
}

QVariant MapLocation::getRole(int role) const
{
	switch (role) {
	case Roles::RoleDivesite:
		return QVariant::fromValue((dive_site *)m_ds);
	case Roles::RoleCoordinate:
		return QVariant::fromValue(m_coordinate);
	case Roles::RoleName:
		return QVariant::fromValue(m_name);
	case Roles::RolePixmap:
		return m_selected ? QString("qrc:///dive-location-marker-selected-icon") :
		       inEditMode() ? QString("qrc:///dive-location-marker-inactive-icon") :
				    QString("qrc:///dive-location-marker-icon");
	case Roles::RoleZ:
		return m_selected ? 1 : 0;
	case Roles::RoleIsSelected:
		return QVariant::fromValue(m_selected);
	default:
		return QVariant();
	}
}

QGeoCoordinate MapLocation::coordinate()
{
	return m_coordinate;
}

void MapLocation::setCoordinate(QGeoCoordinate coord)
{
	m_coordinate = coord;
	emit coordinateChanged();
}

void MapLocation::setCoordinateNoEmit(QGeoCoordinate coord)
{
	m_coordinate = coord;
}

struct dive_site *MapLocation::divesite()
{
	return m_ds;
}

MapLocationModel::MapLocationModel(QObject *parent) : QAbstractListModel(parent)
{
	connect(&diveListNotifier, &DiveListNotifier::diveSiteChanged, this, &MapLocationModel::diveSiteChanged);
}

MapLocationModel::~MapLocationModel()
{
	qDeleteAll(m_mapLocations);
}

QVariant MapLocationModel::data(const QModelIndex & index, int role) const
{
	if (index.row() < 0 || index.row() >= m_mapLocations.size())
		return QVariant();

	return m_mapLocations.at(index.row())->getRole(role);
}

QHash<int, QByteArray> MapLocationModel::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[MapLocation::Roles::RoleDivesite] = "divesite";
	roles[MapLocation::Roles::RoleCoordinate] = "coordinate";
	roles[MapLocation::Roles::RoleName] = "name";
	roles[MapLocation::Roles::RolePixmap] = "pixmap";
	roles[MapLocation::Roles::RoleZ] = "z";
	roles[MapLocation::Roles::RoleIsSelected] = "isSelected";
	return roles;
}

int MapLocationModel::rowCount(const QModelIndex&) const
{
	return m_mapLocations.size();
}

void MapLocationModel::add(MapLocation *location)
{
	beginInsertRows(QModelIndex(), m_mapLocations.size(), m_mapLocations.size());
	m_mapLocations.append(location);
	endInsertRows();
}

const QVector<dive_site *> &MapLocationModel::selectedDs() const
{
	return m_selectedDs;
}

static bool hasVisibleDive(const dive_site *ds)
{
	return std::any_of(&ds->dives.dives[0], &ds->dives.dives[ds->dives.nr],
			   [] (const dive *d) { return !d->hidden_by_filter; });
}

static bool hasSelectedDive(const dive_site *ds)
{
	return std::any_of(&ds->dives.dives[0], &ds->dives.dives[ds->dives.nr],
			   [] (const dive *d) { return d->selected; });
}

void MapLocationModel::selectionChanged()
{
	if (m_mapLocations.isEmpty())
		return;
	for(MapLocation *m: m_mapLocations)
		m->m_selected = m_selectedDs.contains(m->divesite());
	emit dataChanged(createIndex(0, 0), createIndex(m_mapLocations.size() - 1, 0));
}

void MapLocationModel::reload(QObject *map)
{
	beginResetModel();

	qDeleteAll(m_mapLocations);
	m_mapLocations.clear();
	m_selectedDs.clear();

	QMap<QString, MapLocation *> locationNameMap;

#ifdef SUBSURFACE_MOBILE
	bool diveSiteMode = false;
#else
	// In dive site mode (that is when either editing a dive site or on
	// the dive site tab), we want to show all dive sites, not only those
	// of the non-hidden dives. Moreover, the selected dive sites are those
	// that we filter for.
	bool diveSiteMode = MultiFilterSortModel::instance()->diveSiteMode();
	if (diveSiteMode)
		m_selectedDs = MultiFilterSortModel::instance()->filteredDiveSites();
#endif
	for (int i = 0; i < dive_site_table.nr; ++i) {
		struct dive_site *ds = dive_site_table.dive_sites[i];
		QGeoCoordinate dsCoord;

		// Don't show dive sites of hidden dives, unless we're in dive site edit mode.
		if (!diveSiteMode && !hasVisibleDive(ds))
			continue;
		if (!dive_site_has_gps_location(ds)) {
			// Dive sites that do not have a gps location are not shown in normal mode.
			// In dive-edit mode, selected sites are placed at the center of the map,
			// so that the user can drag them somewhere without having to enter coordinates.
			if (!diveSiteMode || !m_selectedDs.contains(ds) || !map)
				continue;
			dsCoord = map->property("center").value<QGeoCoordinate>();
		} else {
			qreal latitude = ds->location.lat.udeg * 0.000001;
			qreal longitude = ds->location.lon.udeg * 0.000001;
			dsCoord = QGeoCoordinate(latitude, longitude);
		}
		if (!diveSiteMode && hasSelectedDive(ds) && !m_selectedDs.contains(ds))
			m_selectedDs.append(ds);
		QString name(ds->name);
		if (!diveSiteMode) {
			// don't add dive locations with the same name, unless they are
			// at least MIN_DISTANCE_BETWEEN_DIVE_SITES_M apart
			if (locationNameMap.contains(name)) {
				MapLocation *existingLocation = locationNameMap[name];
				QGeoCoordinate coord = existingLocation->coordinate();
				if (dsCoord.distanceTo(coord) < MIN_DISTANCE_BETWEEN_DIVE_SITES_M)
					continue;
			}
		}
		bool selected = m_selectedDs.contains(ds);
		MapLocation *location = new MapLocation(ds, dsCoord, name, selected);
		m_mapLocations.append(location);
		if (!diveSiteMode)
			locationNameMap[name] = location;
	}

	endResetModel();
}

void MapLocationModel::setSelected(struct dive_site *ds)
{
	m_selectedDs.clear();
	if (ds)
		m_selectedDs.append(ds);
}

void MapLocationModel::setSelected(const QVector<dive_site *> &divesites)
{
	m_selectedDs = divesites;
}

MapLocation *MapLocationModel::getMapLocation(const struct dive_site *ds)
{
	MapLocation *location;
	foreach(location, m_mapLocations) {
		if (ds == location->divesite())
			return location;
	}
	return NULL;
}

void MapLocationModel::diveSiteChanged(struct dive_site *ds, int field)
{
	// Find dive site
	int row;
	for (row = 0; row < m_mapLocations.size(); ++row) {
		if (m_mapLocations[row]->divesite() == ds)
			break;
	}
	if (row ==  m_mapLocations.size())
		return;

	switch (field) {
	case LocationInformationModel::LOCATION:
		if (has_location(&ds->location)) {
			const qreal latitude_r = ds->location.lat.udeg * 0.000001;
			const qreal longitude_r = ds->location.lon.udeg * 0.000001;
			QGeoCoordinate coord(latitude_r, longitude_r);
			m_mapLocations[row]->setCoordinateNoEmit(coord);
		}
		break;
	case LocationInformationModel::NAME:
		m_mapLocations[row]->setProperty("name", ds->name);
		break;
	default:
		break;
	}

	emit dataChanged(createIndex(row, 0), createIndex(row, 0));
}
