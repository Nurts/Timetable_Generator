#include <iostream>
#include <ctime>//to used srand(time(NULL)) function, which generates random numbers
#include <vector>//to use vectors
#include <fstream>//the library to use file I/O
#include <cstdlib>//to use rand()
#include <iomanip>//to use setw()
using namespace std;
const int wdt=35;//wdt is used in function setw(wdt) increase its value if timetable is ugly,
string CR[100];
int CRhour[100];
string Hours[]={"09.00-09.50","10.00-10.50","11.00-11.50","12.00-12.50","14.00-14.50","15.00-15.50","16.00-16.50","17.00-17.50"};
bool used_gr[100][50]={0};
//Vectors are sequence containers, like arrays, that can change in size, to which you can include (string) elements using push_back()
//for more information http://www.cplusplus.com/reference/vector/vector/?kw=vector

vector<string> years;
int GetIdYear(string s){
    for(int i=0;i<years.size();i++){
        if(years[i]==s)return i;
    }
    years.push_back(s);
    return years.size()-1;
}


class RoomClass{
private:
    vector<string> name; //the names of the rooms
    vector<int> capacity;// the capacity of the rooms
    bool used[50][100];//used[t][i]==1 is the room number i is not empty at time t, else used[t][i]==0;

public:
    void set_value(string s,int cap){//fuction which sets the values
        name.push_back(s);
        capacity.push_back(cap);

    }
    bool IsFit(int id,int g_cap,int t){//function which checks the suitableness of the room (id), g_cap is the number of students in current
        if(capacity[id]<g_cap)return 0;//if(capacity of room is less than number of students
        else if(used[t][id]==1)return 0;//if room is not empty at time t
        else return 1;//1 means that room is suitable 0 not
    }
    int amt(){
        return name.size();//returns the number of rooms
    }
    void IsUsed(int id,int t){
        used[t][id]=1;//it means that the room became not empty during time t
    }
    string takeroom(int rm){
        return name[rm];
    }
}RM;

class FacultyClass{
private:
    vector<string> name;//names of profs
    vector<int> courses[100];//each courses[i] contains the courses which is taught by prof number i
    bool used[100][100];//used[t][i]=1 if prof i is not free at time t, else used[t][i]=0;
public:
    int amt(){//returns the number of Faculty.
        return name.size();
    }
    bool IsFree(int c,int t,int id){
        if(used[t][id]==1)return 0;//checks is the prof. is free at time t
        for(int i=0;i<courses[id].size();i++){
            if(c==courses[id][i])return 1;//checks is the prof. teaches the course, returns the name of suitable professor;
        }
        return 0;
    }
    string takeProf(int id,int t){
        used[t][id]=1;//states that the prof. is no more free at time t
        return name[id];
    }
    void set_value(int c,string s){
        int id=-1;
        for(int i=0;i<name.size();i++){//checks is the prof. already have course;
            if(name[i]==s){
                id=i;
                break;
            }
        }
        if(id==-1){//if the prof. have no course yet, it initializes the values
            name.push_back(s);
            id=name.size()-1;
        }
        courses[id].push_back(c);//adds the course to the prof number id
    }
}FC;

class GroupClass{
private:
    vector<string> name;
    vector<int> courses[100];
public:
    void set_value(int c,string s){
        int id=-1;
        for(int i=0;i<name.size();i++){
            if(name[i]==s){
                id=i;
                break;
            }
        }
        if(id==-1){
            name.push_back(s);
            id=name.size()-1;
        }
        courses[id].push_back(c);
    }
    int nmCR(int y){//returns the number of courses of group y
        return courses[y].size();
    }
    int ReqCourse(int y,int id){//returns required course for group y
        return courses[y][id];
    }
    int idx(string y){
        for(int i=0;i<name.size();i++){
            if(name[i]==y)return i;
        }
        return -1;
    }
}Group;

