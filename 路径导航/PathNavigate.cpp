#include <cmath>
#include <cstring>
#include <iostream>
#include <queue>
using namespace std;
#define maxn 1000

int n, m, s, t;
double x[maxn], y[maxn]; //点的坐标
bool edge[maxn][maxn];   //二维数组存边
int pos[maxn];           //每个节点的前驱结点，记录路径
double length;           //最短路径长度
vector<int> path;        //最短路径

double gi[maxn], f[maxn]; // gi耗散值  f评价值

double distance(int A, int B){ //点A与点B的距离
    return sqrt((x[A] - x[B]) * (x[A] - x[B]) + (y[A] - y[B]) * (y[A] - y[B]));
}

double A_star(int s, int t); // A_star算法主体过程，pos数组记录每个节点的前驱结点

int main(){
    cin >> n >> m >> s >> t;
    for (int i = 0; i < n; i++){
        cin >> x[i] >> y[i]; //保存点的坐标
    }
    for (int i = 0; i < m; i++){
        int A, B;
        cin >> A >> B; //保存边
        edge[A][B] = edge[B][A] = true;
    }

    memset(f, -1, sizeof(f));
    length = A_star(s, t); //算法主体

    int tmp = t;
    while (tmp != s){
        path.push_back(tmp);
        tmp = pos[tmp];
    }
    path.push_back(tmp); //路径记录

    cout << length << endl; //输出答案
    for (int i = path.size() - 1; i > 0; i--)
        cout << path[i] << " ";
    cout << path[0] << endl;

    return 0;
}

/* 你的代码将会被嵌入在这个部分 */
double A_star(int s, int t){
    bool closed[maxn] = {false};
    length = 0.0;
    while(s != t){
        closed[s] = true;
        double closestDistance = 100000;
        int nextPoint = s;

        for (int i = 0; i < n; i++){
            if(edge[s][i] && !closed[i]){
                gi[i] = length + distance(s, i);
                f[i] = gi[i] + distance(i, t);
                if(f[i] < closestDistance){
                    closestDistance = f[i];
                    nextPoint = i;
                }
            }
        }

        if(closed[nextPoint]){
            length -= distance(s, pos[s]);

            edge[s][pos[s]] = false; 
            edge[pos[s]][s] = false;
            
            s = pos[s];
        }else{
            length += distance(s, nextPoint);

            pos[nextPoint] = s;
            s = nextPoint; 
        }
    }
    return length;
}