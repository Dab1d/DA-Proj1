#include "io/DataLoader.h"

#include <algorithm>
#include <fstream>
#include <set>
#include <unordered_set>
#include "io/CsvUtils.h"
#include "io/DataEntryParsers.h"
#include "io/ParameterConfigParsers.h"

bool LoadedConferenceData::isLoaded() const {
    return !sourceFile.empty();
}

bool DataLoader::loadFromCsv(const std::string &filePath,
                             LoadedConferenceData &data,
                             std::vector<std::string> &errors) {
    errors.clear();
    LoadedConferenceData parsedData;
    parsedData.sourceFile = filePath;
    std::ifstream input(filePath);
    if (!input.is_open()) {
        errors.push_back("could not open file '" + filePath + "'");
        return false;
    }


    const std::string submissionsSection = "#Submissions";
    const std::string reviewersSection = "#Reviewers";
    const std::string parametersSection = "#Parameters";
    const std::string controlSection = "#Control";

    std::string currentSection;
    std::unordered_set<int> submissionIds;
    std::unordered_set<int> reviewerIds;
    std::set<std::string> parameterKeys;
    std::set<std::string> controlKeys;
    std::set<std::string> seenSections;

    std::string line;
    int lineNumber = 0;

    while (std::getline(input, line)) {
        ++lineNumber;
        std::string trimmedLine = CsvUtils::trim(line);

        if (trimmedLine.empty()) {
            continue;
        }

        if (trimmedLine == submissionsSection) {
            currentSection = submissionsSection;
            seenSections.insert(currentSection);
            continue;
        }

        if (trimmedLine == reviewersSection) {
            currentSection = reviewersSection;
            seenSections.insert(currentSection);
            continue;
        }

        if (trimmedLine == parametersSection) {
            currentSection = parametersSection;
            seenSections.insert(currentSection);
            continue;
        }

        if (trimmedLine == controlSection) {
            currentSection = controlSection;
            seenSections.insert(currentSection);
            continue;
        }

        if (!trimmedLine.empty() && trimmedLine.front() == '#') {
            continue;
        }

        std::string content = CsvUtils::removeInlineComment(line);
        if (content.empty()) {
            continue;
        }

        std::vector<std::string> fields = CsvUtils::splitCsvLine(content);

        if (currentSection == submissionsSection) {
            Submission submission{};
            std::string error;
            if (!DataEntryParsers::parseSubmission(fields, submission, error)) {
                errors.push_back("line " + std::to_string(lineNumber) + ": " + error);
                continue;
            }

            if (!submissionIds.insert(submission.id).second) {
                errors.push_back("line " + std::to_string(lineNumber) +
                                 ": duplicate submission id " + std::to_string(submission.id));
                continue;
            }

            parsedData.submissions.push_back(submission);
            continue;
        }

        if (currentSection == reviewersSection) {
            Reviewer reviewer{};
            std::string error;
            if (!DataEntryParsers::parseReviewer(fields, reviewer, error)) {
                errors.push_back("line " + std::to_string(lineNumber) + ": " + error);
                continue;
            }

            if (!reviewerIds.insert(reviewer.id).second) {
                errors.push_back("line " + std::to_string(lineNumber) +
                                 ": duplicate reviewer id " + std::to_string(reviewer.id));
                continue;
            }

            parsedData.reviewers.push_back(reviewer);
            continue;
        }

        if (currentSection == parametersSection || currentSection == controlSection) {
            if (fields.size() != 2) {
                errors.push_back("line " + std::to_string(lineNumber) +
                                 ": key/value entries must have exactly 2 fields");
                continue;
            }

            std::string error;
            bool ok = currentSection == parametersSection
                          ? ParameterConfigParsers::setParameter(parsedData.parameters, fields[0], fields[1],
                                                                parameterKeys, error)
                          : ParameterConfigParsers::setControl(parsedData.parameters, fields[0], fields[1],
                                                              controlKeys, error);

            if (!ok) {
                errors.push_back("line " + std::to_string(lineNumber) + ": " + error);
            }
            continue;
        }

        errors.push_back("line " + std::to_string(lineNumber) +
                         ": data found before any valid section header");
    }

    if (!seenSections.count(submissionsSection)) {
        errors.push_back("missing #Submissions section");
    }
    if (!seenSections.count(reviewersSection)) {
        errors.push_back("missing #Reviewers section");
    }
    if (!seenSections.count(parametersSection)) {
        errors.push_back("missing #Parameters section");
    }
    if (!seenSections.count(controlSection)) {
        errors.push_back("missing #Control section");
    }
    if (parsedData.submissions.empty()) {
        errors.push_back("no submissions were loaded");
    }
    if (parsedData.reviewers.empty()) {
        errors.push_back("no reviewers were loaded");
    }

    const std::vector<std::string> requiredParameters = {
        "MinReviewsPerSubmission",
        "MaxReviewsPerReviewer",
        "PrimaryReviewerExpertise",
        "SecondaryReviewerExpertise",
        "PrimarySubmissionDomain",
        "SecondarySubmissionDomain"
    };

    for (const std::string &key : requiredParameters) {
        if (!parameterKeys.count(key)) {
            errors.push_back("missing parameter '" + key + "'");
        }
    }

    const std::vector<std::string> requiredControls = {
        "GenerateAssignments",
        "RiskAnalysis",
        "OutputFileName"
    };

    for (const std::string &key : requiredControls) {
        if (!controlKeys.count(key)) {
            errors.push_back("missing control entry '" + key + "'");
        }
    }

    if (!errors.empty()) {
        return false;
    }

    std::sort(parsedData.submissions.begin(), parsedData.submissions.end(),
              [](const Submission &lhs, const Submission &rhs) {
                  return lhs.id < rhs.id;
              });

    std::sort(parsedData.reviewers.begin(), parsedData.reviewers.end(),
              [](const Reviewer &lhs, const Reviewer &rhs) {
                  return lhs.id < rhs.id;
              });

    data = parsedData;
    return true;
}
