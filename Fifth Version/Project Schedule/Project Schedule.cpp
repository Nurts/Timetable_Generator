#include "timetable.h"
#include <iostream>

using namespace std;
int main(){
    int a;
    getTitles();
    do{
        cout<<"Choose one of the options:"<<endl;
        cout<<"Create New Timetable (Type 1)"<<endl;
        cout<<"Get Timetable for the group (Type 2)"<<endl;
        cout<<"Exit the program (Type anything)"<<endl;
        cin>>a;
        if(a==1)createNewTable();
        else if(a==2){
            int y,g;
            cout<<"Enter the Year: ";
            cin>>y;
            cout<<"Enter the Group number: ";
            cin>>g;
            getTable(y,g);
        }
    }while(a==1 || a==2);
    return 0;
}
