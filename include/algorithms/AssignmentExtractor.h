//
// Created by leo_o on 29/03/2026.
//

#ifndef ASSIGNMENTEXTRACTOR_H
#define ASSIGNMENTEXTRACTOR_H
#pragma once
#include "structs/Graph.h"
//#include "model/LoadedConferenceData.h"
#include "structs/ConferenceData.h" // Assignment, RiskEntry
#include "structs/Reviewer.h"
#include "structs/Submission.h"
#include <utility>
#include <vector>

class AssignmentExtractor {
public:
    /**
     * @brief Extracts assignments from the graph after MaxFlow.
     *        Looks for reviewer→submission edges with flow > 0.
     * @param g Flow network after running max flow.
     * @param data Loaded conference instance used to map graph nodes back to
     * reviewer and submission identifiers.
     * @return Extracted reviewer-submission assignments.
     * @note Time complexity: O(V + E)
     */
    static std::vector<Assignment> extract(
        const Graph<int>& g,
        const LoadedConferenceData& data)
    {
        std::vector<Assignment> results;
        int R = data.reviewers.size();

        for (int i = 0; i < R; i++) {
            int reviewerNode = 2 + i;
            Vertex<int>* v = g.findVertex(reviewerNode);
            if (!v) continue;

            for (Edge<int>* e : v->getAdj()) {
                int dst = e->getDest()->getInfo();
                if (dst >= 2 + R && e->getFlow() > 0) {
                    int submissionIdx = dst - (2 + R);
                    const auto match = resolveMatchDomains(data.submissions[submissionIdx],
                                                           data.reviewers[i],
                                                           data.parameters.generateAssignments);
                    Assignment a;
                    a.reviewerId      = data.reviewers[i].id;
                    a.submissionId    = data.submissions[submissionIdx].id;
                    a.reviewerDomain  = match.second;
                    a.submissionDomain = match.first;
                    results.push_back(a);
                }
            }
        }
        return results;
    }

    /**
     * @brief Identifies submissions with insufficient reviews after MaxFlow.
     * @param g Flow network after running max flow.
     * @param data Loaded conference instance containing submission metadata and
     * minimum review requirements.
     * @return One risk entry per submission that is still missing reviews.
     * @note Time complexity: O(V + E)
     */
    static std::vector<RiskEntry> getMissing(
        const Graph<int>& g,
        const LoadedConferenceData& data)
    {
        std::vector<RiskEntry> missing;
        int R = data.reviewers.size();
        int min = data.parameters.minReviewsPerSubmission;

        for (int j = 0; j < (int)data.submissions.size(); j++) {
            int submissionNode = 2 + R + j;
            Vertex<int>* v = g.findVertex(submissionNode);
            if (!v) continue;

            double flowIn = 0;
            for (Edge<int>* e : v->getIncoming()) {
                int src = e->getOrig()->getInfo();
                if (src >= 2 && src < 2 + R)
                    flowIn += e->getFlow();
            }

            int shortage = min - (int)flowIn;
            if (shortage > 0) {
                RiskEntry r;
                r.submissionId  = data.submissions[j].id;
                r.domain        = preferredSubmissionDomain(data.submissions[j],
                                                           data.parameters.generateAssignments);
                r.missingReviews = shortage;
                missing.push_back(r);
            }
        }
        return missing;
    }

private:
    static int preferredSubmissionDomain(const Submission& submission, int mode) {
        if (submission.primaryTopic != -1) return submission.primaryTopic;
        if (mode >= 2 && submission.secondaryTopic != -1) return submission.secondaryTopic;
        return -1;
    }

    static std::pair<int, int> resolveMatchDomains(const Submission& submission,
                                                   const Reviewer& reviewer,
                                                   int mode) {
        std::vector<int> submissionDomains;
        std::vector<int> reviewerDomains;

        if (submission.primaryTopic != -1) submissionDomains.push_back(submission.primaryTopic);
        if (mode >= 2 && submission.secondaryTopic != -1) submissionDomains.push_back(submission.secondaryTopic);

        if (reviewer.primaryExpertise != -1) reviewerDomains.push_back(reviewer.primaryExpertise);
        if (mode >= 3 && reviewer.secondaryExpertise != -1) reviewerDomains.push_back(reviewer.secondaryExpertise);

        for (int submissionDomain : submissionDomains)
            for (int reviewerDomain : reviewerDomains)
                if (submissionDomain == reviewerDomain)
                    return {submissionDomain, reviewerDomain};

        return {preferredSubmissionDomain(submission, mode), reviewer.primaryExpertise};
    }
};
#endif //ASSIGNMENTEXTRACTOR_H
