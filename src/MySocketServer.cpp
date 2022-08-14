#include "MySocketServer.h"
#include "MySocketClient.h"

#include <stdlib.h>

//! [0]
MySocketServer::MySocketServer(QObject *parent)
    : QTcpServer(parent)
{

}

void MySocketServer::incomingConnection(qintptr socketDescriptor)
{
    cout << "(II) Launching the Network monitor process" << endl;

    // CREATION DE L'OBJET EN CHARGE DES REPONSES RESEAU
    MySocketClient *thread = new MySocketClient(socketDescriptor, this, this->mem_cache ,this->statistiques);
    // ON INDIQUE QUE LORSQU'UN CLIENT SE CONNECTE ON DELEGE LA REPONSE
    // AU PROCESSUS DEFINI CI DESSUS...
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    // ON DEMARRE LE PROCESSUS DE REPONSE POUR LE METTRE EN ATTENTE !
    thread->start();

    // PUIS ON REND LA MAIN EN ATTENTE D'UN CLIENT
    cout << "(II) Network monitor process launch : OK" << endl;
}
