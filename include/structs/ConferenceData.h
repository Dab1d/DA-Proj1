#ifndef INC_1PROJ_CONFERENCEDATA_H
#define INC_1PROJ_CONFERENCEDATA_H

struct Assignment {
    int submissionId;
    int reviewerId;
    int submissionDomain; // domain from the submission side
    int reviewerDomain;   // domain from the reviewer side
};

struct RiskEntry {
    int submissionId;
    int domain;
    int missingReviews;
};

#endif //INC_1PROJ_CONFERENCEDATA_H