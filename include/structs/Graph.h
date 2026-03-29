// Original code by Gonçalo Leão
// Updated by DA 2024/2025 Team

#ifndef DA_TP_CLASSES_GRAPH
#define DA_TP_CLASSES_GRAPH

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include "MutablePriorityQueue.h"

template <class T>
class Edge;

#define INF std::numeric_limits<double>::max()

/************************* Vertex  **************************/

template <class T>
class Vertex {
public:
    /** @brief Creates a vertex storing the given value. */
    Vertex(T in);
    /** @brief Compares vertices by distance for priority-queue ordering. */
    bool operator<(Vertex<T> & vertex) const; // // required by MutablePriorityQueue

    /** @brief Returns the value stored in the vertex. */
    T getInfo() const;
    /** @brief Returns the list of outgoing edges. */
    std::vector<Edge<T> *> getAdj() const;
    /** @brief Returns whether the vertex was marked as visited. */
    bool isVisited() const;
    /** @brief Returns whether the vertex is currently being processed. */
    bool isProcessing() const;
    /** @brief Returns the stored indegree value. */
    unsigned int getIndegree() const;
    /** @brief Returns the current distance label. */
    double getDist() const;
    /** @brief Returns the path edge used to reach this vertex. */
    Edge<T> *getPath() const;
    /** @brief Returns the list of incoming edges. */
    std::vector<Edge<T> *> getIncoming() const;

    /** @brief Updates the value stored in the vertex. */
    void setInfo(T info);
    /** @brief Updates the visited flag. */
    void setVisited(bool visited);
    /** @brief Updates the processing flag. */
    void setProcessing(bool processing);

    /** @brief Returns the Tarjan low-link value. */
    int getLow() const;
    /** @brief Updates the Tarjan low-link value. */
    void setLow(int value);
    /** @brief Returns the Tarjan discovery number. */
    int getNum() const;
    /** @brief Updates the Tarjan discovery number. */
    void setNum(int value);

    /** @brief Updates the indegree value. */
    void setIndegree(unsigned int indegree);
    /** @brief Updates the distance label. */
    void setDist(double dist);
    /** @brief Updates the predecessor path edge. */
    void setPath(Edge<T> *path);
    /**
     * @brief Adds one outgoing edge to the destination vertex.
     * @param dest Destination vertex.
     * @param w Edge weight or capacity.
     * @return Pointer to the newly created edge.
     */
    Edge<T> * addEdge(Vertex<T> *dest, double w);
    /**
     * @brief Removes all outgoing edges to a destination value.
     * @param in Destination vertex value.
     * @return `true` if at least one edge was removed.
     */
    bool removeEdge(T in);
    /** @brief Removes all outgoing edges from the vertex. */
    void removeOutgoingEdges();

    friend class MutablePriorityQueue<Vertex>;
protected:
    T info;                // info node
    std::vector<Edge<T> *> adj;  // outgoing edges

    // auxiliary fields
    bool visited = false; // used by DFS, BFS, Prim ...
    bool processing = false; // used by isDAG (in addition to the visited attribute)
    int low = -1, num = -1; // used by SCC Tarjan
    unsigned int indegree; // used by topsort
    double dist = 0;
    Edge<T> *path = nullptr;

    std::vector<Edge<T> *> incoming; // incoming edges

    int queueIndex = 0; 		// required by MutablePriorityQueue and UFDS

    void deleteEdge(Edge<T> *edge);
};

/********************** Edge  ****************************/

template <class T>
class Edge {
public:
    /** @brief Creates an edge from origin to destination with weight `w`. */
    Edge(Vertex<T> *orig, Vertex<T> *dest, double w);

    /** @brief Returns the destination vertex. */
    Vertex<T> * getDest() const;
    /** @brief Returns the edge weight or capacity. */
    double getWeight() const;
    /** @brief Returns whether the edge is currently selected. */
    bool isSelected() const;
    /** @brief Returns the origin vertex. */
    Vertex<T> * getOrig() const;
    /** @brief Returns the reverse edge, when one exists. */
    Edge<T> *getReverse() const;
    /** @brief Returns the current flow value on the edge. */
    double getFlow() const;

    /** @brief Updates the selected flag. */
    void setSelected(bool selected);
    /** @brief Sets the reverse edge pointer. */
    void setReverse(Edge<T> *reverse);
    /** @brief Updates the current flow value. */
    void setFlow(double flow);
protected:
    Vertex<T> * dest; // destination vertex
    double weight; // edge weight, can also be used for capacity

    // auxiliary fields
    bool selected = false;

    // used for bidirectional edges
    Vertex<T> *orig;
    Edge<T> *reverse = nullptr;

