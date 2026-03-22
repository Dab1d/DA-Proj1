#ifndef INC_1PROJ_DATAENTRYPARSERS_H
#define INC_1PROJ_DATAENTRYPARSERS_H

#include <string>
#include <vector>
#include "structs/Reviewer.h"
#include "structs/Submission.h"

namespace DataEntryParsers {
    bool parseSubmission(const std::vector<std::string> &fields,
                         Submission &submission,
                         std::string &error);

    bool parseReviewer(const std::vector<std::string> &fields,
                       Reviewer &reviewer,
                       std::string &error);
}

#endif // INC_1PROJ_DATAENTRYPARSERS_H
