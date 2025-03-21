#pragma once
#include <QObject>
#include <QFile>
#include <QDebug> 

class FileProcessor : public QObject {
	Q_OBJECT
public:
	Q_INVOKABLE void processFile(const QString& filePath);
};