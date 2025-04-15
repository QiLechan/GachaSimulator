#include "utils.h"

char* json = NULL;
int global_count = 0;
int Utils::Highest_quality = 0;

//抽卡线程类
void Utils::GachaThead::gacha(int counts) {
	srand((unsigned int)time(NULL));
	GachaConfig* config = parse_config(json);
	if (config == NULL) {
		qDebug() << "未选择配置文件";
		return;
	}
    fopen_s(&fp, "result.txt", "wb");
	for (int i = 0; i < counts; i++) {
		global_count++;
        Probability* prob = probability(global_count, &config->global, &config->pools[0]);
        int result = Gacha(prob);
        Item* item = return_Item(&config->pools[0], result, up5star_ptr);
		qDebug() << "第" << global_count << "次抽卡结果:" << item->name;
		if (Highest_quality < result) {
			Highest_quality = result;
			qDebug() << "当前最高品质:" << Highest_quality;
		}
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

//设置动画路径
void Utils::setVideoPath(int n) {
    switch (n)
    {
	case 4:    //单抽+3星
		VideoPath = "qrc:/qt/qml/gachasimulator/resource/one_3star.mp4";
		break;
	case 5:    //单抽+4星
		VideoPath = "qrc:/qt/qml/gachasimulator/resource/one_4star.mp4";
		break;
	case 6:    //单抽+5星
		VideoPath = "qrc:/qt/qml/gachasimulator/resource/one_5star.mp4";
		break;
	case 14:   //十连+4星
		VideoPath = "qrc:/qt/qml/gachasimulator/resource/ten_4star.mp4";
		break;
	case 15:   //十连+5星
		VideoPath = "qrc:/qt/qml/gachasimulator/resource/ten_5star.mp4";
		break;
    default:
        break;
    }
	emit videoPathChanged();
}

//启动抽卡线程
void Utils::run_gacha_thread(int counts) {
    if (json == NULL) {
		onMessageBoxShow("未选择配置文件");
		qDebug() << "未打开文件";
		return;
    }
	Highest_quality = 0;    //每次抽卡前将最高品质清零
    GachaThead* thread = new GachaThead();
    thread->setCounts(counts);
    thread->start(); 
	thread->wait();
	setVideoPath(Highest_quality + counts);
	qDebug() << "线程执行完毕";
	delete thread;
	qDebug() << "线程已删除";
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

QString Utils::videoPath() const {
	return VideoPath;
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