#include <QtGui/QGuiApplication>
#include "qtquick2applicationviewer.h"
#include <QDebug>
//#include <masterthread.h>
#include <QThread>
#include <QMutex>
#include <QCoreApplication>
#include <QDebug>
#include <QTextCodec>
#include <QTimer>
#include <QtMath>
#include <QElapsedTimer>
#include <QQuickImageProvider>
#include <QPixmap>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQmlProperty>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv/cv.h"
#include <opencv2/imgproc.hpp>
#include "opencv2/core.hpp"
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/opencv.hpp>

#include <fsr.h>


#include <QtSerialPort/QSerialPort>
#include <QTextStream>
#include <QTimer>
#include <QByteArray>
#include <QObject>

#include <QObject>

#include <QQmlContext>

#include <QtQuick/QQuickView>

#include <mycounter.h>

#include <QQmlEngine>


 cv::Mat im_color;
 cv::Mat mat;

int aaa_test;



class ColorImageProvider : public QQuickImageProvider
{
public:
    ColorImageProvider();
    ~ColorImageProvider();

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);

private:

};


ColorImageProvider::ColorImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Pixmap)
{
}
ColorImageProvider::~ColorImageProvider()
{
}

QPixmap ColorImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{

    // get camera input
     cv::Mat img;
    if(mat.empty())
        img = cv::imread("/home/pchotpc/Pictures/user.png",1);
        //cv::Mat img = mat;
    else
        img = mat;

        // resize
        *size = QSize(img.cols, img.rows);
        int width  = requestedSize.width()  > 0 ? requestedSize.width()  : img.rows;
        int height = requestedSize.height() > 0 ? requestedSize.height() : img.cols;
        cv::Mat resized_img(width, height, img.type());
        cv::resize(img, resized_img, resized_img.size(), cv::INTER_CUBIC);

        // BGR -> ARGB
        cv::cvtColor(resized_img, resized_img, CV_BGR2BGRA);
        std::vector<cv::Mat> bgra;
        cv::split(resized_img, bgra);
        std::swap(bgra[0], bgra[3]);
        std::swap(bgra[1], bgra[2]);

        QImage video_img(resized_img.data, resized_img.cols, resized_img.rows, QImage::Format_ARGB32);
        return QPixmap::fromImage(video_img);

}

class SerialPortReader : public QObject
{
    Q_OBJECT

public:
    SerialPortReader(QSerialPort *serialPort, QObject *parent = 0);
    ~SerialPortReader();

    int time_mS;
    int FSR_Sensor[100];

    QString strSenA;
    QString strSenB;
    QString strSenC;
    QString strSenD;
    QString strSenE;
    QByteArray  m_readData;

private slots:
    void handleReadyRead();
    void handleTimeout();
    void handleError(QSerialPort::SerialPortError error);
    void CalTimeout();


private:
    QSerialPort *m_serialPort;
    QSerialPort serialPort;
    QTextStream m_standardOutput;

    QTimer      m_timer;
    QTimer     *timer_send;
    QTimer      m_timer_cal;


};




 int num_picture=1;


 int img_width = 880;
 int img_height= 880;

 int data[910][910][100];  // data[800][800][0] = sensor1 ; data[800][800][1] = sensor2
 int sum_data[910][910];

 char pressure_color[] = "PressureMappingColor";
 cv::Mat pressure_image = cv::Mat::zeros( img_width, img_height, CV_8U);

 int sen_global[100];


 // exchange data between class and global data
 void someFn(FSRSensor& refS)
 {
     for(int i=0;i<100;i++)
         sen_global[i] = refS.data[i];
 }


SerialPortReader::SerialPortReader(QSerialPort *serialPort, QObject *parent)
    : QObject(parent)
    , m_serialPort(serialPort)
    , m_standardOutput(stdout)

{
    connect(m_serialPort, &QSerialPort::readyRead, this, &SerialPortReader::handleReadyRead);
    connect(m_serialPort, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &SerialPortReader::handleError);
    connect(&m_timer, &QTimer::timeout, this, &SerialPortReader::handleTimeout);

    connect(&m_timer_cal, &QTimer::timeout, this, &SerialPortReader::CalTimeout);

    m_timer.start(100);
    m_timer_cal.start(1);  // 1mS

}

