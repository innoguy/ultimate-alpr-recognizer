// alpr_engine.cpp
#include "alpr_engine.h"
#include <iostream>
#include <sstream>
#include <chrono>
#include <iomanip>

using namespace ultimateAlprSdk;
using json = nlohmann::json;


static const std::string WINDOW_NAME = "ALPR Results";

AlprEngine::AlprEngine() {
}

AlprEngine::~AlprEngine() {
    if (m_outputFile.is_open()) {
        m_outputFile.close();
    }
    if (m_options.displayResults) {
        cv::destroyWindow(WINDOW_NAME);
    }
}

bool AlprEngine::initialize(const AlprOptions& options) {
	m_options=options;
	
	if (options.saveData) {
        // Generate timestamp in YYYYMMDD_HHMMSS format
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::stringstream timestamp;
        timestamp << std::put_time(std::localtime(&time), "%Y%m%d_%H%M%S");
        std::string filename = "output_" + timestamp.str() + ".json";

        m_outputFile.open(filename, std::ios::out | std::ios::app);
        if (!m_outputFile.is_open()) {
            std::cerr << "Error: Failed to open output file: " << filename << std::endl;
            return false;
        }
        std::cout << "Saving results to: " << filename << std::endl;
    }
	if (options.displayResults) {
        cv::namedWindow(WINDOW_NAME, cv::WINDOW_AUTOSIZE);
    }
	
    std::stringstream jsonConfig;
    jsonConfig << "{\"debug_level\": \"info\","
               << "\"assets_folder\": \"" << options.assets << "\","
               << "\"charset\": \"" << options.charset << "\","
               << "\"recogn_minscore\": " << options.recogn_minscore << "}";
    m_result = UltAlprSdkEngine::init(jsonConfig.str().c_str());
    return m_result.isOK();
}

void AlprEngine::processFrame(const cv::Mat& frame) {
    if (frame.empty()) 
		return;
    m_result = UltAlprSdkEngine::process(ULTALPR_SDK_IMAGE_TYPE_BGR24,
                                       frame.data,
                                       frame.cols,
                                       frame.rows);
    if (m_result.isOK()) {
        std::string jsonResult = m_result.json();
        std::cout << jsonResult << std::endl;
        if (m_outputFile.is_open() && m_options.saveData) {
            m_outputFile << jsonResult << "\n";
            m_outputFile.flush(); // Ensure data is written
        }
		
		if (m_options.displayResults) {
            cv::Mat annotatedFrame = frame.clone();

            try {
                // Parse JSON result
                json j = json::parse(jsonResult);
                if (j.contains("plates") && j["plates"].is_array()) {
                    for (const auto& plate : j["plates"]) {
                        // Extract plate data
                        std::string text = plate.contains("text") ? plate["text"].get<std::string>() : "Unknown";
                        if (plate.contains("warpedBox") && plate["warpedBox"].is_array() && plate["warpedBox"].size() == 8) {
                            // Extract the four corners of the warpedBox
                            std::vector<cv::Point> points = {
                                cv::Point(static_cast<int>(plate["warpedBox"][0].get<float>()),
                                          static_cast<int>(plate["warpedBox"][1].get<float>())),
                                cv::Point(static_cast<int>(plate["warpedBox"][2].get<float>()),
                                          static_cast<int>(plate["warpedBox"][3].get<float>())),
                                cv::Point(static_cast<int>(plate["warpedBox"][4].get<float>()),
                                          static_cast<int>(plate["warpedBox"][5].get<float>())),
                                cv::Point(static_cast<int>(plate["warpedBox"][6].get<float>()),
                                          static_cast<int>(plate["warpedBox"][7].get<float>()))
                            };

                            // Draw the quadrilateral (closed polyline)
                            cv::polylines(annotatedFrame,
                                          points,
                                          true, // Closed shape
                                          cv::Scalar(0, 255, 0), // Green
                                          2);

                            // Draw plate number above the top-left corner
                            cv::Point textPos(points[0].x, points[0].y - 10);
                            cv::putText(annotatedFrame,
                                        text,
                                        textPos,
                                        cv::FONT_HERSHEY_SIMPLEX,
                                        0.6,
                                        cv::Scalar(0, 255, 0), // Green
                                        2);
                        }
                    }
                }
            } catch (const json::exception& e) {
                std::cerr << "Error parsing JSON: " << e.what() << std::endl;
            }

            // Display the annotated image
            cv::imshow(WINDOW_NAME, annotatedFrame);
            cv::waitKey(1); // Short delay to allow window update
		}
    }
}

void AlprEngine::shutdown() {
    UltAlprSdkEngine::deInit();
}