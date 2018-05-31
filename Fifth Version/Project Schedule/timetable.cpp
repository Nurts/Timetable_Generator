#include<iostream>
#include<ctime>
#include<cstdlib>
#include<fstream>
#include "timetable.h"
using namespace std;

string fculName[300],courseT[300];
string getDays(int t){
string s;
if(t<4)s="Monday: ";
else if(t<8)s="Tuesday: ";
else if(t<12)s="Wednesday: ";
else if(t<16)s="Thursday: ";
else s="Friday: ";
return s;
}
string getTimeStr(int t){
    string s;
    t%=4;//to find the duration of the course
    switch (t){
        case 0:
            s="09:00-10:50";
            break;
        case 1:
            s="11:00-12:50";
            break;
        case 2:
            s="14:00-15:50";
            break;
        case 3:
            s="16:00-17:50";
            break;
    }
    return s;
}

void getTitles(){
    ifstream facultyNames ("facultyNames.txt");
    int i=1;//because input is provided from 1
    while(getline(facultyNames,fculName[i]))i++;
    facultyNames.close();

    ifstream courseTitle ("courseTitle.txt");
    i=1;
    while(getline(courseTitle,courseT[i]))i++;
    courseTitle.close();
}

void createNewTable(){//creates New timetable for all groups
    int numberFcul[300],idFcul[300][300],room[300];
    bool freegroup[10][300][50]={0},emptyroom[300][100]={0},freeFcul[300][100]={0};
    srand(time(NULL));
    //srand(time(NULL)) to get different numbers from rand() function after each run of this function
    //ifstream and ofstream are used to work with files they need library <fstream>, for more information: http://www.cplusplus.com/doc/tutorial/files/

    ifstream roomCapacity ("RoomCapacity.txt");
    int roomAmt;
    while(roomCapacity>>room[roomAmt])roomAmt++;//room stores the room capacities for room i if room[i]
    roomCapacity.close();

    ifstream courseList ("courseList.txt");
    int i=1;
    char c;
    while(courseList>>numberFcul[i]){//how many profs teaches this course (i) numberFcul[i]
        courseList>>c;//c is '-' (garbage)
        for(int j=0;j<numberFcul[i];j++){
            courseList>>idFcul[i][j];//IDs of the prof, who teaches course i;
        }
        i++;
    }
    courseList.close();

    ifstream groupData ("groupsData.txt");
    ofstream tableDatabase ("Table Database.txt");
    ofstream allout ("All Tables.txt");
    int y,g,cap;
    string s;

    while(groupData>>s){//s is "Year:" (garbage)
        groupData>>y;//the year number
        groupData>>s;//s is "Group:" (garbage)
        groupData>>g;//the group Number
        groupData>>s;// Garbage
        groupData>>cap;//number of students
        groupData>>s;//Garbage
        int courseQuan;
        groupData>>courseQuan;//number of courses which they are taking
        groupData>>c;//c is '-'
        allout<<endl<<"Year: "<<y<<",Group: "<<g<<endl;//printing to All Tables.txt
        int crsoutput[20],fculoutput[20],roomoutput[20];//needed for printing to All Tables.txt

        for(int i=0;i<20;i++)crsoutput[i]=-1;//initializing the value of the crsoutput

        for(int j=0;j<courseQuan;j++){
            int crs;
            groupData>>crs;//getting the course id, which the group is taking
            int time,idroom=-1,idfaculty=-1,inf=0;//initializing values
            while(idfaculty==-1 && inf<10000000){
                inf++;//if the loop works infinitely
                time=rand()%20;//the time was chosen randomly is less than 20 because we have 20 hours from 0 to 19
                if(freegroup[y][g][time]==1)continue;//if the group is not free at time continues to randomly chose time
                for(int ii=0;ii<roomAmt;ii++){
                    if(emptyroom[ii][time]==0 && room[ii]>=cap){//if the room ii is free at time and capable
                        idroom=ii;//setting the value of idroom and continuing to look for prof
                        break;
                    }
                }

                if(idroom==-1)continue;//if room is not found continues to randomly chose time

                for(int ii=0;ii<numberFcul[crs];ii++){//the number of faculties teaching course crs
                    int jj=idFcul[crs][ii];//the ID of the faculty
                    if(freeFcul[jj][time]==0){//if the prof is free at this time
                        idfaculty=jj;//sets the value of the prof
                        break;
                    }
                }
                //if idfaculty is not '-1' breakes from while;
                //because we found the time idroom and idfaculty for this course
            }
            if(idfaculty==-1){//if something wrong with .txt files
                cout<<"The input data is wrong !!!"<<endl<<"Timetable creating process failed. (Please check .txt files)"<<endl;
                tableDatabase.close();
                remove("Table Database.txt");//Deletes the Table Database.txt because table was not created successfully
                return;
            }
            freeFcul[idfaculty][time]=1;//setting the values of the checkers to 1 because they became not free
            freegroup[y][g][time]=1;
            emptyroom[idroom][time]=1;
            tableDatabase<<y<<" "<<g<<" "<<crs<<" "<<time<<" "<<idfaculty<<" "<<idroom<<endl;//printing the values to Table Database.txt
            crsoutput[time]=crs;//setting the values of the arrays which are needed for output to the All Tables.txt
            fculoutput[time]=idfaculty;
            roomoutput[time]=idroom;
        }
        for(int i=0;i<20;i++){//printing the values to All Tables.txt i is the time
            int crs=crsoutput[i],idfaculty=fculoutput[i],idroom=roomoutput[i];
            if(i%4==0){
                allout<<getDays(i)<<endl;
            }
            allout<<'\t'<<getTimeStr(i)<<" - ";
            if(crs!=-1)allout<<courseT[crs]<<", "<<fculName[idfaculty]<<", "<<"Room #"<<(idroom+1);
            allout<<endl;
        }
    }
    cout<<"Table was created successfully!!!"<<endl;
    tableDatabase.close();
}

