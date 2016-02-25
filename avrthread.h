#ifndef AVRTHREAD_H
#define AVRTHREAD_H

#include <QObject>
#include <QThread>
#include <QProcess>
#include <QDebug>
#include <QSerialPortInfo>
#include <QFile>



class AVRThread : public QThread
{
    Q_OBJECT
public:
    explicit AVRThread(QObject *parent = 0);
    ~AVRThread();
    bool compile();
    bool objcopy();
    bool write();
    QList<QSerialPortInfo> availablePorts;
    QList<QStringList> devices;
    QList<QStringList> programmers;
    QString sourceFileName;

    int portIndex;
    int chipIndex;
    int programmerIndex;
    int baudIndex;
private:
    QProcess *cmd;

    void run();
signals:
    void sendMsg(const QString &msg);

};

#endif // AVRTHREAD_H
