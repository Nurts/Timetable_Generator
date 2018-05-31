#include<iostream>
#include<fstream>
#include<cstdlib>//to use rand()
#include<ctime>//to use srand(time(NULL))
using namespace std;
int NMgroup[5][20];
string Major[5][20];

class ForCourses{
private:
    int amount;//the number of courses
    string title[100];//the titles of the courses
    int TrAmount[100];//the number of teachers teaching each course
    int Teach[100][100];//the id of those teachers
    int Hours[100];//the durations of courses
public:
    void Initial(){
        amount=0;
        for(int i=0;i<100;i++){
            TrAmount[i]=0;
        }
    }
    int GetId(string s){//getting the id of the course s
        for(int i=0;i<amount;i++){
            if(title[i]==s)return i;
        }
        return -1;
    }
    void SetCourse(string s,int h){//Setting new course and its duration
        title[amount]=s;
        Hours[amount]=h;
        amount++;
    }
    void SetTeacherToLast(int y){
        Teach[amount-1][TrAmount[amount-1]]=y;//Setting the teacher with id y to the last course, as the course teacher
        TrAmount[amount-1]++;
    }
    int GetRandomTeacher(int x){//returns random teacher who is teaching course with id x
        int i=rand()%TrAmount[x];//randomly choosing among them;
        return Teach[x][i];
    }
    string GetTitle(int x){
        return title[x];
    }
    int GetHours(int x){
        return Hours[x];
    }
} AllCourses;

class ForFaculty{
private:
    int amt;//the number of techers
    string Teacher[100];//the name of each techer
    bool IsFree[100][100];//IsFree[i][j] is 1 if the Teacher number i is free at time j
public:
    void Initial(){
        amt=0;
        for(int i=0;i<100;i++){
            for(int j=0;j<100;j++){
                IsFree[i][j]=1;
            }
        }
    }
    int SetTeacher(string s){
        Teacher[amt]=s;//setting teacher to the end of an array
        amt++;//increasing the amount
        return amt-1;//returns the last included teacher Id
    }
    int GetId(string s){//returns the id of the teacher with name s
        for(int i=0;i<amt;i++){
            if(Teacher[i]==s)return i;
        }
        return -1;//if teacher was not found
    }
    bool Free(int x,int t,int h){//checks is the Teacher with id x is free between t and t+h

        for(int i=0;i<h;i++){
            if(IsFree[x][t+i]==0)return 0;
        }
        return 1;
    }
    void NotFree(int x,int t,int h){//the Teacher x now is not free between t and t+h
        for(int i=0;i<h;i++){
            IsFree[x][t+i]=0;
        }
    }
    string GetTeacher(int x){
        return Teacher[x];
    }
} AllFaculty;

class ForRooms{
private:
    int amount;//the number of rooms
    string Title[100];//the name of each room
    int Capc[100];//The capacity of each room
    bool IsFree[100][100];//Like in ForFaculty Class
public:
    void Initial(){
        amount=0;
        for(int i=0;i<100;i++){
            for(int j=0;j<100;j++){
                IsFree[i][j]=1;
            }
        }
    }
    void SetRoom(string s,int cap){
        Title[amount]=s;
        Capc[amount]=cap;
        amount++;
    }
    int GetRandomRoom(){
        int i=rand()%amount;
        return i;
    }
    bool Compare(int StudAmt,int x){//Compares the Capacity of room x and the Student Number
        if(Capc[x]<StudAmt)return 0;
        else return 1;
    }
    bool Free(int x,int t,int h){
        for(int i=0;i<h;i++){
            if(IsFree[x][t+i]==0)return 0;
        }
        return 1;
    }
    void NotFree(int x,int t,int h){
        for(int i=0;i<h;i++){
            IsFree[x][t+i]=0;
        }
    }
    string GetRoomTitle(int x){
        return Title[x];
    }

}AllRooms;


