#include "logic/ReviewGraphBuilder.h"

using std::size_t;

namespace {
    struct MatchResult {
        bool eligible = false;
        double score = 0;
    };

    void addPrimaryDomainMatch(const Reviewer &reviewer,
                               const Submission &submission,
                               const Parameters &params,
                               MatchResult &result) {
        if (reviewer.primaryExpertise == submission.primaryTopic) {
            result.eligible = true;
            result.score += params.primaryReviewerExpertise;
        }
    }

    void addSecondarySubmissionMatch(const Reviewer &reviewer,
                                     const Submission &submission,
                                     const Parameters &params,
                                     MatchResult &result) {
        if (submission.secondaryTopic != -1 &&
            reviewer.primaryExpertise == submission.secondaryTopic) {
            result.eligible = true;
            result.score += params.primarySubmissionDomain;
        }
    }

    void addSecondaryReviewerMatches(const Reviewer &reviewer,
                                     const Submission &submission,
                                     const Parameters &params,
                                     MatchResult &result) {
        if (reviewer.secondaryExpertise == -1) {
            return;
        }

        if (reviewer.secondaryExpertise == submission.primaryTopic) {
            result.eligible = true;
            result.score += params.secondaryReviewerExpertise;
        }

        if (submission.secondaryTopic != -1 &&
            reviewer.secondaryExpertise == submission.secondaryTopic) {
            result.eligible = true;
            result.score += params.secondarySubmissionDomain;
        }
    }

    MatchResult computeMatch(const Reviewer &reviewer,
                             const Submission &submission,
                             const Parameters &params) {
        MatchResult result;
        if (params.generateAssignments < 1) {
            return result;
        }

        addPrimaryDomainMatch(reviewer, submission, params, result);

        if (params.generateAssignments >= 2) {
            addSecondarySubmissionMatch(reviewer, submission, params, result);
        }

        if (params.generateAssignments >= 3) {
            addSecondaryReviewerMatches(reviewer, submission, params, result);
        }

        return result;
    }
}

ReviewGraphNode ReviewGraphNode::reviewer(int reviewerId) {
    return {Type::Reviewer, reviewerId};
}

ReviewGraphNode ReviewGraphNode::submission(int submissionId) {
    return {Type::Submission, submissionId};
}

bool ReviewGraphNode::isReviewer() const {
    return type == Type::Reviewer;
}

bool ReviewGraphNode::isSubmission() const {
    return type == Type::Submission;
}

bool ReviewGraphNode::operator==(const ReviewGraphNode &other) const {
    return type == other.type && id == other.id;
}

bool ReviewGraphBuilder::populateGraph(const LoadedConferenceData &data,
                                       Graph<ReviewGraphNode> &graph) {
    if (graph.getNumVertex() != 0) {
        return false;
    }

    for (const Reviewer &reviewer : data.reviewers) {
        graph.addVertex(ReviewGraphNode::reviewer(reviewer.id));
    }

    for (const Submission &submission : data.submissions) {
        graph.addVertex(ReviewGraphNode::submission(submission.id));
    }

    for (const Reviewer &reviewer : data.reviewers) {
        const ReviewGraphNode reviewerNode = ReviewGraphNode::reviewer(reviewer.id);
        for (const Submission &submission : data.submissions) {
            const MatchResult match = computeMatch(reviewer, submission, data.parameters);
            if (!match.eligible) {
                continue;
            }

            graph.addEdge(reviewerNode,
                          ReviewGraphNode::submission(submission.id),
                          match.score);
        }
    }

    return true;
}

size_t ReviewGraphBuilder::countEdges(const Graph<ReviewGraphNode> &graph) {
    size_t edgeCount = 0;
    for (const Vertex<ReviewGraphNode> *vertex : graph.getVertexSet()) {
        edgeCount += vertex->getAdj().size();
    }
    return edgeCount;
}
