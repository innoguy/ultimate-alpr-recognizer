FROM ubuntu:22.04
WORKDIR /app
ARG DEBIAN_FRONTEND=noninteractive
COPY . . 
RUN apt update
RUN apt -y install git build-essential cmake vim wget libopencv-dev pkg-config nlohmann-json3-dev libgtk-3-dev libgl1-mesa-glx
RUN git clone https://github.com/DoubangoTelecom/ultimateALPR-SDK.git
RUN mkdir build
RUN cd build
RUN cmake ..
CMD ["bash"]
