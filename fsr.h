#ifndef FSR_H
#define FSR_H

#include <QtGui/QGuiApplication>
#include "qtquick2applicationviewer.h"
#include <QThread>
#include <QDebug>

class FSRSensor : public QObject
{
Q_OBJECT

public:
    int data[100];

private:

signals:

public slots:

};

#endif // FSR_H
