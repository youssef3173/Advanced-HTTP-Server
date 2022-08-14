#ifndef MY_FILE_H
#define MY_FILE_H

#include <QStringList>
#include <QTcpServer>
#include <QMap>
#include <iostream>
#include <string>
#include <QByteArray>
#include <QString>

using namespace std;


class myfile
{
private:
    QByteArray content;
    int size;
    int nRequested;
public:
    myfile();
    myfile( QByteArray content);
    int getSize();
    int nReq();
    QByteArray requested();
};

#endif
