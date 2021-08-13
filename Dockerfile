# SPDX-FileCopyrightText: 2020 Álvaro Cebrián Juan <acebrianjuan@gmail.com>
# SPDX-License-Identifier: GPL-3.0-or-later

# Use Ubuntu 18.04 Bionic.
FROM ubuntu:bionic

# Install dependencies.
RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates build-essential ninja-build cmake qtbase5-dev qtpositioning5-dev \
    libgeographic-dev geographiclib-tools

# Install EGM96 geoid.
RUN geographiclib-get-geoids egm96-5

# Create /app and /app/build directories.
RUN mkdir -p /app/build

# Copy project source code to /app directory.
COPY . /app

# Set /app/build as working directory.
WORKDIR /app/build

# Run configure step.
RUN cmake -DCMAKE_INSTALL_PREFIX=/usr/ -GNinja ..

# Run build step.
RUN ninja

# Run check step.
RUN ctest --output-on-failure

# Install.
RUN DESTDIR=./AppDir ninja install

# Download linuxdeploy and its Qt plugin.
RUN wget https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
RUN wget https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage

# Make them executable.
RUN chmod +x linuxdeploy*.AppImage

# Initialize AppDir, bundle shared libraries, use Qt plugin 
# to bundle additional resources, and build AppImage.
RUN ./linuxdeploy-x86_64.AppImage --appimage-extract-and-run --appdir AppDir --plugin qt --output appimage

