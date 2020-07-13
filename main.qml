import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Dialogs 1.0
import QtQuick.Controls 2.3

import com.company.fileupload 1.0

Window {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Popup {
        id: popup
        anchors.centerIn: parent

        Column {
            spacing: 10
            padding: 10

            Label {
                id: lblMessage
            }

            Button {
                id: btnclose
                text: "Close"
                onClicked: popup.close()
            }
        }


    }

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        folder: shortcuts.home
        onAccepted: {
            console.log("You chose: " + fileDialog.fileUrls)
            txtFile.text = fileDialog.fileUrl
        }
        onRejected: {
            console.log("Canceled")
            txtFile.text = ""
        }

    }

    FileUpload {
        id: uploader
        onComplete: {
            lblMessage.text = "Status: " + status
            popup.open()
        }
    }

    Column {
        id: column
        width: 442
        height: 267
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        spacing: 10
        padding: 25

        Label {
            id: label
            text: qsTr("URL")
        }

        TextField {
            id: txtURL
            width: 400
            text: qsTr("https://postman-echo.com/post")
        }

        Label {
            id: label1
            text: qsTr("File")
        }

        TextField {
            id: txtFile
            width: 400
        }

        Row {
            id: row
            width: 400
            height: 75
            spacing: 10
            padding: 25

            Button {
                id: btnChoose
                text: qsTr("Choose File")
                onClicked: {
                    fileDialog.visible = true;
                }
            }

            Button {
                id: btnPost
                text: qsTr("HTTP POST")
                onClicked: {
                    uploader.postFile(txtURL.text,txtFile.text,"form-data; name=\"notes\"")
                }
            }
        }

    }

}
