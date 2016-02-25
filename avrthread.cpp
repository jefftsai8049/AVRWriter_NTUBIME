#include "avrthread.h"

AVRThread::AVRThread(QObject *parent)
{
    cmd = new QProcess;
    connect(cmd,SIGNAL(readyReadStandardError()),this,SLOT(receiveMsg()));
    connect(cmd,SIGNAL(finished(int)),this,SLOT(receiveFinished()));
    //    errorCount =0;
    //    warningCount =0;
}

AVRThread::~AVRThread()
{

}

void AVRThread::run()
{
    //qDebug() << step;
    if(step == 0)
    {
        if(!compile())
        {

            step++;
        }
    }
    else if(step == 1)
    {
        if(!objcopy())
        {
            emit sendFinish(step);
            step++;
        }
    }
    else if(step == 2)
    {
        if(!write())
        {
            emit sendFinish(step);
            step++;
        }
    }
    else
    {
        emit sendFinish(step);
        step = 0;
    }
    //    if(this->objcopy())
    //        return;

    //    if(this->write())
    //        return;
}

void AVRThread::receiveMsg()
{
    QByteArray out = cmd->readAllStandardError();
    QStringList outMsg =QString::fromLocal8Bit(out).split("\r");
    for(int i = 0; i < outMsg.size();i++)
    {
        if(outMsg[i].indexOf("error:") > -1)
            errorCount++;
        if(outMsg[i].indexOf("warning:") > -1)
            warningCount++;
        //        outMsg[i].replace("\n","\r\n");
        emit sendMsg(outMsg[i]);
    }
}

void AVRThread::receiveFinished()
{
    //    qDebug() << "finished";
    if(!errorCount > 0)
        run();

}

bool AVRThread::compile()
{


    QString compileCMD
            = "avr8-gnu-toolchain\\bin\\avr-gcc.exe -std=c99 -g -Os -Wall -mcall-prologues -fno-exceptions";
    QString deviceName = devices[chipIndex][0];
    compileCMD.append(" -mmcu="+deviceName);


    compileCMD.append(" -o "+this->sourceFileName.replace(".c",".obj"));
    compileCMD.append(" "+this->sourceFileName.replace(".obj",".c"));

    cmd->start(compileCMD);


    return 0;
}

bool AVRThread::objcopy()
{
    if(errorCount > 0)
    {
        //ui->statusBar->showMessage("Compile Error!");
        //ui->write_pushButton->setEnabled(1);
        return 1;
    }
    QString objcopyCMD = "avr8-gnu-toolchain\\bin\\avr-objcopy.exe -R .eeprom -O ihex";
    objcopyCMD.append(" "+this->sourceFileName.replace(".c",".obj"));
    objcopyCMD.append(" "+this->sourceFileName.replace(".obj",".hex"));

    cmd->start(objcopyCMD);
    //    cmd->waitForFinished();
    //    cmd->waitForReadyRead();

    //    QByteArray out = cmd->readAllStandardError();
    //    QStringList outMsg =QString::fromLocal8Bit(out).split("\r\n");
    //    for(int i = 0; i < outMsg.size(); i++)
    //    {
    //        emit sendMsg(outMsg[i]);
    //    }
    //    qDebug() << outMsg.size();
    //    if(outMsg.size() > 1)
    //    {
    //        //ui->statusBar->showMessage("Objcopy Error!");
    //        //ui->write_pushButton->setEnabled(1);
    //        return 1;
    //    }
    return 0;
}

bool AVRThread::write()
{
    QStringList hexFileNames = this->sourceFileName.split("\\");
    QFile::copy(this->sourceFileName, hexFileNames[hexFileNames.size()-1]);

    QString writeCMD = "avr8-gnu-toolchain\\avrdude.exe";
    writeCMD.append(" -P "+availablePorts[portIndex].portName());
    writeCMD.append(" -b "+QString::number(baudIndex));
    writeCMD.append(" -p "+devices[chipIndex][1]);
    writeCMD.append(" -c "+programmers[programmerIndex][0]);
    writeCMD.append(" -U flash:w:"+hexFileNames[hexFileNames.size()-1]);


    cmd->start(writeCMD);
    //    cmd->waitForFinished();
    //    cmd->waitForReadyRead();


//    QByteArray out = cmd->readAllStandardError();

//    QStringList outMsg =QString::fromLocal8Bit(out).split("\r\n");
//    //qDebug() << out << outMsg.size();
//    for(int i = 0; i < outMsg.size(); i++)
//    {
//        emit sendMsg(outMsg[i]);
//    }

    return 0;
}
