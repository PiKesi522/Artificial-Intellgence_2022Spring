#include <bits/stdc++.h>
using namespace std;

// 每个数据或是整数或是浮点数，实际使用时每个数据仅其中一个值有效
struct Val
{
    int i;
    double d;
    Val(int _i): i(_i), d(0) {}
    Val(double _d): i(0), d(_d) {}
};

bool operator != (const Val& lhs, const Val& rhs)
{
    return lhs.i != rhs.i || lhs.d != rhs.d;
}

// 样本，包括每种属性及标签（1 或 -1）
struct Sample
{
    vector<Val> atr;
    int label;
    Sample(): atr(), label(0) {}
};

typedef vector<Sample> Data;

// 决策树节点
struct Tnode
{
    int label; // 为 0 说明不为叶节点；否则标识叶节点对应标签
    int atrno; // 该节点的划分属性的下标
    int tp; // 0 代表连续值；1代表离散值
    double par; // 若为连续值, 为划分点的值；若为离散值, 则无效
    vector<Tnode> son; // 该节点的所有子节点
    Tnode(): label(0), atrno(-1), tp(-1), par(0), son() {}
};

string readLine(istream& fin)
{
    string s;

    // 读入数据并输入到 stringstream
    if (fin.eof()) {
        return s;
    }
    getline(fin, s);
    for (int i = 0; i < s.length(); ++i) {
        if (s[i] == ',') s[i] = ' ';
    }
    return s;
}

void readData(Data& data_all, vector<int>& type_all)
{
    string s;
    stringstream ss;

    // 读入数据类型
    s = readLine(cin);
    ss << s;
    while (true) {
        int x;
        ss >> x;
        if (ss.fail()) break;
        type_all.push_back(x);
    }

    // 读入数据
    bool flag = true;
    while (flag) {
        string s;
        stringstream ss;
        s = readLine(cin);
        ss << s;
        Sample sp;
        for (int t : type_all) {
            if (t == 0) {
                double x;
                ss >> x;
                if (ss.fail()) {
                    flag = false;
                    break;
                }
                sp.atr.push_back(x);
            }
            else {
                int x;
                ss >> x;
                if (ss.fail()) {
                    flag = false;
                    break;
                }
                sp.atr.push_back(x);
            }
        }
        if (!flag) break;

        // 读取 label
        int x;
        ss >> x;
        if (ss.fail()) break;
        sp.label = x;
        data_all.push_back(sp);
    }
}

// 若数据集中所有样本有相同 label，返回该 label 值；否则返回 0
int getLabel(const Data& data_set)
{
    int label = data_set[0].label;
    for (int i = 1; i < data_set.size(); ++i) {
        if (data_set[i].label != label) {
            return 0;
        }
    }
    return label;
}

// 判断数据集中所有样本是否在 atr_set 中的每个属性上取值都相同
bool sameAtr(const Data& data_set, const vector<int>& atr_set)
{
    Sample sp = data_set[0];
    for (int i = 1; i < data_set.size(); ++i) {
        for (int atrno : atr_set) {
            if (sp.atr[atrno] != data_set[i].atr[atrno]) {
                return false;
            }
        }
    }
    return true;
}

// 计算数据集中样本数最多的类。若相同，随机取一个
int getMostLabel(const Data& data_set)
{
    map<int, int> mp;
    mp[-1] = 0;
    mp[1] = 0;
    for (auto sp : data_set) {
        mp[sp.label]++;
    }
    if (mp[-1] > mp[1]) return -1;
    if (mp[-1] < mp[1]) return 1;
    return rand() % 2 == 0 ? -1 : 1;
}

// 将数据集按某一属性分割，若为连续值则以 par 为分界点分割
vector<Data> splitData(const Data& data_set, int atrno, double par, const vector<int>& type_all)
{
    int tp = type_all[atrno];
    vector<Data> datas(2);

    if (tp == 0) {
        for (auto e : data_set) {
            if (e.atr[atrno].d < par) {
                datas[0].push_back(e);
            }
            else {
                datas[1].push_back(e);
            }
        }
    }
    else {
        for (auto e : data_set) {
            datas[e.atr[atrno].i].push_back(e);
        }
    }

    return datas;
}

