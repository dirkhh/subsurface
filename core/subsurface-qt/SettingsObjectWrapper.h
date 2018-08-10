// SPDX-License-Identifier: GPL-2.0
#ifndef SETTINGSOBJECTWRAPPER_H
#define SETTINGSOBJECTWRAPPER_H

#include <QObject>
#include <QDate>

#include "core/pref.h"
#include "core/settings/qPref.h"

/* Wrapper class for the Settings. This will allow
 * seamlessy integration of the settings with the QML
 * and QWidget frontends. This class will be huge, since
 * I need tons of properties, one for each option. */

/* Control the state of the Partial Pressure Graphs preferences */
class PartialPressureGasSettings : public QObject {
	Q_OBJECT
	Q_PROPERTY(bool   show_po2          READ showPo2         WRITE setShowPo2         NOTIFY showPo2Changed)
	Q_PROPERTY(bool   show_pn2          READ showPn2         WRITE setShowPn2         NOTIFY showPn2Changed)
	Q_PROPERTY(bool   show_phe	    READ showPhe         WRITE setShowPhe         NOTIFY showPheChanged)
	Q_PROPERTY(double po2_threshold_min READ po2ThresholdMin WRITE setPo2ThresholdMin NOTIFY po2ThresholdMinChanged)
	Q_PROPERTY(double po2_threshold_max READ po2ThresholdMax WRITE setPo2ThresholdMax NOTIFY po2ThresholdMaxChanged)
	Q_PROPERTY(double pn2_threshold     READ pn2Threshold    WRITE setPn2Threshold    NOTIFY pn2ThresholdChanged)
	Q_PROPERTY(double phe_threshold     READ pheThreshold    WRITE setPheThreshold    NOTIFY pheThresholdChanged)

public:
	PartialPressureGasSettings(QObject *parent);
	bool showPo2() const;
	bool showPn2() const;
	bool showPhe() const;
	double po2ThresholdMin() const;
	double po2ThresholdMax() const;
	double pn2Threshold() const;
	double pheThreshold() const;

public slots:
	void setShowPo2(bool value);
	void setShowPn2(bool value);
	void setShowPhe(bool value);
	void setPo2ThresholdMin(double value);
	void setPo2ThresholdMax(double value);
	void setPn2Threshold(double value);
	void setPheThreshold(double value);

signals:
	void showPo2Changed(bool value);
	void showPn2Changed(bool value);
	void showPheChanged(bool value);
	void po2ThresholdMaxChanged(double value);
	void po2ThresholdMinChanged(double value);
	void pn2ThresholdChanged(double value);
	void pheThresholdChanged(double value);

private:
	const QString group = QStringLiteral("TecDetails");
};

