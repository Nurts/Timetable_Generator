#include <iostream>
#include <ctime>//to used srand(time(NULL)) function, which generates random numbers
#include <vector>//to use vectors
#include <fstream>//the library to use file I/O
#include <cstdlib>//to use rand()
#include <iomanip>//to use setw() for more information: http://www.cplusplus.com/reference/iomanip/setw/?kw=setw
using namespace std;

string Time[]={"09:00 AM-09:50 AM","10:00 AM-10:50 AM","11:00 AM-11:50 AM","12:00 PM-12:50 PM","02:00 PM-2:50 PM","03:00 PM-03:50 PM","04:00 PM-04:50 PM","05:00 PM-05:00 PM"};
char Day[]={'M','T','W','R','F'};

string Fac[100],Courses[100][100],Rooms[100],Groups[100],tableCourse[100][100];
vector<int>tableFac[100][100],tableRoom[100][100];
int Capacity[100],Number[100];
vector<int> tableTime[100][100];
int RoomSize;
bool uGr[50][100]={0},uRoom[50][100]={0},uFac[50][100]={0};
//Vectors are sequence containers, like arrays, that can change in size, to which you can include (string) elements using push_back()
//for more information http://www.cplusplus.com/reference/vector/vector/?kw=vector

//setToTable(curGrp,curCrs,hrs,CSnum)

void setToTable(string gp,string cs,int hrs,int csnum){//function which generates timetable, it finds suitable time room and faculty to group gp, who is taking course cs
    int i,g=0;

    for(i=0;!Groups[i].empty();i++){//finding the is of the group with name gp
        if(gp==Groups[i]){
            g=i;
            break;
        }
    }
    vector<int> reqFac;//suitable faculties for this course
    for(i=0;!Fac[i].empty();i++){
        int j;
        for(j=0;!Courses[i][j].empty();j++){
            if(Courses[i][j]==cs){
                reqFac.push_back(i);
                break;
            }
        }
    }
    tableCourse[g][csnum]=cs;
    for(i=0;i<hrs;i++){
        int tim,rom,fcul;
        do{
            tim=rand()%40,rom=rand()%RoomSize,fcul=rand()%reqFac.size();//generates random numbers
        }while(uGr[tim][g]==1 || uRoom[tim][rom]==1 || uFac[tim][reqFac[fcul]]==1);//if one of them is not free at time tim (room, group ,faculty)

        uGr[tim][g]=1;
        uRoom[tim][rom]=1;
        uFac[tim][reqFac[fcul]]=1;//updating boolean arrays, which are for checking the emptiness
        tableTime[g][csnum].push_back(tim);
        tableFac[g][csnum].push_back(reqFac[fcul]);
        tableRoom[g][csnum].push_back(rom);//assigning found values
    }
}

int main() {
    //srand(time(NULL));
    //srand() to get different numbers from rand() function after each run, you can remove this line if you want to get same timetable after each run
    //ifstream and ofstream are used to work with files they need library <fstream>, for more information: http://www.cplusplus.com/doc/tutorial/files/
    ifstream file ("Faculty and Courses.txt");

    char a;
    int i=0;
    while(file.get(a)){//getting input from file
        int j=0;
        string cur;
        while(a!=':'){
            if(a!='\n')Fac[i].push_back(a);
            file.get(a);
        }
        file>>cur;
        while(cur!=";"){
            Courses[i][j]=cur;
            j++;
            file>>cur;
        }
        i++;
        file.get(a);//remove the endl at the end of each line
    }
    file.close();

    file.open("Venue and Cap.txt");

    i=0;
    string garbage;

    while(file>>Rooms[i]>>garbage>>garbage>>Capacity[i])i++;//getting roomnames annd capacity

    RoomSize=i;
    file.close();

    file.open("Groups and number.txt");

    i=0;
    //Getting groupnames and number of students
    while(file>>Groups[i]>>Number[i])i++;


    file.close();

    file.open("Courses and Groups.txt");
    int CSnum=0,hrs;
    string curCrs,curGrp;
    while(file>>hrs>>garbage>>curCrs>>garbage>>curGrp){
        while(curGrp!=";"){
            setToTable(curGrp,curCrs,hrs,CSnum);
            file>>curGrp;
        }
        CSnum++;
    }

    file.close();
    string csAbbr[100],csTitle[100];
    file.open("Course Titles.txt");

    i=0;
    while(file>>csAbbr[i]>>garbage && getline(file,csTitle[i]))i++;//getting course titles


    string request;
    cout<<"Enter the group name (without spaces)"<<endl;
    cin>>request;
    cout<<"The schedule was stored in Schedule.txt"<<endl<<"Bye!!";

    ofstream file2 ("Schedule.txt");
    int j;
    for(j=0;!Groups[j].empty();j++){
        if(request==Groups[j]){
            for(int k=0;k<CSnum;k++){
                if(!tableCourse[j][k].empty()){
                    int q;
                    file2<<"Course Title: ";
                    for(int q=0;q<i;q++){//because i is the number of courses
                        if(tableCourse[j][k]==csAbbr[q]){
                            file2<<csTitle[q]<<endl;
                            break;
                        }
                    }
                    file2<<setw(11)<<"Day|"<<setw(21)<<"Time|"<<setw(21)<<"Faculty Name|"<<setw(16)<<"Room|"<<endl;
                    for(int q=0;q<tableTime[j][k].size();q++){
                        //the number in setw() should be 1 less than in the first file2out line
                        file2<<setw(10)<<Day[tableTime[j][k][q]/8]<<"|"<<setw(20)<<Time[tableTime[j][k][q]%8]<<"|"<<setw(20)<<Fac[tableFac[j][k][q]]<<"|"<<setw(15)<<Rooms[tableRoom[j][k][q]]<<"|"<<endl;
                    }//change the number in setw() if table became ugly and the facluty name is longer than20 characters
                }
            }
        }
    }//output
    return 0;
}