void getTable(int y,int g){//prints the timetable for year y group g to the console window
    ifstream tableDatabase ("Table Database.txt");

    if(!tableDatabase.is_open()){//if Table Database does not exist
        cout<<"The table is not created yet!!!"<<endl;
        return;
    }
    int yy,gg;
    bool isfound=0;
    string s;
    int crsoutput[20],fculoutput[20],roomoutput[20];
    for(int i=0;i<20;i++)crsoutput[i]=-1;

    while(tableDatabase>>yy>>gg){//getting values from Table Database.txt
        if(yy!=y || gg!=g){
            getline(tableDatabase,s);//s is garbage because the data in this line is not related to our group
            continue;
        }
        //if the data is related to our group
        isfound=1;
        int crs,time,idfaculty,idroom;
        tableDatabase>>crs>>time>>idfaculty>>idroom;
        crsoutput[time]=crs;
        fculoutput[time]=idfaculty;
        roomoutput[time]=idroom;//saving the values
         //idroom+1 because it is started from 0
    }
    if(isfound==0){//if group is not found
        cout<<"The group does not exist !!! (Please, Try again)"<<endl;
        return;
    }

    for(int i=0;i<20;i++){//printing the  table to the console window
        int crs=crsoutput[i],idfaculty=fculoutput[i],idroom=roomoutput[i];
        if(i%4==0){//if it is the beginning of the day
            cout<<getDays(i)<<endl;//prints the day name
        }
        cout<<'\t'<<getTimeStr(i)<<" - ";//prints the duration
        if(crs!=-1)cout<<courseT[crs]<<", "<<fculName[idfaculty]<<", "<<"Room #"<<(idroom+1);//prints the data course name faculty name and room id
        cout<<endl;
    }


    cout<<endl;
}
