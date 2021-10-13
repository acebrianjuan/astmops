# SPDX-FileCopyrightText: 2020-2021 Álvaro Cebrián Juan <acebrianjuan@gmail.com>
# SPDX-License-Identifier: GPL-3.0-or-later

# Use Ubuntu 18.04 Bionic.
FROM ubuntu:bionic

# Install dependencies.
RUN apt-get update && apt-get install -y --no-install-recommends \
    file ca-certificates build-essential ninja-build cmake \
    qtbase5-dev qtpositioning5-dev libgeographic-dev geographiclib-tools

# Install EGM96 geoid.
RUN geographiclib-get-geoids egm96-5

# Create /app and /app/build directories.
RUN mkdir -p /app/build

# Copy project source code to /app directory.
COPY . /app

# Set /app/build as working directory.
WORKDIR /app/build

# Run configure step.
RUN cmake -DCMAKE_INSTALL_PREFIX=/usr/ -DCMAKE_BUILD_TYPE=Release -GNinja ..

# Run build step.
RUN ninja

# Run check step.
RUN ctest --output-on-failure

# Run install step.
RUN DESTDIR=./AppDir ninja install

# Copy GeographicLib data into AppDir.
RUN cp -r /usr/share/GeographicLib ./AppDir/usr/share/

# Download linuxdeploy and its Qt plugin and make them executable.
RUN wget https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
RUN wget https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage
RUN chmod +x linuxdeploy*.AppImage

# Build AppImage.
RUN APPIMAGE_EXTRACT_AND_RUN=1 QMAKE=/usr/lib/qt5/bin/qmake ./linuxdeploy-x86_64.AppImage --appdir AppDir --plugin qt --output appimage

