#include <QVector>
#include <iostream>
#include <ctime>
#include <string>
#include "connector.h"
using namespace std;

struct holder {
    char sector;
    int place;
};

struct reserved {
    char place[50];
    char car[50];
    char color[50];
    char plate[50];
    TIMESTAMP_STRUCT entry;
    bool night;
};

class Container {
public:
    Connector connect;
    Container() {
        queryString = new char[100];
        holderContainer.resize(0);
        reservedContainer.resize(0);
    }

    ///Добавляет машину в базу данных
    /// Into Database
    bool entryCar(const char* _place, const bool _night, const char* _car, const char* _plate, const char* _color){
        sprintf(queryString,
                "select * from entry('%s', %s, '%s', '%s', '%s')",
                _place, (_night)?"TRUE":"FALSE", _car, _plate, _color
                );
        Query query = connect.createQuery(queryString);
        if(query.exec()) return true;
        else return false;
    }

    bool entryCar(const reserved &value){
        return entryCar(value.place, value.night, value.car, value.plate, value.color);
    }

    ///Показывает одну запись
    /// Into record
    reserved getRecord(const char* id){
        sprintf(queryString, "select * from getRecord('%s')", id);
        Query query = connect.createQuery(queryString);
        if (query.exec()){
            if (query.fetchOne()==FETCH_RESULT::SUCCESS){
                reserved record;
                strcpy(record.place, id);
                strcpy(record.car,connect.readChar(1));
                strcpy(record.color, connect.readChar(2));
                strcpy(record.plate, connect.readChar(3));
                record.entry = connect.readTimestamp(4);
                record.night = connect.readSymbol(5);
                return record;
            }
            else {
                cout<<"Error in "<<__FUNCTION__<<" at "<<52<<endl;
                return *(new reserved);
            }
        }
        else {
            cout<<"Error in "<<__FUNCTION__<<" at "<<52<<endl;
            return *(new reserved);
        }
    }

    ///Освобождает конкретное место
    /// Into Database
    bool setSpaceFree(const char* place){
        sprintf(queryString,"DELETE FROM reserved WHERE place='%s'", place);
        Query query = connect.createQuery(queryString);
        if(query.exec()) return true;
        else return false;
    }

    ///Получить всю базу данных
    /// Into containers
    bool getAllDataBase(){
        reservedContainer.clear();
        holderContainer.clear();
        Query query = connect.createQuery("SELECT * FROM reserved");
        if(query.exec()){
            reserved reservedRecord;
            while(query.fetchOne()==FETCH_RESULT::SUCCESS){
                strcpy(reservedRecord.place, connect.readChar(1));
                strcpy(reservedRecord.car,connect.readChar(2));
                strcpy(reservedRecord.color, connect.readChar(3));
                strcpy(reservedRecord.plate, connect.readChar(4));
                reservedRecord.entry = connect.readTimestamp(5);
                reservedRecord.night = connect.readSymbol(6);
                reservedContainer.push_back(reservedRecord);
            }
            query = connect.createQuery("SELECT * FROM holder");
            if(query.exec()){
                holder holderRecord;
                while(query.fetchOne()==FETCH_RESULT::SUCCESS){
                    holderRecord.sector=connect.readSymbol(1);
                    holderRecord.place=connect.readInt(2);
                    holderContainer.push_back(holderRecord);
                }
                return true;
            }
            else return false;
        }
        else return false;
    }

    ///Обновить запись в базе
    /// Into Database
    bool updateRecord(const reserved &record){
        sprintf(queryString, "select * from editcar('%s', %s, '%s', '%s', '%s')", record.place, record.night?"TRUE":"FALSE", record.car, record.plate, record.color);
        Query query = connect.createQuery(queryString);
        if (query.exec()) return true;
        else return false;
    }

    ///Получить количество ночных стоянок
    /// Into value
    unsigned int countNighters(){
        sprintf(queryString, "select count(*) from reserved where night='TRUE'");
        unsigned int value;
        Query query = connect.createQuery(queryString);
        if(query.exec()){
            if(query.fetchOne()==FETCH_RESULT::SUCCESS){
                value = connect.readInt(1);
                return value;
            }
        }
        else return INT_MAX;
    }

    ///Показывает все свободные места
    /// Into container
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

    ///Фильтрует базу записей по ключу
    /// Into container
    bool findRecord(const char *key){
        sprintf(queryString, "select * from findRecord('%s')", key);
        Query query = connect.createQuery(queryString);
        reservedContainer.clear();
        reserved reservedRecord;
        if(query.exec()){
            while(query.fetchOne()==FETCH_RESULT::SUCCESS){
                strcpy(reservedRecord.place, connect.readChar(1));
                strcpy(reservedRecord.car,connect.readChar(2));
                strcpy(reservedRecord.color, connect.readChar(3));
                strcpy(reservedRecord.plate, connect.readChar(4));
                reservedRecord.entry = connect.readTimestamp(5);
                reservedRecord.night = connect.readSymbol(6);
                reservedContainer.push_back(reservedRecord);
            }
            return true;
        }
        else return false;
    }

    holder holderGetAt(int i){
        if(i<0 || i>holderContainer.size()-1){
            cout<<"Wrong index"<<endl;
            return *(new holder);
        }
        return holderContainer[i];
    }

    reserved reservedGetAt(int i){
        if(i<0 || i>reservedContainer.size()-1){
            cout<<"Wrong index"<<endl;
            return *(new reserved);
        }
        return reservedContainer.at(i);
    }

    unsigned int sizeHolder(){
        return holderContainer.size();
    }

    unsigned int sizeReserved(){
        return reservedContainer.size();
    }

private:
    char *queryString;
    QVector<holder> holderContainer;
    QVector<reserved> reservedContainer;
};
