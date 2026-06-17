#include <iostream>
#include <vector>
#include <queue>
using namespace std;

struct Edge {
    int to, weight;
};

int main() {
    int n, m;
    cin >> n >> m;

    vector<vector<Edge>> graph(n + 1);

    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;

        graph[u].push_back({v, w});
        graph[v].push_back({u, w});
    }

    vector<bool> visited(n + 1, false);

    priority_queue<
        pair<int,int>,
        vector<pair<int,int>>,
        greater<pair<int,int>>
    > pq;

    pq.push({0, 1});

    int mstWeight = 0;

    while (!pq.empty()) {
        auto [w, u] = pq.top();
        pq.pop();

        if (visited[u]) continue;

        visited[u] = true;
        mstWeight += w;

        for (auto e : graph[u]) {
            if (!visited[e.to]) {
                pq.push({e.weight, e.to});
            }
        }
    }

    cout << "Tong trong so MST = "
         << mstWeight << endl;

    return 0;
}