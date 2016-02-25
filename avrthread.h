#ifndef AVRTHREAD_H
#define AVRTHREAD_H

#include <QObject>
#include <QProcess>
#include <QDebug>
#include <QSerialPortInfo>
#include <QFile>



class AVRThread : public QObject
{
    Q_OBJECT
public:
    explicit AVRThread(QObject *parent = 0);
    ~AVRThread();
    bool compile();
    bool objcopy();
    bool write();

    void run();
    QList<QSerialPortInfo> availablePorts;
    QList<QStringList> devices;
    QList<QStringList> programmers;
    QString sourceFileName;

    int portIndex;
    int chipIndex;
    int programmerIndex;
    int baudIndex;

    int errorCount;
    int warningCount;

    int step;
private:
    QProcess *cmd;


signals:
    void sendMsg(const QString &msg);
    void sendFinish(const int &step);
private slots:
    void receiveMsg();
    void receiveFinished();

};

#endif // AVRTHREAD_H
