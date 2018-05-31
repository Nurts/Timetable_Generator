#include<iostream>
#include<cstdlib>//to use rand()
#include<fstream>//to use files
#include "secondary.h"
#include<ctime>
//include if you will use srand()

using namespace std;
Table curTable[30];
string timeHr[]={"9 AM - 11 AM:", "11 AM - 1 PM:", " 2 PM - 4 PM:", " 4 PM - 6 PM:"};

int courseAmt,groupAmt,roomAmt,facultyAmt,roomCap[300],groupCap[300];//Amt==Amount, Cap=Capacity
string courseStr[300],facultyStr[300],roomStr[300],groupStr[300];
vector<int> fclCourses[300],grpCourses[300];
//Vectors are sequence containers, like arrays, that can change in size, to which you can include (string) elements using push_back()
//for more information http://www.cplusplus.com/reference/vector/vector/?kw=vector
void Table::initialise(){
    for(int i=0;i<groupAmt;i++){
        course[i]=0;
        room[i]=0;
        faculty[i]=0;
    }
}
string Table::output(int t,int g){//the data for group g at time t
    if(course[g]==0)return timeHr[t%4];//if group g have no classes at time t
    string outStr=timeHr[t%4]+" "+courseStr[course[g]]+" ("+facultyStr[faculty[g]]+") in "+roomStr[room[g]];
    return outStr;
}
bool Table::canUseFaculty(int f){
    for(int i=0;i<groupAmt;i++){
        if(faculty[i]==f)return 0;
    }
    return 1;
}
bool Table::canUseRoom(int r){
    for(int i=0;i<groupAmt;i++){
        if(room[i]==r)return 0;
    }
    return 1;
}
bool Table::isGroupFree(int g){
    if(course[g]==0)return 1;
    else return 0;
}
void Table::setValues(int g,int c,int f,int r){
    course[g]=c;
    faculty[g]=f;
    room[g]=r;
}
void readfromFiles(){
    //ifstream and ofstream are used to work with files they need library <fstream>, for more information: http://www.cplusplus.com/doc/tutorial/files/
    ifstream input("List of Courses.txt");//opening file
    int i;
    //we started from one because the user will enter the IDs, which are started from 1;
    for(i=1;input>>courseStr[i];i++);//getting the data from file (coursenames)
    courseAmt=i-1;
    input.close();


    input.open("List of Faculty and Courses.txt");
    for(i=1;input>>facultyStr[i];i++){//getting faculty names while we can (until the end of file)
        int a;
        input>>a;
        while(a!=0){//getting the IDs of the course, which are taught by prof number i
                //stops when find 0
            fclCourses[i].push_back(a);
            input>>a;
        }
    }
    facultyAmt=i-1;
    input.close();

    input.open("List of Groups.txt");
    for(i=1;input>>groupStr[i];i++){
        cout<<groupStr[i]<<endl;
        int a;
        input>>a;
        while(a!=0){//getting the IDs of the course, which are taught by prof number i
                //stops when find 0
            grpCourses[i].push_back(a);
            input>>a;
        }
    }
    groupAmt=i-1;
    input.close();

    input.open("List of Classrooms and Capacity.txt");

    for(i=1;input>>roomStr[i];i++)input>>roomCap[i];
    roomAmt=i-1;
    input.close();

    input.open("Number of Students.txt");
    string unknown;
    while(input>>unknown){
        for(i=1;i<=groupAmt;i++){
            if(groupStr[i]==unknown)input>>groupCap[i];
        }
    }
    input.close();
}

int findTimeforGroup(int g,int c){//finding time for group g and course c
    srand(time(NULL));
    //include srand() if you want to get different tables after each run of this program
    int t=-1,f,r;//the time
    for(int i=1;i<=1000000;i++){
        int test=rand()%20;//random time for testing, rand() generates random numbers
        int counter=curTable[test].isGroupFree(g);
        if(counter<1)continue;//if the group is not free at time test
        for(r=1;r<=roomAmt;r++){
            if(roomCap[r]>=groupCap[g])counter+=curTable[test].canUseRoom(r);
            if(counter==2)break;//if we found room
        }
        if(counter<2)continue;//if there is no suitable room at time test
        for(f=1;f<=facultyAmt;f++){
            for(int j=0;j<fclCourses[f].size();j++){//size() returns the size of the vector
                if(fclCourses[f][j]==c){//if prof teaches this course (c)
                    counter+=curTable[test].canUseFaculty(f);
                    break;
                }
            }
            if(counter==3)break;//if we found prof
        }
        if(counter==3){
            t=test;
            break;
        }
    }
    curTable[t].setValues(g,c,f,r);
    return t;
}
int GetgroupAmt(){
    return groupAmt;
}
vector<int> GetgrpCourse(int i){
    return grpCourses[i];
}
string GetgroupStr(int i){
    return groupStr[i];
}
string getoutput(int t,int g){
    return curTable[t].output(t,g);
}
void TableInitialise(int t){
    curTable[t].initialise();
}
