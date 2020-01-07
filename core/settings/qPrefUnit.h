// SPDX-License-Identifier: GPL-2.0
#ifndef QPREFUNIT_H
#define QPREFUNIT_H
#include "core/pref.h"

#include <QObject>


class qPrefUnits : public QObject {
	Q_OBJECT
	Q_PROPERTY(bool coordinates_traditional READ coordinates_traditional WRITE set_coordinates_traditional NOTIFY coordinates_traditionalChanged)
	Q_PROPERTY(bool show_units_table READ show_units_table WRITE set_show_units_table NOTIFY show_units_tableChanged)
	Q_PROPERTY(QString vertical_speed_time READ vertical_speed_time WRITE set_vertical_speed_time NOTIFY vertical_speed_timeStringChanged)
	Q_PROPERTY(QString volume READ volume WRITE set_volume NOTIFY volumeStringChanged)
	Q_PROPERTY(QString weight READ weight WRITE set_weight NOTIFY weightStringChanged)

public:
	static qPrefUnits *instance();

	// Load/Sync local settings (disk) and struct preference
	static void loadSync(bool doSync);
	static void load() { loadSync(false); }
	static void sync() { loadSync(true); }

public:
	static bool coordinates_traditional() { return prefs.coordinates_traditional; }
	static units::DURATION duration_units() { return prefs.units.duration_units; }
	static units::LENGTH length() { return prefs.units.length; }
	static units::PRESSURE pressure() { return prefs.units.pressure; }
	static bool show_units_table() { return prefs.units.show_units_table; }
	static unit_system_values unit_system() { return prefs.unit_system; }
	static units::TEMPERATURE temperature() { return prefs.units.temperature; }
	static QString vertical_speed_time();
	static QString volume();
	static QString weight();

public slots:
	static void set_coordinates_traditional(bool value);
	static void set_duration_units(units::DURATION value);
	static void set_length(units::LENGTH value);
	static void set_pressure(units::PRESSURE value);
	static void set_show_units_table(bool value);
	static void set_temperature(units::TEMPERATURE value);
	static void set_unit_system(unit_system_values value);
	static void set_vertical_speed_time(units::TIME value);
	static void set_vertical_speed_time(const QString& value);
	static void set_volume(units::VOLUME value);
	static void set_volume(const QString& value);
	static void set_weight(units::WEIGHT value);
	static void set_weight(const QString& value);

signals:
	void coordinates_traditionalChanged(bool value);
	void duration_unitsChanged(units::DURATION value);
	void lengthChanged(units::LENGTH value);
	void pressureChanged(units::PRESSURE value);
	void show_units_tableChanged(bool value);
	void temperatureChanged(units::TEMPERATURE value);
	void unit_systemChanged(unit_system_values value);
	void vertical_speed_timeChanged(int value);
	void vertical_speed_timeStringChanged(const QString& value);
	void volumeChanged(int value);
	void volumeStringChanged(const QString& value);
	void weightChanged(int value);
	void weightStringChanged(const QString& value);
private:
	qPrefUnits() {}

	static void disk_coordinates_traditional(bool doSync);
	static void disk_duration_units(bool doSync);
	static void disk_length(bool doSync);
	static void disk_pressure(bool doSync);
	static void disk_show_units_table(bool doSync);
	static void disk_temperature(bool doSync);
	static void disk_unit_system(bool doSync);
	static void disk_vertical_speed_time(bool doSync);
	static void disk_volume(bool doSync);
	static void disk_weight(bool doSync);
};
#endif
