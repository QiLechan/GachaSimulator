#pragma once
#include <QObject>
#include <QFile>
#include <QDebug> 
#include <QByteArray>
#include <QString>
#include <QUrl>
#include <stdbool.h>
#include <qthread.h>
#include <GachaSimulatorLib/GachaSimulatorLib.h>
using namespace std;


class Utils : public QObject {
	Q_OBJECT
	Q_PROPERTY(QString pool_bg READ pool_bg WRITE setPool_bg_Path NOTIFY pool_bg_PathChanged)
	Q_PROPERTY(QString videoPath READ videoPath NOTIFY videoPathChanged)
	Q_PROPERTY(QString card_img READ card_img WRITE setCard_img_Path NOTIFY card_img_PathChanged)
private:
	QString Pool_bg_Path;
	QString VideoPath;
	QString card_img_Path;
	static int Highest_quality;    //本次抽卡的最高品质，用于确定播放的动画
	class GachaThead : public QThread {    //定义抽卡线程类
	private:
		int counts = NULL;
		bool up5star = false;
		bool* up5star_ptr = &up5star;
		FILE* fp;
	public:
		void setCounts(int counts);
		void gacha(int counts);
		void run();
	};
public:
	explicit Utils(QObject* parent = nullptr) : QObject(parent) {};
	Q_INVOKABLE void processFile(const QString& filePath);
	Q_INVOKABLE void run_gacha_thread(int counts);
	Q_INVOKABLE void closeFile();
	QString pool_bg() const;
	QString videoPath() const;
	QString card_img() const;
	void setVideoPath(int n);
signals:
	//定义信号
	void showMessageBox(const QString& message);
	void pool_bg_PathChanged();
	void videoPathChanged();
	void card_img_PathChanged();
public slots:
	//定义槽
	void onMessageBoxShow(const QString& message);
	void setPool_bg_Path(const QString& path);
	void setCard_img_Path(const QString& path);
};