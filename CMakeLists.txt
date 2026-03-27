cmake_minimum_required(VERSION 3.10)
project(1proj)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(main
        src/main.cpp
        src/logic/DataActionUtils.cpp
        src/logic/Control.cpp
        src/logic/DataFileLoader.cpp
        src/logic/AssignmentRunner.cpp
        src/logic/ReviewGraphBuilder.cpp
        src/logic/ReviewGraphExporter.cpp
        src/logic/ReviewGraphVisualizer.cpp
        src/logic/SubmissionViewer.cpp
        src/logic/ReviewerViewer.cpp
        src/logic/ParameterViewer.cpp
        src/graphics/InteractiveMode.cpp
        src/graphics/Menu.cpp
        src/graphics/BatchMode.cpp
        src/io/CsvUtils.cpp
        src/io/DataEntryParsers.cpp
        src/io/DataLoader.cpp
        src/io/ErrorPrinter.cpp
        src/io/ParameterConfigParsers.cpp
)
