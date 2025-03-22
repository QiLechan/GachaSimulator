#pragma once
#include <QObject>
#include <QFile>
#include <QDebug> 
#include <QByteArray>
#include <QUrl>
#include <stdbool.h>
#include <GachaSimulatorLib/GachaSimulatorLib.h>
using namespace std;

class Utils : public QObject {
	Q_OBJECT
public:
	Q_INVOKABLE void processFile(const QString& filePath);
};