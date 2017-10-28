#include <QVector>
#include <iostream>
#include <ctime>
#include "connector.h"

#define SQLBIND(type, field) \
    SQLBindCol(q.getHandler(), ++colId, type, &field, sizeof(field), &field##Len);

struct holder {
    char sector;
    SQLLEN sectorLen;
    int place;
    SQLLEN placeLen;

    void bind(Query &q) {
        int colId = 0;
        SQLBIND(SQL_C_CHAR, sector);
        SQLBIND(SQL_C_SLONG, place);
    }
};


struct reserved {
    char place[50];
    SQLLEN placeLen;
    char car[50];
    SQLLEN carLen;
    char color[50];
    SQLLEN colorLen;
    char plate[50];
    SQLLEN plateLen;
    TIMESTAMP_STRUCT entry;
    SQLLEN entryLen;
    bool night;
    SQLLEN nightLen;
//    reserved() {}
    void bind(Query &q) {
        int colId = 0;
        SQLBIND(SQL_C_CHAR, place);
        SQLBIND(SQL_C_CHAR, car);
        SQLBIND(SQL_C_CHAR, color);
        SQLBIND(SQL_C_CHAR, plate);
        SQLBIND(SQL_TIMESTAMP, entry);
        SQLBIND(SQL_C_BINARY, night);
    }
};



class Container {
    QVector<holder> holders;
    QVector<reserved> reservedContainer;
    Connector connect;
    Container() {

    }

};
