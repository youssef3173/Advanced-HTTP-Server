#ifndef FORTUNETHREAD_H
#define FORTUNETHREAD_H

#include "cache.h"
#include "stats.h"


#include <iostream>
using namespace std;



#include <QThread>
#include <QTcpSocket>
#include <QFile>
#include <QDir>


class MySocketClient : public QThread
{
    Q_OBJECT

public:
    cache *local_cache = new cache();
    stats *local_stats = new stats();

    MySocketClient(int socketDescriptor, QObject *parent, cache *mc, stats *ms);
    void run();

signals:
    void error(QTcpSocket::SocketError socketError);

private:

    int socketDescriptor;
    QString text;
};

#endif