SerialPortReader::~SerialPortReader()
{
}

void SerialPortReader::handleReadyRead()
{
    m_readData.append(m_serialPort->readAll());

    if (!m_timer.isActive())
        m_timer.start(100);
}

void SerialPortReader::CalTimeout()
{
    time_mS++;
}




void SerialPortReader::handleTimeout()
{
    if (m_readData.isEmpty())
    {
        //m_standardOutput << QObject::tr("No data was currently available for reading from port %1").arg(m_serialPort->portName()) << endl;
    }
    else // readable
    {
        QString response(m_readData);


        strSenA = response.mid(response.indexOf("A"),response.indexOf("\\r\\n"));
        strSenB = response.mid(response.indexOf("B"),response.indexOf("\\r\\n"));
        strSenC = response.mid(response.indexOf("C"),response.indexOf("\\r\\n"));
        strSenD = response.mid(response.indexOf("D"),response.indexOf("\\r\\n"));
        strSenE = response.mid(response.indexOf("E"),response.indexOf("\\r\\n"));

        if(strSenA.size() >= 41)
        {

            FSR_Sensor[0] = strSenA.mid(strSenA.indexOf("A")+1,strSenA.indexOf("a")-1).toInt();
            FSR_Sensor[1] = strSenA.mid(strSenA.indexOf("a")+1,strSenA.indexOf("b")-strSenA.indexOf("a")-1).toInt();
            FSR_Sensor[2] = strSenA.mid(strSenA.indexOf("b")+1,strSenA.indexOf("c")-strSenA.indexOf("b")-1).toInt();
            FSR_Sensor[3] = strSenA.mid(strSenA.indexOf("c")+1,strSenA.indexOf("d")-strSenA.indexOf("c")-1).toInt();
            FSR_Sensor[4] = strSenA.mid(strSenA.indexOf("d")+1,strSenA.indexOf("e")-strSenA.indexOf("d")-1).toInt();
            FSR_Sensor[5] = strSenA.mid(strSenA.indexOf("e")+1,strSenA.indexOf("f")-strSenA.indexOf("e")-1).toInt();
            FSR_Sensor[6] = strSenA.mid(strSenA.indexOf("f")+1,strSenA.indexOf("g")-strSenA.indexOf("f")-1).toInt();
            FSR_Sensor[7] = strSenA.mid(strSenA.indexOf("g")+1,strSenA.indexOf("h")-strSenA.indexOf("g")-1).toInt();
            FSR_Sensor[8] = strSenA.mid(strSenA.indexOf("h")+1,strSenA.indexOf("i")-strSenA.indexOf("h")-1).toInt();
            FSR_Sensor[9] = strSenA.mid(strSenA.indexOf("i")+1,strSenA.indexOf("j")-strSenA.indexOf("i")-1).toInt();
            FSR_Sensor[10] = strSenA.mid(strSenA.indexOf("j")+1,strSenA.indexOf("k")-strSenA.indexOf("j")-1).toInt();
            FSR_Sensor[11] = strSenA.mid(strSenA.indexOf("k")+1,strSenA.indexOf("l")-strSenA.indexOf("k")-1).toInt();
            FSR_Sensor[12] = strSenA.mid(strSenA.indexOf("l")+1,strSenA.indexOf("m")-strSenA.indexOf("l")-1).toInt();
            FSR_Sensor[13] = strSenA.mid(strSenA.indexOf("m")+1,strSenA.indexOf("n")-strSenA.indexOf("m")-1).toInt();
            FSR_Sensor[14] = strSenA.mid(strSenA.indexOf("n")+1,strSenA.indexOf("o")-strSenA.indexOf("n")-1).toInt();
            FSR_Sensor[15] = strSenA.mid(strSenA.indexOf("o")+1,strSenA.indexOf("p")-strSenA.indexOf("o")-1).toInt();
            FSR_Sensor[16] = strSenA.mid(strSenA.indexOf("p")+1,strSenA.indexOf("q")-strSenA.indexOf("p")-1).toInt();
            FSR_Sensor[17] = strSenA.mid(strSenA.indexOf("q")+1,strSenA.indexOf("r")-strSenA.indexOf("q")-1).toInt();
            FSR_Sensor[18] = strSenA.mid(strSenA.indexOf("r")+1,strSenA.indexOf("s")-strSenA.indexOf("r")-1).toInt();
            FSR_Sensor[19] = strSenA.mid(strSenA.indexOf("s")+1,strSenA.indexOf("t")-strSenA.indexOf("s")-1).toInt();
        }
        if(strSenB.size() >= 41)
        {
            FSR_Sensor[20] = strSenB.mid(strSenB.indexOf("B")+1,strSenB.indexOf("a")-1).toInt();
            FSR_Sensor[21] = strSenB.mid(strSenB.indexOf("a")+1,strSenB.indexOf("b")-strSenB.indexOf("a")-1).toInt();
            FSR_Sensor[22] = strSenB.mid(strSenB.indexOf("b")+1,strSenB.indexOf("c")-strSenB.indexOf("b")-1).toInt();
            FSR_Sensor[23] = strSenB.mid(strSenB.indexOf("c")+1,strSenB.indexOf("d")-strSenB.indexOf("c")-1).toInt();
            FSR_Sensor[24] = strSenB.mid(strSenB.indexOf("d")+1,strSenB.indexOf("e")-strSenB.indexOf("d")-1).toInt();
            FSR_Sensor[25] = strSenB.mid(strSenB.indexOf("e")+1,strSenB.indexOf("f")-strSenB.indexOf("e")-1).toInt();
            FSR_Sensor[26] = strSenB.mid(strSenB.indexOf("f")+1,strSenB.indexOf("g")-strSenB.indexOf("f")-1).toInt();
            FSR_Sensor[27] = strSenB.mid(strSenB.indexOf("g")+1,strSenB.indexOf("h")-strSenB.indexOf("g")-1).toInt();
            FSR_Sensor[28] = strSenB.mid(strSenB.indexOf("h")+1,strSenB.indexOf("i")-strSenB.indexOf("h")-1).toInt();
            FSR_Sensor[29] = strSenB.mid(strSenB.indexOf("i")+1,strSenB.indexOf("j")-strSenB.indexOf("i")-1).toInt();
            FSR_Sensor[30] = strSenB.mid(strSenB.indexOf("j")+1,strSenB.indexOf("k")-strSenB.indexOf("j")-1).toInt();
            FSR_Sensor[31] = strSenB.mid(strSenB.indexOf("k")+1,strSenB.indexOf("l")-strSenB.indexOf("k")-1).toInt();
            FSR_Sensor[32] = strSenB.mid(strSenB.indexOf("l")+1,strSenB.indexOf("m")-strSenB.indexOf("l")-1).toInt();
            FSR_Sensor[33] = strSenB.mid(strSenB.indexOf("m")+1,strSenB.indexOf("n")-strSenB.indexOf("m")-1).toInt();
            FSR_Sensor[34] = strSenB.mid(strSenB.indexOf("n")+1,strSenB.indexOf("o")-strSenB.indexOf("n")-1).toInt();
            FSR_Sensor[35] = strSenB.mid(strSenB.indexOf("o")+1,strSenB.indexOf("p")-strSenB.indexOf("o")-1).toInt();
            FSR_Sensor[36] = strSenB.mid(strSenB.indexOf("p")+1,strSenB.indexOf("q")-strSenB.indexOf("p")-1).toInt();
            FSR_Sensor[37] = strSenB.mid(strSenB.indexOf("q")+1,strSenB.indexOf("r")-strSenB.indexOf("q")-1).toInt();
            FSR_Sensor[38] = strSenB.mid(strSenB.indexOf("r")+1,strSenB.indexOf("s")-strSenB.indexOf("r")-1).toInt();
            FSR_Sensor[39] = strSenB.mid(strSenB.indexOf("s")+1,strSenB.indexOf("t")-strSenB.indexOf("s")-1).toInt();
        }
        if(strSenC.size() >= 41)
        {
            FSR_Sensor[40] = strSenC.mid(strSenC.indexOf("C")+1,strSenC.indexOf("a")-1).toInt();
            FSR_Sensor[41] = strSenC.mid(strSenC.indexOf("a")+1,strSenC.indexOf("b")-strSenC.indexOf("a")-1).toInt();
            FSR_Sensor[42] = strSenC.mid(strSenC.indexOf("b")+1,strSenC.indexOf("c")-strSenC.indexOf("b")-1).toInt();
            FSR_Sensor[43] = strSenC.mid(strSenC.indexOf("c")+1,strSenC.indexOf("d")-strSenC.indexOf("c")-1).toInt();
            FSR_Sensor[44] = strSenC.mid(strSenC.indexOf("d")+1,strSenC.indexOf("e")-strSenC.indexOf("d")-1).toInt();
            FSR_Sensor[45] = strSenC.mid(strSenC.indexOf("e")+1,strSenC.indexOf("f")-strSenC.indexOf("e")-1).toInt();
            FSR_Sensor[46] = strSenC.mid(strSenC.indexOf("f")+1,strSenC.indexOf("g")-strSenC.indexOf("f")-1).toInt();
            FSR_Sensor[47] = strSenC.mid(strSenC.indexOf("g")+1,strSenC.indexOf("h")-strSenC.indexOf("g")-1).toInt();
            FSR_Sensor[48] = strSenC.mid(strSenC.indexOf("h")+1,strSenC.indexOf("i")-strSenC.indexOf("h")-1).toInt();
            FSR_Sensor[49] = strSenC.mid(strSenC.indexOf("i")+1,strSenC.indexOf("j")-strSenC.indexOf("i")-1).toInt();
            FSR_Sensor[50] = strSenC.mid(strSenC.indexOf("j")+1,strSenC.indexOf("k")-strSenC.indexOf("j")-1).toInt();
            FSR_Sensor[51] = strSenC.mid(strSenC.indexOf("k")+1,strSenC.indexOf("l")-strSenC.indexOf("k")-1).toInt();
            FSR_Sensor[52] = strSenC.mid(strSenC.indexOf("l")+1,strSenC.indexOf("m")-strSenC.indexOf("l")-1).toInt();
            FSR_Sensor[53] = strSenC.mid(strSenC.indexOf("m")+1,strSenC.indexOf("n")-strSenC.indexOf("m")-1).toInt();
            FSR_Sensor[54] = strSenC.mid(strSenC.indexOf("n")+1,strSenC.indexOf("o")-strSenC.indexOf("n")-1).toInt();
            FSR_Sensor[55] = strSenC.mid(strSenC.indexOf("o")+1,strSenC.indexOf("p")-strSenC.indexOf("o")-1).toInt();
            FSR_Sensor[56] = strSenC.mid(strSenC.indexOf("p")+1,strSenC.indexOf("q")-strSenC.indexOf("p")-1).toInt();
            FSR_Sensor[57] = strSenC.mid(strSenC.indexOf("q")+1,strSenC.indexOf("r")-strSenC.indexOf("q")-1).toInt();
            FSR_Sensor[58] = strSenC.mid(strSenC.indexOf("r")+1,strSenC.indexOf("s")-strSenC.indexOf("r")-1).toInt();
            FSR_Sensor[59] = strSenC.mid(strSenC.indexOf("s")+1,strSenC.indexOf("t")-strSenC.indexOf("s")-1).toInt();
        }
        if(strSenD.size() >= 41)
        {
            FSR_Sensor[60] = strSenD.mid(strSenD.indexOf("D")+1,strSenD.indexOf("a")-1).toInt();
            FSR_Sensor[61] = strSenD.mid(strSenD.indexOf("a")+1,strSenD.indexOf("b")-strSenD.indexOf("a")-1).toInt();
            FSR_Sensor[62] = strSenD.mid(strSenD.indexOf("b")+1,strSenD.indexOf("c")-strSenD.indexOf("b")-1).toInt();
            FSR_Sensor[63] = strSenD.mid(strSenD.indexOf("c")+1,strSenD.indexOf("d")-strSenD.indexOf("c")-1).toInt();
            FSR_Sensor[64] = strSenD.mid(strSenD.indexOf("d")+1,strSenD.indexOf("e")-strSenD.indexOf("d")-1).toInt();
            FSR_Sensor[65] = strSenD.mid(strSenD.indexOf("e")+1,strSenD.indexOf("f")-strSenD.indexOf("e")-1).toInt();
            FSR_Sensor[66] = strSenD.mid(strSenD.indexOf("f")+1,strSenD.indexOf("g")-strSenD.indexOf("f")-1).toInt();
            FSR_Sensor[67] = strSenD.mid(strSenD.indexOf("g")+1,strSenD.indexOf("h")-strSenD.indexOf("g")-1).toInt();
            FSR_Sensor[68] = strSenD.mid(strSenD.indexOf("h")+1,strSenD.indexOf("i")-strSenD.indexOf("h")-1).toInt();
            FSR_Sensor[69] = strSenD.mid(strSenD.indexOf("i")+1,strSenD.indexOf("j")-strSenD.indexOf("i")-1).toInt();
            FSR_Sensor[70] = strSenD.mid(strSenD.indexOf("j")+1,strSenD.indexOf("k")-strSenD.indexOf("j")-1).toInt();
            FSR_Sensor[71] = strSenD.mid(strSenD.indexOf("k")+1,strSenD.indexOf("l")-strSenD.indexOf("k")-1).toInt();
            FSR_Sensor[72] = strSenD.mid(strSenD.indexOf("l")+1,strSenD.indexOf("m")-strSenD.indexOf("l")-1).toInt();
            FSR_Sensor[73] = strSenD.mid(strSenD.indexOf("m")+1,strSenD.indexOf("n")-strSenD.indexOf("m")-1).toInt();
            FSR_Sensor[74] = strSenD.mid(strSenD.indexOf("n")+1,strSenD.indexOf("o")-strSenD.indexOf("n")-1).toInt();
            FSR_Sensor[75] = strSenD.mid(strSenD.indexOf("o")+1,strSenD.indexOf("p")-strSenD.indexOf("o")-1).toInt();
            FSR_Sensor[76] = strSenD.mid(strSenD.indexOf("p")+1,strSenD.indexOf("q")-strSenD.indexOf("p")-1).toInt();
            FSR_Sensor[77] = strSenD.mid(strSenD.indexOf("q")+1,strSenD.indexOf("r")-strSenD.indexOf("q")-1).toInt();
            FSR_Sensor[78] = strSenD.mid(strSenD.indexOf("r")+1,strSenD.indexOf("s")-strSenD.indexOf("r")-1).toInt();
            FSR_Sensor[79] = strSenD.mid(strSenD.indexOf("s")+1,strSenD.indexOf("t")-strSenD.indexOf("s")-1).toInt();
        }

       if(strSenE.size() >= 41)
       {
            FSR_Sensor[80] = strSenE.mid(strSenE.indexOf("E")+1,strSenE.indexOf("a")-1).toInt();
            FSR_Sensor[81] = strSenE.mid(strSenE.indexOf("a")+1,strSenE.indexOf("b")-strSenE.indexOf("a")-1).toInt();
            FSR_Sensor[82] = strSenE.mid(strSenE.indexOf("b")+1,strSenE.indexOf("c")-strSenE.indexOf("b")-1).toInt();
            FSR_Sensor[83] = strSenE.mid(strSenE.indexOf("c")+1,strSenE.indexOf("d")-strSenE.indexOf("c")-1).toInt();
            FSR_Sensor[84] = strSenE.mid(strSenE.indexOf("d")+1,strSenE.indexOf("e")-strSenE.indexOf("d")-1).toInt();
            FSR_Sensor[85] = strSenE.mid(strSenE.indexOf("e")+1,strSenE.indexOf("f")-strSenE.indexOf("e")-1).toInt();
            FSR_Sensor[86] = strSenE.mid(strSenE.indexOf("f")+1,strSenE.indexOf("g")-strSenE.indexOf("f")-1).toInt();
            FSR_Sensor[87] = strSenE.mid(strSenE.indexOf("g")+1,strSenE.indexOf("h")-strSenE.indexOf("g")-1).toInt();
            FSR_Sensor[88] = strSenE.mid(strSenE.indexOf("h")+1,strSenE.indexOf("i")-strSenE.indexOf("h")-1).toInt();
            FSR_Sensor[89] = strSenE.mid(strSenE.indexOf("i")+1,strSenE.indexOf("j")-strSenE.indexOf("i")-1).toInt();
            FSR_Sensor[90] = strSenE.mid(strSenE.indexOf("j")+1,strSenE.indexOf("k")-strSenE.indexOf("j")-1).toInt();
            FSR_Sensor[91] = strSenE.mid(strSenE.indexOf("k")+1,strSenE.indexOf("l")-strSenE.indexOf("k")-1).toInt();
            FSR_Sensor[92] = strSenE.mid(strSenE.indexOf("l")+1,strSenE.indexOf("m")-strSenE.indexOf("l")-1).toInt();
            FSR_Sensor[93] = strSenE.mid(strSenE.indexOf("m")+1,strSenE.indexOf("n")-strSenE.indexOf("m")-1).toInt();
            FSR_Sensor[94] = strSenE.mid(strSenE.indexOf("n")+1,strSenE.indexOf("o")-strSenE.indexOf("n")-1).toInt();
            FSR_Sensor[95] = strSenE.mid(strSenE.indexOf("o")+1,strSenE.indexOf("p")-strSenE.indexOf("o")-1).toInt();
            FSR_Sensor[96] = strSenE.mid(strSenE.indexOf("p")+1,strSenE.indexOf("q")-strSenE.indexOf("p")-1).toInt();
            FSR_Sensor[97] = strSenE.mid(strSenE.indexOf("q")+1,strSenE.indexOf("r")-strSenE.indexOf("q")-1).toInt();
            FSR_Sensor[98] = strSenE.mid(strSenE.indexOf("r")+1,strSenE.indexOf("s")-strSenE.indexOf("r")-1).toInt();
            FSR_Sensor[99] = strSenE.mid(strSenE.indexOf("s")+1,strSenE.indexOf("t")-strSenE.indexOf("s")-1).toInt();
      }

        //qDebug() << "Serial data[0]  : " << FSR_Sensor[0] << endl;

        FSRSensor sen;

        for(int i=0;i<100;i++)
           sen.data[i]  = FSR_Sensor[i];
        someFn(sen);

        strSenA.clear();
        strSenB.clear();
        strSenC.clear();
        strSenD.clear();
        strSenE.clear();

        m_readData.clear();

    }
}

