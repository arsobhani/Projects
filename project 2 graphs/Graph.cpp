#include "Graph.h"
#include <list>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <queue>
#include <stack>
#include <unordered_set>

//uh...is structure not taken as a type?
/*
struct node {
    string neighbor;
    int weight;
}
*/

//two unordered maps??? hehehe
unordered_map<string, unordered_map<string, double>> graph;

Graph::Graph(const char* const & edgelist_csv_fn) {
    // TODO
    ifstream my_file(edgelist_csv_fn);
    string line;
    while(getline(my_file, line)) {
        istringstream ss(line);
        string first, second, third;
        getline(ss, first, ',');
        getline(ss, second, ',');
        getline(ss, third, '\n');
        //now my part
        //do both since the graph is unordered
        graph[first][second] = stod(third);
        graph[second][first] = stod(third);
    }
    my_file.close();
}

unsigned int Graph::num_nodes() {
    // TODO
    return graph.size();
}

vector<string> Graph::nodes() {
    // TODO
    vector<string> nodes;
    //I saw this on the documentation. But I should look at the video once more.
    for (auto it = graph.begin(); it != graph.end(); ++it ) {
        nodes.push_back(it->first);
    }
    return nodes;
}

unsigned int Graph::num_edges() {
    // TODO
    //Handshaking lemma????
    unsigned int numEdges = 0;
    for (auto it = graph.begin(); it != graph.end(); ++it ) {
        numEdges += (it->second.size());
    }
    return (numEdges / 2);
}

unsigned int Graph::num_neighbors(string const & node_label) {
    // TODO
    unsigned int numNeighbors = graph[node_label].size();
    return numNeighbors;
}

double Graph::edge_weight(string const & u_label, string const & v_label) {
    // TODO
    //both edges exist, need only one
    if (graph.count(u_label) == 0) {
        return -1;
    }
    if (graph[u_label].count(v_label) == 0) {
        return -1;
    }
    return graph[u_label][v_label];
}

vector<string> Graph::neighbors(string const & node_label) {
    // TODO
    vector<string> neighbors;
    for (auto it = graph[node_label].begin(); it != graph[node_label].end(); ++it ) {
        neighbors.push_back(it->first);
    }
    return neighbors;
}

//do i need deconstructor for this???
class nodePath {
    public:

    //unsigned int d;
    string node;
    nodePath * previous;

    nodePath(string s, nodePath * link) : node(s), previous(link) {
    }
};

vector<string> Graph::shortest_path_unweighted(string const & start_label, string const & end_label) {
    // TODO
    //the tuples are sorted by the first input?
    queue<nodePath *> nodeList;
    unordered_set<string> nodeCheck;
    stack<string> containsPath;
    //to return
    vector<string> path;
    //just to delete the used memory
    vector<nodePath *> toDelete;

    //case for path to itself
    if (start_label == end_label) {
        path.push_back(start_label);
        return path;
    }

    nodeList.push(new nodePath(start_label, nullptr));
    nodePath * finalNode = nullptr;
    //iterate through queue
    while (nodeList.empty() == false) {
        string currNode = nodeList.front()->node;
        vector<string> neighborList = neighbors(currNode);
        //check neighbors
        for (unsigned int i = 0; i < neighborList.size(); i++) {
            if (nodeCheck.count(neighborList[i]) != 0) {
            }
            else {
                //need to exit after finding the node...but for now naive
                if (end_label == neighborList[i]) {
                    nodeCheck.insert(neighborList[i]);
                    finalNode = (new nodePath(neighborList[i], nodeList.front()));
                    nodeList.push(finalNode);
                }
                else {
                    nodeCheck.insert(neighborList[i]);
                    nodeList.push(new nodePath(neighborList[i], nodeList.front()));
                }
            }
        }
        //memory is leaked...
        toDelete.push_back(nodeList.front());
        //nodePath * toDelete = nodeList.front();
        nodeList.pop();
        //deal with memory
        //delete toDelete;
    }
    //build the stack
    nodePath * currentNode = finalNode;
    while(currentNode != nullptr) {
        containsPath.push(currentNode->node);
        currentNode = currentNode->previous;
    }
    //reverse
    while(containsPath.empty() == false) {
        path.push_back(containsPath.top());
        containsPath.pop();
    }

    //maybe unnecessary
    //delete
    for (int i = (toDelete.size() - 1); i >= 0; i--) {
        delete toDelete[i];
    }

    return path;
}

