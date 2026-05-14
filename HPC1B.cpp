%%writefile hpc1b.cpp

#include <iostream>
#include <vector>
#include <stack>
#include <omp.h>
using namespace std;

const int MAX = 100000;
vector<int> graph[MAX];
bool visited[MAX];

void dfs(int start) {
    stack<int> s;
    s.push(start);

    while (!s.empty()) {
        int node = s.top();
        s.pop();

        if (!visited[node]) {
            visited[node] = true;
            cout << node << " ";

            #pragma omp parallel for
            for (int i = 0; i < graph[node].size(); i++) {
                int adj = graph[node][i];
                if (!visited[adj]) {
                    #pragma omp critical
                    s.push(adj);
                }
            }
        }
    }
}

int main() {
    int n, m, start;
    cout << "Enter number of nodes, edges, and start node: ";
    cin >> n >> m >> start;

    cout << "Enter pairs of edges:\n";
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    #pragma omp parallel for
    for (int i = 0; i < n; i++)
        visited[i] = false;

    double begin = omp_get_wtime();
    dfs(start);
    double end = omp_get_wtime();

    cout << "\nExecution Time: " << (end - begin) << " seconds\n";
    return 0;
}

//command:
// !g++ hpc1b.cpp -fopenmp -o hpc1b
// !./hpc1b
