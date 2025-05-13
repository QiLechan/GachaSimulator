import QtQuick 2.9
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Controls
import QtQml
import QtMultimedia
import Qt5Compat.GraphicalEffects
import utils 1.0

ApplicationWindow {
    visible: true
    width: 1280
    height: 720
    //禁止缩放窗口，因为懒得写自适应布局
    minimumWidth: width
    minimumHeight: height
    maximumWidth: width
    maximumHeight: height
    //禁用最大化按钮
    flags: Qt.Window | Qt.WindowTitleHint | Qt.WindowMinimizeButtonHint | Qt.WindowCloseButtonHint

    title: "GachaSimulator"
    
    MessageDialog {
        id: aboutDialog
        buttons: MessageDialog.Ok
        text: "GachaSimulator\n\nVersion: 1.0\nAuthor: QiLechan\nEmail:qilechan@outlook.com"
    }
    
    Utils {
        id: utils
    }
    //定义消息框
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

    //连接信号和槽
    Connections {
        target: utils
        function onShowMessageBox(message) {
            console.log(message);
            messagedialog.messageText = message;
            messagedialog.open();
        }
    }

    Connections {
        target: utils
        function onCard_img_PathChanged() {
            container.refreshAllCards();
        }
    }

    Image {
        id: background
        source: "qrc:/qt/qml/gachasimulator/resource/background.png"
        fillMode: Image.PreserveAspectCrop
        anchors.centerIn: parent
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
            title: qsTr("文件")
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

    Item {
        id: tenButton
        width: 267
        height: 66

        anchors {
            right: parent.right
            bottom: parent.bottom
            rightMargin: 50
            bottomMargin: 50
        }

        Image {
            id: tenbuttonImage
            width: parent.width
            height: parent.height
            source: "qrc:/qt/qml/gachasimulator/resource/button.png"
            fillMode: Image.PreserveAspectFit
            anchors.centerIn: parent
            property int success: 1    // 1表示抽卡失败，0表示成功
        
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    tenbuttonImage.success = utils.run_gacha_thread(10)
                    if (tenbuttonImage.success == 0){
                        videoOutput.visible = true
                        mediaPlayer.play();
                        container.cardCount = 10 // 设置卡片数量
                        container.refreshAllCards();
                    }
                }
            }
        }

        Text {
            text: "10"
            anchors.centerIn: parent
            font.pixelSize: 24   
        }
    }

    Item {
        id: oneButton
        width: 267
        height: 66
    
        anchors {
            right: tenButton.left
            rightMargin: 20
            verticalCenter: tenButton.verticalCenter
        }

        Image {
            id: onebuttonImage
            width: parent.width
            height: parent.height
            source: "qrc:/qt/qml/gachasimulator/resource/button.png"
            fillMode: Image.PreserveAspectFit
            anchors.centerIn: parent
            property int success: 1
        
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    onebuttonImage.success = utils.run_gacha_thread(1)
                    if (onebuttonImage.success == 0){
                        videoOutput.visible = true
                        mediaPlayer.play();
                        container.cardCount = 1 // 设置卡片数量
                        container.refreshAllCards();
                    }
                }
            }
        }

        Text {
            text: "1"
            anchors.centerIn: parent
            font.pixelSize: 24   
        }
    }
    Image {
        id: poolImage
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: oneButton.top
            bottomMargin: -90
        }
        source: utils.pool_bg
        width: 1000
        height: 750
        fillMode: Image.PreserveAspectFit
    }

    Image {
        id: gachaResultbg
        source: "qrc:/qt/qml/gachasimulator/resource/splash-background.png"
        fillMode: Image.PreserveAspectFit
        anchors {
            horizontalCenter: parent.horizontalCenter
            verticalCenter: parent.verticalCenter
        }
        visible: false
        MouseArea {
            anchors.fill: parent
            onClicked: {
                gachaResultbg.visible = false // 点击时隐藏
                container.allCardsVisible = false // 隐藏结果卡片
            }
        }
        onVisibleChanged: {
            if (visible) { // 仅在变为 true 时触发
                container.allCardsVisible = true // 显示结果卡片
            }
        }
    }

Item {
    id: container
    property bool allCardsVisible: false
    property int cardCount: 10
    anchors.fill: parent
    function refreshAllCards() {
        for (var i = 0; i < repeater.count; i++) {
            var card = repeater.itemAt(i);
            if (card) {
                card.refreshSource();
            }
        }
    }
    Component {
        id: gachaResultCard
        Item {
            id: gachaResultcard
            // 通过 property 接收外部传递的序号
            property int cardIndex: index // 这里捕获 Repeater 的 index
            width: 95
            height: 409
            visible: container.allCardsVisible

            function refreshSource() {
                // 使用 cardIndex 替代固定值 0
                gachaResultcardimg.refreshSource(cardIndex);
            }
    
            Image {
                id: gachaResultcardbg
                anchors.fill: parent
                source: "qrc:/qt/qml/gachasimulator/resource/resultcard-bg.png"
                fillMode: Image.PreserveAspectFit
            }

            Image {
                id: gachaResultcardimg
                source: ""
                anchors.fill: parent
                fillMode: Image.PreserveAspectCrop
                // 修改函数接收动态索引参数
                function refreshSource(idx) {
                    source = utils.get_card_img_path(idx)
                }
                scale: 2
                visible: false
            }

            OpacityMask {
                anchors.fill: parent
                source: gachaResultcardimg
                maskSource: gachaResultcardbg
            }
        }
    }

    Row {
        spacing: 0
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        height: parent.height
        Repeater {
            id: repeater
            model: container.cardCount
            delegate: gachaResultCard
        }
    }
}

    MediaPlayer {
        id: mediaPlayer
        source: utils.videoPath
        autoPlay: false
        videoOutput: videoOutput
        audioOutput: audioOutput
        onMediaStatusChanged: {
            if (mediaStatus === MediaPlayer.EndOfMedia) {
                videoOutput.visible = false // 播放结束隐藏
                gachaResultbg.visible = true // 显示结果背景
            }
        }
    }

    VideoOutput {
        id: videoOutput
        anchors.fill: parent
        fillMode: VideoOutput.Stretch
        visible: false
        MouseArea {
            anchors.fill: parent
            onPressed: {
                mediaPlayer.stop();
                videoOutput.visible = false // 点击时隐藏
                gachaResultbg.visible = true // 显示结果背景
            }
        }
    }

    AudioOutput {
        id: audioOutput
        volume: 1.0
    }
}
