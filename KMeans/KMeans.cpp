#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

const double eps = 1e-6;

struct Point
{
    vector<double> coordinate;
    int groupId;
    /**
     * @brief 计算两个 Point 之间的欧氏距离的平方,
     *          要求两个 Point coordinate 长度相等
     */
    double distance(const Point &other) const
    {
        // 采用欧氏距离平方作为指标.
        double ans = 0;
        int sz = max(coordinate.size(), other.coordinate.size());
        for (int i = 0; i < sz; ++i)
            ans += (coordinate[i] - other.coordinate[i]) * (coordinate[i] - other.coordinate[i]);
        return ans;
    }
    /**
     * @brief 将另一个 Point 赋值给当前 Point
     */
    Point &operator=(const Point &other)
    {
        if (this == &other)
            return *this;
        coordinate = other.coordinate;
        groupId = other.groupId;
        return *this;
    }
    /**
     * @brief 按 coordinate 顺序比较两个 Point 的大小,
     *          要求两个 Point coordinate 长度相等
     */
    bool operator<(const Point &other) const
    {
        auto sz = max(coordinate.size(), other.coordinate.size());
        for (auto i = 0; i < sz; ++i)
            if (fabs(coordinate[i] - other.coordinate[i]) > eps)
                return coordinate[i] < other.coordinate[i];
        return false;
    }
};

vector<Point> update(vector<Point> &points, const vector<Point> &last)
{
    const int dimension = last[0].coordinate.size();

    vector<Point> ans(2);
    vector<vector<int>> tempClassify(2);

    Point A = last[0], B = last[1];
    for (int i = 0; i < points.size(); i++){
        int index = points[i].distance(A) < points[i].distance(B) ? 0 : 1;
        points[i].groupId = index;
        tempClassify[index].push_back(i);
    }


    for (int k = 0; k < 2; k++){
        ans[k].coordinate = vector<double>(dimension, 0);
        vector<int> thisClass = tempClassify[k];
        for (int j = 0; j < dimension; j++){
            for (int i = 0; i < thisClass.size(); i++){
                ans[k].coordinate[j] += points[thisClass[i]].coordinate[j];
            }
            ans[k].coordinate[j] /= thisClass.size();
        }
    }
    return ans;
}

bool vectorIsEqual(const vector<Point> &centers, const vector<Point> &temp)
{
    auto v1 = centers;
    sort(v1.begin(), v1.end());
    auto v2 = temp;
    sort(v2.begin(), v2.end());
    auto k = max(v1.size(), v2.size());
    for (auto i = 0; i < k; ++i)
        if (v1[i] < v2[i] || v2[i] < v1[i])
            return false;
    return true;
}

vector<Point> kMeans(vector<Point> &points, const int k)
{
    vector<Point> centers(k);
    vector<Point> temp;
    for (auto i = 0; i < k; ++i)
        centers[i] = points[i];
    do
    {
        temp = centers;
        centers = update(points, temp);
    } while (!vectorIsEqual(centers, temp));
    return centers;
}

double score(const vector<Point> &points, const vector<Point> &centers)
{
    auto n = points.size();
    auto ans = 0.0;
    for (auto i = 0; i < n; ++i)
        ans += points[i].distance(centers[points[i].groupId]);
    return ans;
}

int main(){
    int n, m;
    scanf("%d%d", &n, &m);
    vector<Point> points(n);
    for (int i = 0; i < n; ++i){
        points[i].coordinate.resize(m);
        for (int j = 0; j < m; ++j){
            scanf("%lf", &(points[i].coordinate[j]));
        }
    }
    auto centers = kMeans(points, 2);
    printf("%f\n", (double)score(points, centers));
    return 0;
}
