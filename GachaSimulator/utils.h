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
	Q_PROPERTY(QString imagePath READ imagePath WRITE setImagePath NOTIFY imagePathChanged)
private:
	QString ImagePath;
	QString VideoPath;
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
	QString imagePath() const;
	QString videoPath() const;
	void setVideoPath(int n);
signals:
	//定义信号
	void showMessageBox(const QString& message);
	void imagePathChanged();
public slots:
	//定义槽
	void onMessageBoxShow(const QString& message);
	void setImagePath(const QString& path);
};