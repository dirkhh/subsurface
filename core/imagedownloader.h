// SPDX-License-Identifier: GPL-2.0
#ifndef IMAGEDOWNLOADER_H
#define IMAGEDOWNLOADER_H

#include "metadata.h"
#include <QImage>
#include <QFuture>
#include <QNetworkReply>
#include <QThreadPool>

class ImageDownloader : public QObject {
	Q_OBJECT
public:
	static ImageDownloader *instance();
	ImageDownloader();
public slots:
	void load(QUrl url, QString filename);
signals:
	void loaded(QString filename);
	void failed(QString filename);
private:
	QNetworkAccessManager manager;
	void loadFromUrl(const QString &filename, const QUrl &);
	void saveImage(QNetworkReply *reply);
};

struct PictureEntry;
class Thumbnailer : public QObject {
	Q_OBJECT
public:
	static Thumbnailer *instance();

	// Schedule a thumbnail for fetching or calculation.
	// Returns a placeholder thumbnail. The actual thumbnail will be sent
	// via a signal later.
	QImage fetchThumbnail(const QString &filename);

	// Schedule multiple thumbnails for forced recalculation
	void calculateThumbnails(const QVector<QString> &filenames);

	// If we change dive, clear all unfinished thumbnail creations
	void clearWorkQueue();
	static int maxThumbnailSize();
	static int defaultThumbnailSize();
	static int thumbnailSize(double zoomLevel);
public slots:
	void imageDownloaded(QString filename);
	void imageDownloadFailed(QString filename);
signals:
	void thumbnailChanged(QString filename, QImage thumbnail, duration_t duration);
private:
	struct Thumbnail {
		QImage img;
		mediatype_t type;
		duration_t duration;
	};

	Thumbnailer();
	Thumbnail addPictureThumbnailToCache(const QString &picture_filename, const QImage &thumbnail);
	Thumbnail addVideoThumbnailToCache(const QString &picture_filename, duration_t duration);
	Thumbnail addUnknownThumbnailToCache(const QString &picture_filename);
	void recalculate(QString filename);
	void processItem(QString filename, bool tryDownload);
	Thumbnail getThumbnailFromCache(const QString &picture_filename);
	Thumbnail getPictureThumbnailFromStream(QDataStream &stream);
	Thumbnail getVideoThumbnailFromStream(QDataStream &stream);
	Thumbnail fetchImage(const QString &filename, const QString &originalFilename, bool tryDownload);
	Thumbnail getHashedImage(const QString &filename, bool tryDownload);

	mutable QMutex lock;
	QThreadPool pool;
	QImage failImage;		// Shown when image-fetching fails
	QImage dummyImage;		// Shown before thumbnail is fetched
	QImage videoImage;		// Place holder for videos
	QImage unknownImage;		// Place holder for files where we couldn't determine the type

	QMap<QString,QFuture<void>> workingOn;
};

#endif // IMAGEDOWNLOADER_H
