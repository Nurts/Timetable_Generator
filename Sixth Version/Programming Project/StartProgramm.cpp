#include<iostream>
#include<string>
#include<vector>
#include<cstdlib>//to use rand()
#include<fstream>//to use io files
#include<iomanip>//to use setw()
#include<ctime> //include if you will use strand(time(NULL))

//to use vectors
using namespace std;
//Vectors are sequence containers, like arrays, that can change in size, to which you can include (string) elements using push_back()
//for more information http://www.cplusplus.com/reference/vector/vector/?kw=vector

vector<string> ClassroomName,ProfessorName,CourseTitle;
vector<int> ClassroomCap,StudentAmt;
int WhoTeach[100]={0};

struct{
    string professor;
    string coursename;
    string classroom;
}GroupSchedule[40];

bool TimeProf[100][40]={0},TimeRoom[100][40]={0},TimeGro[100][40]={0};

bool possibleTime(int grp,int crs){
    srand(time(NULL));
    //srand() to get different numbers from rand() function after each run, you can remove this line if you want to get same timetable after each run

    int freeFcl[40],freeRoom[40];

    for(int i=0;i<40;i++){//initializing valuess
        freeFcl[i]=-1;
        freeRoom[i]=-1;
    }
    //
    int k=WhoTeach[crs];
    for(int j=0;j<40;j++){
        if(TimeProf[k][j]==0){//the prof is free at time j
            freeFcl[j]=k;
        }
    }


    for(int i=0;i<ClassroomName.size();i++){
        if(ClassroomCap[i]>=StudentAmt[grp]){//if classroom is capable
            for(int j=0;j<40;j++){
                if(TimeRoom[i][j]==0){//the room is free at time j
                    if(freeRoom[j]!=-1){
                        if((rand()%2)==1){//randomly choosing the room if there are more than 1
                            freeRoom[j]=i;
                        }
                    }
                    else freeRoom[j]=i;//if there is no option
                }
            }
        }
    }
    vector<int> suitTime;
    for(int i=0;i<40;i++){
        if(TimeGro[grp][i]==0 && freeFcl[i]!=-1 && freeRoom[i]!=-1){
            suitTime.push_back(i);//suitTime will contain all times, which are valid
        }
    }
    if(suitTime.size()==0)return 0;//if there is no suitable time for this course
    //the time will be chosen randomly among suitTime
    int chosen=rand()%suitTime.size();//the suitTime[chosen] is the randomly chosen suitable time for this course
    int x=suitTime[chosen];
    GroupSchedule[x].classroom=ClassroomName[freeRoom[x]];
    GroupSchedule[x].coursename=CourseTitle[crs];
    GroupSchedule[x].professor=ProfessorName[freeFcl[x]];
    TimeGro[grp][x]=1;
    TimeProf[freeFcl[x]][x]=1;
    TimeRoom[freeRoom[x]][x]=1;
    return 1;
}


int main(){
    string dow[]={"MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY"};
    string hod[]={" 9 - 10 :","10 - 11 :","11 - 12 :","12 - 13 :","14 - 15 :","15 - 16 :", "16 - 17 :", "17 - 18 :"};


    vector<string> GroupName;
    vector<int> CourseHours,ApprCourse[100];//the appropriate courses for group i (ApprCourse[i]);
    //ifstream and ofstream are used to work with files they need library <fstream>,
    //for more information: http://www.cplusplus.com/doc/tutorial/files/
    ifstream fin ("Student Groups.txt");
    int a;
    string s;
    while(fin>>a && fin>>s){//
        StudentAmt.push_back(a);
        GroupName.push_back(s);
    }
    fin.close();

    fin.open("Classrooms.txt");

    while(fin>>a && fin>>s){
        ClassroomCap.push_back(a);
        ClassroomName.push_back(s);
    }

    fin.close();

    fin.open("Course Data.txt");
    int pos=0;
    while(fin>>s){
            //.back() gets access to the last element
        int profnum=-1,groupnum=-1;
        char x;
        if(s[s.size()-1]==',' || s[s.size()-1]=='-' || s[s.size()-1]=='+' || s[s.size()-1]==';' || s[s.size()-1]=='/'){
            x=s[s.size()-1];
            s.erase(s.size()-1);//removes the last element
        }
        else fin>>x;
        if(x=='+'){//is s is prof name
            for(int i=0;i<ProfessorName.size();i++){
                if(s==ProfessorName[i]){
                    profnum=i;
                    break;
                }
            }
            if(profnum==-1){
                ProfessorName.push_back(s);
                profnum=ProfessorName.size()-1;
            }
            WhoTeach[pos]=profnum;//this prof teaches this course
        }
        else if(x=='/')CourseTitle.push_back(s);
        else if(x==',' || x==';'){//if s is group name
            for(int i=0;i<GroupName.size();i++){
                if(s==GroupName[i]){
                    groupnum=i;
                    break;
                }
            }
            if(groupnum==-1){
                cout<<"The input files are wrong!!!";
                return 0;
            }
            ApprCourse[groupnum].push_back(pos);
            if(x==';')pos++;
        }
        else if(x=='-'){
            CourseHours.push_back(atoi(s.c_str()));
        }
        else {
            cout<<"The input files are wrong!!!";
            return 0;
        }
    }
    fin.close();
    ofstream fout ("Schedule for All.txt");

    for(int i=0;i<GroupName.size();i++){
        for(int j=0;j<ApprCourse[i].size();j++){
            for(int t=0;t<CourseHours[ApprCourse[i][j]];t++){
                bool Right=possibleTime(i,ApprCourse[i][j]);
                if(Right==0){
                    cout<<"Impossible to make Schedule !!!";
                    return 0;
                }
            }
        }
        fout<<"The Schedule for Group "<<GroupName[i]<<endl;
        fout<<"         ";//9 spaces
        for(int i=0;i<5;i++){
            fout<<setw(40)<<dow[i]<<"|";//for more information http://www.cplusplus.com/reference/iomanip/setw/
        }
        fout<<endl;
        for(int i=0;i<8;i++){
            fout<<hod[i];
            for(int j=0;j<5;j++){
                int k=(j*8)+i;
                int wdth=(GroupSchedule[k].professor.size()+GroupSchedule[k].classroom.size()+8);// 8 is the size of " by " and " in "
                if(wdth<9)fout<<setw(40)<<" "<<"|";//if group have no classes at this time
                else fout<<setw(40-wdth)<<GroupSchedule[k].coursename<<" by "<<GroupSchedule[k].professor<<" in "<<GroupSchedule[k].classroom<<"|";
                GroupSchedule[k].coursename.clear();//clearing the memory for another group
                GroupSchedule[k].professor.clear();
                GroupSchedule[k].classroom.clear();
            }
            fout<<endl;
        }
    }
    fout.close();

    fout.open("Schedule for One.txt");

    string ask;
    cout<<"Enter the Group Name: "<<endl;
    cin>>ask;

    int groupnumber=-1;
    for(int i=0;i<GroupName.size();i++){
        if(ask==GroupName[i]){
            groupnumber=i;
            break;
        }
    }
    if(groupnumber==-1){
        cout<<"The Group does not exist !!!"<<endl;
        return 0;
    }

    fin.open("Schedule for All.txt");
    int pst=-1;
    while(getline(fin,s)){
        if(s.size()<150)pst++;

        if(pst==groupnumber){
            fout<<s<<endl;
        }
    }
    return 0;
}
