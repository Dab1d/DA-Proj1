#pragma once
#include "structs/Graph.h"

/**
 * @brief Implements the Edmonds-Karp algorithm (Ford-Fulkerson with BFS)
 *        on the provided Graph structure.
 *
 * Time complexity: O(V * E^2)
 */
class MaxFlow {
public:
    /**
     * @brief Adds a directed edge with capacity w and its residual reverse edge
     *        (capacity 0). Sets the reverse pointers on both edges.
     *
     * Use this instead of addEdge() when building a flow network.
     * @complexity O(1)
     */
    template <class T>
    static void addResidualEdge(Graph<T>& g, const T& src, const T& dst, double capacity) {
        auto v1 = g.findVertex(src);
        auto v2 = g.findVertex(dst);
        if (!v1 || !v2) return;

        Edge<T>* forward  = v1->addEdge(v2, capacity);
        Edge<T>* backward = v2->addEdge(v1, 0);

        forward->setReverse(backward);
        backward->setReverse(forward);

        forward->setFlow(0);
        backward->setFlow(0);
    }

    /**
     * @brief Runs Edmonds-Karp to compute the maximum flow from source to sink.
     *
     * Repeatedly finds augmenting paths using BFS on the residual graph
     * and augments flow until no more paths exist.
     *
     * @note Time complexity: O(V * E^2) — BFS is O(V+E), and there are at most
     * O(V*E) augmenting paths before the algorithm terminates.
     *
     * @return Total max flow value
     */
    template <class T>
    static double run(Graph<T>& g, const T& source, const T& sink) {
        // Reset all flows
        for (auto v : g.getVertexSet())
            for (auto e : v->getAdj())
                e->setFlow(0);

        double totalFlow = 0;
        double pushed;

        while ((pushed = bfs(g, source, sink)) > 0) {
            totalFlow += pushed;
        }

        return totalFlow;
    }

private:
    /**
     * @brief BFS on the residual graph to find an augmenting path.
     *        If found, augments flow along that path.
     *
     * Residual capacity of edge e = e.weight - e.flow
     * For the reverse edge: its weight is 0, so residual = -e_forward.flow
     *
     * @note Time complexity: O(V + E)
     * @return Bottleneck flow of the path found, or 0 if no path exists.
     */
    template <class T>
    static double bfs(Graph<T>& g, const T& source, const T& sink) {
        // Reset visited state
        for (auto v : g.getVertexSet()) {
            v->setVisited(false);
            v->setPath(nullptr);
        }

        Vertex<T>* src = g.findVertex(source);
        Vertex<T>* snk = g.findVertex(sink);
        if (!src || !snk) return 0;

        std::queue<Vertex<T>*> q;
        src->setVisited(true);
        q.push(src);

        while (!q.empty()) {
            Vertex<T>* curr = q.front(); q.pop();

            for (Edge<T>* e : curr->getAdj()) {
                double residual = e->getWeight() - e->getFlow();
                Vertex<T>* next = e->getDest();

                if (!next->isVisited() && residual > 0) {
                    next->setVisited(true);
                    next->setPath(e);   // path[v] = edge used to reach v
                    if (next == snk) {
                        // Reconstruct path and compute bottleneck
                        return augment(src, snk);
                    }
                    q.push(next);
                }
            }
        }
        return 0; // no augmenting path
    }

    /**
     * @brief Traces back the path from sink to source using path pointers,
     *        finds the bottleneck, and updates flow on all edges.
     * @note Time complexity: O(V)
     */
    template <class T>
    static double augment(Vertex<T>* src, Vertex<T>* snk) {
        // Find bottleneck
        double bottleneck = INF;
        Vertex<T>* curr = snk;
        while (curr != src) {
            Edge<T>* e = curr->getPath();
            bottleneck = std::min(bottleneck, e->getWeight() - e->getFlow());
            curr = e->getOrig();
        }

        // Update flows
        curr = snk;
        while (curr != src) {
            Edge<T>* e = curr->getPath();
            e->setFlow(e->getFlow() + bottleneck);
            e->getReverse()->setFlow(e->getReverse()->getFlow() - bottleneck);
            curr = e->getOrig();
        }

        return bottleneck;
    }
};
