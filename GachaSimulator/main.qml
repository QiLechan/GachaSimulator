import QtQuick 2.9
import QtQuick.Controls
import QtQuick.Dialogs

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: "GachaSimulator"

    MessageDialog {
        id: aboutDialog
        buttons: MessageDialog.Ok
        text: "GachaSimulator\n\nVersion: 1.0\nAuthor: QiLechan\nEmail:qilechan@outlook.com"
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit()
            }
        }
        Menu {
            title: qsTr("About")
            MenuItem {
                text: qsTr("About")
                onTriggered: {
                    aboutDialog.open()
                }
            }
        }
    }
}
