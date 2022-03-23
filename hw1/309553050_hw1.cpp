#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <string>  
#include <sstream>   
using namespace std;
struct student{
    int studentID;
    int grade1;
    int grade2;
    int choose;
};
struct company{
    int companyID;
    int grade1;
    int grade2;
    int salary;
};
struct output{
    int studentID;
    int companyID;
};
bool comp(company a, company b){
    return a.salary > b.salary;
}
bool compCID(company a, company b){
    return a.companyID < b.companyID;
}
bool compID(output a, output b){
    return a.studentID < b.studentID;
}
int main(int argc, char *argv[]){
    fstream student_file(argv[1]);
    fstream company_file(argv[2]);  
    ofstream output(argv[3]);
    vector<struct student> student_vector;
    vector<struct company> company_vector;
    vector<struct output> output_vector;
    struct student student_struct;
    struct company company_struct;
    struct output output_struct;
    vector<vector<vector<int> > > grade_table = vector<vector<vector<int> > >( 101, vector<vector<int> >(101, vector<int>(1, 0))); //做一個3維vector
    //student 讀檔
    while (student_file >> student_struct.studentID >> student_struct.grade1 >> student_struct.grade2){
        student_struct.choose = 0; 
        student_vector.push_back(student_struct);
    }
    //company 讀檔
    while (company_file >> company_struct.companyID >> company_struct.grade1 >> company_struct.grade2 >> company_struct.salary){
        company_vector.push_back(company_struct);
    }
    //照公司ID排序
    sort(company_vector.begin(),company_vector.end(),compCID); 
    //照薪水排序
    sort(company_vector.begin(),company_vector.end(),comp);    
    //演算法實作
    vector<struct student>::iterator iters = student_vector.begin();
    for(iters; iters != student_vector.end(); ++iters){ 
        //如果是第一個
        if(grade_table[iters->grade1][iters->grade2][0] == 0){
            grade_table[iters->grade1][iters->grade2][0] = int(iters->studentID);
        }
        else{ //如果不是
            grade_table[iters->grade1][iters->grade2].push_back(int(iters->studentID)); //加人上去
        }
    }
    vector<struct company>::iterator iterc = company_vector.begin();
    for(iterc; iterc != company_vector.end(); ++iterc){ //從最高薪的公司開始挑
        for(int g1 = 100 ; g1 >= iterc->grade1 ; g1--){
            for(int g2 = 100 ; g2 >= iterc->grade2 ; g2--){
                int same_grade = grade_table[g1][g2].size()-1;//在g1 g2這個分數有多少人同分
                for(same_grade ; same_grade >= 0; same_grade--){
                    if(grade_table[g1][g2][same_grade]!=0){
                        output_struct.studentID = grade_table[g1][g2][same_grade];
                        output_struct.companyID = iterc->companyID;
                        output_vector.push_back(output_struct); //結果寫入output vector
                        grade_table[g1][g2][same_grade] = 0; //被挑走了 沒了
                    }
                }
            }
        }
    }
    //撿回收 0號公司 
    for(int i = 0;i<=100;i++){ 
        for (int j = 0;j<=100;j++){
            int same_grade = grade_table[i][j].size()-1;//在g1 g2這個分數有多少人同分
            for(same_grade ; same_grade >= 0; same_grade--){
                if(grade_table[i][j][same_grade]!=0){
                    output_struct.studentID = grade_table[i][j][same_grade];
                    output_struct.companyID = 0;
                    output_vector.push_back(output_struct); //結果寫入output vector
                    grade_table[i][j][same_grade] = 0; //被挑走了 沒了
                }
            }
        }
    }
    sort(output_vector.begin(),output_vector.end(),compID);
    vector<struct output>::iterator itero = output_vector.begin();
    for(itero; itero != output_vector.end(); ++itero){
        //cout << itero->studentID << " " << itero->companyID << " "<<endl; 
        output << itero->studentID << ": " << itero->companyID << " "<<endl; 
    }
    output.close();
    return 0;
}
