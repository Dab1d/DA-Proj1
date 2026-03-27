#include "logic/Control.h"

#include <algorithm>
#include <fstream>
#include <limits>
#include <queue>
#include <utility>

namespace {
    struct MatchInfo {
        bool compatible = false;
        int submissionDomain = -1;
        int reviewerDomain = -1;
        int score = -1;
    };

    struct FlowEdge {
        int to;
        int rev;
        int cap;
        int originalCap;
        int reviewerIndex;
        int submissionIndex;
    };

    class FlowNetwork {
    public:
        explicit FlowNetwork(int nodeCount) : adjacency(nodeCount) {}

        void addEdge(int from,
                     int to,
                     int capacity,
                     int reviewerIndex = -1,
                     int submissionIndex = -1) {
            FlowEdge forward{to, static_cast<int>(adjacency[to].size()), capacity, capacity,
                             reviewerIndex, submissionIndex};
            FlowEdge backward{from, static_cast<int>(adjacency[from].size()), 0, 0, -1, -1};
            adjacency[from].push_back(forward);
            adjacency[to].push_back(backward);
        }

        int maxFlow(int source, int sink) {
            int totalFlow = 0;

            while (true) {
                std::vector<int> parentNode(adjacency.size(), -1);
                std::vector<int> parentEdge(adjacency.size(), -1);
                std::queue<int> pending;
                pending.push(source);
                parentNode[source] = source;

                while (!pending.empty() && parentNode[sink] == -1) {
                    const int current = pending.front();
                    pending.pop();

                    for (int edgeIndex = 0; edgeIndex < static_cast<int>(adjacency[current].size()); ++edgeIndex) {
                        const FlowEdge &edge = adjacency[current][edgeIndex];
                        if (edge.cap == 0 || parentNode[edge.to] != -1) {
                            continue;
                        }

                        parentNode[edge.to] = current;
                        parentEdge[edge.to] = edgeIndex;
                        pending.push(edge.to);
                    }
                }

                if (parentNode[sink] == -1) {
                    return totalFlow;
                }

                int augment = std::numeric_limits<int>::max();
                for (int node = sink; node != source; node = parentNode[node]) {
                    const FlowEdge &edge = adjacency[parentNode[node]][parentEdge[node]];
                    augment = std::min(augment, edge.cap);
                }

                for (int node = sink; node != source; node = parentNode[node]) {
                    FlowEdge &edge = adjacency[parentNode[node]][parentEdge[node]];
                    FlowEdge &reverse = adjacency[edge.to][edge.rev];
                    edge.cap -= augment;
                    reverse.cap += augment;
                }

                totalFlow += augment;
            }
        }

        const std::vector<std::vector<FlowEdge>> &getAdjacency() const {
            return adjacency;
        }

    private:
        std::vector<std::vector<FlowEdge>> adjacency;
    };

    MatchInfo bestMatch(const Submission &submission,
                        const Reviewer &reviewer,
                        const Parameters &params) {
        MatchInfo result;

        if (params.generateAssignments < 1) {
            return result;
        }

        auto consider = [&](int submissionDomain, int reviewerDomain, int score) {
            if (submissionDomain == -1 || reviewerDomain == -1) {
                return;
            }
            if (submissionDomain != reviewerDomain) {
                return;
            }
            if (!result.compatible || score > result.score) {
                result.compatible = true;
                result.submissionDomain = submissionDomain;
                result.reviewerDomain = reviewerDomain;
                result.score = score;
            }
        };

        consider(submission.primaryTopic, reviewer.primaryExpertise, params.primaryReviewerExpertise);

        if (params.generateAssignments >= 2) {
            consider(submission.secondaryTopic, reviewer.primaryExpertise, params.primarySubmissionDomain);
        }

        if (params.generateAssignments >= 3) {
            consider(submission.primaryTopic, reviewer.secondaryExpertise, params.secondaryReviewerExpertise);
            consider(submission.secondaryTopic, reviewer.secondaryExpertise, params.secondarySubmissionDomain);
        }

        return result;
    }

