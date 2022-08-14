#ifndef MEM_CACHE_H
#define MEM_CACHE_H

#include "myfile.h"

#include <QStringList>
#include <QTcpServer>
#include <QMap>
#include <iostream>
#include <string>
#include <QIODevice>
#include <QDataStream>
#include <QByteArray>
#include <QString>

using namespace std;

class cache
{
private:
    // QHash efficace pour les grads tailles, mais l'ordre des elements change aleatoirement.
    QMap<QString, myfile> RAM;
    int cacheSize();
public:
    cache();
    bool check_capacity(QByteArray ba);         // return booleen : est ce que il y assez d'epace ou pas?

    void add(QString _name, QByteArray content);      // add(file)
    QByteArray read(QString name);

    bool exist(QString name);
    void remove();                             // remove(cache.files[end_item])

    void clearCache();

    QByteArray to_page();
};

#endif