//could have done the last one a bit easier I believe
vector<tuple<string,string,double>> Graph::shortest_path_weighted(string const & start_label, string const & end_label) {
    // TODO
    //to return
    vector<tuple<string, string, double>> weightedPath;
    //map for the previous string and the current weight to it
    unordered_map<string, tuple<double, string, bool>> bestPaths;
    //unordered_map<string, pair<double, string>> bestPaths;
    //queue for node and current weight to it
    //hope this works
    priority_queue<tuple<double, string>, vector<tuple<double, string>>, greater<tuple<double, string>>> nextNodes;
    //if the beginning and end are the same
    if (start_label == end_label) {
        weightedPath.push_back(make_tuple(start_label, start_label, -1));
        return weightedPath;
    }
    nextNodes.push(make_tuple(0, start_label));
    //now to start the algorithm
    vector<string> nodeList = nodes();
    for (unsigned int i = 0; i < nodeList.size(); i++) {
        //infinite length and no previous node
        //please no empty node strings
        bestPaths[nodeList[i]] = make_tuple(1.8446744e+19, "", false);
        //bestPaths[nodeList[i]] = make_pair(-1, "");
    }
    //a little fix
    bestPaths[start_label] = make_tuple(0, "", true);
    //until all nodes run out or the target node is achieved
    while ((nextNodes.empty() == false) && (get<2>(bestPaths[end_label]) == false)) {
        bestPaths[get<1>(nextNodes.top())] = 
            make_tuple(get<0>(bestPaths[get<1>(nextNodes.top())]), get<1>(bestPaths[get<1>(nextNodes.top())]), true);
        string currNode = get<1>(nextNodes.top());
        vector<string> neighborList = neighbors(currNode);

        double currLength = get<0>(nextNodes.top());
        string currString = get<1>(nextNodes.top());
        //check neighbors
        for (unsigned int i = 0; i < neighborList.size(); i++) {
            //i might not NEED to check for 'done', but it would
            //reduce runtime
            string neighbor = neighborList[i];
            double neighborLength = get<0>(bestPaths[neighbor]);
            //string neighborPrevious = get<1>(bestPaths[neighbor]);
            bool neighborStatus = get<2>(bestPaths[neighbor]);
            if (neighborLength <= (currLength
                + edge_weight(currString, neighbor))) {
            }
            else {
                //best.insert(neighborList[i]);
                bestPaths[neighbor] = make_tuple(currLength
                + edge_weight(currString, neighbor), currString, neighborStatus);
                //nodeList.push(new nodePath(neighborList[i], nodeList.front()));
                //push new path length
                nextNodes.push(make_tuple(get<0>(bestPaths[neighbor]), neighbor));
            }
        }
        //i need a done status so that the whole function can stop
        //much sooner
        nextNodes.pop();
    }

    stack<tuple<string, string, double>> toReverse;
    tuple<string, string, double> nodeStringCheck = make_tuple(get<1>(bestPaths[end_label]),
                                                end_label, edge_weight(get<1>(bestPaths[end_label]),
                                                          end_label));
    ///*                                      
    //reverse the tuples
    while (get<0>(nodeStringCheck) != "") {
        string firstNode = get<0>(nodeStringCheck);
        string toBeFirstNode = get<1>(bestPaths[firstNode]);
        double toBeEdgeWeight = edge_weight(toBeFirstNode, firstNode);
        toReverse.push(nodeStringCheck);
        nodeStringCheck = make_tuple(toBeFirstNode,
                        firstNode, toBeEdgeWeight);
    }
    while (toReverse.empty() == false) {
        weightedPath.push_back(toReverse.top());
        toReverse.pop();
    }
    //*/
    return weightedPath;

}

