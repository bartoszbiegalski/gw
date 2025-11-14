#include "benchmark.h"

// Benchmark.cpp
// Defines different types of benchmarks

namespace gml_benchmark
{
    void import_benchmark(const std::filesystem::path &filePath, const std::string &benchmarkFilePath)
    {
        uprofile::start(benchmarkFilePath.c_str());
        uprofile::startCPUUsageMonitoring(100);
        uprofile::startSystemMemoryMonitoring(100);
        auto cfg = std::make_unique<XmlConfig>(static_config::staticData);
        std::unique_ptr<Object> obj = std::make_unique<Object>();
        GmlImport::Import(filePath, obj);
        NamespaceTool::Process(cfg, obj);
        uprofile::timeEnd("file_import");
        uprofile::timeBegin("xml_parse");
        XmlParser::SetContent(cfg, obj);
        uprofile::timeEnd("xml_parse");
        obj.reset();
        cfg.reset();
        uprofile::stop();
    }

    void parse_xsd_benchmark(std::unique_ptr<Object> &obj, const std::string &benchmarkFilePath)
    {
        auto cfg = std::make_unique<XmlConfig>(static_config::staticData);

        uprofile::start(benchmarkFilePath.c_str());
        uprofile::startCPUUsageMonitoring(100);
        uprofile::startSystemMemoryMonitoring(100);
        uprofile::timeBegin("xsd_parse");
        XsdParser::Parse(cfg, obj);
        uprofile::timeEnd("xsd_parse");
        cfg.reset();
        uprofile::stop();
    }

    void divide_benchmark(std::unique_ptr<Object> &obj, std::vector<NamespacePrefix> &nsVec, const std::string &benchmarkFilePath)
    {
        auto objVec = std::vector<std::unique_ptr<Object>>();
        auto cfg = std::make_unique<XmlConfig>(static_config::staticData);
        if (nsVec.size() < 1)
        {
            for (auto i : obj.get()->getGmlStorage().getGmlMap())
            {
                nsVec.push_back(i.first);
            }
        }
        uprofile::start(benchmarkFilePath.c_str());
        uprofile::startCPUUsageMonitoring(100);
        uprofile::startSystemMemoryMonitoring(100);
        uprofile::timeBegin("gml_divide");
        GmlDivide::Divide(cfg, obj, nsVec, objVec);
        uprofile::timeEnd("gml_divide");
        cfg.reset();
        uprofile::stop();
    }

    void merge_benchmark(const std::vector<std::unique_ptr<Object>> &baseObjects, std::unique_ptr<Object> &mergedObject, const std::string &benchmarkFilePath)
    {
        auto objVec = std::vector<std::unique_ptr<Object>>();
        auto cfg = std::make_unique<XmlConfig>(static_config::staticData);
        uprofile::start(benchmarkFilePath.c_str());
        uprofile::startCPUUsageMonitoring(100);
        uprofile::startSystemMemoryMonitoring(100);
        uprofile::timeBegin("gml_merge");
        printf("lala\n");
        GmlMerge::Merge(cfg, baseObjects, mergedObject);
        uprofile::timeEnd("gml_merge");
        GmlExport::Export(cfg, mergedObject);
        cfg.reset();
        uprofile::stop();
    }

    void export_benchmark(std::unique_ptr<Object> &obj, const std::string &benchmarkFilePath)
    {
        auto cfg = std::make_unique<XmlConfig>(static_config::staticData);
        uprofile::start(benchmarkFilePath.c_str());
        uprofile::startCPUUsageMonitoring(100);
        uprofile::startSystemMemoryMonitoring(100);
        uprofile::timeBegin("gml_export");
        GmlExport::Export(cfg, obj);
        uprofile::timeEnd("gml_export");
        cfg.reset();
        uprofile::stop();
    }

    // void get_attribute_benchmark(const std::unique_ptr<Object> &obj, const std::string id)
    // {
    //     auto attr = obj.get()->getGmlStorage().getGmlMap().at(id);
    //     std::cout<<attr.size()
    // }
};