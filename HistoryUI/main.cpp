#include <QtGui/QApplication>
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QGraphicsObject>
#include <QObject>
#include <qmlapplicationviewer.h>
#include "HistoryEngine.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    HistoryEngine engine;
#ifdef LOCK_POTRAITORIEN
    Qt::WidgetAttribute attr;
    attr = Qt::WA_LockPortraitOrientation;
    dv.setAttribute(attr);
#endif
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    QScopedPointer<QmlApplicationViewer> viewer(QmlApplicationViewer::create());
    viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    QDeclarativeContext* context = viewer->rootContext();
    context->setContextProperty("engine",(QObject*)&engine);
    viewer->setSource(QUrl("qrc:/qml/main.qml"));
    QObject *rootObject = dynamic_cast<QObject*>(viewer->rootObject());
    QObject::connect(&engine,SIGNAL(updateReady(QVariant)),rootObject,SLOT(onUpdateAvailable()));
    QObject::connect(&engine,SIGNAL(error(QString,QUrl)),rootObject,SLOT(error(QString)));
    viewer->showExpanded();
    return app->exec();
}
