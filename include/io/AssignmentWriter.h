//
// Created by leo_o on 29/03/2026.
//

#ifndef ASSIGNMENTWRITER_H
#define ASSIGNMENTWRITER_H
#pragma once
#include <fstream>
#include <algorithm>
#include <vector>
#include "structs/ConferenceData.h"

class AssignmentWriter {
public:
    static void writeAssignments(const std::vector<Assignment>& assignments,
                                 std::ofstream& out)
    {
        auto sorted = assignments;

        // Submission → Reviewer
        out << "#SubmissionId,ReviewerId,Match\n";
        std::sort(sorted.begin(), sorted.end(), [](const Assignment& a, const Assignment& b){
            return a.submissionId < b.submissionId;
        });
        for (auto& a : sorted)
            out << a.submissionId << ", " << a.reviewerId << ", "
                << a.submissionDomain << "\n";

        // Reviewer → Submission
        out << "#ReviewerId,SubmissionId,Match\n";
        std::sort(sorted.begin(), sorted.end(), [](const Assignment& a, const Assignment& b){
            return a.reviewerId < b.reviewerId;
        });
        for (auto& a : sorted)
            out << a.reviewerId << ", " << a.submissionId << ", "
                << a.reviewerDomain << "\n";

        out << "#Total: " << assignments.size() << "\n";
    }

    static void writeMissing(const std::vector<RiskEntry>& missing,
                             std::ofstream& out)
    {
        out << "#SubmissionId,Domain,MissingReviews\n";
        for (auto& r : missing)
            out << r.submissionId << ", " << r.domain << ", "
                << r.missingReviews << "\n";
    }

    static void writeRiskAnalysis(const std::vector<int>& risky, int K, std::ofstream& out) {
        out << "#Risk Analysis: " << K << "\n";
        for (int i = 0; i < (int)risky.size(); i++) {
            if (i > 0) out << ", ";
            out << risky[i];
        }
        if (!risky.empty()) out << "\n";
    }
};
#endif //ASSIGNMENTWRITER_H
