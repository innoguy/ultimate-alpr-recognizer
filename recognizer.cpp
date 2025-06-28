// recognizer.cpp
#include "alpr_engine.h"
#include "input_handler.h"
#include <iostream>
#include <string>
#include <map>

using namespace std;
using namespace cv;

bool parse(int argc, char* argv[], AlprOptions &options) {
	map<string, string> args;
	
	if (argc % 2 == 0) {
        cerr << "Error: Invalid number of arguments. Each flag must have a value." << endl;
        return false;
    }
	
	for (int i = 1; i + 1 < argc; i += 2) {
		args[argv[i]] = argv[i + 1];
	}

	if (args.count("--image")) 
		options.imagePath = args["--image"];
	if (args.count("--video")) 
		options.videoPath = args["--video"];
	if (args.count("--rtsp")) 
		options.rtspUrl = args["--rtsp"];
	if (args.count("--webcam")) {
        try {
            options.webcamIndex = stoi(args["--webcam"]);
        } catch (const exception& e) {
            cerr << "Error: Invalid webcam index: " << args["--webcam"] << endl;
            return false;
        }
    }
	if (args.count("--assets")) 
		options.assets = args["--assets"];
	if (args.count("--charset")) 
		options.charset = args["--charset"];
	if (args.count("--minscore")) {
		try {
			options.recogn_minscore = stof(args["--minscore"]);
		} catch (const exception& e) {
			cerr << "Invalid minscore value: " << args["--minscore"] << endl;
			return false;
		}
	}
	if (args.count("--save")) 
		options.saveData = (args["--save"] == "true");
	if (args.count("--display")) 
		options.displayResults = (args["--display"] == "true");

	// Must have one valid input
	int validInputs = (!options.imagePath.empty() ? 1 : 0) +
                      (!options.videoPath.empty() ? 1 : 0) +
                      (!options.rtspUrl.empty() ? 1 : 0) +
                      (options.webcamIndex >= 0 ? 1 : 0);

    if (validInputs == 0) {
        cerr << "Error: No input source provided (use --image, --video, --rtsp, or --webcam)." << endl;
        return false;
    }
    if (validInputs > 1) {
        cerr << "Error: Only one input source allowed (use --image, --video, --rtsp, or --webcam)." << endl;
        return false;
    }
	return true;
}

void printUsage() {
	cout << "Usage:\n"
		 << "  --image <path>\n"
		 << "  --video <path>\n"
		 << "  --rtsp <url>\n"
		 << "  --webcam <index>\n"
		 << "  --assets <path-to-assets-folder>\n"
		 << "  --charset <latin|korean|chinese>\n"
		 << "  --save <true|false>\n";
}

int main(int argc, char* argv[]) {
    AlprOptions options;
    if (!parse(argc, argv, options)) {
        printUsage();
        return 1;
    }

    AlprEngine engine;
    if (!engine.initialize(options)) {
        cerr << "Failed to initialize ALPR engine" << endl;
        return 1;
    }

    unique_ptr<InputHandler> input = InputHandler::create(options);
    if (!input || !input->open()) {
        cerr << "Failed to open input source" << endl;
        return 1;
    }

    while (input->hasNextFrame()) {
        Mat frame = input->getNextFrame();
        if (frame.empty()) 
			break;
        engine.processFrame(frame);
    }
	
	// Wait for key press if image input and display is enabled
    if (options.displayResults) {
        cv::waitKey(0); // Wait indefinitely for a key press
    }

    engine.shutdown();
    return 0;
}
