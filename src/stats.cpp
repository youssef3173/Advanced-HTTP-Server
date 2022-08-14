#include "stats.h"
#include <stdlib.h>


QVector<string> remove(QVector<string> v){  //  REMOVE DUPLICATES
    QVector<string> out;
    for( auto i = 0; i < v.size(); i++){
        if(!out.contains(v[i])){
            out.append(v[i]);
        }
    }
    return out;
}


stats::stats(){  // CONSTRUCTEUR
    requests = 0;
    traited = 0;
    errors = 0;
    nConnected = 0;
    fromCache = 0;
    isActive = true;
}


stats::stats(int _a, int _b, int _c, QVector<string> _list, QMap<string, int> _stat){
    requests = _a;
    traited = _b;
    errors = _c;
    nConnected = 0;
    fromCache = 0;
    isActive = true;
    assert(_list.size() == _a);
    list = _list;
    assert(_stat.size() == remove(_list).size());
    stat = _stat;
}


void stats::increm_requests(){
    this->requests++;
}


void stats::increm_traited(){
    this->traited++;
}


void stats::increm_errors(){
    this->errors++;
}


void stats::increm_conn(){
    nConnected++;
}


void stats::decrem_conn(){
    nConnected--;
}


void stats::add_list(string file_name){
    this->list.append(file_name);
}


void stats::add_stat(string key){
    if(!this->stat.contains(key)){
        this->stat.insert(key,0);
    }
    this->stat[key]++;
}


QByteArray stats::to_page(){
    QByteArray page_stat;
    QDataStream page(&page_stat, QIODevice::WriteOnly);
    page.setVersion(QDataStream::Qt_4_0);



    // ADD BODY : add also Nbr of requests, and stat_map
    QString s = "";
    page << "<!DOCTYPE html>";
    page << "<html>";
    page << "<body>";

    page << "<u><b><p><center> STATISTIQUES :: </p></center></b></u>" << "\n";

    page << "<p><center>" << "nbr de requetes recus : " << QString::fromStdString(to_string(requests)) << "</p></center>\n";
    page << "<p><center>" << " nbr de requetes traite : " << QString::fromStdString(to_string(traited)) << "</p></center>\n";
    page << "<p><center>" << " nombre d'erreur : " << QString::fromStdString(to_string(errors)) << "</p></center>\n";
    page << "<p><center>" << " nbr de clients connectes : " << QString::fromStdString(to_string(nConnected)) << "</p></center>\n";
    page << "<p><center>" << " nbr d'octets transmis : " << QString::fromStdString(to_string(nOctets)) << "</p></center>\n";
    page << "<p><center>" << " utilisation du cache : " << QString::fromStdString(to_string(percentCache())) << " % . " << "</p></center>\n";

    page << "<u><b><p><center> LOADED PAGES :: </p></center></b></u>" << "\n";
    for(int i = 0; i<this->list.size(); i++){
        s = QString::fromStdString(this->list.at(i));
        page << "<p><center>" << s << "</p></center>\n";
    }

    QMapIterator<string, int> i(this->stat);
    page << "<u><b><p><center> PAGES STATISTICS :: </p></center></b></u>" << "\n";
    while(i.hasNext()){
        i.next();
        page << "<p><center>" << QString::fromStdString(i.key()) << " : " << QString::fromStdString(to_string(i.value()))  << "</p></center>\n";
    }


    page << "</body>";
    page << "</html>";

    return page_stat;
}


void stats::usedCache(){
    fromCache++;
}


double stats::percentCache(){
    if (requests == 0){
        return (double)0;
    } else {
        return 100*( (double)fromCache/(double)requests );
    }
}


void stats::clearCache(){
    fromCache = 0;
}


void stats::toggle_activation(){
    isActive = !isActive;
}


void stats::addOctets(QByteArray ba){
    nOctets = nOctets + ba.size();
}


