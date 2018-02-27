#include <climits>
#include "vertex.h"
#include <iostream>

/** Creates an unvisited vertex, gives it a label, and clears its
    adjacency list.
    NOTE: A vertex must have a unique label that cannot be changed. */
Vertex::Vertex(std::string label) : vertexLabel(std::move(label)) {
    // Since label is passed by value, to prevent multiple copies, std::move it
}

/** @return  The label of this vertex. */
std::string Vertex::getLabel() const {
    return vertexLabel;
}

/** Marks this vertex as visited. */
void Vertex::visit() {
    visited = true;
}

/** Marks this vertex as not visited. */
void Vertex::unvisit() {
    visited = false;
}

/** Returns the visited status of this vertex.
    @return  True if the vertex has been visited, otherwise returns false */
bool Vertex::isVisited() const {
    return visited;
}

/** Adds an edge between this vertex and the given vertex.
    Cannot have multiple connections to the same endVertex
    Cannot connect back to itself
    @return  True if the connection is successful. */
bool Vertex::connect(const std::string &endVertex,
                     const int edgeWeight) {

    // Check to make sure it hasn't been added already? If so return false 
    // and don't do anything.
    if (adjacencyList.count(endVertex) != 0)
        return false;

    // Connect the vertices
    adjacencyList.insert(std::pair<std::string, Edge>(
            endVertex, Edge(endVertex, edgeWeight)));

    // ASSUMPTION: I assume that there will be no connecting of new Vertices
    // in the middle of a traversal process.
    resetNeighbor();

    return true;
}

/** Gets the weight of the edge between this vertex and the given vertex.
    @return  The edge weight. This value is zero for an unweighted graph and
    is negative if the .edge does not exist */
int Vertex::getEdgeWeight(const std::string &endVertex) const {
    return (*adjacencyList.find(endVertex)).second.getWeight();
}

/** Calculates how many neighbors this vertex has.
    @return  The number of the vertex's neighbors. */
int Vertex::getNumberOfNeighbors() const {
    return static_cast<int>(adjacencyList.size());
}

/** Sets current neighbor to first in adjacency list. */
void Vertex::resetNeighbor() {
    currentNeighbor = adjacencyList.begin();
}

void Vertex::printAdjacencyList() const {
    std::cout << "Start of adjacency list" << std::endl;
    for (const auto &current : adjacencyList) {
        std::cout << "[" << current.first << "]" << std::endl;
    }
    std::cout << "End of adjacency list" << std::endl;
}

/** Gets this vertex's next neighbor in the adjacency list.
    Neighbors are automatically sorted alphabetically via map
    Returns the vertex label if there are no more neighbors
    @return  The label of the vertex's next neighbor. */
std::string Vertex::getNextNeighbor() {

    std::string retVal;

    // As long as there are neighbors at all, and the currentNeighbor 
    // iterator isn't already at the end...
    if (getNumberOfNeighbors() > 0 &&
        currentNeighbor != adjacencyList.end()) {

        retVal = currentNeighbor->first;
        ++currentNeighbor;

    } else {
        retVal = "";
    }

    return retVal;
}

/** Sees whether this vertex is equal to another one.
    Two vertices are equal if they have the same label. */
bool Vertex::operator==(const Vertex &rightHandItem) const {
    return this->vertexLabel == rightHandItem.vertexLabel;
}

/** Sees whether this vertex is < another one.
    Compares vertexLabel. */
bool Vertex::operator<(const Vertex &rightHandItem) const {
    return this->vertexLabel < rightHandItem.vertexLabel;
}