void SerialPortReader::handleError(QSerialPort::SerialPortError serialPortError)
{
    if (serialPortError == QSerialPort::ReadError)
    {
        m_standardOutput << QObject::tr("An I/O error occurred while reading the data from port %1, error: %2").arg(m_serialPort->portName()).arg(m_serialPort->errorString()) << endl;
        QCoreApplication::exit(1);
    }
}



class ThreadDrawing : public QThread
{
    QElapsedTimer timer;
public:


private:

    void run()
    {

        while(1)
        {
            /*qDebug() << "Thread #1 GUI data : " ;

            for(int i=0;i<100;i++)
                qDebug() << sen_global[i];

            qDebug() << "Picture processing..." << endl;*/


            int _width = img_width;
            int _height = img_width;


            float _sigmaX = 40.0f;
            float _sigmaY = 40.0f;
            float _theta = 0.0f;
            float _offset = 0.0f;

            //float _centerX[10] = {80,160,240,320,400,480,560,630,720,800};
            float _centerX[10] = {70,140,210,280,350,420,490,560,630,700};

            float xc[100], yc[100], xm[100], ym[100], tmpX[100], tmpY[100], u[100];

            float cosTheta = (float)qCos(_theta / 180.0f * M_PI);
            float sinTheta = (float)qSin(_theta / 180.0f * M_PI);

             _sigmaX = 1.0f / _sigmaX;
             _sigmaY = 1.0f / _sigmaY;

             //qDebug() << "Start 2D gaussian create" << endl;

             timer.start();

             for(int l=0;l<100;l++)
             {
                 for (int i = 0; i < _width; i++)
                 {
                     if(l>=0 && l<10)
                        xc[l] = i - _centerX[l];
                     else if(l>=10 && l<20)
                        xc[l] = i - _centerX[l%10]-70;
                     else if(l>=20 && l<30)
                        xc[l] = i - _centerX[l%20]-70;
                     else if(l>=30 && l<40)
                        xc[l] = i - _centerX[l%30]-70;
                     else if(l>=40 && l<50)
                        xc[l] = i - _centerX[l%40]-70;
                     else if(l>=50 && l<60)
                        xc[l] = i - _centerX[l%50]-70;
                     else if(l>=60 && l<70)
                        xc[l] = i - _centerX[l%60]-70;
                     else if(l>=70 && l<80)
                        xc[l] = i - _centerX[l%70]-70;
                     else if(l>=80 && l<90)
                        xc[l] = i - _centerX[l%80]-70;
                     else if(l>=90 && l<100)
                         xc[l] = i - _centerX[l%90]-70;

                    for (int j = 0; j < _height; j++)
                    {
                        if( l>=0 && l <10 )
                            yc[l] = j - 700;
                        else if( l>=10 && l <20 )
                            yc[l] = j - 630;
                        else if( l>=20 && l <30 )
                            yc[l] = j - 560;
                        else if( l>=30 && l <40 )
                            yc[l] = j - 490;
                        else if( l>=40 && l <50 )
                            yc[l] = j - 420;
                        else if( l>=50 && l <60 )
                            yc[l] = j - 350;
                        else if( l>=60 && l <70 )
                            yc[l] = j - 280;
                        else if( l>=70 && l <80 )
                            yc[l] = j - 210;
                        else if( l>=80 && l <90 )
                            yc[l] = j - 140;
                        else if( l>=90 && l <100 )
                            yc[l] = j - 70;

                        xm[l] = xc[l] * cosTheta - yc[l] * sinTheta;
                        ym[l] = xc[l] * sinTheta + yc[l] * cosTheta;
                        tmpX[l] = xm[l] * _sigmaX;
                        tmpY[l] = ym[l] * _sigmaY;
                        u[l] = tmpX[l] * tmpX[l] + tmpY[l] * tmpY[l];
                        //data[j][i][l] = _offset + sen_global[l] * (float)qExp(-u[l] / 2);
                        data[j][i][l] = _offset + sen_global[l] * cv::exp(-u[l] / 2);
                    }
                }
             }
             float timer_count = timer.elapsed();
             qDebug() << "Generate 2D Gaussian in " << timer_count/1000 << " Seconds" << endl;
             timer.restart();

             float per = 0;
            for(int i=0;i<img_width;i++)
            {
                for(int j=0;j<img_height;j++)
                {
                     sum_data[i][j] = data[j][i][0] + data[j][i][1] + data[j][i][2] + data[j][i][3]+ data[j][i][4]+ data[j][i][5]+ data[j][i][6]+ data[j][i][7]+ data[j][i][8]+ data[j][i][9]
                         + data[j][i][10]+ data[j][i][11]+ data[j][i][12]+ data[j][i][13]+ data[j][i][14]+ data[j][i][15]+ data[j][i][16]+ data[j][i][17]+ data[j][i][18]+ data[j][i][19]
                         + data[j][i][20]+ data[j][i][21]+ data[j][i][22]+ data[j][i][23]+ data[j][i][24]+ data[j][i][25]+ data[j][i][26]+ data[j][i][27]+ data[j][i][28]+ data[j][i][29]
                         + data[j][i][30]+ data[j][i][31]+ data[j][i][32]+ data[j][i][33]+ data[j][i][34]+ data[j][i][35]+ data[j][i][36]+ data[j][i][37]+ data[j][i][38]+ data[j][i][39]
                         + data[j][i][40]+ data[j][i][41]+ data[j][i][42]+ data[j][i][43]+ data[j][i][44]+ data[j][i][45]+ data[j][i][46]+ data[j][i][47]+ data[j][i][48]+ data[j][i][49]
                         + data[j][i][50]+ data[j][i][51]+ data[j][i][52]+ data[j][i][53]+ data[j][i][54]+ data[j][i][55]+ data[j][i][56]+ data[j][i][57]+ data[j][i][58]+ data[j][i][59]
                         + data[j][i][60]+ data[j][i][61]+ data[j][i][62]+ data[j][i][63]+ data[j][i][64]+ data[j][i][65]+ data[j][i][66]+ data[j][i][67]+ data[j][i][68]+ data[j][i][69]
                         + data[j][i][70]+ data[j][i][71]+ data[j][i][72]+ data[j][i][73]+ data[j][i][74]+ data[j][i][75]+ data[j][i][76]+ data[j][i][77]+ data[j][i][78]+ data[j][i][79]
                         + data[j][i][80]+ data[j][i][81]+ data[j][i][82]+ data[j][i][83]+ data[j][i][84]+ data[j][i][85]+ data[j][i][86]+ data[j][i][87]+ data[j][i][88]+ data[j][i][89]
                         + data[j][i][90]+ data[j][i][91]+ data[j][i][92]+ data[j][i][93]+ data[j][i][94]+ data[j][i][95]+ data[j][i][96]+ data[j][i][97]+ data[j][i][98]+ data[j][i][99];

                     if(sum_data[i][j] > 210)
                         sum_data[i][j] = 210;
                     cv::line(pressure_image,cv::Point( i, j ),cv::Point( i, j ),cv::Scalar( sum_data[i][j], 0, 0 ),1,8,0);
                     //qDebug() << "Percent : " <<  (per/774400)*100 << " %" << endl;
                     //per++
                }
            }


            cv::applyColorMap(pressure_image,im_color,cv::COLORMAP_JET);
            //cv::imshow( pressure_color , im_color );
            qDebug() << "Image generated" << endl;

            mat = im_color;

            timer_count = timer.elapsed();
            qDebug() << "Generate picture in " << timer_count/1000 << " Seconds" << endl;
            timer.restart();

            QString str = QString::fromStdString("/home/pchotpc/Desktop/PressureMappingPictures/" ) + QString::number( num_picture ) + ".jpg";
            qDebug() << str << endl;




            cv::imwrite( str.toStdString() , im_color);
            num_picture++;
        }

    }
};

