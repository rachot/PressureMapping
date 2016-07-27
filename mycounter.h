#ifndef MYCOUNTER_H
#define MYCOUNTER_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTimer>
#include <QtQuick/QQuickView>
#include <QPixmap>
#include <QQmlEngine>

class MyCounter : public QObject
{
Q_OBJECT
public:
    explicit MyCounter(QObject *parent = 0);

    void strat(void);
private:
    int counter;
    QQmlApplicationEngine *engine;
    QQuickView *view;
    QTimer *timer;



signals:

public slots:
    void UpdateDat(void);

};



#endif // MYCOUNTER_H
