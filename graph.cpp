#include <queue>
#include <set>
#include <iostream>
#include <fstream>
#include <map>
#include <stack>

#include "graph.h"

Graph::Graph() {
    numberOfVertices = 0;
    numberOfEdges = 0;
}

Graph::~Graph() {
    // For each Vertex (which were dynamically allocated, delete them.
    for (const auto &it : vertices)
        delete it.second;
}

int Graph::getNumVertices() const {
    return numberOfVertices;
}

int Graph::getNumEdges() const {
    return numberOfEdges;
}

bool Graph::add(std::string start, std::string end,
                int edgeWeight) {

    // Pointer is necessary to connect start to end.
    Vertex *vStart = findOrCreateVertex(start);

    // We don't need a pointer for the end vertex because we have its unique
    // label to find later.
    findOrCreateVertex(end);

    // This Should always be false no matter what the user does, but if 
    // findOrCreateVertex() gets changed and has a bug, this should catch that
    // bug
    if (vStart == nullptr)
        return false;

    // Connect the two vertices with an new edge
    vStart->connect(end, edgeWeight);
    numberOfEdges++;

    return true;
}

void Graph::readFile(std::string filename) {

    // Open the file containing the the adjacency list
    std::ifstream input(filename);

    // Declare temp variables to get reused for each new line.
    // ASSUMPTION: each line has 3 and only 3 values.
    // ASSUMPTION: each line contains 2 strings, separated by a space and an
    //             integer, separated by another space.
    std::string fromVertex, toVertex;
    int edgeWeight;

    // For each 3 values (or lines) add a new connection on the graph
    while (input >> fromVertex >> toVertex >> edgeWeight)
        add(fromVertex, toVertex, edgeWeight);
}

void Graph::depthFirstTraversal(std::string startLabel,
                                void visit(const std::string &)) {

    // Mark all nodes as unvisited
    unvisitAllVertices();

    depthFirstTraversalHelper(findVertex(startLabel), visit);
}

void Graph::depthFirstTraversalHelper(Vertex *startVertex,
                                      void visit(const std::string &)) {

    // ASSUMPTION: startVertex != nullptr
    if (startVertex == nullptr) {
        std::cout << "ERROR! startVertex does not exist" << std::endl;
        return;
    }

    startVertex->visit();
    visit(startVertex->getLabel());

    // For each neighbor of startVertex, go recursively deeper into the next
    // alphabetically sorted neighbor as long as the vertex is unvisited
    for (int i = 0; i < startVertex->getNumberOfNeighbors(); i++) {
        const auto nextNeighbor = startVertex->getNextNeighbor();
        Vertex *nextVertex = findVertex(nextNeighbor);
        if (!nextVertex->isVisited()) {
            depthFirstTraversalHelper(nextVertex, visit);
        }
    }
}

void Graph::breadthFirstTraversal(std::string startLabel,
                                  void visit(const std::string &)) {
    // Mark all nodes as unvisited
    unvisitAllVertices();

    // Start a queue since BFS works as a queue, not a stack
    std::queue<Vertex *> queue;

    // Find the startVertex
    Vertex *startVertex = findVertex(startLabel);

    // ASSUMPTION: startVertex != nullptr
    if (startVertex == nullptr) {
        std::cout << "ERROR! startVertex does not exist" << std::endl;
        return;
    }

    // Enqeueue startVertex to the end of the queue
    queue.push(startVertex);

    // Visit the startVertex
    startVertex->visit();
    visit(startVertex->getLabel());

    while (!queue.empty()) {

        // Capture the oldest vertex for use in this iteration, then 
        // dequeue it from the queue.
        Vertex *currentV = queue.front();
        queue.pop();

        // For each neighbor of the current vertex...
        for (int i = 0; i < currentV->getNumberOfNeighbors(); i++) {
            std::string currentNeighborStr = currentV->getNextNeighbor();
            Vertex *currentNeighborV = findVertex(currentNeighborStr);

            // If the next neighbor vertex is not visited, visit and enqueue it
            if (!currentNeighborV->isVisited()) {
                currentNeighborV->visit();
                visit(currentNeighborV->getLabel());
                queue.push(currentNeighborV);
            }
        }
    }
}

