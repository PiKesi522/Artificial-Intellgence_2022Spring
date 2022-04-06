#include <cmath>
#include <cstring>
#include <iostream>
#include <queue>
using namespace std;
#define maxn 1001

int n;
double x[maxn], y[maxn]; //点的坐标

double distance(int A, int B)
{ //点A与点B的距离
    return sqrt((x[A] - x[B]) * (x[A] - x[B]) + (y[A] - y[B]) * (y[A] - y[B]));
}

double f(const vector<int> &p)
{ //计算方案p的路径长度
    double ans = 0;
    for (unsigned i = 0; i < p.size() - 1; i++){
        ans += distance(p[i], p[i + 1]);
    }
    ans += distance(p[0], p[p.size() - 1]);
    return ans;
}

vector<int> p_new;     //产生的新解
vector<int> p_current; //当前最优解
vector<int> p_best;    //历史最好解
double t; //初始温度

bool flag = false;
int cnt;

void init()
{ //你可以进行的一些初始化操作
    cnt = 0; // 每种温度的迭代次数为cnt次
    t = 280; // 初始温度为280k
}

bool termi(){ //终止条件
    if(cnt < 56 * n){
        cnt += 1;
        return false;
    }
    flag = true;
    for(int i = 0; i < n; i++){
        if(p_current[i] != p_best[i]){
            cnt = 0;
            return false;
        }
    }
    return true;
}

vector<int> N(const vector<int> &p){ //产生新解
    int i = 0, j = 0, k;
    while( i == j){
        i = (rand() % ((int)p.size() - 1)) + 1;
        j = (rand() % ((int)p.size() - 1)) + 1;
    }

    int m = min(i, j);
    i = max(i, j);
    j = m;
    vector<int> temp;

    temp.push_back(1);
    for (k = 1; k < j;k++){
        temp.push_back(p[k]);
    }
    temp.push_back(p[i]);
    for (k = j ; k < i; k++){
        temp.push_back(p[k]);
    }
    temp.push_back(p[j]);
    for (k = i ; k < (int)p.size(); k++){
        temp.push_back(p[k]);
    }

    return temp;
}

double drop(double t){ //降温函数
    if(!flag){
        return t;
    }
    flag = 0;
    return 0.92 * t;
}

double calc_p(){ //状态被接受的概率
    double df = f(p_new) - f(p_current);
    double exp = -1 * df / t;
    return pow(2.718, exp);
}

void SA(){
    p_new.clear();
    for (int i = 1; i <= n; i++)
        p_new.push_back(i); //产生初始解
    p_best = p_current = p_new;

    init();

    while (termi() == false){ //程序未终止

        p_new = N(p_current);

        if (f(p_new) < f(p_current)){
            p_current = p_new;
        }else{
            double pt = calc_p(); //计算概率
            if (pt > (double)(rand() * 1.0 / RAND_MAX))
                p_current = p_new;
        }

        t = drop(t); //降温
        if (f(p_current) < f(p_best))
            p_best = p_current; //更新历史最优值
    }
}

int main()
{
    cin >> n; //输入
    for (int i = 1; i <= n; i++)
        cin >> x[i] >> y[i]; //保存点的坐标

    SA(); //模拟退火算法主体

    cout << f(p_best) << endl; //输出答案
    for (unsigned i = 0; i < p_best.size(); i++)
        cout << p_best[i] << " ";
    cout << 1 << endl;

    return 0;
}
/* 你的代码将会被嵌入在这个部分 */