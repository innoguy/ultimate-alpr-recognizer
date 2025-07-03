# syntax=docker/dockerfile:1

FROM ubuntu:22.04
WORKDIR /app
ARG DEBIAN_FRONTEND=noninteractive
COPY . . 
RUN apt update
RUN apt -y install git vim
RUN chmod +x setup.sh
RUN ./setup.sh
ENTRYPOINT ["/app/ultimate-alpr-recognizer/build/recognizer"]
