#include "logic/DataActionUtils.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <iostream>

using std::cout;

namespace {
    bool isDatasetNumber(const std::string &value) {
        return !value.empty() &&
               std::all_of(value.begin(), value.end(), [](unsigned char ch) {
                   return std::isdigit(ch);
               });
    }

    std::filesystem::path findDefaultInputDirectory() {
        namespace fs = std::filesystem;

        fs::path current = fs::current_path();
        while (!current.empty()) {
            const fs::path candidate = current / "data" / "DataSamples" / "input";
            if (fs::exists(candidate) && fs::is_directory(candidate)) {
                return candidate;
            }

            if (current == current.root_path()) {
                break;
            }

            current = current.parent_path();
        }

        return {};
    }
}

bool DataActionUtils::ensureDataLoaded(const LoadedConferenceData &data) {
    if (data.isLoaded()) {
        return true;
    }

    cout << "No dataset loaded yet. Use option 1 first.\n";
    return false;
}

std::string DataActionUtils::resolveInputFilePath(const std::string &filePath) {
    namespace fs = std::filesystem;

    const fs::path inputPath(filePath);
    const fs::path defaultInputDirectory = findDefaultInputDirectory();

    if (fs::exists(inputPath)) {
        return inputPath.string();
    }

    if (!defaultInputDirectory.empty()) {
        const fs::path defaultDatasetPath = defaultInputDirectory / inputPath;
        if (fs::exists(defaultDatasetPath)) {
            return defaultDatasetPath.string();
        }
    }

    if (!inputPath.has_extension()) {
        fs::path inputPathWithCsv = inputPath;
        inputPathWithCsv += ".csv";

        if (fs::exists(inputPathWithCsv)) {
            return inputPathWithCsv.string();
        }

        if (!defaultInputDirectory.empty()) {
            const fs::path defaultDatasetPathWithCsv = defaultInputDirectory / inputPathWithCsv;
            if (fs::exists(defaultDatasetPathWithCsv)) {
                return defaultDatasetPathWithCsv.string();
            }
        }
    }

    if (!defaultInputDirectory.empty() && isDatasetNumber(filePath)) {
        const fs::path numberedDatasetPath = defaultInputDirectory / ("dataset" + filePath + ".csv");
        if (fs::exists(numberedDatasetPath)) {
            return numberedDatasetPath.string();
        }
    }

    return filePath;
}

std::string DataActionUtils::getDisplayFileName(const std::string &sourceFile) {
    if (sourceFile.empty()) {
        return sourceFile;
    }

    return std::filesystem::path(sourceFile).filename().string();
}

void DataActionUtils::printTopic(int topic) {
    if (topic == -1) {
        cout << "-";
    } else {
        cout << topic;
    }
}
