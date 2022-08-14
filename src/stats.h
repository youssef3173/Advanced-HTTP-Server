#include "MySocketServer.h"

#ifndef STAT_H
#define STAT_H

#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <QMap>
#include <string>
#include <QFile>
#include <QVector>
#include <QDataStream>
#include <QByteArray>


using namespace std;



using namespace std;


class stats // : public MySocketServer
{
private:
    int requests;
    int traited;
    int errors;    // traited + errors = requests
    int nOctets;
    int nConnected;

    int fromCache;

    QVector<string> list;
    QMap<string, int> stat;
public:
    stats();
    stats(int a, int b, int c, QVector<string> list, QMap<string, int> stat);
    void increm_requests();
    void increm_traited();
    void increm_errors();
    void increm_conn();
    void decrem_conn();
    void add_list(string file_name);   // AJOUTER UN ELEMET AU VECTEUR
    void add_stat(string key);      // AJOUTER UN ELEMET AU MAP
    QByteArray to_page();

    void usedCache();   // incremente fromCache si le fichier etait lu de la mem cache
    double percentCache();
    void clearCache();

    bool isActive;
    void toggle_activation();
    void addOctets(QByteArray ba);
};

#endif
