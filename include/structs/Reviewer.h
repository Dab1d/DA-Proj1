#ifndef INC_1PROJ_REVIEWER_H
#define INC_1PROJ_REVIEWER_H
#include <string>

struct Reviewer {
    int id;
    std::string name, email;
    int primaryExpertise;
    int secondaryExpertise; // -1 if absent
};

#endif //INC_1PROJ_REVIEWER_H