// Defining a convenience word PAIR because "std::pair<std::string, int>" is 
// used a lot below.
typedef std::pair<std::string, int> PAIR;

// Used for telling the priority queue in the dijkstraCostToAllVertices 
// function to sort using the second value in the pair and in ascending order.
struct CompareBySecond {
    bool operator()(PAIR pair1, PAIR pair2) const {
        return pair1.second > pair2.second;
    }
};

void Graph::dijkstraCostToAllVertices(
        std::string startLabel,
        std::map<std::string, int> &weight,
        std::map<std::string, std::string> &previous) {

    unvisitAllVertices();

    // Start with a blank slate. Make sure both weight and previous are empty.
    weight.erase(weight.begin(), weight.end());
    previous.erase(previous.begin(), previous.end());

    // Start a priority queue of PAIRs sorted in ascending order in a vector
    // container.
    std::priority_queue<PAIR, std::vector<PAIR>, CompareBySecond> pq;

    // vertexSet keeps track of which vertices this function has traversed
    std::map<std::string, Vertex *> vertexSet;

    Vertex *startVertex = findVertex(startLabel);

    // For each neighbor of startVertex, record the weight and the previous 
    // vertex (startVertex) to weight and previous maps using the label of 
    // the neighbor as the unique identifier. Then add each neighbor to the 
    // priority queue to explore later.
    for (int i = 0; i < startVertex->getNumberOfNeighbors(); i++) {
        std::string u = startVertex->getNextNeighbor();
        weight[u] = startVertex->getEdgeWeight(u);
        previous[u] = startLabel;
        pq.push(std::make_pair(u, weight[u]));
    }

    // Before traversing other vertices, add startVertex to the vertexSet so
    // that it doesn't get traverse, causing an infinite loop.
    vertexSet[startLabel] = startVertex;

    // Repeat this until there are no more potentially faster routes to explore
    while (!pq.empty()) {

        // Grab the next alphabetically sorted vertex in ascending order
        std::string v = pq.top().first;
        pq.pop(); // We have v now, so we don't need top() anymore. pop() now

        Vertex *pVertex = findVertex(v);

        // If the next vertex v is not stored in vertexSet...
        if (vertexSet.count(v) == 0) {

            // For each neighbor u of vertex v...
            for (int i = 0; i < pVertex->getNumberOfNeighbors(); i++) {
                std::string u = pVertex->getNextNeighbor();

                // Get edge cost to go from v to u
                int v2uCost = pVertex->getEdgeWeight(u);

                // We could not get to u before and this is the only path
                if (weight.count(u) == 0) {
                    // We could not get to u before
                    // This is the only path
                    weight[u] = weight[v] + v2uCost;
                    previous[u] = v;
                    pq.push(std::make_pair(u, weight[u]));
                }
                else {
                    // We could get to u before
                    // Is going via v better?
                    if (weight.at(u) > weight.at(v) + v2uCost) {
                        // Yes, it's better
                        weight[u] = weight[v] + v2uCost;
                        previous[u] = v;
                        pq.push(std::make_pair(u, weight[u]));
                    }
                }
            }
        }
    } // end of outermost while loop

    // Make sure that startVertex doesn't show up in either weight or previous
    weight.erase(startLabel);
    previous.erase(startLabel);
}

void Graph::unvisitAllVertices() {

    // For each vertex in vertices map, mark it as unvisited
    for (const auto &vertex : vertices) {
        vertex.second->unvisit();
        vertex.second->resetNeighbor();
    }
}

Vertex *Graph::findVertex(const std::string &vertexLabel) const {

    if (!vertexLabel.empty()) {
        // For each vertex in vertices map
        for (const auto &vertex : vertices) {
            if (vertexLabel == vertex.first) {
                return vertex.second;
            }
        }
    }

    // Went through the whole graph and found nothing.
    return nullptr;
}

Vertex *Graph::findOrCreateVertex(const std::string &vertexLabel) {

    // Try to find if the target Vertex exists yet
    Vertex *retVal = findVertex(vertexLabel);

    // Allocate new Vertex object if target does not exist yet
    if (retVal == nullptr) {
        retVal = new Vertex(vertexLabel);
        vertices.insert(std::pair<std::string, Vertex *>(vertexLabel, retVal));
        numberOfVertices++;
    }

    return retVal;
}
