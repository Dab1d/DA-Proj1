#ifndef INC_1PROJ_REVIEWGRAPHBUILDER_H
#define INC_1PROJ_REVIEWGRAPHBUILDER_H

#include <cstddef>
#include "io/DataLoader.h"
#include "structs/Graph.h"

/**
 * @brief Identifies one vertex in the reviewer-submission graph.
 */
struct ReviewGraphNode {
    /**
     * @brief Differentiates reviewer vertices from submission vertices.
     */
    enum class Type {
        Reviewer,
        Submission
    };

    /** @brief Logical side of the bipartite graph. */
    Type type;
    /** @brief Identifier of the original reviewer or submission. */
    int id;

    /**
     * @brief Creates a reviewer-side graph node.
     * @param reviewerId Reviewer identifier.
     * @return Tagged reviewer node key.
     */
    static ReviewGraphNode reviewer(int reviewerId);

    /**
     * @brief Creates a submission-side graph node.
     * @param submissionId Submission identifier.
     * @return Tagged submission node key.
     */
    static ReviewGraphNode submission(int submissionId);

    /**
     * @brief Checks whether this node belongs to the reviewer side.
     * @return `true` for reviewer nodes, `false` otherwise.
     */
    bool isReviewer() const;

    /**
     * @brief Checks whether this node belongs to the submission side.
     * @return `true` for submission nodes, `false` otherwise.
     */
    bool isSubmission() const;

    /**
     * @brief Compares node keys by type and identifier.
     * @param other Node being compared.
     * @return `true` when both nodes refer to the same graph vertex.
     */
    bool operator==(const ReviewGraphNode &other) const;
};

/**
 * @brief Builds a bipartite compatibility graph from loaded conference data.
 */
class ReviewGraphBuilder {
public:
    /**
     * @brief Populates an empty graph with reviewer and submission vertices.
     *
     * Reviewer vertices are connected to submission vertices whenever the
     * loaded parameters consider the domain match eligible for assignment.
     *
     * @param data Loaded conference instance.
     * @param graph Empty graph to be populated.
     * @return `true` when the graph was built, `false` when @p graph was not
     * empty and was left unchanged.
     * @time_complexity O(R * S * V), where R is the number of reviewers, S is
     * the number of submissions, and V is the current graph lookup cost.
     */
    static bool populateGraph(const LoadedConferenceData &data,
                              Graph<ReviewGraphNode> &graph);

    /**
     * @brief Counts the total number of directed compatibility edges.
     * @param graph Reviewer-submission compatibility graph.
     * @return Number of edges currently stored in the graph.
     * @time_complexity O(V + E), where V is the number of vertices and E is the
     * number of edges.
     */
    static std::size_t countEdges(const Graph<ReviewGraphNode> &graph);
};

#endif // INC_1PROJ_REVIEWGRAPHBUILDER_H