/**
 * @brief 对决策树中一个节点的划分方案作出评估
 * 
 * @param label_group 当前待评估划分方案的标签。label_group[i][j] 为该划分方案第 i 组中的第 j 个标签。保证标签仅有 -1 和 1 两种取值。
 * @return 对划分的评估，数值越大代表该划分越优秀。
 */
double calcScore(const vector<vector<int> >& label_group){
    const int groupNum = label_group.size();
    vector<vector<int>> cnt;
    int totalNeg = 0, totalPos = 0, totalSam = 0;
    for (int i = 0; i < groupNum; i++){
        vector<int> temp;
        temp.push_back(0);
        temp.push_back(0);
        temp.push_back(0);
        vector<int> pointer = label_group[i];
        for (int j = 0; j < (int)label_group[i].size(); j++){
            if(pointer[j] == 1){
                temp[0] += 1;
                totalPos += 1;
            }else{
                temp[1] += 1;
                totalNeg += 1;
            }
            temp[2] += 1;
            totalSam += 1;
        }
        cnt.push_back(temp);
    }
    double ED = -1 * ((1.0 * totalPos / totalSam) * log2(1.0 * totalPos / totalSam) + 
                        (1.0 * totalNeg / totalSam) * log2(1.0 * totalNeg / totalSam));
    int tPos, tNeg, tSam;
    double IV = 0.0;
    for (int i = 0; i < groupNum; i++){
        tPos = cnt[i][0];
        tNeg = cnt[i][1];
        tSam = cnt[i][2];
        IV -= (1.0 * tSam / totalSam) * log2(1.0 * tSam / totalSam);

        if(tPos != 0 && tNeg != 0){
            double tD = -1 * ((1.0 * tPos / tSam) * log2(1.0 * tPos / tSam) + (1.0 * tNeg / tSam) * log2(1.0 * tNeg / tSam));
            ED -= (1.0 * tSam / totalSam) * tD;
        }
    }   
    return (ED / IV);
}

// 将 datas 中的 label 提取出来，调用 calcScore 获取得分
double getScore(const vector<Data>& datas)
{
    vector<vector<int> > label_group;
    for (const Data& dt : datas) {
        vector<int> labels;
        for (auto sp : dt) {
            labels.push_back(sp.label);
        }
        label_group.push_back(labels);
    }
    return calcScore(label_group);
}

// 获取某一划分的分数及划分点（若为连续变量）
pair<double, double> getScoreAndPar(const Data& data_set, int atrno, const vector<int>& type_all)
{
    if (type_all[atrno] == 0) {
        vector<double> points;
        for (auto dt : data_set) {
            points.push_back(dt.atr[atrno].d);
        }
        sort(points.begin(), points.end());
        points.push_back(points.back() + 1);
        vector<pair<double, double> > rets;
        for (int i = 0; i < points.size() - 1; ++i) {
            if (points[i] == points[i + 1]) continue;
            double par = (points[i] + points[i + 1]) / 2;
            vector<Data> datas = splitData(data_set, atrno, par, type_all);
            rets.push_back({getScore(datas), par});
        }
        assert(!rets.empty());
        sort(rets.begin(), rets.end(), greater<pair<double, double> >());
        while (rets.size() > 1 && rets.back().first != rets.front().first) {
            rets.pop_back();
        }
        int no = rand() % rets.size();
        return rets[no];
    }
    else {
        vector<Data> datas = splitData(data_set, atrno, 0, type_all);
        return {getScore(datas), 0};
    }
}

