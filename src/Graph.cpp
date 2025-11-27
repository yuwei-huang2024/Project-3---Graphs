#include "Graph.h"
#include <algorithm>

Edge::Edge(int gFrom, int gTo, int gTime) {
    from = gFrom;
    to = gTo;
    time = gTime;
}
int Edge::getTo() {
    return to;
}
int Edge::getTime() {
    return time;
}
bool Edge::getClosed() {
    return closed;
}

void Edge::toggleClosed() {
    closed = !closed;
}

bool Graph::addEdge(int from, int to, int time) {
    int size = max(from, to);
    if (size >= graph.size()) {
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

bool Graph::isConnected(int from, int to) {
    return true;
}
int Graph::printShortestEdges(int from, int to) {
    return 0;
}
int Graph::zoneCalc(int residenceId, const vector<int>& classes) const {
    return 0;
}
