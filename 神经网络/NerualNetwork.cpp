#include <bits/stdc++.h>

using namespace std;

double mseLoss(double x, double target) {
    return 0.5 * (x - target) * (x - target);
}

struct NeuralNetwork {
    vector<double*> values, caches;
    //  value[0] -> x0, x1, x2 ... x12
    //  value[1] -> d1, d2, d3
    //  value[2] -> y

    vector<double**> weights;
    // weights[0] ->     b0  b1  b2
    //  (14 * 3)    x0
    //              x1
    //              x2
    //              ...
    //              x12
    //             x13(1)

    // weights[1] ->     y
    //  (3 * 1)     b0
    //              b1
    //             b2(1)
};

double sigmoid(double x){
    return 1 / (1 + exp(-x));
}

double dSigmoid(double f){
    return f * (1 - f);
}

void initValueAndCache(NeuralNetwork &network, double* data){
    vector<double *> temp;
    double *Input = (double *)malloc(sizeof(double) * 13);
    double *Hidden = (double *)malloc(sizeof(double) * 3);
    double *Output = (double *)malloc(sizeof(double) * 1);
    for (int i = 0; i < 13; i++){
        Input[i] = data[i];
    }
    memset(Hidden, 1, sizeof(double) * 3);
    Output[0] = data[14];
    temp.push_back(Input);
    temp.push_back(Hidden);
    temp.push_back(Output);
    network.values = temp;

    vector<double *> ttemp;
    double *g = (double *)malloc(sizeof(double) * 1);
    double *e = (double *)malloc(sizeof(double) * 3);
    memset(g, 0, sizeof(double) * 1);
    memset(e, 0, sizeof(double) * 3);
    ttemp.push_back(g);
    ttemp.push_back(e);
    network.caches = ttemp;
}

double forward(NeuralNetwork &network, double* data){
    double **weightInput2Hidden  = network.weights[0];
    double **weightHidden2Output = network.weights[1];
    initValueAndCache(network, data);
    double *inputValue = network.values[0];
    double *HiddenValue  = network.values[1];
    double *outputValue = network.values[2];

    double calcu;
    for (int i = 0; i < 2; i++){
        calcu = 0;
        for (int j = 0; j < 13; j++){
            calcu += inputValue[j] * weightInput2Hidden[j][i];
        }
        calcu -= 1 * weightInput2Hidden[13][i];
        HiddenValue[i] = sigmoid(calcu);
    }

    for (int i = 0; i < 1; i++){
        calcu = 0;
        for (int j = 0; j < 2; j++){
            calcu += HiddenValue[j] * weightHidden2Output[j][i];
        }
        calcu -= 1 * weightHidden2Output[2][i];
        outputValue[i] = sigmoid(calcu);
    }
    return outputValue[0];
}   

// output -> yHat, target -> y
void backward(NeuralNetwork &network, double output, double target){
    double **w = network.weights[1];
    double *b  = network.values[1]; 
    double *g  = network.caches[0];
    double *e  = network.caches[1];  
    for (int i = 0; i < 1; i++){
        g[i] = dSigmoid(output) * (target - output);
    }
    double sum;
    for (int i = 0; i < 3; i++){
        sum = 0;
        for (int j = 0; j < 1; j++){
            sum += w[i][j];
        }
        e[i] = b[i] * (1 - b[i]) * sum;
    }
}

void step(NeuralNetwork &network, double learning_rate){
    double **weightInput2Hidden  = network.weights[0];
    double **weightHidden2Output = network.weights[1];
    double *inputValue  = network.values[0];
    double *HiddenValue = network.values[1];
    double *outputValue = network.values[2];
    double *g = network.caches[0];
    double *e = network.caches[1];  

    free(outputValue);
    for (int i = 0; i < 1; i++){
        for (int j = 0; j < 2; j++){
            weightHidden2Output[j][i] += learning_rate * g[i] * HiddenValue[j];
        }
        weightHidden2Output[2][i] = -1 * learning_rate * g[i];
    }
    free(g);
    free(HiddenValue);

    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 13; j++){
            weightInput2Hidden[j][i] += learning_rate * e[i] * inputValue[j];
        }
        weightInput2Hidden[13][i] = -1 * learning_rate * e[i];
    }
    free(e);
    free(inputValue);
}