// 构建决策树
Tnode buildTree(const Data& data_set, const vector<int>& atr_set, const vector<int>& type_all)
{
    Tnode node;

    // 处理样本类别全部相同的情况
    node.label = getLabel(data_set);
    if (node.label) {
        // printf("*1*\n");
        return node;
    }

    // 处理属性集为空或所有样本在属性集上取值全部相同的情况
    if (atr_set.empty() || sameAtr(data_set, atr_set)) {
        // printf("*2*\n");
        node.label = getMostLabel(data_set);
        return node;
    }

    // 选择最优属性. 若有多个, 随机选择其中一个
    vector<pair<pair<double, double>, int> > v;
    for (auto atrno : atr_set) {
        v.push_back({getScoreAndPar(data_set, atrno, type_all), atrno});
    }

    sort(v.begin(), v.end(), greater<pair<pair<double, double>, int> >());
    while (v.size() > 1 && v.back().first.first != v.front().first.first) {
        v.pop_back();
    }

    int no = rand() % v.size();
    node.atrno = v[no].second;
    node.tp = type_all[node.atrno];
    node.par = v[no].first.second;

    // 生成分支
    vector<Data> datas = splitData(data_set, node.atrno, node.par, type_all);
    vector<int> new_atr_set;
    if (node.tp == 0) {
        new_atr_set = atr_set;
    }
    else {
        for (auto atrno : atr_set) {
            if (atrno != node.atrno) new_atr_set.push_back(atrno);
        }
    }
    for (int i = 0; i < 2; ++i) {
        if (datas[i].empty()) {
            Tnode nd;
            nd.label = getMostLabel(data_set);
            node.son.push_back(nd);
        }
        else {
            node.son.push_back(buildTree(datas[i], new_atr_set, type_all));
        }
    }

    return node;
}

void print(const Tnode& now, int tab = 0)
{
    putchar('~');
    for (int i = 0; i < tab; ++i) {
        putchar('|');
    }
    printf("(%d, %d, %d, %f)\n", now.label, now.atrno, now.tp, now.par);
    if (!now.label) {
        for (const Tnode& p : now.son) {
            print(p, tab + 1);
        }
    }
}

// 预测一个数据的值
int fit(const Tnode& root, const Sample& sample_in)
{
    if (root.label != 0) return root.label;
    if (root.tp == 1) return fit(root.son[sample_in.atr[root.atrno].i], sample_in);
    if (sample_in.atr[root.atrno].d < root.par) return fit(root.son[0], sample_in);
    return fit(root.son[1], sample_in);
}

// 返回一个vector，内容为对每个输入数据的预测
vector<int> predict(const Tnode& root, const Data& data_set)
{
    vector<int> ret;
    for (auto e : data_set) {
        ret.push_back(fit(root, e));
    }
    return ret;
}

double trainAndTest(const Data& data_train, const Data& data_test, const vector<int>& type_all)
{
    vector<int> atr_set;
    for (int i = 0; i < type_all.size(); ++i)
    {
        atr_set.push_back(i);
    }
    Tnode root = buildTree(data_train, atr_set, type_all);
    vector<int> pred = predict(root, data_test);

    int total_num = data_test.size();
    int correct = 0;
    for (int i = 0; i < total_num; ++i)
    {
        if (pred[i] == data_test[i].label)
            ++correct;
    }
    return double(correct) / total_num;
}

double crossValidate(const Data& data_all, const vector<int>& type_all, int folder_num)
{
    vector<Data> datas(2);
    for (auto dt : data_all) {
        if (dt.label == -1) datas[0].push_back(dt);
        else datas[1].push_back(dt);
    }
    for (Data& data : datas) {
        // random_shuffle(data.begin(), data.end());
    }
    vector<Data> data_folder;
    for (int i = 0; i < folder_num; ++i) {
        Data folder;
        for (const Data& data : datas) {
            int l = i * data.size() / folder_num;
            int r = (i + 1) * data.size() / folder_num;
            for (int i = l; i < r; ++i) {
                folder.push_back(data[i]);
            }
        }
        data_folder.push_back(folder);
    }
    double precision = 0;
    for (int i = 0; i < folder_num; ++i) {
        Data data_train, data_test;
        for (int j = 0; j < folder_num; ++j) {
            if (i == j) {
                data_test.insert(data_test.end(), data_folder[j].begin(), data_folder[j].end());
            }
            else {
                data_train.insert(data_train.end(), data_folder[j].begin(), data_folder[j].end());
            }
        }
        precision += trainAndTest(data_train, data_test, type_all) / folder_num;
    }
    return precision;
}

int main()
{
    Data data_all;
    vector<int> type_all;
    srand(19260817);

    readData(data_all, type_all);
    double precion = crossValidate(data_all, type_all, 10);
    // 输出密码，防止偷鸡行为
    printf("%.2f\n", precion * 100);
    return 0;
}