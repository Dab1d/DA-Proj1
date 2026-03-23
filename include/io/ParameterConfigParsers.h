#ifndef INC_1PROJ_PARAMETERCONFIGPARSERS_H
#define INC_1PROJ_PARAMETERCONFIGPARSERS_H

#include <set>
#include <string>
#include "structs/Parameters.h"

/**
 * @brief Parsers for parameter and control key/value entries.
 */
namespace ParameterConfigParsers {
    /**
     * @brief Applies one parameter entry to the loaded configuration.
     * @param parameters Configuration object being updated.
     * @param key Parameter name.
     * @param value Parameter value as text.
     * @param seenKeys Set of parameter keys already processed.
     * @param error Human-readable error message on failure.
     * @return `true` if the parameter was accepted; `false` otherwise.
     * @time_complexity O(log k + |key| + |value|), where k is the number of
     * previously seen parameter keys.
     */
    bool setParameter(Parameters &parameters,
                      const std::string &key,
                      const std::string &value,
                      std::set<std::string> &seenKeys,
                      std::string &error);

    /**
     * @brief Applies one control entry to the loaded configuration.
     * @param parameters Configuration object being updated.
     * @param key Control entry name.
     * @param value Control entry value as text.
     * @param seenKeys Set of control keys already processed.
     * @param error Human-readable error message on failure.
     * @return `true` if the control entry was accepted; `false` otherwise.
     * @time_complexity O(log k + |key| + |value|), where k is the number of
     * previously seen control keys.
     */
    bool setControl(Parameters &parameters,
                    const std::string &key,
                    const std::string &value,
                    std::set<std::string> &seenKeys,
                    std::string &error);
}

#endif // INC_1PROJ_PARAMETERCONFIGPARSERS_H
