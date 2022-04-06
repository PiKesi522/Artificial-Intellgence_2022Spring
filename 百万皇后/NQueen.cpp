#include <bits/stdc++.h>
using namespace std;
#define TIMES 3 //重复试验的次数
#define ll long long
unsigned ll RandSeed = (unsigned)time(NULL);
ll N;
ll *queen, *pdiag, *cdiag, *pdiag1, *cdiag1;

unsigned ll random(ll start = 0, ll end = 100){ //一个可以产出很大数值的随机函数，不用细看
    unsigned ll x;
    double i;

    x = 0x7fffffff;
    x += 1;
    RandSeed *= ((unsigned ll)134775813);
    RandSeed += 1;
    RandSeed = RandSeed % x;
    i = ((double)RandSeed) / (double)0x7fffffff;
    if (end > start)
        return start + (unsigned ll)((end - start) * i);
    else
        return end;
}

void swap(ll a, ll b, int flag){
    ll t; //临时变量，用来存放queen[a]
    //先把a,b两行的皇后拿起，对应的四个对角线（主副各两条）上的皇后数减一
    pdiag[queen[a] - a + N - 1]--;
    cdiag[queen[a] + a]--;
    pdiag[queen[b] - b + N - 1]--;
    cdiag[queen[b] + b]--;
    if (flag == 0){                     //当第a行与第b行交换之后，发现交换后所在的主/副对角线上已经被前几行的皇后占用了，就回退
        pdiag1[queen[a] - a + N - 1]--; //用来记录第0~a-1行皇后所占用的主对角线，1代表被占用
        cdiag1[queen[a] + a]--;         //用来记录第0~a-1行皇后所占用的副对角线
    }

    //交换两个皇后位置
    t = queen[a];
    queen[a] = queen[b];
    queen[b] = t;

    //交换完成后把两个皇后对应的四条对角线上的棋子数加一
    pdiag[queen[a] - a + N - 1]++;
    cdiag[queen[a] + a]++;
    pdiag[queen[b] - b + N - 1]++;
    cdiag[queen[b] + b]++;

    if (flag == 1){ //打乱顺序时，先让第a行尝试与第b行交换皇后顺序
        pdiag1[queen[a] - a + N - 1]++;
        cdiag1[queen[a] + a]++;
    }
}

int collisions(ll k){ //若某个棋子所在的主副对角线上的棋子数都为1，则它跟其他棋子无冲突
    return pdiag[queen[k] - k + N - 1] > 1 || cdiag[queen[k] + k] > 1;
}

ll search(){
    ll i, j, m;
    ll x = N * 3.08; //确认迭代次数，3.08为网上找的较优系数

    //把每行的皇后都充分交换
    for (j = 0, i = 0; i < x && j < N; i++){
        m = random(j, N);
        swap(j, m, 1);
        if (pdiag1[queen[j] - j + N - 1] > 1 || cdiag1[queen[j] + j] > 1) //说明这条主/副对角线已经被前几行的皇后占用了
            swap(j, m, 0);                                                //发现与前几行的皇后冲突后，要换回去
        else
            j++;
    }
    for (i = j; i < N; i++){ //要保证每一行的皇后都和其他皇后交换过位置
        m = random(i, N);
        swap(i, m, 1);
    }
    return N - j; //记录下了交换的最后几行
}

void finalSearch(ll k){ // k等于上面那个函数search的返回值，即最后k行没有好好交换的行
    ll b;
    for (ll i = N - k - 1; i < N; i++){
        ll c = 0;
        if (collisions(i)){ //如果这一行的皇后跟其他皇后冲突了，则要进行调整
            do{
                ll j = random(0, N);
                c++;
                swap(i, j, 2);
                b = (collisions(i) || collisions(j)); //若这一行的皇后不与其他皇后冲突，则可以开始调整下一行
                if (b)
                    swap(i, j, 2);
                if (c == 7000){       //如果超过7000次还没找到合适的位置，说明很大概率是前N-k行的摆放位置不行
                    k = search(); //重新排列一次
                    i = N - k - 1;
                    break;
                }
            } while (b);
        }
    }
}

int main(){
    cin >> N;

    if (N == 1){
        cout << 0 << endl;
        return 0;
    }

    queen = (ll *)malloc(sizeof(ll) * N);
    pdiag = (ll *)malloc(sizeof(ll) * (2 * N - 1));
    cdiag = (ll *)malloc(sizeof(ll) * (2 * N - 1));
    pdiag1 = (ll *)malloc(sizeof(ll) * (2 * N - 1));
    cdiag1 = (ll *)malloc(sizeof(ll) * (2 * N - 1));

    for (ll i = 0; i < 2 * N - 1; i++){
        pdiag[i] = 0;
        cdiag[i] = 0;
        pdiag1[i] = 0;
        cdiag1[i] = 0;
    }

    for (ll i = 0; i < N; i++){ //先把所有皇后放在主对角线上
        queen[i] = i;   //放好皇后 记录其对应的主副对角线上棋子的数量
        pdiag[queen[i] - i + N - 1]++;
        cdiag[queen[i] + i]++;
    }

    finalSearch(search());

    for (int i = 0; i < N; i++)
        cout << queen[i] << "\n";

    free(queen);
    free(pdiag);
    free(cdiag);
    free(pdiag1);
    free(cdiag1);
    return 0;
}