void MyCounter::UpdateDat()
{

// qDebug() << aaa_test << endl;
    aaa_test++;


    engine = new QQmlApplicationEngine();
    engine->load(QUrl(QStringLiteral("/home/pchotpc/PressureMapping/qml/PressureMapping/main.qml")));





}

void MyCounter::strat()
{
    timer->start(100);
}


MyCounter::MyCounter(QObject *parent) : QObject(parent)
{
    engine = new QQmlApplicationEngine();
    engine->load(QUrl(QStringLiteral("/home/pchotpc/PressureMapping/qml/PressureMapping/main.qml")));

    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(UpdateDat()));




}



int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);


    QtQuick2ApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("/home/pchotpc/PressureMapping/qml/PressureMapping/main.qml"));
    viewer.showExpanded();


    /*int argumentCount = QGuiApplication::arguments().size();
    QStringList argumentList = QCoreApplication::arguments();

    QTextStream standardOutput(stdout);

    if (argumentCount == 1) {
        qDebug() << QObject::tr("Usage: %1 <serialportname> [baudrate]").arg(argumentList.first()) << endl;
        return 1;
    }

    QSerialPort serialPort;
    //QString serialPortName = argumentList.at(1);
    QString serialPortName = "/dev/ttyACM0";
    serialPort.setPortName(serialPortName);

    int serialPortBaudRate = (argumentCount > 2) ? argumentList.at(2).toInt() : QSerialPort::Baud115200;
    serialPort.setBaudRate(serialPortBaudRate);

    if (!serialPort.open(QIODevice::ReadWrite))
    {
        standardOutput << QObject::tr("Failed to open port %1, error: %2").arg(serialPortName).arg(serialPort.errorString()) << endl;
        return 1;
    }


    SerialPortReader serialPortReader(&serialPort);

    //MyCounter *c = new MyCounter();
    //c->strat();


    ThreadDrawing t;
    t.start();*/


    MyCounter *c = new MyCounter();
    c->strat();

    QQmlEngine enginea;
    QQmlComponent component(&enginea, "/home/pchotpc/PressureMapping/qml/PressureMapping/main.qml");
    QObject *object = component.create();
    QQmlProperty::write(object, "abc", 23232);





    return app.exec();
}
