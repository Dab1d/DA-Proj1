#include "graphics/BatchMode.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "io/DataLoader.h"
#include "io/ErrorHandler.h"
#include "io/AssignmentWriter.h"
#include "logic/DataActionUtils.h"
#include "logic/NetworkBuilder.h"
#include "algorithms/MaxFlow.h"
#include "algorithms/AssignmentExtractor.h"
#include "logic/RiskAnalyzer.h"
#include "structs/ConferenceData.h"

using std::cout;
using std::string;
using std::vector;

void BatchMode::run(const string& inputFile, const string& outputFile) {
    // 1. Load data
    LoadedConferenceData data;
    vector<string> errors;
    const string resolved = DataActionUtils::resolveInputFilePath(inputFile);

    if (!DataLoader::loadFromCsv(resolved, data, errors)) {
        ErrorHandler::printLoadErrors(errors);
        return;
    }

    data.parameters.outputFileName = outputFile;

    // 2. Build flow network
    Graph<int> g = NetworkBuilder::build(data);

    // 3. Run Edmonds-Karp
    double flow = MaxFlow::run(g, 0, 1);

    // 4. Check if assignment is feasible
    int S = data.submissions.size();
    int required = S * data.parameters.minReviewsPerSubmission;
    std::vector<Assignment> assignments;
    if (flow > 0) {
        assignments = AssignmentExtractor::extract(g, data);
    }

    std::ofstream out(outputFile);
    if (!out.is_open()) {
        std::cerr << "Could not open output file: " << outputFile << "\n";
        return;
    }

    if (data.parameters.generateAssignments > 0 && (int)flow >= required) {
        AssignmentWriter::writeAssignments(assignments, out);
        cout << "Assignment successful. Output written to: " << outputFile << "\n";
    } else if ((int)flow < required) {
        // 5a. Infeasible — report missing reviews
        auto missing = AssignmentExtractor::getMissing(g, data);
        AssignmentWriter::writeMissing(missing, out);
        cout << "Assignment not fully feasible. Missing reviews written to: " << outputFile << "\n";
    } else {
        cout << "Assignment computed but not reported (GenerateAssignments=0).\n";
    }

    if (data.parameters.riskAnalysis == 1) {
        auto risky = RiskAnalyzer::analyzeK1(data);
        std::sort(risky.begin(), risky.end());
        AssignmentWriter::writeRiskAnalysis(risky, 1, out);
    } else if (data.parameters.riskAnalysis > 1) {
        auto riskyCombos = RiskAnalyzer::analyzeK(data, data.parameters.riskAnalysis);
        AssignmentWriter::writeRiskAnalysisCombinations(riskyCombos, data.parameters.riskAnalysis, out);
    }
}
