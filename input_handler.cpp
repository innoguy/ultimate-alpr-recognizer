// input_handler.cpp
#include "input_handler.h"
#include "alpr_options.h"
#include <memory>
#include <iostream>

class VideoInput : public InputHandler {
    cv::VideoCapture cap;
    std::string source;
public:
    VideoInput(const std::string& src) : source(src) {}
    bool open() override {
        bool success = cap.open(source);
        if (!success) {
            std::cerr << "Failed to open video source: " << source << std::endl;
        }
        return success;
    }
    bool hasNextFrame() override { 
		return cap.isOpened(); 
	}
    cv::Mat getNextFrame() override {
        cv::Mat frame;
        cap >> frame;
        return frame;
    }
    ~VideoInput() override { 
		cap.release(); 
	}
};

class WebcamInput : public InputHandler {
    cv::VideoCapture cap;
    int index;
public:
    WebcamInput(int idx) : index(idx) {}
    bool open() override {
        bool success = cap.open(index);
        if (!success) {
            std::cerr << "Failed to open webcam index: " << index << std::endl;
        }
        return success;
    }
    bool hasNextFrame() override { 
		return cap.isOpened(); 
	}
    cv::Mat getNextFrame() override {
        cv::Mat frame;
        cap >> frame;
        return frame;
    }
    ~WebcamInput() override { 
		cap.release(); 
	}
};

class ImageInput : public InputHandler {
    cv::Mat image;
    bool consumed = false;
    std::string path;
public:
    ImageInput(const std::string& p) : path(p) {}
    bool open() override {
        image = cv::imread(path);
        if (image.empty()) {
            std::cerr << "Failed to load image: " << path << std::endl;
            return false;
        }
        return true;
    }
    bool hasNextFrame() override {
		return !consumed; 
	}
    cv::Mat getNextFrame() override {
        consumed = true;
        return image;
    }
};

std::unique_ptr<InputHandler> InputHandler::create(const AlprOptions& options) {
    if (!options.imagePath.empty()) return std::make_unique<ImageInput>(options.imagePath);
    if (!options.videoPath.empty()) return std::make_unique<VideoInput>(options.videoPath);
    if (!options.rtspUrl.empty()) return std::make_unique<VideoInput>(options.rtspUrl);
    if (options.webcamIndex >= 0) return std::make_unique<WebcamInput>(options.webcamIndex);
    return nullptr;
}