    std::vector<Assignment> computeAssignments(const std::vector<Submission> &submissions,
                                               const std::vector<Reviewer> &reviewers,
                                               const Parameters &params) {
        if (params.generateAssignments == 0) {
            return {};
        }

        const int submissionCount = static_cast<int>(submissions.size());
        const int reviewerCount = static_cast<int>(reviewers.size());
        const int source = 0;
        const int reviewerStart = 1;
        const int submissionStart = reviewerStart + reviewerCount;
        const int sink = submissionStart + submissionCount;

        FlowNetwork network(sink + 1);

        for (int reviewerIndex = 0; reviewerIndex < reviewerCount; ++reviewerIndex) {
            network.addEdge(source, reviewerStart + reviewerIndex, params.maxReviewsPerReviewer);
        }

        for (int submissionIndex = 0; submissionIndex < submissionCount; ++submissionIndex) {
            network.addEdge(submissionStart + submissionIndex, sink, params.minReviewsPerSubmission);
        }

        struct CandidateEdge {
            int reviewerIndex;
            int submissionIndex;
            MatchInfo match;
        };

        std::vector<CandidateEdge> candidates;
        for (int reviewerIndex = 0; reviewerIndex < reviewerCount; ++reviewerIndex) {
            for (int submissionIndex = 0; submissionIndex < submissionCount; ++submissionIndex) {
                const MatchInfo match = bestMatch(submissions[submissionIndex], reviewers[reviewerIndex], params);
                if (!match.compatible) {
                    continue;
                }
                candidates.push_back({reviewerIndex, submissionIndex, match});
            }
        }

        std::sort(candidates.begin(), candidates.end(),
                  [&](const CandidateEdge &lhs, const CandidateEdge &rhs) {
                      if (lhs.match.score != rhs.match.score) {
                          return lhs.match.score > rhs.match.score;
                      }
                      if (lhs.submissionIndex != rhs.submissionIndex) {
                          return lhs.submissionIndex < rhs.submissionIndex;
                      }
                      return lhs.reviewerIndex < rhs.reviewerIndex;
                  });

        for (const CandidateEdge &candidate : candidates) {
            network.addEdge(reviewerStart + candidate.reviewerIndex,
                            submissionStart + candidate.submissionIndex,
                            1,
                            candidate.reviewerIndex,
                            candidate.submissionIndex);
        }

        network.maxFlow(source, sink);

        std::vector<Assignment> assignments;
        for (int reviewerIndex = 0; reviewerIndex < reviewerCount; ++reviewerIndex) {
            const auto &edges = network.getAdjacency()[reviewerStart + reviewerIndex];
            for (const FlowEdge &edge : edges) {
                if (edge.reviewerIndex == -1 || edge.submissionIndex == -1) {
                    continue;
                }
                if (edge.originalCap != 1 || edge.cap != 0) {
                    continue;
                }

                const MatchInfo match = bestMatch(submissions[edge.submissionIndex], reviewers[edge.reviewerIndex], params);
                assignments.push_back({
                    submissions[edge.submissionIndex].id,
                    reviewers[edge.reviewerIndex].id,
                    match.submissionDomain,
                    match.reviewerDomain
                });
            }
        }

        std::sort(assignments.begin(), assignments.end(),
                  [](const Assignment &lhs, const Assignment &rhs) {
                      if (lhs.submissionId != rhs.submissionId) {
                          return lhs.submissionId < rhs.submissionId;
                      }
                      return lhs.reviewerId < rhs.reviewerId;
                  });

        return assignments;
    }
}

std::vector<Assignment> generateAssignments(const std::vector<Submission> &submissions,
                                            const std::vector<Reviewer> &reviewers,
                                            const Parameters &params) {
    return computeAssignments(submissions, reviewers, params);
}

std::vector<RiskEntry> analyzeRisk(const std::vector<Submission> &submissions,
                                   const std::vector<Assignment> &assignments,
                                   const Parameters &params) {
    std::vector<RiskEntry> risks;
    for (const Submission &submission : submissions) {
        int assignedCount = 0;
        for (const Assignment &assignment : assignments) {
            if (assignment.submissionId == submission.id) {
                ++assignedCount;
            }
        }

        if (assignedCount < params.minReviewsPerSubmission) {
            risks.push_back({submission.id, submission.primaryTopic,
                             params.minReviewsPerSubmission - assignedCount});
        }
    }
    return risks;
}

std::vector<int> analyzeReviewerRisk(const std::vector<Submission> &submissions,
                                     const std::vector<Reviewer> &reviewers,
                                     const std::vector<Assignment> &assignments,
                                     const Parameters &params) {
    if (params.riskAnalysis <= 0 || assignments.empty()) {
        return {};
    }

    std::vector<int> riskyReviewers;
    for (const Reviewer &reviewer : reviewers) {
        std::vector<Reviewer> remainingReviewers;
        remainingReviewers.reserve(reviewers.size());
        for (const Reviewer &candidate : reviewers) {
            if (candidate.id != reviewer.id) {
                remainingReviewers.push_back(candidate);
            }
        }

        const std::vector<Assignment> reassigned =
            computeAssignments(submissions, remainingReviewers, params);
        if (!analyzeRisk(submissions, reassigned, params).empty()) {
            riskyReviewers.push_back(reviewer.id);
        }
    }

    return riskyReviewers;
}

void writeOutput(const std::vector<Assignment> &assignments,
                 const std::vector<RiskEntry> &risks,
                 const std::vector<int> &riskyReviewers,
                 const Parameters &params) {
    std::ofstream output(params.outputFileName);
    if (!output.is_open()) {
        return;
    }

    output << "#SubmissionId,ReviewerId,Match\n";
    for (const Assignment &assignment : assignments) {
        output << assignment.submissionId << ", "
               << assignment.reviewerId << ", "
               << assignment.submissionDomain << "\n";
    }

    output << "#ReviewerId,SubmissionId,Match\n";
    std::vector<Assignment> byReviewer = assignments;
    std::sort(byReviewer.begin(), byReviewer.end(),
              [](const Assignment &lhs, const Assignment &rhs) {
                  if (lhs.reviewerId != rhs.reviewerId) {
                      return lhs.reviewerId < rhs.reviewerId;
                  }
                  return lhs.submissionId < rhs.submissionId;
              });
    for (const Assignment &assignment : byReviewer) {
        output << assignment.reviewerId << ", "
               << assignment.submissionId << ", "
               << assignment.reviewerDomain << "\n";
    }

    output << "#Total: " << assignments.size() << "\n";

    if (!risks.empty()) {
        output << "#SubmissionId,Domain,MissingReviews\n";
        for (const RiskEntry &risk : risks) {
            output << risk.submissionId << ", "
                   << risk.domain << ", "
                   << risk.missingReviews << "\n";
        }
    }

    if (params.riskAnalysis > 0) {
        output << "#Risk Analysis: " << params.riskAnalysis << "\n";
        for (int reviewerId : riskyReviewers) {
            output << reviewerId << "\n";
        }
    }
}
