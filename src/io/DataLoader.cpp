#include "io/DataLoader.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <set>
#include <unordered_set>
#include "io/CsvUtils.h"
#include "io/DataEntryParsers.h"
#include "io/ParameterConfigParsers.h"

using std::string;
using std::unordered_set;
using std::vector;
using std::set;


static void addError(vector<string> &errors, const string &message) {
    errors.push_back(message);
}

static void addLineError(vector<string> &errors, int lineNumber, const string &message) {
    addError(errors, "line " + std::to_string(lineNumber) + ": " + message);
}

static void addMissingSectionError(vector<string> &errors, const string &sectionName) {
    addError(errors, "missing " + sectionName + " section");
}

static void addMissingNamedValueError(vector<string> &errors,
                                      const string &label,
                                      const string &key) {
    addError(errors, "missing " + label + " '" + key + "'");
}

static bool updateCurrentSection(const string &trimmedLine,
                                 const string &submissionsSection,
                                 const string &reviewersSection,
                                 const string &parametersSection,
                                 const string &controlSection,
                                 string &currentSection,
                                 set<string> &seenSections) {
    if (trimmedLine != submissionsSection && trimmedLine != reviewersSection &&
        trimmedLine != parametersSection && trimmedLine != controlSection)
        return false;

    currentSection = trimmedLine;
    seenSections.insert(currentSection);
    return true;
}

static bool isIgnoredLine(const string &trimmedLine) {
    return trimmedLine.empty() || trimmedLine.front() == '#';
}

static bool isUnknownSectionHeader(const string &trimmedLine) {
    return trimmedLine.size() > 1 &&
           trimmedLine.front() == '#' &&
           trimmedLine.find(',') == string::npos &&
           !std::isspace(static_cast<unsigned char>(trimmedLine[1]));
}

static bool parseSubmissionLine(const vector<string> &fields, int lineNumber,
                                unordered_set<int> &submissionIds,
                                LoadedConferenceData &parsedData,
                                vector<string> &errors) {
    Submission submission{};
    string error;
    if (!DataEntryParsers::parseSubmission(fields, submission, error)) {
        addLineError(errors, lineNumber, error);
        return false;
    }

    if (!submissionIds.insert(submission.id).second) {
        addLineError(errors, lineNumber, "duplicate submission id " + std::to_string(submission.id));
        return false;
    }

    parsedData.submissions.push_back(submission);
    return true;
}

static bool parseReviewerLine(const vector<string> &fields, int lineNumber, unordered_set<int> &reviewerIds,
                              LoadedConferenceData &parsedData,
                              vector<string> &errors) {
    Reviewer reviewer{};
    string error;
    if (!DataEntryParsers::parseReviewer(fields, reviewer, error)) {
        addLineError(errors, lineNumber, error);
        return false;
    }

    if (!reviewerIds.insert(reviewer.id).second) {
        addLineError(errors, lineNumber, "duplicate reviewer id " + std::to_string(reviewer.id));
        return false;
    }

    parsedData.reviewers.push_back(reviewer);
    return true;
}

static bool parseConfigLine(const vector<string> &fields,
                            const string &currentSection,
                            const string &parametersSection,
                            int lineNumber,
                            LoadedConferenceData &parsedData,
                            set<string> &parameterKeys,
                            set<string> &controlKeys,
                            vector<string> &errors) {
    if (fields.size() != 2) {
        addLineError(errors, lineNumber, "key/value entries must have exactly 2 fields");
        return false;
    }

    string error;
    const bool ok = currentSection == parametersSection
                        ? ParameterConfigParsers::setParameter(parsedData.parameters, fields[0], fields[1],
                                                               parameterKeys, error)
                        : ParameterConfigParsers::setControl(parsedData.parameters, fields[0], fields[1],
                                                             controlKeys, error);

    if (!ok) {
        addLineError(errors, lineNumber, error);
    }

    return ok;
}

static void validateSeenSections(const set<string> &seenSections,
                                 const string &submissionsSection,
                                 const string &reviewersSection,
                                 const string &parametersSection,
                                 const string &controlSection,
                                 vector<string> &errors) {
    if (!seenSections.count(submissionsSection)) {
        addMissingSectionError(errors, submissionsSection);
    }
    if (!seenSections.count(reviewersSection)) {
        addMissingSectionError(errors, reviewersSection);
    }
    if (!seenSections.count(parametersSection)) {
        addMissingSectionError(errors, parametersSection);
    }
    if (!seenSections.count(controlSection)) {
        addMissingSectionError(errors, controlSection);
    }
}

