#include "connector.h"
#include "container.h"
#include <iostream>
#include <sstream>
using namespace std;

string getReserved(const reserved &value){
    stringstream ss;
    ss<<"\nPlace:\t"<<value.place
     <<"\nCar:\t"<<value.car
     <<"\nNumber:\t"<<value.plate
     <<"\nColor:\t"<<value.color
     <<"\nNighter:\t"<<((value.night)?"TRUE":"FALSE");
    return ss.str();
}

string getHolder(const holder &value){
    stringstream ss;
    ss<<value.sector<<value.place;
    return ss.str();
}

reserved setReserved(){
    reserved inserted;
    cout<<"Place: ";
    cin>>inserted.place;
    cout<<"Car: ";
    cin>>inserted.car;
    cout<<"Color: ";
    cin>>inserted.color;
    cout<<"Registry number: ";
    cin>>inserted.plate;
    cout<<"On night";
    cin>>inserted.night;
    return inserted;
}


reserved setReservedOnPlace(const char *place){
    reserved inserted;
    strcpy(inserted.place, place);
    cout<<"Car: ";
    cin>>inserted.car;
    cout<<"Color: ";
    cin>>inserted.color;
    cout<<"Registry number: ";
    cin>>inserted.plate;
    cout<<"On night";
    cin>>inserted.night;
    return inserted;
}

string menu = "1) Entry to parking\n2) Free place\n3) Update info\n4) See info about car\n\
5) Count all nighters\n6) Show all free places\n7) Find car\n8) Exit\n";
int main()
{
    Container container;
    bool running=true;
    char switcher=0;
    char place[5];
    char filter[50];
    reserved singleton;
    while(running){
        cout<<menu;
        cin>>switcher;
        switch (switcher) {
        case '1':
            container.entryCar(setReserved());
            break;
        case '2':
            cout<<"Enter place: ";
            cin>>place;
            container.setSpaceFree(place);
            break;
        case '3':
            cout<<"Enter place: ";
            cin>>place;
            container.updateRecord(setReservedOnPlace(place));
            break;
        case '4':
            cout<<"Enter place: ";
            cin>>place;
            cout<<getReserved(container.getRecord(place))<<endl;
            break;
        case '5':
            cout<<container.countNighters()<<endl;
            break;
        case '6':
            container.getFreeSpace();
            cout<<endl;
            break;
        case '7':
            cout<<"Enter filter string: ";
            cin>>filter;
            container.findRecord(filter);
            for(int i=0; i<container.sizeReserved(); i++){
                cout<<getReserved(container.reservedGetAt(i))<<endl<<endl;
            }
            break;
        case '8':
            running=false;
            break;
        default:
            break;
        }
    }
    return 0;
}