class forTimetable{
private:
    string course[50],faculty[50],room[50];
    bool IsFree[50];
public:
    void Initial(){
        for(int i=0;i<50;i++){
            IsFree[i]=1;
        }
    }
    bool Free(int t,int h){
        for(int i=0;i<h;i++){
            if(IsFree[t+i]==0)return 0;
        }
        return 1;
    }
    void NotFree(int t,int h){
        for(int i=0;i<h;i++){
            IsFree[t+i]=0;
        }
    }
    void SetTable(int StudAmt,int CrId){
        int h=AllCourses.GetHours(CrId);
        int t,r,x;//t is time r is room x is faculty
        while(true){//always true condition
            t=rand()%40;

            if(Free(t,h)==0)continue;//checks is the group free at time t if not then it will (continue;)
            if((t%4)+h>4)continue;//checks will the half of the course be after lunch or next day if <=4 then no (everything is Ok)
            x=AllCourses.GetRandomTeacher(CrId);

            if(AllFaculty.Free(x,t,h)==0)continue;//if the teacher is not free between t and t+h

            r=AllRooms.GetRandomRoom();
            if(AllRooms.Compare(StudAmt,r)==0)continue;//if the Student number is greater than room capacity
            if(AllRooms.Free(r,t,h)==0)continue;//if the room is not free

            break;//if it passed through all conditions
        }
        AllRooms.NotFree(r,t,h);//Now all of this are not free
        AllFaculty.NotFree(x,t,h);
        NotFree(t,h);

        for(int i=0;i<h;i++){
            course[t+i]=AllCourses.GetTitle(CrId);//saving the title of the course to time t+h
            faculty[t+i]=AllFaculty.GetTeacher(x);
            room[t+i]=AllRooms.GetRoomTitle(r);
        }
    }
    void getTable(string filename, int y, int mjr,int grp){

        string timeformat[]={"9:00 - 9:50", "10:00-10:50","11:00-11:50","12:00-12:50","14:00-14:50","15:00-15:50","16:00-16:50","17:00-17:50"};

        filename+=".txt";//adding .txt
        ofstream outfile (filename.c_str());//.c_str() changes the string to char string , it is necessary to use filename in outfile ();
        outfile<<"Year: "<<y<<" Major: "<<Major[y][mjr]<<endl;
        outfile<<"The Schedule of the Group #"<<grp<<endl;

        for(int i=0;i<40;i++){

            if(i%8!=0 && i%4==0)outfile<<"13:00-14:00 : Lunch Time"<<endl;

            if(i==0)outfile<<"Monday"<<endl;
            else if(i==8)outfile<<"Tuesday"<<endl;
            else if(i==16)outfile<<"Wednesday"<<endl;
            else if(i==24)outfile<<"Thursday"<<endl;
            else if(i==32)outfile<<"Friday"<<endl;

            outfile<<timeformat[i%8]<<" : ";

            if(IsFree[i]==0){//if the group have courses at time i
                outfile<<course[i]<<" - Faculty: "<<faculty[i]<<" - Room: "<<room[i];
            }
            outfile<<endl;
        }
        outfile.close();

        cout<<"The Schedule is stored in "<<filename<<endl;
    }
}Table[10][20][20];

int main(){
    //ifstream and ofstream are used to work with files they need library <fstream>
    // for more information: http://www.cplusplus.com/doc/tutorial/files/

    string req;

    cout<<"Do you want to create new timetable: (Yes, No): ";

    cin>>req;
    if(req=="Yes")srand(time(NULL));

    AllCourses.Initial();
    AllFaculty.Initial();
    AllRooms.Initial();

    for(int i=0;i<10;i++){
        for(int j=0;j<20;j++){
            for(int l=0;l<20;l++){
                Table[i][j][l].Initial();
            }
        }
    }

    ifstream infile ("Course Faculty.txt");//infile is created to get information from file "Year Major Course.txt"

    string title;
    int h;

    while(infile>>title){//works till the end of the file
        infile>>h;//getting the number of hours of this course
        AllCourses.SetCourse(title,h);

        string s;
        infile>>s;//the names of the faculties teaching this course
        while(s!=";"){//works till the end of the line (until ';')
            int id=AllFaculty.GetId(s);

            if(id==-1)id=AllFaculty.SetTeacher(s);//if the Faculty is not set into the AllFaculty

            AllCourses.SetTeacherToLast(id);
            infile>>s;
        }
    }
    infile.close();//the file is closed

    infile.open("Room Capacity.txt");
    string room;
    int cap;
    while(infile>>room){
        infile>>cap;
        AllRooms.SetRoom(room,cap);
    }

    infile.close();

    infile.open("Year Major Course.txt");

    int year,prev=0,mjr=0;
    int StudAmt;
    while(infile>>year){//works as "cin>>", but gets from file

        if(prev==year)mjr++;//if we have more majors this year
        else mjr=0;//if we get another year

        infile>>Major[year][mjr];//saving the name of this major
        infile>>NMgroup[year][mjr];//saving the number of groups in this major
        infile>>StudAmt;
        string s;
        infile>>s;
        while(s!=";"){
            int id=AllCourses.GetId(s);
            if(id==-1){//if the course is not found
                cout<<"The Error Occurred ! (Check data files the course "<<s<<" is not found.)"<<endl;
                return 0;
            }
            for(int i=1;i<=NMgroup[year][mjr];i++){//start from one because user will enter groups from 1
                Table[year][mjr][i].SetTable(StudAmt,id);//Creates new table for group (year, mjr, i) with StudAmt number of students
                //And for course with ID id
            }
            infile>>s;//gets another course
        }

        prev=year;//previous saves the previous year number
    }
    infile.close();


    int y,m=-1,g;
    string maj,str;
    cout<<"Enter the Year:";
    cin>>y;
    cout<<"Enter the Major:";
    cin>>maj;
    cout<<"Enter the Group Number:";
    cin>>g;
    cout<<"Enter the Output File Name:";
    cin>>str;
    for(int i=0;i<20;i++){
        if(maj==Major[y][i]){
            m=i;
            break;
        }
    }
    if(m==-1 || g>NMgroup[y][m]){
        cout<<"The entered values are wrong!!!"<<endl;
        return 0;
    }

    Table[y][m][g].getTable(str,y,m,g);

    return  0;
}
