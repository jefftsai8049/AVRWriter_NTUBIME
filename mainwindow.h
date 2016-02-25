#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QFileDialog>
#include <QSerialPortInfo>
#include <QFile>
#include <QDebug>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>

#include "avrthread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_write_pushButton_clicked();

    void on_load_pushButton_clicked();

    void receiveMsg(const QString &msg);

    void receiveFinish(const int &step);

private:
    Ui::MainWindow *ui;
    QTextCodec *codec;
    AVRThread *avr;

    QString sourceFileName;


    QList<QStringList> loadDeviceList();
    QList<QStringList> loadProgrammerList();
};

#endif // MAINWINDOW_H
