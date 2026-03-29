//
// Created by leo_o on 29/03/2026.
//

#ifndef ASSIGNMENTEXTRACTOR_H
#define ASSIGNMENTEXTRACTOR_H
#pragma once
#include "structs/Graph.h"
//#include "model/LoadedConferenceData.h"
#include "structs/ConferenceData.h" // Assignment, RiskEntry

class AssignmentExtractor {
public:
    /**
     * @brief Extracts assignments from the graph after MaxFlow.
     *        Looks for reviewer→submission edges with flow > 0.
     * @complexity O(V + E)
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
                    Assignment a;
                    a.reviewerId      = data.reviewers[i].id;
                    a.submissionId    = data.submissions[submissionIdx].id;
                    a.reviewerDomain  = data.reviewers[i].primaryExpertise;
                    a.submissionDomain = data.submissions[submissionIdx].primaryTopic;
                    results.push_back(a);
                }
            }
        }
        return results;
    }

    /**
     * @brief Identifies submissions with insufficient reviews after MaxFlow.
     * @complexity O(V + E)
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
                r.domain        = data.submissions[j].primaryTopic;
                r.missingReviews = shortage;
                missing.push_back(r);
            }
        }
        return missing;
    }
};
#endif //ASSIGNMENTEXTRACTOR_H
