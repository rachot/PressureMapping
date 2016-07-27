/*
#include "masterthread.h"

#include <QCoreApplication>
#include <QDebug>
#include <QTextCodec>
#include <QTimer>
#include <QtMath>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv/cv.h"
#include <opencv2/imgproc.hpp>

#include <opencv2/imgproc.hpp>
#include "opencv2/core.hpp"

#include <opencv2/contrib/contrib.hpp>

#include <fsr.h>


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

        qDebug() << "Serial data[0]  : " << FSR_Sensor[0] << endl;

        strSenA.clear();
        strSenB.clear();
        strSenC.clear();
        strSenD.clear();
        strSenE.clear();

        m_readData.clear();

    }

   // QCoreApplication::quit();
}

void SerialPortReader::handleError(QSerialPort::SerialPortError serialPortError)
{
    if (serialPortError == QSerialPort::ReadError)
    {
        m_standardOutput << QObject::tr("An I/O error occurred while reading the data from port %1, error: %2").arg(m_serialPort->portName()).arg(m_serialPort->errorString()) << endl;
        QCoreApplication::exit(1);
    }
}*/



