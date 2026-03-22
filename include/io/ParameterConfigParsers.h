#ifndef INC_1PROJ_PARAMETERCONFIGPARSERS_H
#define INC_1PROJ_PARAMETERCONFIGPARSERS_H

#include <set>
#include <string>
#include "structs/Parameters.h"

namespace ParameterConfigParsers {
    bool setParameter(Parameters &parameters,
                      const std::string &key,
                      const std::string &value,
                      std::set<std::string> &seenKeys,
                      std::string &error);

    bool setControl(Parameters &parameters,
                    const std::string &key,
                    const std::string &value,
                    std::set<std::string> &seenKeys,
                    std::string &error);
}

#endif // INC_1PROJ_PARAMETERCONFIGPARSERS_H
