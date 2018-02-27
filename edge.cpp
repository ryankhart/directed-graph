#include <string>
#include <iostream>

#include "edge.h"

Edge::Edge() = default;

/** constructor with label and weight */
Edge::Edge(const std::string &end, int weight) : endVertex(end),
                                                 edgeWeight(weight) {
}

/** return the weight/cost of travlleing via this edge */
int Edge::getWeight() const {
    return edgeWeight;
}