class GeneralSettingsObjectWrapper : public QObject {
	Q_OBJECT
	Q_PROPERTY(QString default_filename              READ defaultFilename                WRITE setDefaultFilename                NOTIFY defaultFilenameChanged)
	Q_PROPERTY(QString default_cylinder              READ defaultCylinder                WRITE setDefaultCylinder                NOTIFY defaultCylinderChanged)
	Q_PROPERTY(short default_file_behavior           READ defaultFileBehavior            WRITE setDefaultFileBehavior            NOTIFY defaultFileBehaviorChanged)
	Q_PROPERTY(bool use_default_file                 READ useDefaultFile                 WRITE setUseDefaultFile                 NOTIFY useDefaultFileChanged)
	Q_PROPERTY(int defaultsetpoint                   READ defaultSetPoint                WRITE setDefaultSetPoint                NOTIFY defaultSetPointChanged)
	Q_PROPERTY(int o2consumption                     READ o2Consumption                  WRITE setO2Consumption                  NOTIFY o2ConsumptionChanged)
	Q_PROPERTY(int pscr_ratio                        READ pscrRatio                      WRITE setPscrRatio                      NOTIFY pscrRatioChanged)
	Q_PROPERTY(bool auto_recalculate_thumbnails      READ autoRecalculateThumbnails      WRITE setAutoRecalculateThumbnails      NOTIFY autoRecalculateThumbnailsChanged)
	Q_PROPERTY(bool extract_video_thumbnails         READ extractVideoThumbnails         WRITE setExtractVideoThumbnails         NOTIFY extractVideoThumbnailsChanged)
	Q_PROPERTY(int extract_video_thumbnails_position READ extractVideoThumbnailsPosition WRITE setExtractVideoThumbnailsPosition NOTIFY extractVideoThumbnailsPositionChanged)
	Q_PROPERTY(QString ffmpeg_executable             READ ffmpegExecutable               WRITE setFfmpegExecutable               NOTIFY ffmpegExecutableChanged)

public:
	GeneralSettingsObjectWrapper(QObject *parent);
	QString defaultFilename() const;
	QString defaultCylinder() const;
	short defaultFileBehavior() const;
	bool useDefaultFile() const;
	int defaultSetPoint() const;
	int o2Consumption() const;
	int pscrRatio() const;
	bool autoRecalculateThumbnails() const;
	bool extractVideoThumbnails() const;
	int extractVideoThumbnailsPosition() const;
	QString ffmpegExecutable() const;

public slots:
	void setDefaultFilename           (const QString& value);
	void setDefaultCylinder           (const QString& value);
	void setDefaultFileBehavior       (short value);
	void setUseDefaultFile            (bool value);
	void setDefaultSetPoint           (int value);
	void setO2Consumption             (int value);
	void setPscrRatio                 (int value);
	void setAutoRecalculateThumbnails (bool value);
	void setExtractVideoThumbnails    (bool value);
	void setExtractVideoThumbnailsPosition (int value);
	void setFfmpegExecutable          (const QString &value);

signals:
	void defaultFilenameChanged(const QString& value);
	void defaultCylinderChanged(const QString& value);
	void defaultFileBehaviorChanged(short value);
	void useDefaultFileChanged(bool value);
	void defaultSetPointChanged(int value);
	void o2ConsumptionChanged(int value);
	void pscrRatioChanged(int value);
	void autoRecalculateThumbnailsChanged(int value);
	void extractVideoThumbnailsChanged(bool value);
	void extractVideoThumbnailsPositionChanged(int value);
	void ffmpegExecutableChanged(const QString &value);
private:
	const QString group = QStringLiteral("GeneralSettings");
};

class SettingsObjectWrapper : public QObject {
	Q_OBJECT

	Q_PROPERTY(qPrefTechnicalDetails*   techical_details MEMBER techDetails CONSTANT)
	Q_PROPERTY(PartialPressureGasSettings* pp_gas           MEMBER pp_gas CONSTANT)
	Q_PROPERTY(qPrefFacebook*           facebook         MEMBER facebook CONSTANT)
	Q_PROPERTY(qPrefGeocoding*       geocoding        MEMBER geocoding CONSTANT)
	Q_PROPERTY(qPrefProxy*              proxy            MEMBER proxy CONSTANT)
	Q_PROPERTY(qPrefCloudStorage*       cloud_storage    MEMBER cloud_storage CONSTANT)
	Q_PROPERTY(qPrefDivePlanner*        planner          MEMBER planner_settings CONSTANT)
	Q_PROPERTY(qPrefUnits*              units            MEMBER unit_settings CONSTANT)
	Q_PROPERTY(GeneralSettingsObjectWrapper*         general   MEMBER general_settings CONSTANT)
	Q_PROPERTY(qPrefDisplay*         display   MEMBER display_settings CONSTANT)
	Q_PROPERTY(qPrefLanguage*        language  MEMBER language_settings CONSTANT)
	Q_PROPERTY(qPrefAnimations*      animation MEMBER animation_settings CONSTANT)
	Q_PROPERTY(qPrefLocationService* Location  MEMBER location_settings CONSTANT)

	Q_PROPERTY(qPrefUpdateManager* update MEMBER update_manager_settings CONSTANT)
	Q_PROPERTY(qPrefDiveComputer* dive_computer MEMBER dive_computer_settings CONSTANT)
public:
	static SettingsObjectWrapper *instance();

	qPrefTechnicalDetails *techDetails;
	PartialPressureGasSettings *pp_gas;
	qPrefFacebook *facebook;
	qPrefGeocoding *geocoding;
	qPrefProxy *proxy;
	qPrefCloudStorage *cloud_storage;
	qPrefDivePlanner *planner_settings;
	qPrefUnits *unit_settings;
	GeneralSettingsObjectWrapper *general_settings;
	qPrefDisplay *display_settings;
	qPrefLanguage *language_settings;
	qPrefAnimations *animation_settings;
	qPrefLocationService *location_settings;
	qPrefUpdateManager *update_manager_settings;
	qPrefDiveComputer *dive_computer_settings;

	void sync();
	void load();
private:
	SettingsObjectWrapper(QObject *parent = NULL);
};

#endif
