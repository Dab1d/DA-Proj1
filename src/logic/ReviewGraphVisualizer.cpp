#include "logic/ReviewGraphVisualizer.h"

#include <iomanip>
#include <iostream>
#include <string>

#include "logic/DataActionUtils.h"
#include "logic/ReviewGraphBuilder.h"
#include "logic/ReviewGraphExporter.h"

using std::cout;
using std::getline;
using std::left;
using std::setw;
using std::string;

namespace {
    void printGraphPreview(const LoadedConferenceData &data) {
        Graph<ReviewGraphNode> graph;
        if (!ReviewGraphBuilder::populateGraph(data, graph)) {
            cout << "Could not build compatibility graph.\n";
            return;
        }

        cout << "\nCompatibility graph preview\n";
        cout << "---------------------------\n";
        cout << "Vertices: " << graph.getNumVertex()
             << " | Edges: " << ReviewGraphBuilder::countEdges(graph) << "\n\n";

        for (const Vertex<ReviewGraphNode> *vertex : graph.getVertexSet()) {
            const ReviewGraphNode node = vertex->getInfo();
            if (!node.isReviewer()) {
                continue;
            }

            cout << left << setw(12) << ("Reviewer " + std::to_string(node.id)) << " -> ";

            const auto edges = vertex->getAdj();
            if (edges.empty()) {
                cout << "(no compatible submissions)\n";
                continue;
            }

            bool first = true;
            for (const Edge<ReviewGraphNode> *edge : edges) {
                const ReviewGraphNode destination = edge->getDest()->getInfo();
                if (!first) {
                    cout << ", ";
                }
                first = false;
                cout << "S" << destination.id << " [" << edge->getWeight() << "]";
            }
            cout << "\n";
        }

        cout << "\n";
    }
}

void ReviewGraphVisualizer::exportGraph(const LoadedConferenceData &data) {
    if (!DataActionUtils::ensureDataLoaded(data)) {
        return;
    }

    printGraphPreview(data);

    cout << "DOT output path [review_graph.dot]: ";

    string outputPath;
    getline(std::cin, outputPath);
    if (outputPath.empty()) {
        outputPath = "review_graph.dot";
    }

    if (!ReviewGraphExporter::exportToDot(data, outputPath)) {
        cout << "Could not export graph to '" << outputPath << "'.\n";
        return;
    }

    cout << "Graph exported to " << outputPath << "\n";

    string svgPath = outputPath;
    const size_t extensionPos = svgPath.rfind('.');
    if (extensionPos != string::npos) {
        svgPath.erase(extensionPos);
    }
    svgPath += ".svg";

    if (ReviewGraphExporter::exportToSvg(data, svgPath)) {
        cout << "SVG preview exported to " << svgPath << "\n";
    } else {
        cout << "Could not render SVG preview.\n";
    }

    if (ReviewGraphExporter::exportConstructionStepsToSvg(data, svgPath)) {
        cout << "Intermediate SVG snapshots exported next to " << svgPath << "\n";
    } else {
        cout << "Could not export intermediate SVG snapshots.\n";
    }

    string gifPath = outputPath;
    const size_t gifExtensionPos = gifPath.rfind('.');
    if (gifExtensionPos != string::npos) {
        gifPath.erase(gifExtensionPos);
    }
    gifPath += ".gif";

    if (ReviewGraphExporter::exportConstructionStepsToGif(data, gifPath)) {
        cout << "Animated GIF exported to " << gifPath << "\n";
    } else {
        cout << "Could not export animated GIF.\n";
    }
}
