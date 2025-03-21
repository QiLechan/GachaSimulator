#include "utils.h"

char* json = NULL;
int global_count = 1; 

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

void Utils::gacha(int counts) {
GachaConfig* config = parse_config(json);
int i = 1;
FILE* fp2;
fopen_s(&fp2, "result.txt", "wb");
int five_star_count = 0;
bool value = false;
bool* up5star = &value;    // 大保底机制
for (int i = 1; i <= counts ; i++)
{
	Probability* prob = probability(global_count, &config->global, &config->pools[0]);
	int result = Gacha(prob);
	Item* item = return_Item(&config->pools[0], result, up5star);
	if (result == 5)
	{
		i = 1;
		five_star_count++;
	}
	parse_result(item, fp2);
	global_count++;
	free(prob);
}
fclose(fp2);
qDebug() << "五星次数：" << five_star_count;
}