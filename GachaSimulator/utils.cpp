#include "utils.h"

char* json = NULL;
int global_count = 0;

//抽卡线程类
void Utils::GachaThead::gacha(int counts) {
	GachaConfig* config = parse_config(json);
	if (config == NULL) {
		qDebug() << "未选择配置文件";
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

//处理文件
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
    GachaConfig* config = parse_config(json);
    int pool_count = config->pool_count;
    GachaPool* pools = (GachaPool*)malloc(sizeof(GachaPool) * pool_count);
	char* pool_background = (char*)malloc(sizeof(char) * 50 * pool_count);
    for (int i = 0; i < pool_count; i++)
    {
		pools[i] = config->pools[i];
        pool_background = config->pools[i].pool_background;
		qDebug() << "卡池" << i << "的名称:" << pools[i].pool_name;
    }
	QString pool_background_path = QString::fromUtf8(&pool_background[0]);
	setImagePath(pool_background_path);
}

//启动抽卡线程
void Utils::run_gacha_thread(int counts) {
    if (json == NULL) {
		onMessageBoxShow("未选择配置文件");
		qDebug() << "未打开文件";
		return;
    }
    GachaThead* thread = new GachaThead();
    thread->setCounts(counts);
    thread->start(); 
	thread->wait();
}

//清除已加载文件
void Utils::closeFile() {
    if (json != NULL)
        json = NULL;
    else {
        qDebug() << "未打开文件";
    }
}

QString Utils::imagePath() const {
	return ImagePath;
}

void Utils::setImagePath(const QString& path) {
    if (ImagePath != path) {
        ImagePath = path;
        emit imagePathChanged();
    }
}

//向前端发送显示消息框信号
void Utils::onMessageBoxShow(const QString& message) {
	emit showMessageBox(message);    //转发信号
}