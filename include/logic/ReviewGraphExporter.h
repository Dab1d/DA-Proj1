#ifndef INC_1PROJ_REVIEWGRAPHEXPORTER_H
#define INC_1PROJ_REVIEWGRAPHEXPORTER_H

#include <string>
#include <vector>

#include "structs/ConferenceData.h"
#include "logic/ReviewGraphBuilder.h"

/**
 * @brief Exports the reviewer-submission compatibility graph to Graphviz DOT.
 */
class ReviewGraphExporter {
public:
    /**
     * @brief Builds the compatibility graph from loaded data and writes it to a
     * DOT file.
     *
     * @param data Loaded conference instance.
     * @param outputPath Destination DOT file path.
     * @return `true` when the file was written successfully, `false`
     * otherwise.
     * @time_complexity O(R * S * V + V + E), where R is the number of
     * reviewers, S is the number of submissions, V is the number of vertices,
     * and E is the number of edges.
     */
    static bool exportToDot(const LoadedConferenceData &data,
                            const std::string &outputPath);

    /**
     * @brief Builds the compatibility graph from loaded data and renders it as
     * an SVG image through Graphviz.
     *
     * @param data Loaded conference instance.
     * @param outputPath Destination SVG file path.
     * @return `true` when the SVG file was written successfully, `false`
     * otherwise.
     * @time_complexity O(R * S * V + V + E), where R is the number of
     * reviewers, S is the number of submissions, V is the number of vertices,
     * and E is the number of edges.
     */
    static bool exportToSvg(const LoadedConferenceData &data,
                            const std::string &outputPath);

    /**
     * @brief Exports intermediate SVG snapshots of the compatibility-graph
     * construction.
     *
     * The exporter writes three SVG files next to @p outputPath:
     * submissions only, all vertices without edges, and the final graph.
     *
     * @param data Loaded conference instance.
     * @param outputPath Base SVG path used to derive snapshot filenames.
     * @return `true` when all snapshots were written successfully, `false`
     * otherwise.
     * @time_complexity O(R * S * V + V + E), where R is the number of
     * reviewers, S is the number of submissions, V is the number of vertices,
     * and E is the number of edges.
     */
    static bool exportConstructionStepsToSvg(const LoadedConferenceData &data,
                                             const std::string &outputPath);

    /**
     * @brief Exports an animated GIF showing the compatibility-graph
     * construction steps.
     *
     * The GIF is built from intermediate rendered frames: submissions only,
     * all vertices, and the final graph with compatibility edges.
     *
     * @param data Loaded conference instance.
     * @param outputPath Destination GIF file path.
     * @return `true` when the GIF was written successfully, `false` otherwise.
     * @time_complexity O(R * S * V + V + E), where R is the number of
     * reviewers, S is the number of submissions, V is the number of vertices,
     * and E is the number of edges.
     */
    static bool exportConstructionStepsToGif(const LoadedConferenceData &data,
                                             const std::string &outputPath);

    /**
     * @brief Exports an animated GIF showing the assignment-selection process.
     *
     * The animation starts from the compatibility graph and progressively
     * highlights one selected reviewer-submission edge per frame.
     *
     * @param data Loaded conference instance.
     * @param assignments Final reviewer assignments.
     * @param outputPath Destination GIF file path.
     * @return `true` when the GIF was written successfully, `false` otherwise.
     * @time_complexity O((A + 1) * (R * S * V + V + E)), where A is the number
     * of assignments, R is the number of reviewers, S is the number of
     * submissions, V is the number of vertices, and E is the number of edges.
     */
    static bool exportAssignmentStepsToGif(const LoadedConferenceData &data,
                                           const std::vector<Assignment> &assignments,
                                           const std::string &outputPath);
};

#endif // INC_1PROJ_REVIEWGRAPHEXPORTER_H