    double flow; // for flow-related problems
};

/********************** Graph  ****************************/

template <class T>
class Graph {
public:
    /** @brief Destroys the graph and auxiliary matrices. */
    ~Graph();
    /**
     * @brief Finds a vertex by its stored value.
     * @param in Vertex value to search for.
     * @return Matching vertex pointer, or `nullptr` if absent.
     */
    Vertex<T> *findVertex(const T &in) const;
    /**
     * @brief Adds a new vertex to the graph.
     * @param in Vertex value to insert.
     * @return `true` if inserted, `false` if that value already exists.
     */
    bool addVertex(const T &in);
    /**
     * @brief Removes a vertex and all incident edges.
     * @param in Vertex value to remove.
     * @return `true` if the vertex existed and was removed.
     */
    bool removeVertex(const T &in);

    /**
     * @brief Adds a directed edge between two existing vertices.
     * @param sourc Source vertex value.
     * @param dest Destination vertex value.
     * @param w Edge weight or capacity.
     * @return `true` if both vertices exist and the edge was added.
     */
    bool addEdge(const T &sourc, const T &dest, double w);
    /**
     * @brief Removes a directed edge between two vertices.
     * @param source Source vertex value.
     * @param dest Destination vertex value.
     * @return `true` if the edge existed and was removed.
     */
    bool removeEdge(const T &source, const T &dest);
    /**
     * @brief Adds two symmetric directed edges between the given vertices.
     * @param sourc First vertex value.
     * @param dest Second vertex value.
     * @param w Edge weight or capacity for both directions.
     * @return `true` if both vertices exist and the edges were added.
     */
    bool addBidirectionalEdge(const T &sourc, const T &dest, double w);

    /** @brief Returns the number of vertices in the graph. */
    int getNumVertex() const;
    /** @brief Returns the internal vertex container. */
    std::vector<Vertex<T> *> getVertexSet() const;

protected:
    std::vector<Vertex<T> *> vertexSet;    // vertex set

    double ** distMatrix = nullptr;   // dist matrix for Floyd-Warshall
    int **pathMatrix = nullptr;   // path matrix for Floyd-Warshall

    /**
     * @brief Finds the index of a vertex by value.
     * @param in Vertex value to search for.
     * @return Zero-based index, or `-1` if absent.
     */
    int findVertexIdx(const T &in) const;
};

void deleteMatrix(int **m, int n);
void deleteMatrix(double **m, int n);


/************************* Vertex  **************************/

template <class T>
Vertex<T>::Vertex(T in): info(in) {}
template <class T>
Edge<T> * Vertex<T>::addEdge(Vertex<T> *d, double w) {
    auto newEdge = new Edge<T>(this, d, w);
    adj.push_back(newEdge);
    d->incoming.push_back(newEdge);
    return newEdge;
}

template <class T>
bool Vertex<T>::removeEdge(T in) {
    bool removedEdge = false;
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge<T> *edge = *it;
        Vertex<T> *dest = edge->getDest();
        if (dest->getInfo() == in) {
            it = adj.erase(it);
            deleteEdge(edge);
            removedEdge = true; // allows for multiple edges to connect the same pair of vertices (multigraph)
        }
        else {
            it++;
        }
    }
    return removedEdge;
}

template <class T>
void Vertex<T>::removeOutgoingEdges() {
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge<T> *edge = *it;
        it = adj.erase(it);
        deleteEdge(edge);
    }
}

template <class T>
bool Vertex<T>::operator<(Vertex<T> & vertex) const {
    return this->dist < vertex.dist;
}

template <class T>
T Vertex<T>::getInfo() const {
    return this->info;
}

template <class T>
int Vertex<T>::getLow() const {
    return this->low;
}

template <class T>
void Vertex<T>::setLow(int value) {
    this->low = value;
}

template <class T>
int Vertex<T>::getNum() const {
    return this->num;
}

template <class T>
void Vertex<T>::setNum(int value) {
    this->num = value;
}

template <class T>
std::vector<Edge<T>*> Vertex<T>::getAdj() const {
    return this->adj;
}

template <class T>
bool Vertex<T>::isVisited() const {
    return this->visited;
}

template <class T>
bool Vertex<T>::isProcessing() const {
    return this->processing;
}

template <class T>
unsigned int Vertex<T>::getIndegree() const {
    return this->indegree;
}

template <class T>
double Vertex<T>::getDist() const {
    return this->dist;
}

template <class T>
Edge<T> *Vertex<T>::getPath() const {
    return this->path;
}

template <class T>
std::vector<Edge<T> *> Vertex<T>::getIncoming() const {
    return this->incoming;
}

