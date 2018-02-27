#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#include "graph.h"

using namespace std;

ostringstream graphOut; // NOLINT

map<string, int> weight;
map<string, string> previous;

template<typename T>
string isOK(const T &got, const T &expected) {
    if (got == expected) {
        return "SUCCESS: ";
    } else {
        cout << "    Got   " << got << "\n expected " << expected << endl;
        return "ERROR: ";
    }
}

void graphVisitor(const string &anItem) {
    graphOut << anItem << " ";
}

void graphCostDisplayPath(const string &vertex) {
    vector<string> v;
    auto prevIt = previous.find(vertex);
    while (prevIt != previous.end()) {
        v.push_back(prevIt->second);
        prevIt = previous.find(prevIt->second);
    }
    auto size = static_cast<int>(v.size());
    if (size <= 1) {
        return;
    }
    graphOut << "via [";
    for (int i{size - 2}; i > 0; --i)
        graphOut << v[i] << " ";
    if (size > 0)
        graphOut << v[0];
    graphOut << "] ";
}

void graphCostDisplay() {
    graphOut.str("");
    for (const auto &it: weight) {
        int cost = it.second;
        if (cost == INT_MAX)
            continue;
        string vertex = it.first;
        graphOut << vertex << "(" << cost << ") ";
        graphCostDisplayPath(vertex);
    }
}

void testGraph0() {
    cout << "testGraph0" << endl;
    Graph g;
    g.readFile("graph0.txt");
    cout << isOK(g.getNumVertices(), 3) << "3 vertices" << endl;
    cout << isOK(g.getNumEdges(), 3) << "3 edges" << endl;

    graphOut.str("");
    g.depthFirstTraversal("A", graphVisitor);
    cout << isOK(graphOut.str(), "A B C "s) << "DFS" << endl;

    graphOut.str("");
    g.breadthFirstTraversal("A", graphVisitor);
    cout << isOK(graphOut.str(), "A B C "s) << "BFS" << endl;

    g.dijkstraCostToAllVertices("A", weight, previous);
    graphCostDisplay();
    cout << isOK(graphOut.str(), "B(1) C(4) via [B] "s)
         << "Djisktra" << endl;
}

void testGraph1() {
    cout << "testGraph1" << endl;
    Graph g;
    g.readFile("graph1.txt");
    cout << isOK(g.getNumVertices(), 10) << "10 vertices" << endl;
    cout << isOK(g.getNumEdges(), 9) << "9 edges" << endl;

    graphOut.str("");
    g.depthFirstTraversal("A", graphVisitor);
    cout << isOK(graphOut.str(), "A B C D E F G H "s) << "DFS" << endl;

    graphOut.str("");
    g.breadthFirstTraversal("A", graphVisitor);
    cout << isOK(graphOut.str(), "A B H C G D E F "s) << "BFS" << endl;

    g.dijkstraCostToAllVertices("A", weight, previous);
    graphCostDisplay();
    cout << isOK(graphOut.str(),
                 "B(1) C(2) via [B] "s +
                 "D(3) via [B C] E(4) via [B C D] "s +
                 "F(5) via [B C D E] " +
                 "G(4) via [H] "s +
                 "H(3) "s)
         << "Djisktra" << endl;
}

void testGraph2() {
    cout << "testGraph2" << endl;
    Graph g;

    g.readFile("graph2.txt");
    cout << isOK(g.getNumVertices(), 21) << "21 vertices" << endl;
    cout << isOK(g.getNumEdges(), 24) << "24 edges" << endl;

    graphOut.str("");
    g.depthFirstTraversal("A", graphVisitor);
    cout << isOK(graphOut.str(), "A B E F J C G K L D H M I N "s)
         << "DFS from A" << endl;

    graphOut.str("");
    g.depthFirstTraversal("O", graphVisitor);
    cout << isOK(graphOut.str(), "O P R S T U Q "s)
         << "DFS from O" << endl;

    graphOut.str("");
    g.breadthFirstTraversal("A", graphVisitor);
    cout << isOK(graphOut.str(), "A B C D E F G H I J K L M N "s)
         << "BFS from A" << endl;

    graphOut.str("");
    g.breadthFirstTraversal("D", graphVisitor);
    cout << isOK(graphOut.str(), "D H I M N "s)
         << "BFS from D" << endl;

    graphOut.str("");
    g.depthFirstTraversal("U", graphVisitor);
    cout << isOK(graphOut.str(), "U "s)
         << "DFS from U" << endl;

    graphOut.str("");
    g.breadthFirstTraversal("U", graphVisitor);
    cout << isOK(graphOut.str(), "U "s)
         << "BFS from U" << endl;

    g.dijkstraCostToAllVertices("O", weight, previous);
    graphCostDisplay();
    cout << isOK(graphOut.str(),
                 "P(5) Q(2) R(3) via [Q] S(6) via [Q R] " +
                 "T(8) via [Q R S] U(9) via [Q R S] "s)
         << "Djisktra O" << endl;
}

void ryanTest0() {
    cout << "ryanTest0" << endl;
    Vertex v("A");
    cout << v.getNextNeighbor() << endl;
    cout << v.getNumberOfNeighbors() << endl;
    cout << "printAdjacencyList() (below)" << endl;
    v.printAdjacencyList();
}

void ryanTest1() {
    cout << "ryanTest0" << endl;
    Graph g;

    graphOut.str("");
    g.depthFirstTraversal("O", graphVisitor);
    cout << isOK(graphOut.str(), ""s) << "DFS from O" << endl;

    graphOut.str("");
    g.breadthFirstTraversal("A", graphVisitor);
    cout << isOK(graphOut.str(), ""s) << "BFS from A" << endl;
}

void ryanTest2() {
    cout << "ryanTest2" << endl;
    Graph g;

    g.readFile("graph2.txt");
    g.dijkstraCostToAllVertices("O", weight, previous);
    cout << "Frome O to..." << endl;
    for (auto it : weight) {
        cout << it.first << " " << it.second << endl;
    }
}

void ryanTest3() {
    cout << "ryanTest3" << endl;
    Graph g;

    g.add("F", "B", 2);
//    g.add("B", "F", 0);

    g.dijkstraCostToAllVertices("F", weight, previous);
    graphCostDisplay();
    cout << isOK(graphOut.str(),
                 "B(2) "s)
         << "Djisktra O" << endl;
}

void ryanTest4() {
    cout << "ryanTest4" << endl;
    Graph g;

    g.add("A", "B");
    g.add("B", "C");

}

int main() {
    testGraph0();
    testGraph1();
    testGraph2();
    ryanTest0();
    ryanTest1();
    cout << "Errors above are expected." << endl;
    ryanTest2();
    ryanTest3();
    ryanTest4();
    return 0;
}