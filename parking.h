#include <QVector>
#include <iostream>
using std::

struct holder{
    char sector;
    short place;
};

struct reserved
{
    char place[5];
    reserved() {}
};

class Container{
    QVector<holder> holders;
    QVector<reserved>
    Container(){
    }

};


