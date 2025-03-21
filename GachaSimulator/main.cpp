#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <GachaSimulatorLib/GachaSimulatorLib.h>
#include <FileProcessor.h>

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN) && QT_VERSION_CHECK(5, 6, 0) <= QT_VERSION && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    time_t t;
    srand((unsigned)time(&t));
    FILE* fp;
    if (fopen_s(&fp, "Genshin.json", "rb") != 0)
    {
        char* genshinjson = createGenshinJson();
        FILE* fw;
        if (fopen_s(&fw, "Genshin.json", "wb") == 0)
        {
            fwrite(genshinjson, sizeof(char), strlen(genshinjson), fw);
            free(genshinjson);
            fclose(fw);
        }
        fopen_s(&fp, "Genshin.json", "rb");
    }

    QGuiApplication app(argc, argv);

	qmlRegisterType<FileProcessor>("fileprocessor", 1, 0, "FileProcessor");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/qt/qml/gachasimulator/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
