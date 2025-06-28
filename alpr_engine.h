// alpr_engine.h
#ifndef ALPR_ENGINE_H
#define ALPR_ENGINE_H

#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include "alpr_options.h"
#include <ultimateALPR-SDK-API-PUBLIC.h>
#include <fstream>

class AlprEngine {
public:
	AlprEngine();
	~AlprEngine();
    bool initialize(const AlprOptions& options);
    void processFrame(const cv::Mat& frame);
    void shutdown();

private:
    AlprOptions m_options;
	std::ofstream m_outputFile;
	ultimateAlprSdk::UltAlprSdkResult m_result;
};

#endif // ALPR_ENGINE_H