static void validateLoadedData(const LoadedConferenceData &parsedData,
                               const set<string> &parameterKeys,
                               const set<string> &controlKeys,
                               vector<string> &errors) {
    if (parsedData.submissions.empty()) {
        addError(errors, "no submissions were loaded");
    }
    if (parsedData.reviewers.empty()) {
        addError(errors, "no reviewers were loaded");
    }

    const vector<string> requiredParameters = {
        "MinReviewsPerSubmission",
        "MaxReviewsPerReviewer",
        "PrimaryReviewerExpertise",
        "SecondaryReviewerExpertise",
        "PrimarySubmissionDomain",
        "SecondarySubmissionDomain"
    };

    for (const string &key: requiredParameters) {
        if (!parameterKeys.count(key)) {
            addMissingNamedValueError(errors, "parameter", key);
        }
    }

    const vector<string> requiredControls = {
        "GenerateAssignments",
        "RiskAnalysis",
        "OutputFileName"
    };

    for (const string &key: requiredControls) {
        if (!controlKeys.count(key)) {
            addMissingNamedValueError(errors, "control entry", key);
        }
    }
}

static void sortLoadedData(LoadedConferenceData &parsedData) {
    std::sort(parsedData.submissions.begin(), parsedData.submissions.end(),
              [](const Submission &lhs, const Submission &rhs) {
                  return lhs.id < rhs.id;
              });

    std::sort(parsedData.reviewers.begin(), parsedData.reviewers.end(),
              [](const Reviewer &lhs, const Reviewer &rhs) {
                  return lhs.id < rhs.id;
              });
}


bool LoadedConferenceData::isLoaded() const {
    return !sourceFile.empty();
}

bool DataLoader::loadFromCsv(const string &filePath,
                             LoadedConferenceData &data,
                             vector<string> &errors) {
    errors.clear();
    LoadedConferenceData parsedData;
    parsedData.sourceFile = filePath;
    std::ifstream input(filePath);
    if (!input.is_open()) {
        addError(errors, "could not open file '" + filePath + "'");
        return false;
    }


    const string submissionsSection = "#Submissions";
    const string reviewersSection = "#Reviewers";
    const string parametersSection = "#Parameters";
    const string controlSection = "#Control";

    string currentSection;
    unordered_set<int> submissionIds;
    unordered_set<int> reviewerIds;
    set<string> parameterKeys;
    set<string> controlKeys;
    set<string> seenSections;

    string line;
    int lineNumber = 0;

    while (std::getline(input, line)) {
        ++lineNumber;
        string trimmedLine = CsvUtils::trim(line);

        if (updateCurrentSection(trimmedLine,  submissionsSection, reviewersSection,
                                 parametersSection, controlSection, currentSection, seenSections)) {
            continue;
        }

        if (isUnknownSectionHeader(trimmedLine)) {
            addLineError(errors, lineNumber, "unknown section header '" + trimmedLine + "'");
            continue;
        }

        if (isIgnoredLine(trimmedLine)) {
            continue;
        }

        string content = CsvUtils::removeInlineComment(line);
        if (content.empty()) {
            continue;
        }

        vector<string> fields = CsvUtils::splitCsvLine(content);

        if (currentSection == submissionsSection) {
            parseSubmissionLine(fields, lineNumber, submissionIds, parsedData, errors);
            continue;
        }

        if (currentSection == reviewersSection) {
            parseReviewerLine(fields, lineNumber, reviewerIds, parsedData, errors);
            continue;
        }

        if (currentSection == parametersSection || currentSection == controlSection) {
            parseConfigLine(fields, currentSection, parametersSection, lineNumber,
                            parsedData, parameterKeys, controlKeys, errors);
            continue;
        }

        addLineError(errors, lineNumber, "data found before any valid section header");
    }

    validateSeenSections(seenSections, submissionsSection, reviewersSection,
                         parametersSection, controlSection, errors);
    validateLoadedData(parsedData, parameterKeys, controlKeys, errors);

    if (!errors.empty()) {
        return false;
    }

    sortLoadedData(parsedData);

    data = parsedData;
    return true;
}
