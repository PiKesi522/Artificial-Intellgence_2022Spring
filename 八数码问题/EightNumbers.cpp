#include<bits/stdc++.h>
using namespace std;
 
string block;
set<string> closed;
queue<string> que;

struct node{
	string pre;
	char direction;
};
 
map<string, node>road;

bool moveUp(string &g){
	int posx;
	for (int i = 0; i < 9; i++){
		if (g[i] == 'x'){
			posx = i;
			break;
		}
	}
	if (posx <= 2) return false;
	else{
		swap(g[posx], g[posx - 3]);
		return true;
	}
}
 
bool moveDown(string &g){
	int posx;
	for (int i = 0; i < 9; i++){
		if (g[i] == 'x'){
			posx = i;
			break;
		}
	}
	if (posx >= 6) 
        return false;
	else{
		swap(g[posx], g[posx + 3]);
		return true;
	}
}
 
bool moveLeft(string &g){
	int posx;
	for (int i = 0; i < 9; i++){
		if (g[i] == 'x'){
			posx = i;
			break;
		}
	}
	if (posx == 0 || posx==3 || posx==6) 
        return false;
	else{
		swap(g[posx], g[posx - 1]);
		return true;
	}
}
 
bool moveRight(string &g){
	int posx;
	for (int i = 0; i < 9; i++){
		if (g[i] == 'x'){
			posx = i;
			break;
		}
	}
	if (posx == 2 || posx==5 ||posx==8 )
        return false;
	else{
		swap(g[posx], g[posx + 1]);
		return true;
	}
}
 
bool bfs(){
	que.push(block);
	closed.insert(block);
 
	while (!que.empty())
	{
		string loc = que.front();
		que.pop();
 
		if (loc == "12345678x")
			return true;
 
		for (int i = 0; i < 4; i++){
			string pCondition = loc;
			if (i == 0){
				if (moveUp(pCondition) && closed.count(pCondition)==0){
					que.push(pCondition);
					closed.insert(pCondition);
					road[pCondition].pre = loc;
					road[pCondition].direction = 'u';
				}
			}else if (i == 1){
				if (moveDown(pCondition) && closed.count(pCondition) == 0){
					que.push(pCondition);
					closed.insert(pCondition);
					road[pCondition].pre = loc;
					road[pCondition].direction = 'd';
				}
			}else if (i == 2){
				if (moveLeft(pCondition) && closed.count(pCondition) == 0){
					que.push(pCondition);
					closed.insert(pCondition);
					road[pCondition].pre = loc;
					road[pCondition].direction = 'l';
				}
			}else if (i == 3){
				if (moveRight(pCondition) && closed.count(pCondition) == 0){
					que.push(pCondition);
					closed.insert(pCondition);
					road[pCondition].pre = loc;
					road[pCondition].direction = 'r';
				}
			}
		}
	}
	return false;
}
 
int main(){
	char c;
	for (int i = 0; i < 9; i++){
		cin >> c;
		block += c;
	}
	
	if (!bfs())
		cout << "unsolvable" << endl;
	else{
		string endStatus = "12345678x";
		stack<char> path;

		while (endStatus != block){
			path.push(road[endStatus].direction);
			endStatus = road[endStatus].pre;
		}

		while (!path.empty()){
			cout << path.top();
			path.pop();
		}
 
		cout << endl;
	}
}