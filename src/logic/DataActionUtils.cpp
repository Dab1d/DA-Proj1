#include "logic/DataActionUtils.h"

#include <filesystem>
#include <iostream>

using std::cout;
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
    if (fs::exists(inputPath)) {
        return inputPath.string();
    }

    const fs::path defaultDatasetPath = fs::current_path() / "data" / "DataSamples" / "input" / inputPath;
    if (fs::exists(defaultDatasetPath)) {
        return defaultDatasetPath.string();
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
