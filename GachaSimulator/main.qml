import QtQuick 2.9
import QtQuick.Controls
import QtQuick.Dialogs
import utils 1.0

ApplicationWindow {
    visible: true
    width: 1280
    height: 720
    title: "GachaSimulator"

    MessageDialog {
        id: aboutDialog
        buttons: MessageDialog.Ok
        text: "GachaSimulator\n\nVersion: 1.0\nAuthor: QiLechan\nEmail:qilechan@outlook.com"
    }
    
    Utils {
        id: utils
    }

    FileDialog {
        id: fileDialog
        title: "打开配置文件"
        nameFilters: ["Json files (*.json)"]
        acceptLabel: "确定"
        rejectLabel: "取消"
        fileMode: FileDialog.OpenFile
        onAccepted: {
            const file = selectedFile.toString();
            utils.processFile(file);
        }
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("Open")
                onTriggered: {
                    fileDialog.open()
                }
            }
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
