#include "io/DataEntryParsers.h"

#include "io/CsvUtils.h"

namespace DataEntryParsers {
    bool parseSubmission(const std::vector<std::string> &fields,
                         Submission &submission,
                         std::string &error) {
        if (fields.size() != 6) {
            error = "submission entry must have 6 fields";
            return false;
        }

        if (!CsvUtils::tryParseInteger(fields[0], submission.id)) {
            error = "submission id must be an integer";
            return false;
        }

        submission.title = CsvUtils::stripQuotes(fields[1]);
        submission.authors = CsvUtils::stripQuotes(fields[2]);
        submission.email = CsvUtils::stripQuotes(fields[3]);

        if (submission.title.empty() || submission.authors.empty() || submission.email.empty()) {
            error = "submission title, authors and e-mail are mandatory";
            return false;
        }

        if (!CsvUtils::tryParseInteger(fields[4], submission.primaryTopic)) {
            error = "submission primary topic must be an integer";
            return false;
        }

        if (!CsvUtils::parseOptionalInteger(fields[5], submission.secondaryTopic)) {
            error = "submission secondary topic must be an integer when present";
            return false;
        }

        return true;
    }

    bool parseReviewer(const std::vector<std::string> &fields,
                       Reviewer &reviewer,
                       std::string &error) {
        if (fields.size() != 5) {
            error = "reviewer entry must have 5 fields";
            return false;
        }

        if (!CsvUtils::tryParseInteger(fields[0], reviewer.id)) {
            error = "reviewer id must be an integer";
            return false;
        }

        reviewer.name = CsvUtils::stripQuotes(fields[1]);
        reviewer.email = CsvUtils::stripQuotes(fields[2]);

        if (reviewer.name.empty() || reviewer.email.empty()) {
            error = "reviewer name and e-mail are mandatory";
            return false;
        }

        if (!CsvUtils::tryParseInteger(fields[3], reviewer.primaryExpertise)) {
            error = "reviewer primary expertise must be an integer";
            return false;
        }

        if (!CsvUtils::parseOptionalInteger(fields[4], reviewer.secondaryExpertise)) {
            error = "reviewer secondary expertise must be an integer when present";
            return false;
        }

        return true;
    }
}
