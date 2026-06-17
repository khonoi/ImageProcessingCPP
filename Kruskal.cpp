#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Edge {
    int u, v, w;
};

vector<int> parent, sz;

int Find(int x) {
    if (parent[x] == x) return x;
    return parent[x] = Find(parent[x]);
}

bool Union(int a, int b) {
    a = Find(a);
    b = Find(b);

    if (a == b) return false;

    if (sz[a] < sz[b]) swap(a, b);

    parent[b] = a;
    sz[a] += sz[b];

    return true;
}

int main() {
    int n, m;
    cin >> n >> m;

    vector<Edge> edges(m);

    for (int i = 0; i < m; i++) {
        cin >> edges[i].u >> edges[i].v >> edges[i].w;
    }

    sort(edges.begin(), edges.end(),
         [](Edge a, Edge b) {
             return a.w < b.w;
         });

    parent.resize(n + 1);
    sz.assign(n + 1, 1);

    for (int i = 1; i <= n; i++)
        parent[i] = i;

    int mstWeight = 0;

    vector<Edge> mst;

    for (auto e : edges) {
        if (Union(e.u, e.v)) {
            mst.push_back(e);
            mstWeight += e.w;
        }
    }

    cout << "Tong trong so MST = "
         << mstWeight << endl;

    cout << "Cac canh trong MST:\n";

    for (auto e : mst)
        cout << e.u << " "
             << e.v << " "
             << e.w << endl;

    return 0;
}