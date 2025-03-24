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
private:
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
signals:
	void showMessageBox(const QString& message);    //定义信号
public slots:
	void onMessageBoxShow(const QString& message);  //定义槽
};