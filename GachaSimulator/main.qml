import QtQuick 2.9
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Controls
import QtQml
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

    Dialog {
        id: messagedialog
        title: "提示"
        standardButtons: Dialog.Ok
        anchors.centerIn: parent
        property string messageText: ""
        Label {
            text: messagedialog.messageText
        }
    }

    Connections {
        target: utils
        function onShowMessageBox(message) {
            console.log(message);
            messagedialog.messageText = message;
            messagedialog.open();
        }
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
                text: qsTr("打开配置文件")
                onTriggered: {
                    fileDialog.open()
                }
            }
            MenuItem {
                text: qsTr("关闭文件")
                onTriggered: {
                    utils.closeFile();
                }
            }
            MenuItem {
                text: qsTr("退出")
                onTriggered: Qt.quit()
            }
        }
        Menu {
            title: qsTr("帮助")
            MenuItem {
                text: qsTr("关于")
                onTriggered: {
                    aboutDialog.open()
                }
            }
        }
    }
    Button {
        text: "10"
        highlighted: true
        onClicked: {
            utils.run_gacha_thread(10);
        }
    }
}
