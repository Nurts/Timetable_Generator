#include "secondary.h"
#include<fstream>
using namespace std;
int main(){
    readfromFiles();

    for(int i=0;i<20;i++){
        TableInitialise(i);
    }

    for(int i=1;i<=GetgroupAmt();i++){
        vector<int> cur=GetgrpCourse(i);
        for(int j=0;j<cur.size();j++){
            findTimeforGroup(i,cur[j]);
        }
    }
    string s,filename;
    cout<<"The Table was generated successfully!"<<endl<<"Enter the Group Name: ";
    getline(cin,s);


    int groupnumber=-1;
    for(int i=1;i<=GetgroupAmt();i++){
        if(GetgroupStr(i)==s){
            groupnumber=i;
            break;
        }
    }
    if(groupnumber==-1){
        cout<<"The name of the group is wrong!!!"<<endl;
        return 0;
    }
    cout<<"Enter the text file name (where you want to save the table) :";
    getline(cin,filename);
    filename+=".txt";
    ofstream output (filename.c_str());//.c_str() was used to be able to type the filename by the user
    //for more information http://www.cplusplus.com/reference/string/string/c_str
    string daysOfWeek[]={"Monday","Tuesday","Wednesday","Thursday","Friday"};
    output<<"The time table for group "<<s<<endl;
    for(int i=0;i<20;i++){
        if(i%4==0)output<<endl<<daysOfWeek[i/4]<<endl;
        output<<getoutput(i,groupnumber)<<endl;
    }
    output.close();
    output.open("AllTables.txt");

    for(int  j=1;j<=GetgroupAmt();j++){
        output<<endl<<"The time table for group "<<GetgroupStr(j)<<endl;
        for(int i=0;i<20;i++){
            if(i%4==0)output<<endl<<daysOfWeek[i/4]<<endl;
            output<<getoutput(i,j)<<endl;
        }
    }
    return 0;
}
