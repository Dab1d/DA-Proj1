#include "logic/ReviewGraphExporter.h"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <set>
#include <vector>

namespace {
    enum class ExportStage {
        SubmissionsOnly,
        VerticesOnly,
        FullGraph
    };

    struct AssignmentEdgeKey {
        int reviewerId;
        int submissionId;

        bool operator<(const AssignmentEdgeKey &other) const {
            if (reviewerId != other.reviewerId) {
                return reviewerId < other.reviewerId;
            }
            return submissionId < other.submissionId;
        }
    };

    std::string nodeName(const ReviewGraphNode &node) {
        const std::string prefix = node.isReviewer() ? "reviewer_" : "submission_";
        return prefix + std::to_string(node.id);
    }

    bool renderDotToSvg(const std::filesystem::path &dotPath,
                        const std::filesystem::path &svgPath) {
        const std::string command = "dot -Tsvg \"" + dotPath.string() +
                                    "\" -o \"" + svgPath.string() + "\"";
        return std::system(command.c_str()) == 0;
    }

    bool renderDotToPng(const std::filesystem::path &dotPath,
                        const std::filesystem::path &pngPath) {
        const std::string command = "dot -Tpng \"" + dotPath.string() +
                                    "\" -o \"" + pngPath.string() + "\"";
        return std::system(command.c_str()) == 0;
    }

    bool renderPngSequenceToGif(const std::vector<std::filesystem::path> &pngPaths,
                                const std::filesystem::path &gifPath) {
        if (pngPaths.empty()) {
            return false;
        }

        std::string command = "magick -delay 120 -loop 0";
        for (const std::filesystem::path &pngPath : pngPaths) {
            command += " \"" + pngPath.string() + "\"";
        }
        command += " \"" + gifPath.string() + "\"";
        return std::system(command.c_str()) == 0;
    }

    bool writeDotFile(const LoadedConferenceData &data,
                      const std::string &outputPath,
                      ExportStage stage = ExportStage::FullGraph) {
        Graph<ReviewGraphNode> graph;
        if (!ReviewGraphBuilder::populateGraph(data, graph)) {
            return false;
        }

        std::ofstream output(outputPath);
        if (!output.is_open()) {
            return false;
        }

        output << "digraph ReviewGraph {\n";
        output << "  rankdir=LR;\n";
        output << "  graph [layout=dot, splines=true, overlap=false, pad=0.2, nodesep=0.5, ranksep=1.0];\n";
        output << "  node [shape=circle, width=0.65, fixedsize=true, style=filled, fillcolor=\"white\", color=\"black\", penwidth=2, fontname=\"Helvetica\", fontsize=13];\n";
        output << "  edge [fontname=\"Helvetica\", fontsize=11, color=\"gray50\", penwidth=1.5];\n";
        output << "\n";
        output << "  {\n";
        output << "    rank=same;\n";
        for (const Submission &submission : data.submissions) {
            const ReviewGraphNode node = ReviewGraphNode::submission(submission.id);
            output << "    " << nodeName(node)
                   << " [label=\"" << submission.id << "\"];\n";
        }
        output << "  }\n";
        output << "\n";

        if (stage == ExportStage::SubmissionsOnly) {
            output << "}\n";
            return true;
        }

        output << "  {\n";
        output << "    rank=same;\n";
        for (const Reviewer &reviewer : data.reviewers) {
            const ReviewGraphNode node = ReviewGraphNode::reviewer(reviewer.id);
            output << "    " << nodeName(node)
                   << " [label=\"" << reviewer.id << "\"];\n";
        }
        output << "  }\n";
        output << "\n";

        if (stage == ExportStage::VerticesOnly) {
            output << "}\n";
            return true;
        }

        for (const Vertex<ReviewGraphNode> *vertex : graph.getVertexSet()) {
            const ReviewGraphNode source = vertex->getInfo();
            for (const Edge<ReviewGraphNode> *edge : vertex->getAdj()) {
                const ReviewGraphNode destination = edge->getDest()->getInfo();
                output << "  " << nodeName(source)
                       << " -> " << nodeName(destination)
                       << " [label=\"" << edge->getWeight() << "\"];\n";
            }
        }

        output << "}\n";
        return true;
    }

