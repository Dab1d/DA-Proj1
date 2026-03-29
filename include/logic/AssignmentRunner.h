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

        // Always extract assignments if any flow exists
        if (flow > 0 && data.parameters.generateAssignments > 0) {
            auto assignments = AssignmentExtractor::extract(g, data);

            auto sorted = assignments;
            std::cout << "\n#SubmissionId,ReviewerId,Match\n";
            std::sort(sorted.begin(), sorted.end(), [](const Assignment& a, const Assignment& b){
                return a.submissionId < b.submissionId;
            });
            for (auto& a : sorted)
                std::cout << a.submissionId << ", " << a.reviewerId
                          << ", " << a.submissionDomain << "\n";

            std::cout << "\n#ReviewerId,SubmissionId,Match\n";
            std::sort(sorted.begin(), sorted.end(), [](const Assignment& a, const Assignment& b){
                return a.reviewerId < b.reviewerId;
            });
            for (auto& a : sorted)
                std::cout << a.reviewerId << ", " << a.submissionId
                          << ", " << a.reviewerDomain << "\n";

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
    }
};
#endif //ASSIGNMENTRUNNER_H
