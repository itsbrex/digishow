import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.0

import DigiShow 1.0

import "components"

Item {
    id: itemAPlay

    COptionButton {
        id: buttonType
        width: 100
        height: 28
        anchors.left: parent.left
        anchors.top: parent.top
        text: menuType.selectedItemText
        onClicked: menuType.showOptions()

        COptionMenu {
            id: menuType

            onOptionSelected: refreshMoreOptions()
        }
    }

    COptionButton {
        id: buttonMediaControl
        width: 100
        height: 28
        anchors.left: buttonType.right
        anchors.leftMargin: 10
        anchors.top: parent.top
        text: menuMediaControl.selectedItemText
        visible: menuType.selectedItemValue === DigishowEnvironment.EndpointAPlayMedia
        onClicked: menuMediaControl.showOptions()

        COptionMenu {
            id: menuMediaControl

            onOptionSelected: refreshMoreOptions()
        }
    }

    CButton {
        id: buttonMediaSelect
        width: 70
        height: 28
        anchors.left: buttonMediaControl.right
        anchors.leftMargin: 20
        anchors.top: parent.top
        label.font.bold: false
        label.font.pixelSize: 11
        label.text: qsTr("File ...")
        box.radius: 3
        visible: textMediaUrl.visible

        onClicked: {
            dialogSelectFile.setDefaultFileUrl(textMediaUrl.text)
            dialogSelectFile.open()
        }
    }

    CButton {
        id: buttonMediaOptions
        width: 70
        height: 28
        anchors.left: buttonMediaSelect.right
        anchors.leftMargin: 10
        anchors.top: parent.top
        label.font.bold: false
        label.font.pixelSize: 11
        label.text: qsTr("Options ...")
        box.radius: 3
        visible: textMediaUrl.visible &&
                 menuMediaControl.selectedItemValue === DigishowEnvironment.ControlMediaStart

        onClicked: popupMediaOptions.open()
    }

    CTextInputBox {

        id: textMediaUrl

        anchors.bottom: parent.top
        anchors.bottomMargin: 10
        anchors.left: buttonMediaSelect.left
        anchors.right: parent.right
        anchors.rightMargin: 38
        text: "file://"
        //input.anchors.rightMargin: 30
        visible: menuType.selectedItemValue === DigishowEnvironment.EndpointAPlayMedia &&
                 menuMediaControl.selectedItemValue !== DigishowEnvironment.ControlMediaStopAll &&
                 menuMediaControl.selectedItemValue !== DigishowEnvironment.ControlMediaMaster

        onTextEdited: isModified = true
        onEditingFinished: if (text === "") text = "file://"
    }

    MwFileDialog {
        id: dialogSelectFile
        title: qsTr("Select Audio File")
        folder: shortcuts.home
        selectExisting: true
        nameFilters: [ qsTr("Audio files") + " (*.wav *.aif *.aiff *.mp3 *.m4a)",
                       qsTr("All files") + " (*)" ]
        onAccepted: {
            console.log("select audio file: ", fileUrl)
            textMediaUrl.text = fileUrl
            isModified = true
        }
    }

    Popup {
        id: popupMediaOptions

        width: 280
        height: 360
        x: 100
        y: -height-10
        transformOrigin: Popup.BottomRight
        modal: true
        focus: true

        //onVisibleChanged: if (!visible) isModified = true

        background: Rectangle {
            anchors.fill: parent
            color: "#333333"
            radius: 5
            border.width: 1
            border.color: "#999999"
        }

        // @disable-check M16
        Overlay.modal: Item {}

        Column {
            anchors.fill: parent
            anchors.leftMargin: 16
            spacing: 10

            Text {
                height: 25
                verticalAlignment: Text.AlignBottom
                color: "#cccccc"
                font.pixelSize: 12
                font.bold: true
                text: qsTr("Audio Clip Playback Options")
            }

            Item {
                width: 5
                height: 5
            }

            CheckBox {
                id: checkMediaAlone

                height: 28
                anchors.left: parent.left
                anchors.leftMargin: 105
                padding: 0
                checked: true

                onClicked: isModified = true

                Text {
                    anchors.right: parent.left
                    anchors.rightMargin: 15
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#cccccc"
                    font.pixelSize: 12
                    text: qsTr("Play Alone")
                }
            }

            CSpinBox {
                id: spinMediaVolume

                width: 120
                anchors.left: parent.left
                anchors.leftMargin: 105
                from: 0
                to: 100
                value: 100
                stepSize: 5
                unit: "%"

                onValueModified: isModified = true

                Text {
                    anchors.right: parent.left
                    anchors.rightMargin: 15
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#cccccc"
                    font.pixelSize: 12
                    text: qsTr("Volume")
                }
            }

            CSpinBox {
                id: spinMediaSpeed

                width: 120
                anchors.left: parent.left
                anchors.leftMargin: 105
                from: 10
                to: 400
                value: 100
                stepSize: 5
                unit: qsTr("%")

                onValueModified: isModified = true

                Text {
                    anchors.right: parent.left
                    anchors.rightMargin: 15
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#cccccc"
                    font.pixelSize: 12
                    text: qsTr("Speed")
                }
            }

            CSpinBox {
                id: spinMediaPosition

                width: 120
                anchors.left: parent.left
                anchors.leftMargin: 105
                from: 0
                to: 99999000
                value: 0
                stepSize: 1000
                unit: qsTr("ms")

                onValueModified: isModified = true

                Text {
                    anchors.right: parent.left
                    anchors.rightMargin: 15
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#cccccc"
                    font.pixelSize: 12
                    text: qsTr("Position")
                }
            }

            CSpinBox {
                id: spinMediaDuration

                width: 120
                anchors.left: parent.left
                anchors.leftMargin: 105
                from: 0
                to: 99999999
                value: 0
                stepSize: 1000
                unit: qsTr("ms")

                onValueModified: isModified = true

                Text {
                    anchors.right: parent.left
                    anchors.rightMargin: 15
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#cccccc"
                    font.pixelSize: 12
                    text: qsTr("Duration")
                }
            }

            CheckBox {
                id: checkMediaRepeat

                height: 28
                anchors.left: parent.left
                anchors.leftMargin: 105
                padding: 0
                checked: false

                onClicked: isModified = true

                Text {
                    anchors.right: parent.left
                    anchors.rightMargin: 15
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#cccccc"
                    font.pixelSize: 12
                    text: qsTr("Repeat")
                }
            }

            Item {
                width: 5
                height: 5
            }

            CButton {
                width: 100
                height: 28
                anchors.left: parent.left
                anchors.leftMargin: 105
                label.font.bold: false
                label.font.pixelSize: 11
                label.text: qsTr("Defaults")
                box.radius: 3

                onClicked: {
                    setEndpointMediaOptions({})
                    isModified = true
                }
            }

        }

    }

    function refresh() {

        var items
        var n
        var v

        // init type option menu
        if (menuType.count === 0) {
            items = []
            items.push({ text: qsTr("Audio Clip"), value: DigishowEnvironment.EndpointAPlayMedia,  tag:"media" })
            menuType.optionItems = items
            menuType.selectedIndex = 0
        }

        // init media control option menu
        if (menuMediaControl.count === 0) {
            items = []

            v = DigishowEnvironment.ControlMediaStart;   items.push({ text: digishow.getMediaControlName(v), value: v })
            v = DigishowEnvironment.ControlMediaStop;    items.push({ text: digishow.getMediaControlName(v), value: v })
            v = DigishowEnvironment.ControlMediaStopAll; items.push({ text: digishow.getMediaControlName(v), value: v })
            items.push({ text: "-", value: -1 })
            v = DigishowEnvironment.ControlMediaVolume;  items.push({ text: digishow.getMediaControlName(v), value: v })
            v = DigishowEnvironment.ControlMediaMaster;  items.push({ text: digishow.getMediaControlName(v), value: v })

            menuMediaControl.optionItems = items
            menuMediaControl.selectedIndex = 0
        }

        // init more options
        refreshMoreOptions()
    }

    function refreshMoreOptions() {

        var endpointType = menuType.selectedItemValue
        var mediaControl = menuMediaControl.selectedItemValue
        var enables = {}

        if (endpointType === DigishowEnvironment.EndpointAPlayMedia) {
            if (mediaControl === DigishowEnvironment.ControlMediaStart ||
                mediaControl === DigishowEnvironment.ControlMediaStop ||
                mediaControl === DigishowEnvironment.ControlMediaStopAll) {

                enables["optInitialB"] = true

            } else {

                enables["optInitialA"] = true
            }
        }

        popupMoreOptions.resetOptions()
        popupMoreOptions.enableOptions(enables)
        buttonMoreOptions.visible = (Object.keys(enables).length > 0)
    }

    function setEndpointMediaOptions(options) {

        var v
        v = options["mediaAlone"];    checkMediaAlone.checked     = (v === undefined ? true  : v )
        v = options["mediaVolume"];   spinMediaVolume.value       = (v === undefined ? 100   : v / 100)
        v = options["mediaSpeed"];    spinMediaSpeed.value        = (v === undefined ? 100   : v / 100)
        v = options["mediaPosition"]; spinMediaPosition.value     = (v === undefined ? 0     : v )
        v = options["mediaDuration"]; spinMediaDuration.value     = (v === undefined ? 0     : v )
        v = options["mediaRepeat"];   checkMediaRepeat.checked    = (v === undefined ? false : v )
    }

    function getEndpointMediaOptions() {

        var options = {
            mediaAlone:    checkMediaAlone.checked,
            mediaVolume:   spinMediaVolume.value * 100,
            mediaSpeed:    spinMediaSpeed.value * 100,
            mediaPosition: spinMediaPosition.value,
            mediaDuration: spinMediaDuration.value,
            mediaRepeat:   checkMediaRepeat.checked,
        }

        return options
    }

    function setEndpointOptions(endpointInfo, endpointOptions) {

        menuType.selectOption(endpointInfo["type"])

        switch (endpointInfo["type"]) {
        case DigishowEnvironment.EndpointAPlayMedia:
            menuMediaControl.selectOption(endpointInfo["control"])

            var mediaName = endpointOptions["media"]
            var mediaIndex = digishow.findMediaWithName(interfaceIndex, mediaName)
            var mediaOptions = digishow.getMediaOptions(interfaceIndex, mediaIndex)
            var mediaUrl = "file://"
            if (mediaOptions["url"] !== undefined) mediaUrl = mediaOptions["url"]
            textMediaUrl.text = mediaUrl

            setEndpointMediaOptions(endpointOptions)

            break
        }
    }

    function getEndpointOptions() {

        var options = {}
        options["type"] = menuType.selectedItemTag

        switch (menuType.selectedItemValue) {
        case DigishowEnvironment.EndpointAPlayMedia:
            options["control"] = menuMediaControl.selectedItemValue

            if (textMediaUrl.visible) {
                var interfaceIndex = menuInterface.selectedItemValue
                var mediaUrl = textMediaUrl.text
                var mediaIndex = digishow.makeMedia(interfaceIndex, mediaUrl, "audio")

                if (mediaIndex !== -1) {
                    options["media"] = digishow.getMediaName(interfaceIndex, mediaIndex)
                    if (menuMediaControl.selectedItemValue === DigishowEnvironment.ControlMediaStart)
                        options = utilities.merge(options, getEndpointMediaOptions())
                } else {
                    messageBox.show(qsTr("Please select an audio clip file exists on your computer disks or enter a valid url of the audio clip."), qsTr("OK"))
                }
            }
            break
        }

        return options
    }

}

