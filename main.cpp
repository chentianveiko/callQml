#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "cppsrc/changeColor/changecolor.h"
#include <QMetaObject>
#include <QDebug>
#include <QVariant>
#include <QColor>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    QObject * root = NULL;
    QList<QObject *> rootObjects = engine.rootObjects();
    int count = rootObjects.size();
    for(int i=0; i < count; i++){
        if(rootObjects.at(i)->objectName() == "rootObject"){
            root = rootObjects.at(i);
            break;
        }
    }

    new ChangeQmlColor(root);  // 这里传入的是root,所以定时器溢出时访问的属性永远都是rootObject的属性

    // 使用QML对象的信号
    QObject *quitButton = root->findChild<QObject *>("quitButton");
    if(quitButton){
        QObject::connect(quitButton,SIGNAL(clicked()),&app,SLOT(quit()));
    }

    // 设置QML对象的属性
    QObject *textLabel = root->findChild<QObject *>("textLabel");
    if(textLabel){
        //1. failed call
        bool bRet = QMetaObject::invokeMethod(textLabel,"setText",Q_ARG(QString,"world hello"));
        qDebug() << "Call setText return - " << bRet;
        textLabel->setProperty("color",QColor::fromRgb(255,0,0));

        bRet = QMetaObject::invokeMethod(textLabel,"doLayout");
        qDebug() << "Call doLayout return - " << bRet;
    }

    return app.exec();
}