template <class T>
void Vertex<T>::setInfo(T in) {
    this->info = in;
}

template <class T>
void Vertex<T>::setVisited(bool visited) {
    this->visited = visited;
}

template <class T>
void Vertex<T>::setProcessing(bool processing) {
    this->processing = processing;
}

template <class T>
void Vertex<T>::setIndegree(unsigned int indegree) {
    this->indegree = indegree;
}

template <class T>
void Vertex<T>::setDist(double dist) {
    this->dist = dist;
}

template <class T>
void Vertex<T>::setPath(Edge<T> *path) {
    this->path = path;
}

template <class T>
void Vertex<T>::deleteEdge(Edge<T> *edge) {
    Vertex<T> *dest = edge->getDest();
    // Remove the corresponding edge from the incoming list
    auto it = dest->incoming.begin();
    while (it != dest->incoming.end()) {
        if ((*it)->getOrig()->getInfo() == info) {
            it = dest->incoming.erase(it);
        }
        else {
            it++;
        }
    }
    delete edge;
}

/********************** Edge  ****************************/

template <class T>
Edge<T>::Edge(Vertex<T> *orig, Vertex<T> *dest, double w): orig(orig), dest(dest), weight(w) {}

template <class T>
Vertex<T> * Edge<T>::getDest() const {
    return this->dest;
}

template <class T>
double Edge<T>::getWeight() const {
    return this->weight;
}

template <class T>
Vertex<T> * Edge<T>::getOrig() const {
    return this->orig;
}

template <class T>
Edge<T> *Edge<T>::getReverse() const {
    return this->reverse;
}

template <class T>
bool Edge<T>::isSelected() const {
    return this->selected;
}

template <class T>
double Edge<T>::getFlow() const {
    return flow;
}

template <class T>
void Edge<T>::setSelected(bool selected) {
    this->selected = selected;
}

template <class T>
void Edge<T>::setReverse(Edge<T> *reverse) {
    this->reverse = reverse;
}

template <class T>
void Edge<T>::setFlow(double flow) {
    this->flow = flow;
}

/********************** Graph  ****************************/

template <class T>
int Graph<T>::getNumVertex() const {
    return vertexSet.size();
}

template <class T>
std::vector<Vertex<T> *> Graph<T>::getVertexSet() const {
    return vertexSet;
}

template <class T>
Vertex<T> * Graph<T>::findVertex(const T &in) const {
    for (auto v : vertexSet)
        if (v->getInfo() == in)
            return v;
    return nullptr;
}

template <class T>
int Graph<T>::findVertexIdx(const T &in) const {
    for (unsigned i = 0; i < vertexSet.size(); i++)
        if (vertexSet[i]->getInfo() == in)
            return i;
    return -1;
}
template <class T>
bool Graph<T>::addVertex(const T &in) {
    if (findVertex(in) != nullptr)
        return false;
    vertexSet.push_back(new Vertex<T>(in));
    return true;
}

template <class T>
bool Graph<T>::removeVertex(const T &in) {
    for (auto it = vertexSet.begin(); it != vertexSet.end(); it++) {
        if ((*it)->getInfo() == in) {
            auto v = *it;
            v->removeOutgoingEdges();
            for (auto u : vertexSet) {
                u->removeEdge(v->getInfo());
            }
            vertexSet.erase(it);
            delete v;
            return true;
        }
    }
    return false;
}

template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    v1->addEdge(v2, w);
    return true;
}

template <class T>
bool Graph<T>::removeEdge(const T &sourc, const T &dest) {
    Vertex<T> * srcVertex = findVertex(sourc);
    if (srcVertex == nullptr) {
        return false;
    }
    return srcVertex->removeEdge(dest);
}

template <class T>
bool Graph<T>::addBidirectionalEdge(const T &sourc, const T &dest, double w) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    auto e1 = v1->addEdge(v2, w);
    auto e2 = v2->addEdge(v1, w);
    e1->setReverse(e2);
    e2->setReverse(e1);
    return true;
}

inline void deleteMatrix(int **m, int n) {
    if (m != nullptr) {
        for (int i = 0; i < n; i++)
            if (m[i] != nullptr)
                delete [] m[i];
        delete [] m;
    }
}

inline void deleteMatrix(double **m, int n) {
    if (m != nullptr) {
        for (int i = 0; i < n; i++)
            if (m[i] != nullptr)
                delete [] m[i];
        delete [] m;
    }
}

template <class T>
Graph<T>::~Graph() {
    deleteMatrix(distMatrix, vertexSet.size());
    deleteMatrix(pathMatrix, vertexSet.size());
}

#endif /* DA_TP_CLASSES_GRAPH */
