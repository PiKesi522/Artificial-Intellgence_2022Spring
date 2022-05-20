#include <bits/stdc++.h>
using namespace std;
const int maxn = 21;
const int epics = 50000;
const int target_success = 5000;
const double inf = 1e8;

bool cmp(pair<int,double> X, pair<int,double> Y) 
{
    if(X.second == Y.second) return X.first < Y.first;
    return X.second > Y.second;
}


// 迷宫尺寸 n*m
// x,y坐标从1开始 起点(1,1) 终点(n,m)
// 对于每个位置(i,j),可以向U,L,R,D四个方向移动，不可走出迷宫边界，不可走到障碍物上
int n,m;

// maze[maxn+5][maxn+5]存储迷宫 0代表可走通 1代表陷阱 (maze[i][j], 1<=i<=n,1<=j<=m)
// 答案保证有解 即 maze[1][1] = 0, maze[n][m] = 0, 起点与终点之间保证至少存在一条路径
int maze[maxn+5][maxn+5];

// 奖励函数R 设置普通格子奖励为-(abs(i-n)+abs(j-m)) 陷阱奖励为-10000 终点奖励为100000 衰减因子alpha = 0.9 , gama = 0.9
const double alpha = 0.9;
const double gama = 0.9;
double Reward[maxn+5][maxn+5];

// 状态函数Q 下一步的选取采用1-ϵ贪心
// 关于Qstate[pos]中的pos索引到二维坐标(px,py)的转换:
//  px = (pos - 1) / m + 1
//  py = (pos - (px-1) * m) % (m+1);
const double greedy_factor = 0.1;
vector< pair<int,double> > Qstate[(maxn+5)*(maxn+5)];

// 存储答案
vector< pair<int,int> > ans;

// 初始化奖励函数R和状态函数Q
// 状态函数Q，一维映射二维，依次保存上下左右四个方向，如果方向上可以行进，则将可选的Action放入State，Reward为-1
void init_RQ(int i,int j) 
{
    // 陷阱不设置奖励函数（可理解成均为-inf)
    if (maze[i][j] == 1) return;
    // Up
    if (Reward[i-1][j]) {
        Qstate[(i-1)*m+j].push_back({(i-2)*m+j,-1});
    }
    // Left
    if (Reward[i][j-1]) {
        Qstate[(i-1)*m+j].push_back({(i-1)*m+j-1,-1});
    }
    // Right
    if (Reward[i][j+1]) {
        Qstate[(i-1)*m+j].push_back({(i-1)*m+j+1,-1});
    }
    // Down
    if (Reward[i+1][j]) {
        Qstate[(i-1)*m+j].push_back({i*m+j,-1});
    }
}


int success_time = 0;
bool isEnd(int i,int j) 
{   
    // 终点
    if (i == n && j == m) {
        ++success_time;
        return true;
    }
    // 死亡
    if (maze[i][j] == 1) {
        return true;
    }
    return false;
}


void Qlearning()
{
    // 
    int nx = 1 + rand() % n, ny = 1 + rand() % m;
    int cnt = 0;
    // 没有到达终点，或者没有遍历整个迷宫
    while (!isEnd(nx,ny) && cnt < n*m) {
        // 当前位置可以做的所有<行动A，奖励R>
        auto &s0 = Qstate[(nx-1)*m+ny];
        // 有多少动作可以选择
        int siz = s0.size();
        int _next = -1, idx = 0;
        // 如果没有方向可以走，就返回
        if (!siz) return;
        // 如果只有一个方向可以走，就走第一个方向
        if (siz == 1) _next = s0[0].first;
        else {
            // 寻找价值最高的一条路；如果价值相同，按照上左下右循环
            sort(s0.begin(),s0.end(),cmp);

            double p = rand() / double(RAND_MAX);
            // 大概率选择最优情况
            if (p < 1 - greedy_factor) {
                _next = s0[0].first;
            }
            else {
                // 小概率选择其他一个随机情况
                 idx = 1 + rand() % (siz - 1);
                _next = s0[idx].first;
            }
        }
        // s1是选择到的下一个状态，用于反馈此状态
        auto &s1 = Qstate[_next];
        // 找到s1的最大的次选择价值
        sort(s1.begin(),s1.end(),cmp);
        // 查看下一步是否有路径，如果有就是R，没有就是0
        double _nextMax = (int)s1.size() ? s1[0].second : 0.0;

        // 将下一步的一维位置，映射到二维
        nx = (_next - 1) / m + 1;
        ny = (_next - (nx-1) * m) % (m+1);
        // 更新QLearning的状态
        s0[idx].second = s0[idx].second + alpha * (Reward[nx][ny] + gama * _nextMax - s0[idx].second);
        cnt++;
    }
}


int main()
{
    ios::sync_with_stdio(false);
    srand(2022);
    cin >> n >> m;

    for(int i = 1; i <= n; i++)
    {
        for(int j = 1; j <= m; j++) {
            int t;
            cin >> t;
            maze[i][j] = t;
            // 奖励是到目的地的曼哈顿距离的负值
            Reward[i][j] = (t == 0 ? -(abs(i-n)+abs(j-m)) : -10000.0);
        }
    }
    Reward[n][m] = 100000.0;

    for(int i = 1; i <= n; i++) {
        for(int j = 1; j <= m; j++) {
            
            init_RQ(i,j);
        }
    }


    int cnt = 0;
    while (cnt < epics && success_time != target_success) {
        Qlearning();
        ++cnt;
    }
    cnt = 0;
    int x = 1, y = 1;
    while ((x!=n || y!=m) && cnt < n*m) {
        auto &s = Qstate[(x-1)*m+y];
        if (!(int)s.size()) {
            // 无解情况，可忽略
            cout << "Something Wrong!" << endl;
            return 0;
        }
        ans.push_back({x,y});
        sort(s.begin(),s.end(),cmp);
        int _next = s[0].first;
        x = (_next - 1) / m + 1; 
        y = (_next - (x-1) * m) % (m+1);
        ++cnt;
    }
    ans.push_back({n,m});
    cout << (int)ans.size() << endl;
    for(auto& u : ans) {
        cout << u.first << ' ' << u.second << endl;
    }

    return 0;
}