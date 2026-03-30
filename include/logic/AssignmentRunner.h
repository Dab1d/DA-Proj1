//
// Created by leo_o on 29/03/2026.
//

#ifndef ASSIGNMENTRUNNER_H
#define ASSIGNMENTRUNNER_H
#pragma once
#include <iostream>
#include <fstream>
#include "structs/ConferenceData.h"
#include "io/DataLoader.h"
#include "io/AssignmentWriter.h"
#include "logic/DataActionUtils.h"
#include "logic/NetworkBuilder.h"
#include "algorithms/MaxFlow.h"
#include "algorithms/AssignmentExtractor.h"
#include "logic/RiskAnalyzer.h"

class AssignmentRunner {
public:
    static void run(const LoadedConferenceData& data) {
        if (!DataActionUtils::ensureDataLoaded(data)) return;

        // 1. Build network
        Graph<int> g = NetworkBuilder::build(data);

        // 2. Run Max Flow
        double flow = MaxFlow::run(g, 0, 1);

        int S = data.submissions.size();
        int required = S * data.parameters.minReviewsPerSubmission;

        std::cout << "\nMax flow computed: " << (int)flow
                  << " / " << required << " required\n";

        std::vector<Assignment> assignments;
        if (flow > 0)
            assignments = AssignmentExtractor::extract(g, data);

        if (data.parameters.generateAssignments > 0 && !assignments.empty()) {
            auto sorted = assignments;
            std::cout << "\n#SubmissionId,ReviewerId,Match\n";
            std::sort(sorted.begin(), sorted.end(), [](const Assignment& a, const Assignment& b){
                return a.submissionId < b.submissionId;
            });
            for (auto& a : sorted)
                std::cout << a.submissionId << ", " << a.reviewerId
                          << ", " << AssignmentWriter::formatMatch(a) << "\n";

            std::cout << "\n#ReviewerId,SubmissionId,Match\n";
            std::sort(sorted.begin(), sorted.end(), [](const Assignment& a, const Assignment& b){
                return a.reviewerId < b.reviewerId;
            });
            for (auto& a : sorted)
                std::cout << a.reviewerId << ", " << a.submissionId
                          << ", " << AssignmentWriter::formatMatch(a) << "\n";

            std::cout << "#Total: " << assignments.size() << "\n";

            // Write to file
            std::ofstream out(data.parameters.outputFileName);
            if (out.is_open())
                AssignmentWriter::writeAssignments(assignments, out);
        }

        // Always report missing if flow < required
        if ((int)flow < required) {
            auto missing = AssignmentExtractor::getMissing(g, data);
            std::cout << "\n#SubmissionId,Domain,MissingReviews\n";
            for (auto& r : missing)
                std::cout << r.submissionId << ", " << r.domain
                          << ", " << r.missingReviews << "\n";

            // Also append to file
            std::ofstream out(data.parameters.outputFileName, std::ios::app);
            if (out.is_open())
                AssignmentWriter::writeMissing(missing, out);
        }
        // Risk Analysis
        if (data.parameters.riskAnalysis > 0) {
            int K = data.parameters.riskAnalysis;
            std::cout << "\n#Risk Analysis: " << K << "\n";
            std::ofstream out(data.parameters.outputFileName, std::ios::app| std::ios::out);

            if (K == 1) {
                auto risky = RiskAnalyzer::analyzeK1(data);
                std::sort(risky.begin(), risky.end());

                // todos na mesma linha separados por vírgula
                for (int i = 0; i < (int)risky.size(); i++) {
                    if (i > 0) std::cout << ", ";
                    std::cout << risky[i];
                }
                if (!risky.empty()) std::cout << "\n";
                if (out.is_open())
                    AssignmentWriter::writeRiskAnalysis(risky, K, out);
            } else {
                auto riskyCombos = RiskAnalyzer::analyzeK(data, K);
                for (auto& combo : riskyCombos) {
                    for (int i = 0; i < (int)combo.size(); i++) {
                        if (i > 0) std::cout << ", ";
                        std::cout << combo[i];
                    }
                    std::cout << "\n";
                }
                if (out.is_open())
                    AssignmentWriter::writeRiskAnalysisCombinations(riskyCombos, K, out);
            }
        }
    }
};
#endif //ASSIGNMENTRUNNER_H
