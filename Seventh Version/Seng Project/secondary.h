#ifndef defsecond//to avoid multi inclusion of this header file
#define defsecond 1
#include<vector>
#include<iostream>
using namespace std;

class Table{
private:
    int course[100],faculty[100],room[100];//the course faculty and room of group i if i is faculty[i],course[i],room[i];
public:
    void initialise();
    string output(int t,int g);
    bool canUseFaculty(int f);
    bool canUseRoom(int r);
    bool isGroupFree(int g);
    void setValues(int g,int c,int f,int r);
};//all courses at time i if curTable[i]

void readfromFiles();

int findTimeforGroup(int g, int c);



int GetgroupAmt();
vector<int> GetgrpCourse(int i);
string GetgroupStr(int i);
string getoutput(int t,int g);
void TableInitialise(int t);//all these functions were included because of the error with multi inclusion
#endif // defsecond
