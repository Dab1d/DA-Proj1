//
// Created by leo_o on 29/03/2026.
//

#ifndef RISKANALYZER_H
#define RISKANALYZER_H
#pragma once
#include <vector>
#include "structs/Graph.h"
#include "io/DataLoader.h"
#include "logic/NetworkBuilder.h"
#include "algorithms/MaxFlow.h"

class RiskAnalyzer {
public:
    /**
     * @brief Risk Analysis K=1: for each reviewer, checks if removing them
     *        still allows a feasible assignment.
     * @complexity O(R * (R*S + V*E^2)) where R=reviewers, S=submissions
     * @return list of reviewer IDs whose absence makes assignment infeasible
     */
    static std::vector<int> analyzeK1(const LoadedConferenceData& data) {
        std::vector<int> riskyReviewers;
        int R = data.reviewers.size();
        int S = data.submissions.size();
        int required = S * data.parameters.minReviewsPerSubmission;

        for (int i = 0; i < R; i++) {
            // Build network excluding reviewer i
            Graph<int> g = NetworkBuilder::buildExcluding(data, i);
            double flow = MaxFlow::run(g, 0, 1);

            if ((int)flow < required) {
                riskyReviewers.push_back(data.reviewers[i].id);
            }
        }
        return riskyReviewers;
    }

    /**
     * @brief Risk Analysis K>1: checks all combinations of K reviewers.
     * @complexity O(C(R,K) * (R*S + V*E^2)) — exponential in K
     * @return list of reviewer ID combinations whose absence makes assignment infeasible
     */
    static std::vector<std::vector<int>> analyzeK(const LoadedConferenceData& data, int K) {
        std::vector<std::vector<int>> riskyCombinations;
        int R = data.reviewers.size();
        int S = data.submissions.size();
        int required = S * data.parameters.minReviewsPerSubmission;

        // Generate all combinations of K reviewers
        std::vector<int> indices(R);
        std::iota(indices.begin(), indices.end(), 0);

        std::vector<int> combo(K);
        combinationsHelper(data, indices, K, 0, combo, required, riskyCombinations);

        return riskyCombinations;
    }

private:
    static void combinationsHelper(
        const LoadedConferenceData& data,
        const std::vector<int>& indices,
        int K, int start,
        std::vector<int>& combo,
        int required,
        std::vector<std::vector<int>>& result)
    {
        if (K == 0) {
            // Build network excluding all reviewers in combo
            Graph<int> g = NetworkBuilder::buildExcludingSet(data, combo);
            double flow = MaxFlow::run(g, 0, 1);
            if ((int)flow < required) {
                std::vector<int> ids;
                for (int idx : combo)
                    ids.push_back(data.reviewers[idx].id);
                result.push_back(ids);
            }
            return;
        }
        for (int i = start; i <= (int)indices.size() - K; i++) {
            combo[combo.size() - K] = indices[i];
            combinationsHelper(data, indices, K-1, i+1, combo, required, result);
        }
    }
};
#endif //RISKANALYZER_H
