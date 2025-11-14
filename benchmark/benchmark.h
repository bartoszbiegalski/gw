#pragma once
#include "core/types.h"
#include "core/Object.h"
#include "io/GmlImport.h"
#include "io/GmlExport.h"
#include "release/static_config.h"
#include "utils/string_operations.h"
#include "utils/tree_operations.h"
#include "services/XmlConfig.h"
#include "services/XmlParser.h"
#include "services/XsdParser.h"
#include "logic/GmlDivide.h"
#include "logic/GmlMerge.h"
#include "uprofile.h"

// Benchmark.cpp
// Defines different types of benchmarks

namespace gml_benchmark
{
    void import_benchmark(const std::filesystem::path &filePath, const std::string &benchmarkFilePath);

    void parse_xsd_benchmark(std::unique_ptr<Object> &obj, const std::string &benchmarkFilePath);

    void divide_benchmark(std::unique_ptr<Object> &obj, std::vector<NamespacePrefix> &nsVec, const std::string &benchmarkFilePath);

    void merge_benchmark(const std::vector<std::unique_ptr<Object>> &baseObjects, std::unique_ptr<Object> &mergedObject, const std::string &benchmarkFilePath);

    void export_benchmark(std::unique_ptr<Object> &obj, const std::string &benchmarkFilePath);

    // void get_attribute_benchmark(const std::unique_ptr<Object> &obj, const std::string id)
    // {
    //     auto attr = obj.get()->getGmlStorage().getGmlMap().at(id);
    //     std::cout<<attr.size()
    // }
};