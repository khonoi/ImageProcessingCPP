#include <iostream>
#include <vector>
#include <queue>
#include <climits>
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

    int x, y;
    cin >> x >> y;

    vector<int> dist(n + 1, INT_MAX);
    vector<int> parent(n + 1, -1);

    priority_queue<
        pair<int,int>,
        vector<pair<int,int>>,
        greater<pair<int,int>>
    > pq;

    dist[x] = 0;
    pq.push({0, x});

    while (!pq.empty()) {
        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (d > dist[u]) continue;

        for (auto e : graph[u]) {
            int v = e.to;
            int w = e.weight;

            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    if (dist[y] == INT_MAX) {
        cout << "Khong ton tai duong di\n";
    } else {
        cout << "Do dai ngan nhat: " << dist[y] << endl;
    }

    return 0;
}