    bool writeAssignmentStepDotFile(const LoadedConferenceData &data,
                                    const std::vector<Assignment> &assignments,
                                    std::size_t highlightedCount,
                                    const std::string &outputPath) {
        Graph<ReviewGraphNode> graph;
        if (!ReviewGraphBuilder::populateGraph(data, graph)) {
            return false;
        }

        std::ofstream output(outputPath);
        if (!output.is_open()) {
            return false;
        }

        std::set<AssignmentEdgeKey> highlightedEdges;
        for (std::size_t i = 0; i < highlightedCount && i < assignments.size(); ++i) {
            highlightedEdges.insert({assignments[i].reviewerId, assignments[i].submissionId});
        }

        output << "digraph ReviewGraph {\n";
        output << "  rankdir=LR;\n";
        output << "  labelloc=\"t\";\n";
        output << "  graph [layout=dot, splines=true, overlap=false, pad=0.2, nodesep=0.5, ranksep=1.0];\n";
        output << "  label=\"Assignment selection: " << highlightedCount
               << "/" << assignments.size() << "\";\n";
        output << "  node [shape=circle, width=0.65, fixedsize=true, style=filled, fillcolor=\"white\", color=\"black\", penwidth=2, fontname=\"Helvetica\", fontsize=13];\n";
        output << "  edge [fontname=\"Helvetica\", fontsize=11, color=\"gray75\", penwidth=1.0];\n";
        output << "\n";
        output << "  {\n";
        output << "    rank=same;\n";
        for (const Submission &submission : data.submissions) {
            const ReviewGraphNode node = ReviewGraphNode::submission(submission.id);
            output << "    " << nodeName(node)
                   << " [label=\"" << submission.id << "\"];\n";
        }
        output << "  }\n\n";

        output << "  {\n";
        output << "    rank=same;\n";
        for (const Reviewer &reviewer : data.reviewers) {
            const ReviewGraphNode node = ReviewGraphNode::reviewer(reviewer.id);
            output << "    " << nodeName(node)
                   << " [label=\"" << reviewer.id << "\"];\n";
        }
        output << "  }\n\n";

        for (const Vertex<ReviewGraphNode> *vertex : graph.getVertexSet()) {
            const ReviewGraphNode source = vertex->getInfo();
            for (const Edge<ReviewGraphNode> *edge : vertex->getAdj()) {
                const ReviewGraphNode destination = edge->getDest()->getInfo();

                output << "  " << nodeName(source)
                       << " -> " << nodeName(destination)
                       << " [label=\"" << edge->getWeight() << "\"";

                if (source.isReviewer() && destination.isSubmission() &&
                    highlightedEdges.count({source.id, destination.id})) {
                    output << ", color=\"firebrick3\", penwidth=3.5";
                }

                output << "];\n";
            }
        }

        output << "}\n";
        return true;
    }
}

bool ReviewGraphExporter::exportToDot(const LoadedConferenceData &data,
                                      const std::string &outputPath) {
    return writeDotFile(data, outputPath);
}

bool ReviewGraphExporter::exportToSvg(const LoadedConferenceData &data,
                                      const std::string &outputPath) {
    namespace fs = std::filesystem;

    fs::path svgPath(outputPath);
    fs::path dotPath = svgPath;
    dotPath.replace_extension(".dot");

    if (!writeDotFile(data, dotPath.string())) {
        return false;
    }

    return renderDotToSvg(dotPath, svgPath);
}

bool ReviewGraphExporter::exportConstructionStepsToSvg(const LoadedConferenceData &data,
                                                       const std::string &outputPath) {
    namespace fs = std::filesystem;

    fs::path basePath(outputPath);
    const fs::path parentPath = basePath.parent_path();
    const std::string stem = basePath.stem().string().empty() ? "review_graph" : basePath.stem().string();

    struct SnapshotInfo {
        std::string suffix;
        ExportStage stage;
    };

    const std::vector<SnapshotInfo> snapshots = {
        {"step1_submissions", ExportStage::SubmissionsOnly},
        {"step2_vertices", ExportStage::VerticesOnly},
        {"step3_full", ExportStage::FullGraph}
    };

    for (const SnapshotInfo &snapshot : snapshots) {
        const fs::path dotPath = parentPath / (stem + "_" + snapshot.suffix + ".dot");
        const fs::path svgPath = parentPath / (stem + "_" + snapshot.suffix + ".svg");

        if (!writeDotFile(data, dotPath.string(), snapshot.stage)) {
            return false;
        }

        if (!renderDotToSvg(dotPath, svgPath)) {
            return false;
        }
    }

    return true;
}

bool ReviewGraphExporter::exportConstructionStepsToGif(const LoadedConferenceData &data,
                                                       const std::string &outputPath) {
    namespace fs = std::filesystem;

    fs::path basePath(outputPath);
    const fs::path parentPath = basePath.parent_path();
    const std::string stem = basePath.stem().string().empty() ? "review_graph" : basePath.stem().string();

    struct SnapshotInfo {
        std::string suffix;
        ExportStage stage;
    };

    const std::vector<SnapshotInfo> snapshots = {
        {"step1_submissions", ExportStage::SubmissionsOnly},
        {"step2_vertices", ExportStage::VerticesOnly},
        {"step3_full", ExportStage::FullGraph}
    };

    std::vector<fs::path> pngPaths;
    for (const SnapshotInfo &snapshot : snapshots) {
        const fs::path dotPath = parentPath / (stem + "_" + snapshot.suffix + ".dot");
        const fs::path pngPath = parentPath / (stem + "_" + snapshot.suffix + ".png");

        if (!writeDotFile(data, dotPath.string(), snapshot.stage)) {
            return false;
        }

        if (!renderDotToPng(dotPath, pngPath)) {
            return false;
        }

        pngPaths.push_back(pngPath);
    }

    return renderPngSequenceToGif(pngPaths, basePath);
}

bool ReviewGraphExporter::exportAssignmentStepsToGif(const LoadedConferenceData &data,
                                                     const std::vector<Assignment> &assignments,
                                                     const std::string &outputPath) {
    namespace fs = std::filesystem;

    fs::path basePath(outputPath);
    const fs::path parentPath = basePath.parent_path();
    const std::string stem = basePath.stem().string().empty() ? "assignment_process" : basePath.stem().string();

    std::vector<fs::path> pngPaths;
    pngPaths.reserve(assignments.size() + 1);

    for (std::size_t step = 0; step <= assignments.size(); ++step) {
        const fs::path dotPath = parentPath / (stem + "_step" + std::to_string(step) + ".dot");
        const fs::path pngPath = parentPath / (stem + "_step" + std::to_string(step) + ".png");

        if (!writeAssignmentStepDotFile(data, assignments, step, dotPath.string())) {
            return false;
        }

        if (!renderDotToPng(dotPath, pngPath)) {
            return false;
        }

        pngPaths.push_back(pngPath);
    }

    return renderPngSequenceToGif(pngPaths, basePath);
}
