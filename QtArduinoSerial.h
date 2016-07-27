#ifndef QTARDUINOSERIAL_H
#define QTARDUINOSERIAL_H

#include <QtSerialPort/QtSerialPort>




class QtArduinoSerial : public QObject
{
    Q_OBJECT

public:
    explicit QtArduinoSerial(QObject *parent = 0);


private slots:
    void readSerial();
    void initSerialPort();
    void testWrite();

private:
    QSerialPort *arduino;
    static const quint16 arduino_uno_vendor_id = 9025;
    static const quint16 arduino_uno_product_id = 61;
    QByteArray serialData;
    QString serialBuffer;
    QString parsed_data;
    double temperature_value;
};

#endif // QTARDUINOSERIAL_H
