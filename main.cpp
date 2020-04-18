#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "sensormodel.h"
#include"sensorlist.h"


int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	QGuiApplication app(argc, argv);

	qmlRegisterType<SensorModel>("Sensor", 1, 0, "SensorModel");
	qmlRegisterUncreatableType<SensorList>("Sensor", 1, 0, "SensorList",
										   QStringLiteral("creat QML"));
	SensorList sensorList;

	QQmlApplicationEngine engine;
	engine.rootContext()->setContextProperty(QStringLiteral("sensorList"), &sensorList);
	const QUrl url(QStringLiteral("qrc:/main.qml"));
	QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
					 &app, [url](QObject *obj, const QUrl &objUrl) {
		if (!obj && url == objUrl)
			QCoreApplication::exit(-1);
	}, Qt::QueuedConnection);
	engine.load(url);

	return app.exec();
}
