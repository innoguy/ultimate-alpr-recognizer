# ultimate-alpr-recognizer

This project is a modular C++ ALPR (Automatic License Plate Recognition) tool that uses the [ultimateALPR-SDK](https://github.com/DoubangoTelecom/ultimateALPR-SDK) for high-performance license plate recognition.

It supports multiple input sources:
- Static images (JPEG, PNG, etc.)
- Video files (MP4, AVI, etc.)
- Webcam
- RTSP streams

## 📦 Dependencies

- CMake ≥ 3.10
- OpenCV (`libopencv-dev`)
- `nlohmann-json3-dev`
- `ultimateALPR-SDK`
- `Intel OpenVINO`
---

## 🔧 Setup

Run the provided script to install everything:

```bash
bash setup.sh
