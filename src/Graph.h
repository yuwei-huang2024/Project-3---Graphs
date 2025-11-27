#pragma once
#include <vector>
#include <string>
using namespace std;

class Edge {
    int from;
    int to;
    int time;
    bool closed = false; //default is opened
public:
    Edge(int gFrom, int gTo, int gTime);
    int getTo();
    int getTime();
    bool getClosed();
    void toggleClosed();
};

class pathTracker {
    vector<int> distance;
    vector<string> parent;
};

class Graph {
    vector<vector<Edge>> graph;
public:
    Graph();
    bool addEdge(int from, int to, int time);
    bool toggleEdge(int from, int to);
    string checkEdge(int from, int to);
    bool isConnected(int from, int to);
    int printShortestEdges(int from, int to);
    int zoneCalc(int residenceId, const vector<int>& classes) const;
};
