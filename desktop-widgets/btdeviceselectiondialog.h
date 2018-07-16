// SPDX-License-Identifier: GPL-2.0
#ifndef BTDEVICESELECTIONDIALOG_H
#define BTDEVICESELECTIONDIALOG_H

#include <QDialog>
#include <QListWidgetItem>
#include <QPointer>
#include <QtBluetooth/QBluetoothLocalDevice>
#include <QtBluetooth/QBluetoothDeviceDiscoveryAgent>
#include "core/libdivecomputer.h"

namespace Ui {
	class BtDeviceSelectionDialog;
}

class BtDeviceSelectionDialog : public QDialog {
	Q_OBJECT

public:
	explicit BtDeviceSelectionDialog(const QString &address, dc_descriptor_t *dc, QWidget *parent = 0);
	~BtDeviceSelectionDialog();
	QString getSelectedDeviceAddress();
	QString getSelectedDeviceName();
	QString getSelectedDeviceText();
	static QString formatDeviceText(const QString &address, const QString &name);

private slots:
	void on_changeDeviceState_clicked();
	void on_save_clicked();
	void on_clear_clicked();
	void on_scan_clicked();
	void on_quit_clicked();
	void remoteDeviceScanFinished();
	void hostModeStateChanged(QBluetoothLocalDevice::HostMode mode);
	void addRemoteDevice(const QBluetoothDeviceInfo &remoteDeviceInfo);
	void currentItemChanged(QListWidgetItem *item,QListWidgetItem *previous);
	void displayPairingMenu(const QPoint &pos);
	void pairingFinished(const QBluetoothAddress &address,QBluetoothLocalDevice::Pairing pairing);
	void error(QBluetoothLocalDevice::Error error);
	void deviceDiscoveryError(QBluetoothDeviceDiscoveryAgent::Error error);
	void localDeviceChanged(int);

private:
	Ui::BtDeviceSelectionDialog *ui;
	QString previousDevice;
	dc_descriptor_t *dcDescriptor;
	int maxPriority;
	QScopedPointer<QBluetoothLocalDevice> localDevice;
	QScopedPointer<QBluetoothDeviceDiscoveryAgent> remoteDeviceDiscoveryAgent;
	QScopedPointer<QBluetoothDeviceInfo> selectedRemoteDeviceInfo;

	void updateLocalDeviceInformation();
	void initializeDeviceDiscoveryAgent();
	bool isPoweredOn() const;
	void setScanStatusOn();
	void setScanStatusOff();
	void setScanStatusInvalid();
	void startScan();
	void stopScan();
	void showEvent(QShowEvent *event);
	void initializeDeviceList();
	int getDevicePriority(bool connectable, const QBluetoothDeviceInfo &remoteDeviceInfo);
};

#endif // BTDEVICESELECTIONDIALOG_H
