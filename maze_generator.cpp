#include <vector>
#include <iostream>
#include <string>
#include <cstdlib>
#include <time.h>
#include <random>
#include <algorithm> 
#include <set> 

using namespace std;

vector<string> G;
vector<int> v;
int N;

int find(int a) {
    if (v[a] != a)    return v[a] = find(v[a]);
    return a;
}

void unions(int a, int b) {
    v[find(b)] = find(a);
}

bool canRemove(int i) {
    if (i % N == 0 || i % N == N - 1 || i / N == 0 || i / N == N - 1)
        return false;

    vector<int> v;
    if (G[i / N + 1][i % N] == ' ') 
        v.push_back(i + N);
    if (G[i / N - 1][i % N] == ' ') 
        v.push_back(i - N);
    if (G[i / N][i % N + 1] == ' ') 
        v.push_back(i + 1);
    if (G[i / N][i % N - 1] == ' ') 
        v.push_back(i - 1);

    if (v.size() <= 1){
        if (v.size() == 1) unions(v[0], i);
        return true;
    }
    sort(v.begin(), v.end());
    if (find(v.front()) == find(v.back()))   return false;

    for (int n : v){
        unions(n, i);
    }

    return true;
}

void generate() {
    G.resize(N, string(N, '#'));
    v.resize(N * N);

    for (int i = 0; i < N * N; i++) {
        v[i] = i;
    }
    int count = 0;

    vector<int> perm(N * N);

    for (int i = 0; i < N * N; i++) perm[i] = i;

    shuffle(perm.begin(), perm.end(), default_random_engine(rand()));

    for (int i : perm) {
        if (canRemove(i)) {
            G[i / N][i % N] = ' ';
        }
    }

    for (int i = 0; i < N; i++) {
        cout << G[i];
        cout << endl;
    }

}

int main() {
    srand(time(NULL));
    N = 40;
    generate();
    cout << endl;
}