void standardize(double **data, int dataSize, int featureSize) {
    for (int j = 0; j <= featureSize; ++j) {
        double mean = 0;
        for (int i = 0; i < dataSize; ++i)
            mean += data[i][j] / dataSize;
        double variance = 0;
        for (int i = 0; i < dataSize; ++i)
            variance += (data[i][j] - mean) * (data[i][j] - mean) / (dataSize - 1);
        for (int i = 0; i < dataSize; ++i)
            data[i][j] = (data[i][j] - mean) / sqrt(variance);
    }
}

pair<vector<double*>, vector<double*>> make_dataset(int dataSize, int featureNum) {
    // data二维数组， 每一行有 （x + 1 + y）个数据， 作为一组数据
    double **data = (double**)malloc(dataSize * sizeof(double*));
    for (int i = 0; i < dataSize; ++i) {
        data[i] = (double*)malloc((featureNum + 2) * sizeof(double));
        data[i][0] = 0;
        for (int j = 0; j <= featureNum; ++j)
            scanf("%lf", &data[i][j]);
    }
    standardize(data, dataSize, featureNum);
    for (int i = 0; i < dataSize; ++i) {
        data[i][featureNum + 1] = data[i][featureNum];
        data[i][featureNum] = 1;    // 偏置项
    }

    int trainSize = 0.8 * dataSize;
    vector<int> index(dataSize);
    for (int i = 0; i < dataSize; ++i)
        index[i] = i;
    random_shuffle(index.begin(), index.end());
    vector<double*> train, valid;
    for (int i = 0; i < trainSize; ++i)
        train.push_back(data[index[i]]);
    for (int i = trainSize; i < dataSize; ++i)
        valid.push_back(data[index[i]]);
    return make_pair(train, valid);
}

NeuralNetwork initNeuralNetwork() {
    uniform_real_distribution<> random{-1, 1};
    default_random_engine eng{0};

    NeuralNetwork network{};
    double **weight = (double**)malloc(14 * sizeof(double*));
    for (int i = 0; i < 14; ++i) {
        weight[i] = (double*)malloc(3 * sizeof(double));
        for (int j = 0; j < 3; ++j)
            weight[i][j] = random(eng);
    }
    network.weights.push_back(weight);
    weight = (double**)malloc(3 * sizeof(double*));
    for (int i = 0; i < 3; ++i) {
        weight[i] = (double*)malloc(sizeof(double));
        for (int j = 0; j < 1; ++j)
            weight[i][j] = random(eng);
    }
    network.weights.push_back(weight);
    return network;
}

// 你的代码会被嵌入在这

double evaluate(NeuralNetwork &network, vector<double*> &valid, int featureNum) {
    double loss = 0;
    for (int i = 0; i < valid.size(); ++i) {
        double output = forward(network, valid[i]);
        loss += mseLoss(output, valid[i][featureNum + 1]);
    }
    return loss / valid.size();
}

int main() {
    int dataSize, featureNum;
    scanf("%d%d", &dataSize, &featureNum);

    auto pair = make_dataset(dataSize, featureNum);
    auto train = pair.first, valid = pair.second;
    uniform_int_distribution<int> random{0, (int)train.size() - 1};
    default_random_engine eng{0};

    auto network = initNeuralNetwork();
    for (int i = 0; i < 10000; ++i) {
        int idx = random(eng);
        double output = forward(network, train[idx]);
        backward(network, output, train[idx][featureNum + 1]);
        step(network, 0.01);
    }
    printf("%lf\n", evaluate(network, valid, featureNum));
    return 0;
}
