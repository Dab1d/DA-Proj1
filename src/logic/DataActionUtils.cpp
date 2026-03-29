#include "logic/DataActionUtils.h"
#include <filesystem>
#include <iostream>

using std::cout;
namespace fs = std::filesystem;

static bool isDatasetNumber(const std::string &value) {
    int n = std::stoi(value);
    return n >= 1 && n <= 100;
}


static fs::path findDefaultInputDirectory() {
    for (fs::path p = fs::current_path(); p != p.root_path(); p = p.parent_path())
        if (fs::is_directory(p / "data" / "DataSamples" / "input"))
            return p / "data" / "DataSamples" / "input";
    return {};
}

bool DataActionUtils::ensureDataLoaded(const LoadedConferenceData &data) {
    if (!data.isLoaded())
        cout << "No dataset loaded yet. Use option 1 first.\n";
    return data.isLoaded();
}

std::string DataActionUtils::resolveInputFilePath(const std::string &filePath) {
    const fs::path inputPath(filePath);
    const fs::path defaultDir = findDefaultInputDirectory();

    auto resolve = [](const fs::path &p) -> std::string {
        return fs::exists(p) ? p.string() : "";
    };

    if (auto r = resolve(inputPath); !r.empty()) return r;
    if (!defaultDir.empty())
        if (auto r = resolve(defaultDir / inputPath); !r.empty()) return r;

    if (!inputPath.has_extension()) {
        const fs::path withCsv = fs::path(inputPath) += ".csv";
        if (auto r = resolve(withCsv); !r.empty()) return r;
        if (!defaultDir.empty())
            if (auto r = resolve(defaultDir / withCsv); !r.empty()) return r;
    }

    if (!defaultDir.empty() && isDatasetNumber(filePath))
        if (auto r = resolve(defaultDir / ("dataset" + filePath + ".csv")); !r.empty()) return r;

    return filePath;
}

std::string DataActionUtils::getDisplayFileName(const std::string &sourceFile) {
    return sourceFile.empty() ? sourceFile : fs::path(sourceFile).filename().string();
}

void DataActionUtils::printTopic(int topic) {
    topic == -1 ? cout << "-" : cout << topic;
}
