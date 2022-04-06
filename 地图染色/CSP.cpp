#include <bits/stdc++.h>

using namespace std;

#define MAXN 30
#define MAXM 300

int n, m;
int AdjMartix[MAXN][MAXN] = {{0}};
int AdjCnt[2][MAXN] = {-1};
int Color[MAXN] = {0};

void init(){
    int tA, tB;
    for (int i = 0; i < n; i++){
        AdjCnt[1][i] = i;
        AdjCnt[0][i] = 0;
        Color[i] = -1;
    }
    for (int i = 0; i < m; i++){
        cin >> tA >> tB;
        AdjMartix[tA][tB] = 1;
        AdjMartix[tB][tA] = 1;
        AdjCnt[0][tA] += 1;
        AdjCnt[0][tB] += 1;
    }
    for (int i = n - 1; i >= 0; i--){
        tA = i;
        for (int j = i - 1; j >= 0; j--){
            if(AdjCnt[0][j] < AdjCnt[0][tA]){
                tA = j;
            }
        }

        tB = AdjCnt[0][i];
        AdjCnt[0][i] = AdjCnt[0][tA];
        AdjCnt[0][tA] = tB;

        tB = AdjCnt[1][i];
        AdjCnt[1][i] = AdjCnt[1][tA];
        AdjCnt[1][tA] = tB;
    }
}

int getAvailableColor(int loc, int N){
    // 需要两个要素，一个是邻居的颜色，一个是用过的颜色
    // 如果访问loc的话，loc的颜色取值直接往上增加到可行的值，如果找不到更高的颜色了，就返回-1
    bool avbc[MAXN] = {false};
    for(int i = 0; i < n; i++){
        avbc[i] = true;
    }

    for(int i = 0; i < n; i++){
        // 获取邻居的颜色列表，如果有相同的就不使用
        if(AdjMartix[loc][i] == 1){
            if(Color[i] != -1){
                avbc[Color[i]] = false;
            }
        }
    }

    int start = Color[loc] + 1;

    for (int i = start; i < N; i++){
        if(avbc[i] == true){
            return i;
        }
    }
    return -1;
}

bool solve(int N){
    int lastIndex = 0, startIndex = n, pt;
    int *d = AdjCnt[0];
    int *point = AdjCnt[1];

    // 孤立点颜色为0
    for (int i = n - 1; i >= 0; i--){
        if(d[i] == 0){
            Color[point[i]] = 0;
        }else{
            lastIndex = i;
            break;
        }
    }

    // 从度最大的点出发
    for (int i = 0; i <= lastIndex; i++){
        if(d[i] == 1){
            // 执行到这一步的时候已经算是成功了：之前的点的颜色符合规范，之后只剩度为1的点，只用两种颜色也足够了
            startIndex = i;
            break;
        }


        pt = point[i];
        Color[pt] = getAvailableColor(pt, N);
        // 如果不等于-1，则认为目前还是有解的，往下一个位置走
        if(Color[pt] == -1){
            i -= 2;
            if(i == -2){
                // 如果第一个位置上的点都没有可用的颜色了，说明此条件下不可能，return false
                return false;
            }
        }
    }

    // 存在度为1的点,访问唯一接邻点. 如果邻居为0，则颜色置为1；反之置为0
    for (int i = startIndex; i <= lastIndex; i++){
        pt = point[i];
        for (int j = 0; j < n; j++){
            if(AdjMartix[pt][j] == 1){
                Color[pt] = (Color[j] == 0 ? 1 : 0);
                break;
            }
        }
    }

    return true;
}

int main(){
    cin >> n >> m;
    init();
    for (int i = 2; i <= n; i++){
        if(solve(i)){
            cout << i << endl;
            for (int j = 0; j < n; j++)
                cout << Color[j] << endl;
            break;
        }else{
            for (int j = 0; j < n; j++)
                Color[j] = -1;
        }
    }
    return 0;
}