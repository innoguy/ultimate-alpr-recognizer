# 🚗 ultimate-alpr-recognizer

This project is a C++ implementation of a ALPR (Automatic License Plate Recognition) tool powered by the [ultimateALPR-SDK](https://github.com/DoubangoTelecom/ultimateALPR-SDK).  
Supports processing JPEG images, MP4 videos, webcams, and RTSP streams.  
Built for Linux (x86_64).

---

## ✨ Features

- 📷 Image input (JPEG, PNG, etc.)
- 📼 Video input (MP4, AVI, etc.)
- 🎥 Webcam live capture
- 🌐 RTSP stream support
- 🟩 Overlays green boxes + recognized text on plates
- 📁 Saves results in timestamped JSON (e.g., `output_20250628_144500.json`)

---

## 📦 Dependencies

Installable via the provided setup script:

- CMake (≥ 3.10)
- OpenCV (`libopencv-dev`)
- `nlohmann-json3-dev`
- ultimateALPR SDK
- Intel OpenVINO (x86_64 only)

---

## ⚙️ Setup

```bash
bash setup.sh
```

This will:
1. Clone the ultimateALPR-SDK
2. Install required packages
3. Download TensorFlow runtime
4. Build the project via CMake
5. Output a sample run command

---

## 🚀 Run Example

```bash
LD_LIBRARY_PATH=./ultimateALPR-SDK/binaries/linux/x86_64 ./build/recognizer \
  --webcam 0 \
  --assets ./ultimateALPR-SDK/assets \
  --save true \
  --display true
```

---

## 🛠️ Command Line Options

| Option             | Description                                             |
|--------------------|---------------------------------------------------------|
| `--image <path>`   | Path to a static image (e.g. `.jpg`)                    |
| `--video <path>`   | Path to a video file (e.g. `.mp4`)                      |
| `--rtsp <url>`     | RTSP stream URL                                         |
| `--webcam <index>` | Webcam device index (e.g., `0`)                         |
| `--assets <path>`  | Path to the ultimateALPR assets folder                  |
| `--charset <type>` | Character set (`latin`, `korean`, `chinese`) - default: `latin` |
| `--minscore <val>` | Minimum score to accept result - default: `0.3`         |
| `--save <bool>`    | Save results as JSON (default: `true`)                  |
| `--display <bool>` | Show detection in window (default: `true`)              |

---

## 🧪 Architecture

- `recognizer.cpp`: Program entry point
- `alpr_engine.*`: ALPR engine wrapper using ultimateALPR SDK
- `input_handler.*`: Abstracts input sources (image, video, webcam, RTSP)
---

