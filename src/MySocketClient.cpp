#include "MySocketClient.h"

#include <stdlib.h>
#include <fstream>
#include <string>
#include <iostream>
#include <QtNetwork>
#include <QByteArray>

using namespace std;

string extension = "html";


inline string removeEndLine(string s){
        while( s.at(s.length()-1) == '\r' ||
                        s.at(s.length()-1) == '\n' ){
                s = s.substr(0, s.length()-1);
        }
        return s;
}


inline string getExtension(string s){
    string out;
    int l = s.size();
    int i = 1;
    while( s[l-i] != '.' && i < l){
            out = s[l-i] + out;
            i++;
    }
    return out;
}


QByteArray qfile2ba(QString qstr){
    QFile* qfile = new QFile( qstr );
    QByteArray ba;
    if (!qfile->open(QIODevice::ReadOnly))
     {
             delete qfile;
             return ba;
     }
    ba = qfile->readAll();
    qfile->close();

    return ba;
}



MySocketClient::MySocketClient(int socketDescriptor, QObject *parent, cache *mc, stats *ms)
    : QThread(parent), socketDescriptor(socketDescriptor)
{
    local_cache = mc;
    local_stats = ms;
}



void MySocketClient::run()
{
    cout << "Starting MySocketClient::run()" << endl;
    local_stats->increm_conn();
    QTcpSocket tcpSocket;

    // ON RECUPERE LE LIEN DE COMMUNICATION AVEC LE CLIENT ET ON QUITTE EN CAS
    // DE PROBLEME...
    if (!tcpSocket.setSocketDescriptor(socketDescriptor)) {
        emit error(tcpSocket.error());
        return;
    }

    // SI LE CLIENT N'A PAS EU LE TEMPS DE NOUS TRANSMETTRE SA REQUETE,
    // ON SE MET EN ATTENTE PENDANT 1s
    while (tcpSocket.bytesAvailable() < (int)sizeof(quint16)) {
        if (!tcpSocket.waitForReadyRead( 1000 )) {
                cout << "(EE) Erreur de time out !" << endl;
                local_stats->decrem_conn();
                return;    //  TCP CLIENT SOCKET ENDS HERE
        }
    }

    // LA PREMIERE REQUETE CORRESPOND AU GET NORMALEMENT
    char tampon[65536];

    // ON RECUPERE LA REQUETE ET SA TAILLE
    int lineLength = tcpSocket.readLine(tampon, 65536);

    // ON TRANSFORME LA REQUETE SOUS FORME DE STRING
    string ligne( tampon );
    ligne = removeEndLine( ligne );

    // ON AFFICHE LA COMMANDE A L'ECRAN...
    cout << "COMMANDE : =>" << ligne << "<=" << endl;

    int pos1 = ligne.find(" ");
    string cmde = ligne.substr(0, pos1);
    ligne = ligne.substr(pos1+1, ligne.length()-pos1);

    cout << "1. : " << cmde  << endl;
    cout << "2. : " << ligne << endl;

    int pos2 = ligne.find(" ");
    string file_name = ligne.substr(0, pos2);
    ligne = ligne.substr(pos2+1, ligne.length()-pos2);

    cout << "3. : " << file_name  << endl;
    cout << "4. : '" << ligne << "'" << endl;

    text = QString::fromStdString(file_name);

    while( tcpSocket.bytesAvailable() > 0 ){
    int lineLength = tcpSocket.readLine(tampon, 65536);
    if (lineLength != -1 &&  lineLength != 0) {
                //cout << "C" << lineLength << " :: " << tampon;
        }else if(lineLength != -1 ){
                cout << "Nothing for the moment !" << endl;
        }
    }

   QString err = tr("./private_html/error.html");        // PAGE D'ERREUR 404
   // QString isDir = tr("./private_html/isDir.html");      // SI C'EST UN REPERTOIRE

   QString str = tr("./public_html") + tr(file_name.c_str());


   QFile f( str );
   QDir  d( str );

   cout << " - Chemin du fichier : " << str.toStdString() << endl;
   cout << " - isFile :          : " << f.exists() << endl;
   cout << " - isDirectory       : " << d.exists() << endl;

   if ( !local_stats->isActive ) {
       if (file_name == "/private/activate.html"){
           local_stats->toggle_activation();

           // RESPONSE PAGE ::
           QByteArray des_active;
           QDataStream page(&des_active, QIODevice::WriteOnly);
           page.setVersion(QDataStream::Qt_4_0);

           page << "<u><b><p><center>  LE SERVEUR EST ACTIVE  </p></center></b></u>" << "\n";

           tcpSocket.write( "HTTP/1.1 200 OK\n" );
           tcpSocket.write( "Content-Type: text/html\n" );
           tcpSocket.write( "\n" );

           tcpSocket.write( des_active );
       } else {
           // RESPONSE PAGE ::
           QByteArray des_active;
           QDataStream page(&des_active, QIODevice::WriteOnly);
           page.setVersion(QDataStream::Qt_4_0);

           page << "<u><b><p><center>  DESOLE, LE SERVEUR EST DESACTIVE  </p></center></b></u>" << "\n";

           tcpSocket.write( "HTTP/1.1 200 OK\n" );
           tcpSocket.write( "Content-Type: text/html\n" );
           tcpSocket.write( "\n" );

           tcpSocket.write( des_active );
       }

   } else if ( local_stats->isActive ){        // NORMALEMENT : local_stats->isActive == 1
       if( file_name == "/private/desactivate.html" ){
             local_stats->toggle_activation();

             // RESPONSE PAGE ::
             QByteArray des_active;
             QDataStream page(&des_active, QIODevice::WriteOnly);
             page.setVersion(QDataStream::Qt_4_0);

             page << "<!DOCTYPE html>";
             page << "<html>";
             page << "<body>";

             page << "<u><b><p><center>  LE SERVEUR EST DESACTIVE  </p></center></b></u>" << "\n";

             page << "</body>" << "\n";
             page << "</html>" << "\n";

             tcpSocket.write( "HTTP/1.1 200 OK\n" );
             tcpSocket.write( "Content-Type: text/html\n" );
             tcpSocket.write( "\n" );

             tcpSocket.write( des_active );

       } else if( file_name == "/private/activate.html" ){

           // RESPONSE PAGE ::
           QByteArray des_active;
           QDataStream page(&des_active, QIODevice::WriteOnly);
           page.setVersion(QDataStream::Qt_4_0);

           page << "<u><b><p><center>  LE SERVEUR EST ACTIVE  </p></center></b></u>" << "\n";

           tcpSocket.write( "HTTP/1.1 200 OK\n" );
           tcpSocket.write( "Content-Type: text/html\n" );
           tcpSocket.write( "\n" );

           tcpSocket.write( des_active );

       } else if( file_name == "/private/statistiques.html" ){
              QByteArray page_stat = local_stats->to_page();

              tcpSocket.write( "HTTP/1.1 200 OK\n" );
              tcpSocket.write( "Content-Type: text/html\n" );
              tcpSocket.write( "\n" );

              tcpSocket.write( page_stat );

              cout << " file_name == /stats.html" << endl;

       } else if( file_name == "/private/cache.html" ){
           QByteArray page_cache = local_cache->to_page();
           // QByteArray page_cache = local_stats->to_page();

           tcpSocket.write( "HTTP/1.1 200 OK\n" );
           tcpSocket.write( "Content-Type: text/html\n" );
           tcpSocket.write( "\n" );

           tcpSocket.write( page_cache );


       } else if( file_name == "/private/clear_cache.html" ){
              local_cache->clearCache();
              local_stats->clearCache();

              // RESPONSE PAGE ::
              QByteArray cache_vide;
              QDataStream page(&cache_vide, QIODevice::WriteOnly);
              page.setVersion(QDataStream::Qt_4_0);

              page << "<u><b><p><center>  LA MEMOIRE CACHE EST VIDE  </p></center></b></u>" << "\n";

              tcpSocket.write( "HTTP/1.1 200 OK\n" );
              tcpSocket.write( "Content-Type: text/html\n" );
              tcpSocket.write( "\n" );

              tcpSocket.write( cache_vide );

              cout << " file_name == /clear_cache.html" << endl;

       } else if( f.exists() == false &&  d.exists() == false ){  // ERREUR 404 LE FICHIER N'EXISTE PAS...
            cout << "ERREUR 404" << endl;
            QFile* fileErr = new QFile( err );
            if (!fileErr->open(QIODevice::ReadOnly))
            {
                delete fileErr;
                return;
            }

            tcpSocket.write( "HTTP/1.1 200 OK\n" );
            tcpSocket.write( "Content-Type: text/html\n" );
            tcpSocket.write( "\n" );

            tcpSocket.write( fileErr->readAll() );
            fileErr->close();

            local_stats->increm_requests();   // REMPLACER parent->statistiques PAR statics
            local_stats->increm_errors();
            local_stats->add_list(file_name);
            local_stats->add_stat("error.html");

            cout << "f.exists() == false &&  d.exists() == false" << endl;

       } else if( d.exists() == true ){
           // C'EST UN REPERTOIRE !

           QByteArray isDir;
           QDataStream page(&isDir, QIODevice::WriteOnly);
           page.setVersion(QDataStream::Qt_4_0);

           page << "<u><b><p><center>  C'EST UN REPERTOIRE QUI CONTIENT :  </p></center></b></u>" << "\n";
           // OPERATIONS SUR d POUR EXTRAIRE LES NOMS FICHIERS :

           QFileInfoList files = d.entryInfoList(QDir::Files | QDir::NoDotAndDotDot , QDir::Time);
           int ss =  files.size();
           for (int i = 0; i < ss; i++){
               const QFileInfo& info = files.at(i);
               QString fileName = info.fileName();
               page << "<u><p><center>   file N " << QString::fromStdString(to_string(i)) << " :: " << fileName << " </center></p></u>" << "\n";
           }

           tcpSocket.write( "HTTP/1.1 200 OK\n" );
           tcpSocket.write( "Content-Type: text/html\n" );
           tcpSocket.write( "\n" );

           tcpSocket.write( isDir );

           local_stats->increm_requests();
           local_stats->increm_traited();
           local_stats->add_list(file_name);
           local_stats->add_stat("isDir");

           cout << "d.exists() == true" << endl;

       } else if( f.exists() == true){
           string chemin = str.toStdString();
           extension = getExtension(chemin);
           cout << "chemin :: " << chemin[0] << endl;

           if( local_cache->exist(str) ){

                QByteArray ba = local_cache->read( str );

                tcpSocket.write( "HTTP/1.1 200 OK\n" );
                if( extension == "jpg"){
                    tcpSocket.write( "Content-Type: image/jpeg\n" );
                } else {
                    tcpSocket.write( "Content-Type: text/html\n" );
                }
                tcpSocket.write( "\n" );

                tcpSocket.write( ba );


                local_stats->usedCache();

                cout << "FROM CACHE (" << ba.size() << ")"  << endl;

           } else {
               QFile* file = new QFile( str );
               if (!file->open(QIODevice::ReadOnly))
                {
                        delete file;
                        return;
                }

               tcpSocket.write( "HTTP/1.1 200 OK\n" );
               if( extension == "jpg"){
                   tcpSocket.write( "Content-Type: image/jpeg\n" );
               } else {
                   tcpSocket.write( "Content-Type: text/html\n" );
               }
               tcpSocket.write( "\n" );

               tcpSocket.write( file->readAll() );
               file->close();

               QByteArray ba = qfile2ba( str );
               local_stats->addOctets(ba);

               if ( local_cache->check_capacity(ba) ){     // EST CE QU'IL Y A ASSEZ D'ESPACE DANS LA RAM
                   local_cache->add(str, ba);
               } else {   }

                cout << "TO CACHE (" << ba.size() << ")"  << endl;
           }

            local_stats->increm_requests();
            local_stats->increm_traited();
            local_stats->add_list(file_name);
            local_stats->add_stat(file_name);
            cout << "f.exists() == true " << endl;

       } else{  }
   }

    // sleep(5);   //  combien de client sont connectes simultaniment,

    tcpSocket.disconnectFromHost();
    tcpSocket.waitForDisconnected();
    local_stats->decrem_conn();

    cout << "Finishing MySocketClient::run()" << endl;
}
//! [4]