vector<vector<string>> Graph::connected_components(double const & threshold) {
    // TODO
    //to return
    vector<vector<string>> connectedComponents;
    unordered_map<string, bool> markedNodes;
    queue<string> connectedNodes;

    //now to start the algorithm
    vector<string> nodeList = nodes();
    //make all unmarked
    for (unsigned int i = 0; i < nodeList.size(); i++) {
        string node = nodeList[i];
        markedNodes[node] = false;
    }
    //let this for loop go through all nodes
    for (unsigned int i = 0; i < nodeList.size(); i++) {
        string startNode = nodeList[i];
        vector<string> connectedComponent;
        //check if it has been marked
        if (markedNodes[startNode] == true) {
        }
        else {
            connectedNodes.push(startNode);
            while (connectedNodes.empty() == false) {
                string currNode = connectedNodes.front();
                connectedComponent.push_back(currNode);
                markedNodes[currNode] = true;

                vector<string> nodeNeighbors = neighbors(currNode);
                for (unsigned int i = 0; i < nodeNeighbors.size(); i++) {
                    string neighbor = nodeNeighbors[i];
                    if (markedNodes[neighbor] == false) {
                        if (edge_weight(currNode, neighbor) <= threshold) {
                            connectedNodes.push(neighbor);
                        }
                    }
                }
                connectedNodes.pop();
            }
            connectedComponents.push_back(connectedComponent);
        }
    }
    return connectedComponents;
}


class DisjointSetNode {
    public:
    string node;
    double threshold;
    DisjointSetNode * parent;

    DisjointSetNode(string s, double d, DisjointSetNode * link) : node(s), threshold(d), parent(link) {
    }
};

    
void unionDisjointSet(double newThreshold, DisjointSetNode * x, DisjointSetNode * y) {
    DisjointSetNode * xcurrNode = x;
    DisjointSetNode * ycurrNode = y;
    while (xcurrNode->parent != nullptr) {
        xcurrNode = xcurrNode->parent;
    }
    while (ycurrNode->parent != nullptr) {
        ycurrNode = ycurrNode->parent;
    }
    //if they are in the same set
    if (xcurrNode == ycurrNode) {
        return;
    }
    //y becomes the set representative
    xcurrNode->parent = ycurrNode;
    ycurrNode->threshold = newThreshold;
}

DisjointSetNode* findDisjointSet(DisjointSetNode * z) {
    DisjointSetNode * zcurrNode = z;
    while (zcurrNode->parent != nullptr) {
        zcurrNode = zcurrNode->parent;
    }
    return zcurrNode;
}


double Graph::smallest_connecting_threshold(string const & start_label, string const & end_label) {
    // TODO
    //basic edge case I seemed to have an issue with without:
    if (start_label == end_label) {
        return 0;
    }
    //use a priority queue to arrange the edges?
    priority_queue<tuple<double, string, string>, vector<tuple<double, string, string>>, greater<tuple<double, string, string>>> edges;
    vector<string> nodeList = nodes();
    //create edge priority queue
    for (unsigned int i = 0; i < nodeList.size(); i++) {
        string node = nodeList[i];
        vector<string> nodeNeighbors = neighbors(node);
        for (unsigned int j = 0; j < nodeNeighbors.size(); j++) {
            string neighbor = nodeNeighbors[j];
            edges.push(make_tuple(edge_weight(node, neighbor), node, neighbor));
        }
    }

    //fill up with pointers
    unordered_map<string, DisjointSetNode *> disjointSetNodes;
    for (unsigned int i = 0; i < nodeList.size(); i++) {
        string node = nodeList[i];
        disjointSetNodes[node] = (new DisjointSetNode(node, 0, nullptr));
    }

    bool connected = false;
    //value to return if not changed/no path exists
    double finalThreshold = -1;
    //if the queue is empty then a path wasnt found
    while ((connected == false) && (edges.empty() == false)) {
        double inputThreshold = get<0>(edges.top());
        string first = get<1>(edges.top());
        string second = get<2>(edges.top());
        DisjointSetNode * a = disjointSetNodes[first];
        DisjointSetNode * b = disjointSetNodes[second];
        unionDisjointSet(inputThreshold, a, b);
        DisjointSetNode * startPointer = disjointSetNodes[start_label];
        DisjointSetNode * endPointer = disjointSetNodes[end_label];
        if (findDisjointSet(startPointer) == findDisjointSet(endPointer)) {
            finalThreshold = findDisjointSet(startPointer)->threshold;
            connected = true;
        }
        //forgot to pop
        edges.pop();
    }

    //delete allocated nodes
    for (unsigned int i = 0; i < disjointSetNodes.size(); i++) {
         string node = nodeList[i];
         delete disjointSetNodes[node];
    }

    return finalThreshold;

}
