// alpr_options.h
#ifndef ALPR_OPTIONS_H
#define ALPR_OPTIONS_H

#include <string>

struct AlprOptions {
    std::string imagePath;
    std::string videoPath;
    std::string rtspUrl;
    int webcamIndex = -1;
    std::string assets = "../ultimateALPR-SDK/assets"; // Default path
    std::string charset = "latin";
	float recogn_minscore = 0.3f;
	bool saveData = true;
	bool displayResults = true;
};

#endif // ALPR_OPTIONS_H