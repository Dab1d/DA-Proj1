#include "io/ParameterConfigParsers.h"

#include "io/CsvUtils.h"

namespace ParameterConfigParsers {
    bool setParameter(Parameters &parameters,
                      const std::string &key,
                      const std::string &value,
                      std::set<std::string> &seenKeys,
                      std::string &error) {
        int parsedValue = 0;

        if (!seenKeys.insert(key).second) {
            error = "duplicate parameter '" + key + "'";
            return false;
        }

        if (key == "MinReviewsPerSubmission") {
            if (!CsvUtils::tryParseInteger(value, parsedValue) || parsedValue < 0) {
                error = "MinReviewsPerSubmission must be a non-negative integer";
                return false;
            }
            parameters.minReviewsPerSubmission = parsedValue;
            return true;
        }

        if (key == "MaxReviewsPerReviewer") {
            if (!CsvUtils::tryParseInteger(value, parsedValue) || parsedValue < 0) {
                error = "MaxReviewsPerReviewer must be a non-negative integer";
                return false;
            }
            parameters.maxReviewsPerReviewer = parsedValue;
            return true;
        }

        if (key == "PrimaryReviewerExpertise") {
            if (!CsvUtils::tryParseInteger(value, parsedValue) || parsedValue < 0) {
                error = "PrimaryReviewerExpertise must be a non-negative integer";
                return false;
            }
            parameters.primaryReviewerExpertise = parsedValue;
            return true;
        }

        if (key == "SecondaryReviewerExpertise") {
            if (!CsvUtils::tryParseInteger(value, parsedValue) || parsedValue < 0) {
                error = "SecondaryReviewerExpertise must be a non-negative integer";
                return false;
            }
            parameters.secondaryReviewerExpertise = parsedValue;
            return true;
        }

        if (key == "PrimarySubmissionDomain") {
            if (!CsvUtils::tryParseInteger(value, parsedValue) || parsedValue < 0) {
                error = "PrimarySubmissionDomain must be a non-negative integer";
                return false;
            }
            parameters.primarySubmissionDomain = parsedValue;
            return true;
        }

        if (key == "SecondarySubmissionDomain") {
            if (!CsvUtils::tryParseInteger(value, parsedValue) || parsedValue < 0) {
                error = "SecondarySubmissionDomain must be a non-negative integer";
                return false;
            }
            parameters.secondarySubmissionDomain = parsedValue;
            return true;
        }

        error = "unknown parameter '" + key + "'";
        return false;
    }

    bool setControl(Parameters &parameters,
                    const std::string &key,
                    const std::string &value,
                    std::set<std::string> &seenKeys,
                    std::string &error) {
        int parsedValue = 0;

        if (!seenKeys.insert(key).second) {
            error = "duplicate control entry '" + key + "'";
            return false;
        }

        if (key == "GenerateAssignments") {
            if (!CsvUtils::tryParseInteger(value, parsedValue) || parsedValue < 0 || parsedValue > 3) {
                error = "GenerateAssignments must be an integer between 0 and 3";
                return false;
            }
            parameters.generateAssignments = parsedValue;
            return true;
        }

        if (key == "RiskAnalysis") {
            if (!CsvUtils::tryParseInteger(value, parsedValue) || parsedValue < 0) {
                error = "RiskAnalysis must be a non-negative integer";
                return false;
            }
            parameters.riskAnalysis = parsedValue;
            return true;
        }

        if (key == "OutputFileName") {
            parameters.outputFileName = CsvUtils::stripQuotes(value);
            if (parameters.outputFileName.empty()) {
                error = "OutputFileName cannot be empty";
                return false;
            }
            return true;
        }

        error = "unknown control entry '" + key + "'";
        return false;
    }
}
