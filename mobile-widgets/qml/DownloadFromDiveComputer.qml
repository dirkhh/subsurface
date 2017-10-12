// SPDX-License-Identifier: GPL-2.0
import QtQuick 2.6
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.3
import org.subsurfacedivelog.mobile 1.0
import org.kde.kirigami 2.0 as Kirigami

Kirigami.Page {
	id: diveComputerDownloadWindow
	anchors.top:parent.top
	width: parent.width
	height: parent.height
	Layout.fillWidth: true;
	title: qsTr("Dive Computer")

	property alias dcImportModel: importModel
	property bool divesDownloaded: false
	property bool btEnabled: manager.btEnabled
	property string btMessage: manager.btEnabled ? "" : qsTr("Bluetooth is not enabled")

	DCDownloadThread {
		id: downloadThread
		deviceData.vendor : comboVendor.currentText
		deviceData.product : comboProduct.currentText
		deviceData.devName : comboConnection.currentText

		//TODO: Make this the default on the C++
		deviceData.bluetoothMode : true
		deviceData.forceDownload : false
		deviceData.createNewTrip : false
		deviceData.deviceId : 0
		deviceData.diveId : 0
		deviceData.saveDump : false
		deviceData.saveLog : false

		onFinished : {
			importModel.repopulate()
			progressBar.visible = false
			if (dcImportModel.rowCount() > 0) {
				console.log(dcImportModel.rowCount() + " dive downloaded")
				divesDownloaded = true
			} else {
				console.log("no new dives downloaded")
				divesDownloaded = false
			}
			manager.appendTextToLog("DCDownloadThread finished")
		}
	}

	DCImportModel {
		id: importModel
	}

	ColumnLayout {
		anchors.top: parent.top
		height: parent.height
		width: parent.width
		Layout.fillWidth: true
		GridLayout {
			columns: 2
			Kirigami.Label { text: qsTr(" Vendor name: ") }
			property var vendoridx: downloadThread.data().getDetectedVendorIndex()
			ComboBox {
				id: comboVendor
				Layout.fillWidth: true
				model: vendorList
				currentIndex: parent.vendoridx
				delegate: ItemDelegate {
					width: comboVendor.width
					contentItem: Text {
						text: modelData
						font.pointSize: subsurfaceTheme.regularPointSize
						verticalAlignment: Text.AlignVCenter
						elide: Text.ElideRight
					}
					highlighted: comboVendor.highlightedIndex === index
				}
				contentItem: Text {
					text: comboVendor.displayText
					font.pointSize: subsurfaceTheme.regularPointSize
					leftPadding: Kirigami.Units.gridUnit * 0.5
					horizontalAlignment: Text.AlignLeft
					verticalAlignment: Text.AlignVCenter
					elide: Text.ElideRight
				}
				onCurrentTextChanged: {
					comboProduct.model = downloadThread.data().getProductListFromVendor(currentText)
					if (currentIndex == downloadThread.data().getDetectedVendorIndex())
						comboProduct.currentIndex = downloadThread.data().getDetectedProductIndex(currentText)
				}
			}
			Kirigami.Label { text: qsTr(" Dive Computer:") }
			ComboBox {
				id: comboProduct
				property var productidx: downloadThread.data().getDetectedProductIndex(comboVendor.currentText)
				Layout.fillWidth: true
				model: null
				currentIndex: productidx
				delegate: ItemDelegate {
					width: comboProduct.width
					contentItem: Text {
						text: modelData
						font.pointSize: subsurfaceTheme.regularPointSize
						verticalAlignment: Text.AlignVCenter
						elide: Text.ElideRight
					}
					highlighted: comboProduct.highlightedIndex === index
				}
				contentItem: Text {
					text: comboProduct.displayText
					font.pointSize: subsurfaceTheme.regularPointSize
					leftPadding: Kirigami.Units.gridUnit * 0.5
					horizontalAlignment: Text.AlignLeft
					verticalAlignment: Text.AlignVCenter
					elide: Text.ElideRight
				}
				onCurrentTextChanged: {
					var newIdx = downloadThread.data().getMatchingAddress(comboVendor.currentText, currentText)
					if (newIdx != -1)
						comboConnection.currentIndex = newIdx
				}

				onModelChanged: {
					currentIndex = productidx
				}
			}
			Kirigami.Label { text: qsTr(" Connection:") }
			ComboBox {
				id: comboConnection
				Layout.fillWidth: true
				model: connectionListModel
				currentIndex: -1
				delegate: ItemDelegate {
					width: comboConnection.width
					contentItem: Text {
						text: modelData
						// color: "#21be2b"
						font.pointSize: subsurfaceTheme.smallPointSize
						verticalAlignment: Text.AlignVCenter
						elide: Text.ElideRight
					}
					highlighted: comboConnection.highlightedIndex === index
				}
				contentItem: Text {
					text: comboConnection.displayText
					font.pointSize: subsurfaceTheme.smallPointSize
					leftPadding: Kirigami.Units.gridUnit * 0.5
					horizontalAlignment: Text.AlignLeft
					verticalAlignment: Text.AlignVCenter
					elide: Text.ElideRight
				}
				onCurrentTextChanged: {
					// pattern that matches BT addresses
					var btAddr = /[0-9A-Fa-f][0-9A-Fa-f]:[0-9A-Fa-f][0-9A-Fa-f]:[0-9A-Fa-f][0-9A-Fa-f]:[0-9A-Fa-f][0-9A-Fa-f]:[0-9A-Fa-f][0-9A-Fa-f]:[0-9A-Fa-f][0-9A-Fa-f]/ ;
					if (btAddr.test(currentText))
						downloadThread.deviceData.bluetoothMode = true
					else
						downloadThread.deviceData.bluetoothMode = false
				}
			}
		}

		ProgressBar {
			id: progressBar
			Layout.fillWidth: true
			indeterminate: true
			visible: false
		}

		RowLayout {
			anchors.left: parent.left
			anchors.right: parent.right
			anchors.margins: Kirigami.Units.smallSpacing
			spacing: Kirigami.Units.smallSpacing
			SsrfButton {
				id: download
				text: qsTr("Download")
				onClicked: {
					text = qsTr("Retry")
					// strip any BT Name from the address
					var devName = downloadThread.deviceData.devName
					downloadThread.deviceData.devName = devName.replace(/ (.*)$/, "")
					manager.appendTextToLog("DCDownloadThread started for " + downloadThread.deviceData.product + " on "+ downloadThread.deviceData.devName)
					progressBar.visible = true
					downloadThread.start()
				}
			}
			SsrfButton {
				id:quitbutton
				text: progressBar.visible ? qsTr("Cancel") : qsTr("Quit")
				onClicked: {
					manager.cancelDownloadDC()
					if (!progressBar.visible)
						stackView.pop();
					manager.appendTextToLog("exit DCDownload screen")
				}
			}
			Kirigami.Label {
				Layout.maximumWidth: parent.width - download.width - quitbutton.width
				text: divesDownloaded ? qsTr(" Downloaded dives") :
							(manager.progressMessage != "" ? qsTr("Info:") + " " + manager.progressMessage : btMessage)
				wrapMode: Text.WrapAtWordBoundaryOrAnywhere
			}
		}

		ListView {
			Layout.fillWidth: true
			Layout.fillHeight: true

			model : importModel
			delegate : DownloadedDiveDelegate {
				id: delegate
				datetime: model.datetime ? model.datetime : ""
				duration: model.duration ? model.duration : ""
				depth: model.depth ? model.depth : ""
				selected: model.selected ? model.selected : false

				onClicked : {
					console.log("Selecting index" + index);
					importModel.selectRow(index)
				}
			}
		}

		RowLayout {
			Layout.fillWidth: true
			Kirigami.Label {
				text: ""  // Spacer on the left for hamburger menu
				Layout.fillWidth: true
			}
			SsrfButton {
				id: acceptButton
				enabled: divesDownloaded
				text: qsTr("Accept")
				onClicked: {
					manager.appendTextToLog("Save downloaded dives that were selected")
					importModel.recordDives()
					manager.saveChangesLocal()
					diveModel.clear()
					diveModel.addAllDives()
					stackView.pop();
				}
			}
			Kirigami.Label {
				text: ""  // Spacer between 2 button groups
				Layout.fillWidth: true
			}
			SsrfButton {
				id: select
				enabled: divesDownloaded
				text: qsTr("Select All")
				onClicked : {
					importModel.selectAll()
				}
			}
			SsrfButton {
				id: unselect
				enabled: divesDownloaded
				text: qsTr("Unselect All")
				onClicked : {
					importModel.selectNone()
				}
			}
		}
	}
}
