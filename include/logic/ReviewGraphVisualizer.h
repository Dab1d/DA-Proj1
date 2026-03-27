#ifndef INC_1PROJ_REVIEWGRAPHVISUALIZER_H
#define INC_1PROJ_REVIEWGRAPHVISUALIZER_H

#include "io/DataLoader.h"

/**
 * @brief Handles CLI actions related to compatibility-graph visualization.
 */
class ReviewGraphVisualizer {
public:
    /**
     * @brief Exports the currently loaded graph to a DOT file.
     * @param data Loaded conference instance.
     * @time_complexity O(R * S * V + V + E), where R is the number of
     * reviewers, S is the number of submissions, V is the number of vertices,
     * and E is the number of edges.
     */
    static void exportGraph(const LoadedConferenceData &data);
};

#endif // INC_1PROJ_REVIEWGRAPHVISUALIZER_H
