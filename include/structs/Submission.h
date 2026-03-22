#ifndef INC_1PROJ_SUBMISSION_H
#define INC_1PROJ_SUBMISSION_H
#include <string>

struct Submission {
    int id;
    std::string title, authors, email;
    int primaryTopic;
    int secondaryTopic; // -1 if absent
};

#endif //INC_1PROJ_SUBMISSION_H