#include "utils.h"

char* json = NULL;
int global_count = 0;

void Utils::GachaThead::gacha(int counts) {
	GachaConfig* config = parse_config(json);
	if (config == NULL) {
		qDebug() << "解析配置失败";
		return;
	}
    fopen_s(&fp, "result.txt", "wb");
	for (int i = 0; i < counts; i++) {
        Probability* prob = probability(global_count, &config->global, &config->pools[0]);
		global_count++;
        int result = Gacha(prob);
        Item* item = return_Item(&config->pools[0], result, up5star_ptr);
		qDebug() << "第" << global_count << "次抽卡结果:" << item->name;
        if (result == 5)
            global_count = 0;
		parse_result(item, fp);
		free(prob);
	}
    fclose(fp);
}
void Utils::GachaThead::setCounts(int counts) {
	this->counts = counts;
}

void Utils::GachaThead::run() {
    qDebug() << "当前子线程ID:" << QThread::currentThreadId();
    qDebug() << "开始执行线程";
	gacha(counts);
	qDebug() << "全局计数:" << global_count;
	qDebug() << "线程执行完毕";
}
void Utils::processFile(const QString& filePath) {
   QUrl url(filePath);

   if (!url.isValid() || !url.isLocalFile()) {
       qWarning() << "无效的文件 URL：" << filePath;
       return;
   }

   QString localPath = url.toLocalFile();

   if (localPath.isEmpty()) {
       qWarning() << "转换后的路径为空";
       return;
   }
   if (!QFile::exists(localPath)) {
       qWarning() << "文件不存在：" << localPath;
       return;
   }
    QByteArray ba = localPath.toLatin1();
    qDebug() << "C++ 收到文件路径:" << ba.data();
   
    FILE* fp;
    fopen_s(&fp, ba.data(), "rb");
    if (fp == NULL) {
	    qDebug() << "文件打开失败";
	    return;
    }
    fseek(fp, 0, SEEK_END);
    long len = ftell(fp);
    json = new char[len + 1];
    fseek(fp, 0, SEEK_SET);
    fread(json, sizeof(char), len + 1, fp);
    fclose(fp);
    qDebug() << "C++ 读取文件成功:\n" << json;
}

void Utils::runThread(int counts) {
    GachaThead* thread = new GachaThead();
    thread->setCounts(counts);
    thread->start(); 
	thread->wait();
}