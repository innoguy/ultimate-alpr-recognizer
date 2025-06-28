// input_handler.h
#ifndef INPUT_handler_H
#define INPUT_handler_H

#include <memory>
#include <opencv2/opencv.hpp>
#include "alpr_options.h"

class InputHandler {
public:
    static std::unique_ptr<InputHandler> create(const AlprOptions& options);
    virtual ~InputHandler() = default;
    virtual bool open() = 0;
    virtual bool hasNextFrame() = 0;
    virtual cv::Mat getNextFrame() = 0;
};

#endif // INPUT_handler_H