#include "Graph.h"
#include <algorithm>
#include <limits>
#include <queue>
#include <vector>
#include <unordered_set>

using namespace std;

Edge::Edge(int gFrom, int gTo, int gTime) {
    from = gFrom;
    to = gTo;
    time = gTime;
}
int Edge::getTo() const {
    return to;
}
int Edge::getTime() const {
    return time;
}
bool Edge::getClosed() const {
    return closed;
}

void Edge::toggleClosed() {
    closed = !closed;
}

Graph::Graph() {
    vector<vector<Edge>> graph;
}

bool Graph::addEdge(int from, int to, int time) {
    int size = max(from, to);
    if (size >= static_cast<int>(graph.size())) {
        graph.resize(size + 1);
    }
    graph[from].emplace_back(from, to, time);
    graph[to].emplace_back(to, from, time);
    return true;
}

bool Graph::toggleEdge(int from, int to) {
    for (Edge& e : graph[from]) {
        if (e.getTo() == to) {
            e.toggleClosed();
        }
    }
    for (Edge& e : graph[to]) {
        if (e.getTo() == from) {
            e.toggleClosed();
        }
    }
    return true;
}

string Graph::checkEdge(int from, int to) {
    for (Edge& e : graph[from]) {
        if (e.getTo() == to) {
            if (e.getClosed()) {
                return "closed";
            }
            return "open";
        }
    }
    return "DNE";
}

//for me later, initialize dist and predecessor
bool Graph::dijkstra(int src, vector<int>& dist, vector<int>& predecessor) {
    int n = graph.size();
    if (src < 0 || src >= n) {
        return false;
    }

    const int infinity = numeric_limits<int>::max();

    dist.assign(n, infinity);
    predecessor.assign(n, -1);

    using Node = pair<int, int>;
    priority_queue<Node, vector<Node>, greater<Node>> pq;

    dist[src] = 0;
    pq.push({0, src});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d > dist[u]) {
            continue;
        }
        for (Edge& e : graph[u]) {
            if (!e.getClosed()) {
                int v = e.getTo();
                int w = e.getTime();

                if (dist[u] != infinity && dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    predecessor[v] = u;
                    pq.push({dist[v], v});
                }
            }
        }
    }
    return true;
}

bool Graph::isConnected(int from, int to) {
    const int infinity = numeric_limits<int>::max();
    vector<int> dist, predecessor;
    dijkstra(from, dist, predecessor);
    if (dist[to] != infinity) {
        return true;
    }
    return false;
}

int Graph::printShortestEdges(int from, int to) {
    const int infinity = numeric_limits<int>::max();
    vector<int> dist, predecessor;
    dijkstra(from, dist, predecessor);

    if (dist[to] != infinity) {
        return dist[to];
    }
    return -1;
}

int Graph::MST(unordered_set<int>& vertices) {
    int n = graph.size();
    vector<bool> inZone(n, false);
    for (int v : vertices) {
        if (v >= 0 && v < n) {
            inZone[v] = true;
        }
    }

    int needed = 0;
    int start = -1;
    for (int i = 0; i < n; ++i) {
        if (inZone[i]) {
            ++needed;
            if (start == -1) start = i;
        }
    }

    vector<bool> inMST(n, false);
    using Node = pair<int, int>;
    priority_queue<Node, vector<Node>, greater<Node>> pq;
    pq.push(make_pair(0, start));

    int totalCost = 0;
    int visitedCount = 0;

    // recursive minimum tree, works logically like bfs
    while (!pq.empty() && visitedCount < needed) {
        auto top = pq.top();
        pq.pop();
        int w = top.first;
        int u = top.second;

        if (!inZone[u] || inMST[u]) {
            continue;
        }

        inMST[u] = true;
        visitedCount++;
        totalCost += w;

        // add all neighbors here
        for (Edge &e : graph[u]) {
            int v = e.getTo();
            if (!e.getClosed() && inZone[v] && !inMST[v]) {
                pq.push(make_pair(e.getTime(), v));
            }
        }
    }
    return totalCost;
}

int Graph::zoneCalc(int residenceId, const vector<int>& classes) {
    vector<int> dist, prev;
    dijkstra(residenceId, dist, prev);
    unordered_set<int> nodes;
    nodes.insert(residenceId);
    for (int classLocation : classes) {
        int v = classLocation;
        while (v != residenceId) {
            int u = prev[v];
            nodes.insert(u);
            nodes.insert(v);
            v = u;
        }
    }

    int cost = MST(nodes);
    return cost;
}

//for testing
void Graph::printEdges() {

}