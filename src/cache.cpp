#include "cache.h"

using namespace std;

cache::cache()
{
    RAM = {};
}


int cache::cacheSize(){
    int s = 0;
    QMapIterator<QString, myfile> i(RAM);
    while(i.hasNext()){
        i.next();
        myfile *f = new myfile();
        *f = i.value();
        s = s +  f->getSize();
    }
    return s ;
}


// la taille de la memoire max est 10kB = 10240B
bool cache::check_capacity(QByteArray ba)
{
    int s1 = this->cacheSize();
    int s2 = ba.size();
    if( 10240 - s1 > s2 ){
        return true;
    }
    return false;
}


QByteArray cache::read(QString name){
    myfile f = RAM.value(name);         // .requested();
    QByteArray ba = f.requested();
    RAM[name] = f;
    return ba;
}


void cache::add(QString name, QByteArray content)
{
    myfile file = myfile(content);
    RAM.insert(name, file);
}


bool cache::exist(QString _name){
    return RAM.contains(_name);
}


void cache::remove()
{
    QMap<QString, myfile>::iterator end = RAM.end();
    --end;
    RAM.erase(end);
}


void cache::clearCache(){
    RAM = {};
}


QByteArray cache::to_page(){
    QByteArray page_cache;
    QDataStream page(&page_cache, QIODevice::WriteOnly);
    page.setVersion(QDataStream::Qt_4_0);


    page << "<!DOCTYPE html>";
    page << "<html>";
    page << "<body>";

    QMapIterator<QString, myfile> i(this->RAM);
    page << "<u><b><p><center> MEMOIRE CACHE :: </p></center></b></u>" << "\n";
    while(i.hasNext()){
        i.next();
        myfile *f = new myfile();
        *f = i.value();
        page << "<pre class=\"tab\"><p><center> FILE_NAME : " << i.key() << " ,               NREQ : " << QString::fromStdString(to_string(f->nReq()))  <<  " ,              SIZE: " << QString::fromStdString(to_string(f->getSize()))  <<  "</p></center></pre>\n";
        delete f;
    }


    page << "</body>";
    page << "</html>";

    return page_cache;

}
