#include<iostream>
#include<string>
#include<fstream>
#include<ctime>
#include<stdlib.h>
#include<iomanip>
using namespace std;


char days[]={'M','T','W','R','F'};
string timedur[]={"09.00-09.50","10.00-10.50","11.00-11.50","12.00-12.50","14.00-14.50","15.00-15.50","16.00-16.50","17.00-17.50"};

class timetable{
private:
    string coursename[40];
    string faculty[40];
    string room[40];
public:
    void printtable(ofstream* output){
        for(int i=0;i<40;i++){
            if(!coursename[i].empty())*output<<setw(20)<<days[i/8]<<" "<<timedur[i%8]<<" Course: "<<coursename[i]<<", Faculty: "<<faculty[i]<<", Room: "<<room[i]<<endl;
        }
        *output<<endl;
    }
    void set_val(string cs,string fc,string rm,int t){
        coursename[t]=cs;
        faculty[t]=fc;
        room[t]=rm;
    }
    bool isEmpty(int t){
        return !coursename[t].empty();
    }
} Table[100];

class ListofFacluty{
private:
    string name[100];
    int idx;
public:
    int idToProf(string s){
        for(int i=0;i<idx;i++){
            if(name[i]==s)return i;
        }
        name[idx]=s;
        idx++;
        return idx-1;
    }
    string getname(int i){
        return name[i];
    }
} profList;

string courseList[100],roomName[100],groupList[100];
int Cr_prof[100][20],roomCap[100],groupCap[100],size_crprof[100];

bool empRoom[100][40]={0},empProf[100][40]={0};

int nRm,nCr;

int findRoom(int t,int cap){
    int step=0;
    do{
        step++;
        int r=rand()%nRm;
        if(empRoom[r][t]== 0 && cap<=roomCap[r])return r;
    }while(step<1000);
    return -1;

}

int findFaculty(int t,string cr){
    for(int i=0;i<nCr;i++){
        if(courseList[i]==cr){
            int step=0;
            do{
                step++;
                int k=rand()%size_crprof[i];
                if(empProf[Cr_prof[i][k]][t]==0){
                    return Cr_prof[i][k];
                }
            }while(step<1000);
            return -1;
        }
    }
}

bool assign_course(int id, string cr){
    int step=0;
    do{
        step++;
        int t=rand()%40;
        if(Table[id].isEmpty(t))continue;
        int rm=findRoom(t,groupCap[id]);
        if(rm==-1)continue;
        int fc=findFaculty(t,cr);
        if(fc==-1)continue;
        empRoom[rm][t]=1;
        empProf[fc][t]=1;
        Table[id].set_val(cr,profList.getname(fc),roomName[rm],t);
        return 1;
    }while(step<10000);
    return 0;
}


int main(){

    ifstream firsttxt ("Courses and Faculty.txt");

    if(!firsttxt.is_open()){
        cout<<"File (Courses and Faculty.txt) was not found !!!";
        return 0;
    }

    nCr=0;
    char c=0;
    string s;

    while(firsttxt>>c){
        int i=0;
        string s;
        while(c!='-'){
            courseList[nCr].push_back(c);
            firsttxt>>c;
        }
        do{
            firsttxt>>c;
            if(c==',' || c=='.'){
                Cr_prof[nCr][i]=profList.idToProf(s);
                i++;
                s.clear();
            }
            else s.push_back(c);

        }while(c!='.');
        size_crprof[nCr]=i;
        nCr++;
    }
    firsttxt.close();


    ifstream secondtxt ("Rooms and Capacity.txt");

    if(!secondtxt.is_open()){
        cout<<"File (Rooms and Capacity.txt) was not found !!!";
        return 0;
    }

    nRm=0;
    while(secondtxt>>roomName[nRm] && secondtxt>>roomCap[nRm]){
            nRm++;
    }

    secondtxt.close();

    ifstream thirdtxt ("Groups and Courses.txt");

    if(!thirdtxt.is_open()){
        cout<<"File (Groups and Courses.txt) was not found !!!";
        return 0;
    }

    srand(time(NULL));
    int nGp=0;

    while(thirdtxt>>c){
        int i=0;
        string s;
        while(c!='-'){
            groupList[nGp].push_back(c);
            thirdtxt>>c;
        }
        if(c=='-')thirdtxt>>groupCap[nGp]>>c;

        int hours=0;
        do{
            if(c=='-')thirdtxt>>hours;
            thirdtxt>>c;
            if(c==',' || c=='.'){
                while(hours>0){
                    if(assign_course(nGp, s)==0){
                        cout<<"The input files are wrong";
                        return 0;
                    }
                    hours--;
                }
                if(c==',')thirdtxt>>hours;
                s.clear();
            }
            else s.push_back(c);
        }while(c!='.');

        nGp++;
    }
    thirdtxt.close();

    ofstream resultTable ("Timetable for Groups.txt");

    string grp;

    for(int i=0;i<nGp;i++){
        resultTable<<endl<<"The timetable of group "<<groupList[i]<<endl;
        Table[i].printtable(&resultTable);

    }
    return 0;
}
