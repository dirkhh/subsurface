// SPDX-License-Identifier: GPL-2.0
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.3
import org.subsurfacedivelog.mobile 1.0
import org.kde.kirigami 2.4 as Kirigami

Kirigami.ScrollablePage {
	title: qsTr("Hier bauen wir für Sie eine Statistikseite")

	TextArea {
	    width: parent.width
	    height: parent.height
	    readOnly: true

	    text: manager.statisticsPageText
	}

	ColumnLayout {
		width: parent.width
		spacing: 1
		Layout.margins: 10

	}
}
