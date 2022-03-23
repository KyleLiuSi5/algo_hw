
#include <iostream>
#include <vector>
#include <fstream> 
#include <bits/stdc++.h>
#include <algorithm>

using namespace std;
struct airport{
  int number;
  int x_axis;
  int y_axis;
  vector<int> link_to;
};
struct add_path{
  float distance;
  int start;
  int end;
};
class Graph {
    int V; 
    list<int>* adj;
    void DFSUtil(int v, bool visited[]);
public:
    Graph(int V);
    void addEdge(int v, int w);
    bool* DFS(int v);
};
Graph::Graph(int V)
{
    this->V = V;
    adj = new list<int>[V];
}
void Graph::addEdge(int v, int w)
{
    adj[v].push_back(w); 
}
void Graph::DFSUtil(int v, bool visited[])
{
    visited[v] = true;
    //cout << v << " ";
    list<int>::iterator i;
    for (i = adj[v].begin(); i != adj[v].end(); ++i)
        if (!visited[*i])
            DFSUtil(*i, visited);
}

bool* Graph::DFS(int v)
{
    bool* visited = new bool[V];
    for (int i = 0; i < V; i++)
        visited[i] = false;
    DFSUtil(v, visited);
    //cout << endl;
    return visited;
}
int counter(bool* visited, bool* baseline,int airport_num){ //回傳從某個點出發可以走到幾個沒被走過的點 用來找出可以走到最多的 然後連到他 //且baseline走過的你就不能走了
  int count = 0;
  for(int i=0;i<airport_num;i++){
    if (visited[i]==1 && baseline[i]==0) count++;
    //cout << "v"<<i <<"="<<visited[i]<<" & " << "b"<<i <<"="<<baseline[i]<<endl;
  }
  return count;
}

add_path find_nearest(bool* visited, vector<struct airport> airport_vec, int max,int airport_num){
  int shortest = 0; //預設是0(一定有)
  float temp_short = 999999.0;
  float distance;
  add_path data;
  for(int i =0; i<airport_num ;i++){
    if(visited[i]==1){
      distance = sqrt(pow((airport_vec[i].x_axis - airport_vec[max].x_axis),2) + pow((airport_vec[i].y_axis - airport_vec[max].y_axis),2));
      if (distance < temp_short){
        temp_short = distance;
        shortest = i;
      }
    }
  }
  data.distance = temp_short;
  data.start = shortest;
  data.end = max;
  return data;
}

bool compare(const add_path &a, const add_path &b)
{
    return a.distance < b.distance;
}

int main(int argc, char* argv[]) {

	ifstream input(argv[1]);
	ofstream output(argv[2]);
  int airport_num;
  int x,y;
  int Start,End;
  double distance;
  airport temp_airport;
  vector<struct airport> airport_vec;
  
  input >> airport_num; //load airport number
  
  for (int i = 0 ; i < airport_num ; i++){ //load position
    input >> x >> y;
    temp_airport.number = i;
    temp_airport.x_axis = x;
    temp_airport.y_axis = y;
    airport_vec.push_back(temp_airport);
  }
  
  bool* base_visited = new bool[airport_num];
  int* connect_number = new int[airport_num];
  add_path data;
  vector<struct add_path> flight_info;
  Graph g(airport_num);
  while(input >>Start >>End){ //load flight information
    //airport_vec[Start].link_to.push_back(End);
    g.addEdge(Start, End);
  }
  int max,temp_max;
  bool all_connected =0;
  while (all_connected == 0){
    base_visited = g.DFS(0);
    temp_max = 0;
    max = 0;
    int a = 0;
    for(int i =0; i<airport_num ;i++){
      if(base_visited[i]==0){
        a = counter(g.DFS(i),base_visited,airport_num);
        //cout <<"v = " <<i<<" counter = " <<a << endl; 
        if (a > temp_max){
          
          temp_max = counter(g.DFS(i),base_visited,airport_num);
          max = i;
        }
      }
    }
    
    if (max !=0){
     data = find_nearest(base_visited,airport_vec,max,airport_num); // shortest是現有的(0可以到的) 且連到max是最短距離
     flight_info.push_back(data);
     g.addEdge(data.start, max); 
     //cout << "add "<< data.start<< " -> "<<max << endl;
     }//有要加的就加
    else if (max == 0) all_connected = 1; //如果到最後都沒有增加邊的話 結束
  }
  //以下輸出
  output << flight_info.size() << endl; //先印航班個數
  //出發目的地用距離重新排序
  std::sort(flight_info.begin(), flight_info.end(), compare);
  for (int i =0;i<flight_info.size() ;i++){
    output << flight_info[i].start <<" "<< flight_info[i].end << endl;
  }
  
  
	input.close();
	output.close();
	return 0;
}












