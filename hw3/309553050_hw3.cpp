#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream> 
#include <algorithm>
using namespace std;
#define INT_MIN -2147483648
#define INT_MAX 2147483647

int A(vector<int> value,int K){
    vector<int> largest(value.size()); //存全部最大的
    int maximum = INT_MIN;
    for (size_t i = 0; i < value.size(); ++i) {
        if (i < K){ //前K個
          if (value[i] > maximum) maximum = value[i];
          else  maximum = maximum;
        }
        else{ //第K+1個
          maximum = max(largest[i-1],largest[i-K] + value[i]);
          maximum = max(maximum,0+value[i]); //如果前面都是負的 只取現在值是最大的
        }
        largest[i] = maximum;
    }
    return largest[value.size()-1];
}

int B(vector<int> value,int K){
    int maximum = INT_MIN;
    int sum = 0;
    vector<int> new_value(value.size(),0);
    for (size_t i = 0; i < value.size(); ++i) {
        if(value[i]>=0) new_value[i] = value[i];
    }
    for(int i=0;i<value.size();i++){
      if(i<K+1) sum += new_value[i];
      else sum = sum - new_value[i-K-1] + new_value[i];
      if (sum >maximum) maximum=sum;
    }
    if (maximum == 0){
      maximum = INT_MIN;
      for (size_t i = 0; i < value.size(); ++i) {
          if(value[i]>maximum) maximum = value[i];
      }
    }
    return maximum;
}

int C(vector<int> value,int K){

    vector<int> largest(value.size()); //存全部最大的
    vector<int> smallest(value.size());
    int maximum = INT_MIN;
    int minimum = INT_MAX;
    for (size_t i = 0; i < value.size(); ++i) {
        if (i < K){ //前K個
          if (value[i] > maximum) maximum = value[i];
          else  maximum = maximum;
          if (value[i] < minimum) minimum = value[i];
          else  minimum = minimum;
        }
        else{ //第K+1個
          minimum = min(smallest[i-1],smallest[i-1-K+1]*value[i]);
          minimum = min(largest[i-1-K+1]*value[i],minimum);
          minimum = min(minimum,1*value[i]);
          maximum = max(largest[i-1],largest[i-1-K+1]*value[i]);
          maximum = max(smallest[i-1-K+1]*value[i],maximum);
          maximum = max(maximum,1*value[i]);
        }
        largest[i] = maximum;
        smallest[i] = minimum;
    }
    return largest[value.size()-1];
}


int main(int argc, char* argv[]) {
	ifstream input(argv[1]);
	ofstream output(argv[2]);
  vector<int> value;
  string line;
  int K;
  char Case;
  int temp;
  istringstream ss;
  bool firsttime = true;
  
  while (getline(input, line)){ //讀檔
      istringstream ss(line);
      if(line[0]=='A'){
          if (firsttime==false) output << endl;
          ss >> Case >> K;
          //cout << A(value,K) << endl;
          output << A(value,K);
          firsttime = false;
      }else if(line[0]=='B'){
          if (firsttime==false) output << endl;
          ss >> Case >> K;
          //cout << B(value,K) << endl;
          output << B(value,K);
          firsttime = false;
      }else if(line[0]=='C'){
          if (firsttime==false) output << endl;
          ss >> Case >> K;
          //cout << C(value,K) << endl;
          output << C(value,K);
          firsttime = false;
      }else{ 
          value.clear();
          while(ss>>temp) value.push_back(temp);
      }  
  }
	input.close();
	output.close();
	return 0;
}


















/* //舊方法
  while(input >> token){

      if(token=="A"){
          input >> token;
          K = stoi(token,&sz);
          // Do A here
          A(value,K);
          //cout << "A "<< K <<endl;
          change = true;
      }else if(token=="B"){
          input >> token;
          K = stoi(token,&sz);
          // Do B here
          B(value,K);
          //cout << "B "<< K <<endl;
          change = true;
      }else if(token=="C"){
          input >> token;
          K = stoi(token,&sz);
          // Do C here
          C(value,K);
          //cout << "C "<< K <<endl;
          change = true;
      }else{
      //TODO 要讓他可以連續置換 這方法要改掉
          if(change == true){
            value.clear();
            change = false; 
          }
          value.push_back(stoi(token));
      }
  }  
*/