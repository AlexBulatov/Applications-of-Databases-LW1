#include <QVector>
#include <iostream>
#include <ctime>
#include "connector.h"

//#define SQLBIND(type, field) \
    std::cout << SQLBindCol(q.getHandler(), ++colId, type, &field, (type==SQL_C_CHAR)?50:0, &field##Len) << std::endl;

struct holder {
    char sector;
    SQLLEN sectorLen;
    int place;
    SQLLEN placeLen;
};


struct reserved {
    char place[50];
    SQLLEN placeLen=50;
    char car[50];
    SQLLEN carLen=50;
    char color[50];
    SQLLEN colorLen=50;
    char plate[50];
    SQLLEN plateLen=50;
    TIMESTAMP_STRUCT entry;
    SQLLEN entryLen;
    bool night;
    SQLLEN nightLen;
};



class Container {
public:
    Connector connect;
    Container() {
        holderContainer.resize(0);
        reservedContainer.resize(0);
    }

    /*bool entryCar(char* _place, bool _night, char* _car, char* _plate, char* color){
        return false;
    }*/

    bool getFreeSpace(){
        Query query=connect.createQuery("select * from freespace()");
        if (query.exec()){
            holder tmp;
            while(query.fetchOne()==FETCH_RESULT::SUCCESS){
                tmp.sector=connect.readSymbol(1);
                tmp.place=connect.readInt(2);
                holderContainer.push_back(tmp);
            }
            for(int i=0; i<holderContainer.size(); i++){
                std::cout<<holderContainer[i].sector<<holderContainer[i].place<<std::endl;
            }
            return true;
        }
        return false;
    }

    bool setSpaceFree(char* place){
        return false;
    }
private:
    QVector<holder> holderContainer;
    QVector<reserved> reservedContainer;
};
