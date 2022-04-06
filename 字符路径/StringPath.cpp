#include <bits/stdc++.h>
using namespace std;
#define MAXN 100
#define MAXM 100
#define MAXK 26
int n, m, k;
char Martix[MAXN][MAXM] = {{'\0'}};
char SearchPath[MAXK] = {'\0'};
struct rec{
    int locx;
    int locy;
    int val = 0;
    rec(int i1, int i2){
        this->locx = i1;
        this->locy = i2;
        this->val = 0;
    }
    rec(int i1, int i2, int v){
        this->locx = i1;
        this->locy = i2;
        this->val = v;
    }
};
vector<rec> Record;

int getLeastValue(int x, int y){
    int ans = INT_MAX, temp;
    for(rec it : Record){
        temp = abs(it.locx - x) + abs(it.locy - y) + it.val;
        if(temp < ans){
            ans = temp;
        }
    }
    return ans;
}

void calcuValue(char tar){
    vector<rec> temp;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            if(Martix[i][j] == tar){
                int v = getLeastValue(i, j);
                temp.push_back(rec(i, j, v));
            }
        }
    }
    Record = temp;
}

int main(){
    cin >> n >> m >> k;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            cin >> Martix[i][j];
        }
    }

    cin >> SearchPath;

    char start = SearchPath[0];
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            if(Martix[i][j] == start){
                Record.push_back(rec(i, j));
            }
        }
    }

    for (int i = 1; i < k; i++){
        calcuValue(SearchPath[i]);
    }

    int ans = INT_MAX;
    for(rec it : Record){
        if(ans > it.val){
            ans = it.val;
        }
    }
    cout << ans << endl;
    return 0;
}
