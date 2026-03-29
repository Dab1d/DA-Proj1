//
// Created by leo_o on 29/03/2026.
//

#ifndef NETWORKBUILDER_J_H
#define NETWORKBUILDER_J_H
#pragma once
#include "structs/Graph.h"
#include "algorithms/MaxFlow.h"
#include "structs/ConferenceData.h"
#include "io/DataLoader.h"

/**
 * @brief Builds the flow network from the loaded conference data.
 *
 * Node layout:
 *   0         = Source
 *   1         = Sink
 *   2 + i     = Reviewer i (index in data.reviewers)
 *   2 + R + j = Submission j (index in data.submissions)
 */
class NetworkBuilder {
public:
    /**
     * @brief Constructs the flow network for GenerateAssignments = 1
     *        (primary domains only).
     * @complexity O(R * S) where R = reviewers, S = submissions
     */
    static Graph<int> build(const LoadedConferenceData& data) {
        int R = data.reviewers.size();
        int S = data.submissions.size();
        int totalNodes = 2 + R + S;

        Graph<int> g;
        for (int i = 0; i < totalNodes; i++)
            g.addVertex(i);

        int source = 0, sink = 1;
        int maxRev = data.parameters.maxReviewsPerReviewer;
        int minSub = data.parameters.minReviewsPerSubmission;

        // Source → each reviewer
        for (int i = 0; i < R; i++)
            MaxFlow::addResidualEdge(g, source, 2 + i, maxRev);

        // Reviewer → Submission (if primary topics match)
        for (int i = 0; i < R; i++) {
            for (int j = 0; j < S; j++) {
                if (data.reviewers[i].primaryExpertise ==
                    data.submissions[j].primaryTopic) {
                    MaxFlow::addResidualEdge(g, 2 + i, 2 + R + j, 1.0);
                    }
            }
        }

        // Each submission → Sink
        for (int j = 0; j < S; j++)
            MaxFlow::addResidualEdge(g, 2 + R + j, sink, minSub);

        return g;
    }
};
#endif //NETWORKBUILDER_J_H
