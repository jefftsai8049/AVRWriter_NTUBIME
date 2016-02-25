#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    avr = new AVRThread;
    connect(avr,SIGNAL(sendMsg(QString)),this,SLOT(receiveMsg(QString)));
    connect(avr,SIGNAL(sendFinish(int)),this,SLOT(receiveFinish(int)));

    this->setWindowIcon(QIcon("icon/ic.png"));

    avr->availablePorts = QSerialPortInfo::availablePorts();
    for(int i = 0; i < avr->availablePorts.size(); i ++)
    {
        ui->port_comboBox->addItem(avr->availablePorts[i].portName()+" "+avr->availablePorts[i].description());
        if(avr->availablePorts[i].description().indexOf("Arduino") > -1)
            ui->port_comboBox->setCurrentIndex(i);
    }
    QList<qint32> bauds = QSerialPortInfo::standardBaudRates();
    for(int i = 0; i < bauds.size(); i ++)
    {
        ui->baudrate_comboBox->addItem(QString::number(bauds[i]));
        if(bauds[i] == 19200)
            ui->baudrate_comboBox->setCurrentIndex(i);
    }
    avr->devices = loadDeviceList();
    avr->programmers = loadProgrammerList();
    for(int i = 0; i < avr->devices.size(); i ++)
    {
        ui->chip_comboBox->addItem(avr->devices[i][2]);
        if(avr->devices[i][2] == "ATmega328P")
            ui->chip_comboBox->setCurrentIndex(i);
    }
    for(int i = 0; i < avr->programmers.size(); i ++)
    {
        ui->programmer_comboBox->addItem(avr->programmers[i][1]);
        if(avr->programmers[i][1] == "Arduino")
            ui->programmer_comboBox->setCurrentIndex(i);
    }

}

MainWindow::~MainWindow()
{
    delete avr;
    delete ui;
}

void MainWindow::on_write_pushButton_clicked()
{
    ui->write_pushButton->setEnabled(0);

    avr->sourceFileName = ui->source_file_name_lineEdit->text();
    avr->portIndex = ui->port_comboBox->currentIndex();
    avr->chipIndex = ui->chip_comboBox->currentIndex();
    avr->programmerIndex = ui->programmer_comboBox->currentIndex();
    avr->baudIndex = ui->baudrate_comboBox->currentText().toInt();
    avr->errorCount = 0;
    avr->warningCount = 0;
    avr->step = 0;

    avr->run();

    ui->write_pushButton->setEnabled(1);
}

void MainWindow::on_load_pushButton_clicked()
{
    avr->sourceFileName = QFileDialog::getOpenFileName(this, tr("Open File"),"C:\\",tr("C Source File (*.c)"));
    avr->sourceFileName.replace('/','\\');
    ui->source_file_name_lineEdit->setText(avr->sourceFileName);
}

void MainWindow::receiveMsg(const QString &msg)
{
    ui->textBrowser->insertPlainText(msg);
}

void MainWindow::receiveFinish(const int &step)
{
    qDebug() << step;
    if(step == 1)
    {
        ui->error_count_pushButton->setText("Error ("+QString::number(avr->errorCount)+")");
        ui->warning_count_pushButton->setText("Warning ("+QString::number(avr->warningCount)+")");
    }
    else if(step == 3)
    {
        QStringList fileNames = ui->source_file_name_lineEdit->text().split("\\");
        QFile::remove(fileNames[fileNames.size()-1].replace(".c",".hex"));
    }
}

QList<QStringList> MainWindow::loadDeviceList()
{
    QFile file("device.csv");
    if(!file.exists())
    {
        ui->statusBar->showMessage("Device List no Found!");
        return QList<QStringList>();
    }
    file.open(QIODevice::ReadOnly);
    QList<QStringList> devices;
    while(!file.atEnd())
    {
        QString msg = file.readLine();
        msg = msg.trimmed();
        QStringList device = msg.split(",");
        devices.append(device);
    }
    return devices;
}

QList<QStringList> MainWindow::loadProgrammerList()
{
    QFile file("programmer.csv");
    if(!file.exists())
    {
        ui->statusBar->showMessage("Programmer List no Found!");
        return QList<QStringList>();
    }
    file.open(QIODevice::ReadOnly);
    QList<QStringList> programmers;
    while(!file.atEnd())
    {
        QString msg = file.readLine();
        msg = msg.trimmed();
        QStringList programmer = msg.split(",");
        programmers.append(programmer);
    }
    return programmers;
}
