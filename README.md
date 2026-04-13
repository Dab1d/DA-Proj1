# Conference Assignment System

> Automated conference paper-to-reviewer assignment using max-flow network algorithms.

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/CMake-3.10%2B-blue.svg)](https://cmake.org/)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](#license)

---

## Overview

Loads paper submissions and reviewer profiles from CSV, validates the dataset, and computes optimal paper-to-reviewer assignments while respecting domain expertise, capacity constraints, conflict-of-interest rules, and reviewer risk scoring.

The core algorithm models the assignment problem as a max-flow network: submissions and reviewers become nodes, edges encode compatibility and capacity, and a standard augmenting-path solver finds the optimal assignment in one pass.

---

## Table of Contents

- [Quick Start](#quick-start)
- [Usage](#usage)
- [Project Structure](#project-structure)
- [Architecture](#architecture)
- [CSV Format](#csv-format)
- [Test Datasets](#test-datasets)
- [Building the Docs](#building-the-docs)

---

## Quick Start

**Prerequisites:** C++17 compiler (GCC, Clang, or MSVC), CMake 3.10+

```bash
git clone https://github.com/Dab1d/Conference-Assignment.git
cd Conference-Assignment
mkdir build && cd build
cmake ..
cmake --build .
```

---

## Usage

### Interactive mode

```bash
./main
```

1. Select **Load Data File** and choose a CSV from `data/DataSamples/`
2. Browse loaded data — submissions, reviewers, parameters
3. Select **Run Assignment** to execute the max-flow solver
4. Inspect the generated assignments

### Batch mode

```bash
./main -b input.csv output.csv
```

Validates the input CSV, runs the assignment algorithm, writes results to `output.csv`, and prints a summary to stdout. Suitable for scripting and CI pipelines.

---

## Project Structure

```
Conference-Assignment/
├── data/
│   └── DataSamples/          # 14 test datasets (.csv)
├── include/
│   ├── algorithms/           # Max-flow solver & assignment logic
│   ├── graphics/             # Menu, interactive & batch mode UI
│   ├── io/                   # CSV parsing, loading, error handling
│   ├── logic/                # Business logic & data viewers
│   └── structs/              # Core data models (Submission, Reviewer, …)
├── src/
│   ├── main.cpp
│   ├── graphics/
│   ├── io/
│   └── logic/
├── docs/                     # Doxygen output (git-ignored)
├── CMakeLists.txt
├── Doxyfile
└── README.md
```

---

## Architecture

### Components

| Component | Location | Responsibility |
|-----------|----------|----------------|
| Entry point | `src/main.cpp` | Mode detection (interactive vs. batch) |
| UI layer | `src/graphics/` | Menu rendering, user interaction |
| CSV I/O | `src/io/` | File reading, tokenising, validation |
| Business logic | `src/logic/` | Assignment actions, data viewers |
| Data models | `include/structs/` | `Submission`, `Reviewer`, `Parameters`, graph nodes |
| Algorithm | `include/algorithms/` | Max-flow solver, assignment output |

### Data Flow

```
User input (file path or -b flags)
  └─▶ main.cpp — mode detection
        └─▶ DataActionUtils — path resolution
              └─▶ DataLoader
                    ├── CsvUtils            — tokenise rows
                    ├── DataEntryParsers     — submissions & reviewers
                    └── ParameterConfigParsers — params & controls
                          └─▶ LoadedConferenceData
                                ├── SubmissionViewer
                                ├── ReviewerViewer
                                ├── ParameterViewer
                                └── MaxFlowAssigner — produces assignments
```

### Key classes

- `Submission` — paper model (id, domains, required reviews, conflicts)
- `Reviewer` — reviewer model (id, domains, capacity, risk flag)
- `Parameters` — solver configuration (min reviews, max conflicts)
- `LoadedConferenceData` — in-memory dataset container
- `DataLoader` — orchestrates CSV reading and validation
- `MaxFlowAssigner` — builds the flow network and computes the assignment
- `ErrorHandler` — centralised error reporting

---

## CSV Format

A single input file contains four sections. See `data/DataSamples/` for complete examples.

### `#Submissions`

```csv
id,domain1,domain2,num_reviews,conflict
S1,AI,NLP,3,
S2,Databases,Data_Mining,3,R4
```

| Field | Required | Description |
|-------|----------|-------------|
| `id` | yes | Unique paper identifier |
| `domain1` | yes | Primary research domain |
| `domain2` | no | Secondary domain |
| `num_reviews` | yes | Number of reviewers required |
| `conflict` | no | Comma-separated conflicted reviewer IDs |

### `#Reviewers`

```csv
id,domain1,domain2,capacity,is_risky
R1,AI,NLP,5,0
R2,AI,Databases,4,0
```

| Field | Required | Description |
|-------|----------|-------------|
| `id` | yes | Unique reviewer identifier |
| `domain1` | yes | Primary expertise domain |
| `domain2` | no | Secondary domain |
| `capacity` | yes | Maximum papers this reviewer can handle |
| `is_risky` | yes | `1` if reviewer is flagged as risky, else `0` |

### `#Parameters`

```csv
parameter,value
min_reviews,3
max_conflict,1
```

### `#Control`

```csv
control,enabled
secondary_domains,1
risk_analysis,1
```

| Control | Effect |
|---------|--------|
| `secondary_domains` | Include `domain2` fields when matching |
| `risk_analysis` | Apply penalty weights for risky reviewers |

---

## Data Validation

The loader checks:

- All four sections (`#Submissions`, `#Reviewers`, `#Parameters`, `#Control`) are present
- Required fields exist and contain valid types
- Submission and reviewer IDs are unique
- Total reviewer capacity satisfies total review demand

Validation errors are printed to stderr with the offending row and field.

---

## Test Datasets

14 datasets in `data/DataSamples/`. See `data/DataSamples/README.md` for full descriptions.

| Dataset | Difficulty | Scenario |
|---------|------------|----------|
| `dataset1.csv` | Basic | Minimal input, single domain, no conflicts |
| `dataset2–5.csv` | Intermediate | Increasing scale, capacity stress |
| `dataset6–10.csv` | Intermediate | Conflict-of-interest and mixed domains |
| `dataset11–13.csv` | Advanced | Secondary domains enabled |
| `dataset14.csv` | Advanced | Full feature set — risk analysis + conflicts + secondary domains |

---

## Building the Docs

```bash
doxygen Doxyfile
# Output: docs/html/index.html
```

---

## Contributing

```bash
git checkout -b feature/your-feature
git commit -m "feat: describe your change"
git push origin feature/your-feature
# Open a pull request
```