class Result{
private:
    string CRname[5][8][100];//selected course
    string FCname[5][8][100];//selected faculty
    string RMname[5][8][100];//Selected room
    vector<string> GPname;
public:
    void findTime(int y,string g,int g_cap,int g_id){//function which randomly finds suitable time room and faculty for the given course number(c),year number(y), number(g)
        int h,rm,f_id,sz=GPname.size();
        for(int i=0;i<Group.nmCR(g_id);i++){
            int c=Group.ReqCourse(g_id,i);

            for(int j=0;j<CRhour[c];j++){
                while(1==1){//always true statement
                    h=(rand()%40),rm=(rand()%RM.amt()),f_id=(rand()%FC.amt());//id is the randomly selected faculty number, h is randomly selected time (between 0 and 39), rm is randomly selected room number

                    if(!used_gr[g_id][h] && RM.IsFit(rm,g_cap,h)==1 && FC.IsFree(c,h,f_id)==1){//array and functions which checks the availability of room and time
                        break;
                    }
                }
                int dy=h/8,hr=h%8;
                CRname[dy][hr][sz]=CR[c];
                FCname[dy][hr][sz]=FC.takeProf(f_id,h);
                RMname[dy][hr][sz]=RM.takeroom(rm);//assigning the found values
                RM.IsUsed(rm,h);//updating the arrays to check;
                used_gr[g_id][h]=1;
            }
        }
        GPname.push_back(g);
    }
    void CoutTable(ofstream* tout){//function of class which outputs the table for all s in selected year to the given file(*tout);
        for(int i=0;i<GPname.size();i++){
            *tout<<"The name of the : "<<GPname[i]<<endl;
            *tout<<setw(wdt+11)<<"Monday"<<setw(wdt)<<"Tuesday"<<setw(wdt)<<"Wednesday"<<setw(wdt)<<"Thursday"<<setw(wdt)<<"Friday"<<endl;
            for(int j=0;j<8;j++){//j is the hour number (ex 9:00-9:50)
                *tout<<Hours[j];

                for(int l=0;l<5;l++){//l is the day number (Monday, Tuesday)
                    if(!CRname[l][j][i].empty()){
                            string res=CRname[l][j][i]+"|"+FCname[l][j][i]+" Venue: "+RMname[l][j][i];
                            *tout<<setw(wdt)<<res;//setw sets the size of the res to wdt (35), it adds additional spaces in front if characters are not enough
                    }
                    else *tout<<setw(wdt)<<"-";
                }
                *tout<<endl;
            }
            *tout<<endl;
            *tout<<endl;
        }
    }
} TimeTable[10];


int main() {
    srand(time(NULL));//to get different numbers from rand() function after each run, you can remove this line if you want to get same timetable after each run
    //ifstream and ofstream are used to work with files they need library <fstream>, for more information: http://www.cplusplus.com/doc/tutorial/files/
    ifstream input ("course_prof_group.txt");

    if(input.is_open()!=1){//Checks is the file successfully opened
        cout<<"Error opening files!!!";
        return 0;
    }

    char a;
    int ind=0;
    bool aregrp=0;//checks are we reading years or not
    string str;
    input>>CRhour[0];
    while(input>>a){//program reads from file one by one character
        if(a==':'){
            CR[ind]=str;
            str.clear();//str is cleared because we need to get the next string
        }
        else if(a==','){
            if(aregrp==0){
                FC.set_value(ind,str);//Sets the value of FC class
                str.clear();
            }
            else{
                Group.set_value(ind,str);
                str.clear();
            }
        }
        else if(a=='-'){
            aregrp=1;
            FC.set_value(ind,str);
            str.clear();
        }
        else if(a==';'){
            aregrp=0;
            Group.set_value(ind,str);
            str.clear();
            ind++;
            input>>CRhour[ind];
        }
        else str.push_back(a);
    }
    input.close();

    input.open("room.txt");//next file opened
    if(input.is_open()!=1){
        cout<<"Error opening files!!!";
        return 0;
    }
    int cap;
    while(input>>str && input>>cap){//Getting the name of room and its capacity from file "room.txt", and saving it in RM
        RM.set_value(str,cap);
    }

    input.close();

    input.open("group_year.txt");
    if(input.is_open()!=1){
        cout<<"Error opening files!!!";
        return 0;
    }

    int g_cap;
    string GRstr;
    ind=0;
    str.clear();
    input>>g_cap;
    while(input>>a){
        if(a==':'){
            ind=Group.idx(str);//Finding the index of this year
            GRstr=str;
            str.clear();
            //Getting the number of students in one
        }
        else if(a==';'){
            int idy=GetIdYear(str);
            TimeTable[idy].findTime(idy,GRstr,g_cap, ind);//Finding suitable place in timetable for courses of  str, year ind
            str.clear();
            input>>g_cap;
        }
        else str.push_back(a);
    }
    input.close();

    string yr;

    cout<<"Available years: ";
    for(int i=0;i<years.size();i++){
        cout<<years[i]<<" ";
    }
    cout<<"(Enter without spaces and click ENTER, Type 'Close' to end the program)"<<endl;
    cout<<"Enter the Years :"<<endl;

    ofstream output ("SENG Timetable.txt");

    do{
        cin>>yr;
        if(yr=="Close")break;

        int ind=GetIdYear(yr);

        output<<"The timetable for: "<<yr<<endl;
        TimeTable[ind].CoutTable(&output);
        output<<endl;
        output<<endl;

    }while(yr!="Close");
    return 0;
}
