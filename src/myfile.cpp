#include "myfile.h"

using namespace std;


myfile::myfile(){
    content = {};
    size = content.size();
    nRequested = 0;
}


myfile::myfile( QByteArray _content){
    content = _content;
    size = _content.size();
    nRequested = 0;
}


int myfile::getSize(){
    return size;
}


QByteArray myfile::requested(){
    nRequested++;
    return content;
}


int myfile::nReq(){
    return nRequested;
}

