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
#include <unordered_set>
#include <numeric>
#include <utility>
#include <vector>

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
     * @brief Constructs the flow network using all reviewers.
     * @param data Loaded conference instance.
     * @return Flow network with source, sink, reviewer nodes, and submission
     * nodes connected according to primary-domain compatibility.
     * @complexity O(R * S) where R = reviewers, S = submissions
     */
    static Graph<int> build(const LoadedConferenceData&data ){
      return buildExcludingSet(data,{});
    }

    /**
     * @brief Builds the network excluding one reviewer by index.
     * @param data Loaded conference instance.
     * @param excludeIdx Reviewer index in `data.reviewers` to exclude.
     * @return Flow network without the selected reviewer node capacity.
     * @complexity O(R * S) where R = reviewers, S = submissions
     */
    static Graph<int> buildExcluding(const LoadedConferenceData& data, int excludeIdx) {
        std::vector<int> excluded = {excludeIdx};
        return buildExcludingSet(data, excluded);
    }

    /**
     * @brief Builds the network excluding a set of reviewers by index.
     * @param data Loaded conference instance.
     * @param excludeIndices Reviewer indices in `data.reviewers` to exclude.
     * @return Flow network without the selected reviewers.
     * @complexity O(R * S) where R = reviewers, S = submissions
     */
    static Graph<int> buildExcludingSet(const LoadedConferenceData& data,
                                         const std::vector<int>& excludeIndices) {
        int R = data.reviewers.size();
        int S = data.submissions.size();
        int totalNodes = 2 + R + S;

        Graph<int> g;
        for (int i = 0; i < totalNodes; i++)
            g.addVertex(i);

        int source = 0, sink = 1;
        int maxRev = data.parameters.maxReviewsPerReviewer;
        int minSub = data.parameters.minReviewsPerSubmission;

        std::unordered_set<int> excluded(excludeIndices.begin(), excludeIndices.end());

        for (int i = 0; i < R; i++) {
            if (excluded.count(i)) continue; // skip excluded reviewers

            MaxFlow::addResidualEdge(g, source, 2 + i, maxRev);

            for (int j = 0; j < S; j++) {
                if (hasValidMatch(data.submissions[j], data.reviewers[i],
                                  data.parameters.generateAssignments)) {
                    MaxFlow::addResidualEdge(g, 2 + i, 2 + R + j, 1.0);
                }
            }
        }

        for (int j = 0; j < S; j++)
            MaxFlow::addResidualEdge(g, 2 + R + j, sink, minSub);

        return g;
    }

private:
    static std::vector<int> submissionDomains(const Submission& submission, int mode) {
        std::vector<int> domains;
        if (submission.primaryTopic != -1) domains.push_back(submission.primaryTopic);
        if (mode >= 2 && submission.secondaryTopic != -1) domains.push_back(submission.secondaryTopic);
        return domains;
    }

    static std::vector<int> reviewerDomains(const Reviewer& reviewer, int mode) {
        std::vector<int> domains;
        if (reviewer.primaryExpertise != -1) domains.push_back(reviewer.primaryExpertise);
        if (mode >= 3 && reviewer.secondaryExpertise != -1) domains.push_back(reviewer.secondaryExpertise);
        return domains;
    }

    static bool hasValidMatch(const Submission& submission,
                              const Reviewer& reviewer,
                              int mode) {
        if (mode <= 0) mode = 1;

        const std::vector<int> submissionTopics = submissionDomains(submission, mode);
        const std::vector<int> reviewerTopics = reviewerDomains(reviewer, mode);

        for (int submissionTopic : submissionTopics)
            for (int reviewerTopic : reviewerTopics)
                if (submissionTopic == reviewerTopic)
                    return true;

        return false;
    }
};
#endif //NETWORKBUILDER_J_H
