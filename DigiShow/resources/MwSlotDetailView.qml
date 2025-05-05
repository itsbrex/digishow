import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

import DigiShow 1.0

import "components"

Item {

    id: slotDetailView

    property int slotIndex: -1
    property bool isEdited: false

    signal slotDetailUpdated

    onSlotIndexChanged: {

        //console.log("onSlotIndexChanged", slotIndex, isEdited)

        if (isEdited) {
            if (slotIndex !== -1) undoManager.archive()
            isEdited = false
        }
    }


    Rectangle {

        anchors.fill: parent
        color: "#181818"
        visible: (slotIndex !== -1)

        Rectangle {
            id: rectSource
            height: 95
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 20
            color: "#222222"
            border.color: epInSelector.isDetecting ? "#aa0000" : "#333333"
            border.width: 1
            radius: 3

            MwEndpointSelector {
                id: epInSelector
                anchors.fill: parent
                forInput: true
                forOutput: false
                onEndpointUpdated: {
                    slotOptionsView.refresh()
                    slotDetailUpdated() // emit signal
                    window.isModified = true
                    isEdited = true
                }
            }
        }

        Image {
            width: 16
            height: 16
            anchors.top: rectSource.bottom
            anchors.topMargin: 12
            anchors.horizontalCenter: rectOptions.horizontalCenter
            source: "qrc:///images/icon_arrow_down_white.png"
            opacity: 0.5
        }

        Rectangle {
            id: rectDestination
            height: 95
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 20
            color: "#222222"
            border.color: "#333333"
            border.width: 1
            radius: 3

            MwEndpointSelector {
                id: epOutSelector
                anchors.fill: parent
                forInput: false
                forOutput: true
                onEndpointUpdated: {
                    slotOptionsView.refresh()
                    slotDetailUpdated() // emit signal
                    window.isModified = true
                    isEdited = true
                }
            }
        }

        Image {
            width: 16
            height: 16
            anchors.top: rectOptions.bottom
            anchors.topMargin: 12
            anchors.horizontalCenter: rectOptions.horizontalCenter
            source: "qrc:///images/icon_arrow_down_white.png"
            opacity: 0.5
        }

        Rectangle {
            id: rectOptions
            anchors.top: rectSource.bottom
            anchors.bottom: rectDestination.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 20
            anchors.rightMargin: 20
            anchors.topMargin: 40
            anchors.bottomMargin: 40
            color: "#222222"
            border.color: "#333333"
            border.width: 1
            radius: 3

            MwSlotOptionsView {
                id: slotOptionsView
                anchors.fill: parent
                onSlotOptionUpdated: {

                    if (key === "inputInverted" || key === "outputInverted" || key === "inputExpression" || key === "outputExpression") {
                        slotDetailUpdated() // emit signal
                    }
                    window.isModified = true
                    isEdited = true
                }
            }
        }

    }

    Rectangle {
        anchors.fill: parent
        color: "#111111"
        visible: (slotIndex === -1)

        Text {
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            color: "#555555"
            text: qsTr("Please select an item in the signal link table \r\n on the left")
            visible: !window.isEmpty
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 14
            font.bold: true
            lineHeight: 2.0
        }
    }

    function refresh() {

        if (slotIndex === -1) {
            // no slot is selected
            epInSelector.interfaceIndex = -1
            epInSelector.endpointIndex = -1
            epOutSelector.interfaceIndex = -1
            epOutSelector.endpointIndex  = -1
        } else {
            // a slot is selected
            epInSelector.interfaceIndex = digishow.getSourceInterfaceIndex(slotIndex)
            epInSelector.endpointIndex  = digishow.getSourceEndpointIndex(slotIndex)
            epOutSelector.interfaceIndex = digishow.getDestinationInterfaceIndex(slotIndex)
            epOutSelector.endpointIndex  = digishow.getDestinationEndpointIndex(slotIndex)
        }

        epInSelector.refresh()
        epOutSelector.refresh()
        slotOptionsView.refresh()
    }

}
