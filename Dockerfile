# Base Image
FROM ubuntu:latest

# Add Required Packages [Layer 1]
RUN apt update && \
    apt install -y ca-certificates cmake g++ git make --no-install-recommends

# Download Sources [Layer 2]
RUN cd ~ && \
    git clone https://github.com/remote-wiring/serial-wiring.git && \
    cd serial-wiring/ && \
    mkdir build

# Verify Build and Install
CMD cd ~/serial-wiring/build/ && \
    cmake .. && \
    make
