FROM debian:bookworm AS install-esp-idf

RUN apt-get update && apt-get -y install --no-install-recommends \
    git \
    wget \
    flex \
    bison \
    gperf \
    python3 \
    python3-venv \
    cmake \
    ninja-build \
    ccache \
    libffi-dev \
    libssl-dev \
    dfu-util \
    libusb-1.0-0

ARG ESP_IDF_VERSTION="latest"
ARG TARGET_DEVICE="esp32"

RUN mkdir -p /opt && cd /opt \
    && git clone --filter=blob:none --no-checkout -b ${ESP_IDF_VERSTION} --recursive https://github.com/espressif/esp-idf.git \
    && cd /opt/esp-idf \
    && git sparse-checkout init --cone \
    && git sparse-checkout set components tools \
    && git checkout \
    && git submodule update --init --recursive \
    && ./install.sh ${TARGET_DEVICE}

FROM mcr.microsoft.com/devcontainers/cpp:1-debian-12

COPY --from=install-esp-idf /opt/esp-idf /opt/esp-idf
COPY --from=install-esp-idf --chown=vscode /root/.espressif /home/vscode/.espressif

ARG REINSTALL_CMAKE_VERSION_FROM_SOURCE="none"

# Optionally install the cmake for vcpkg
COPY ./reinstall-cmake.sh /tmp/

RUN if [ "${REINSTALL_CMAKE_VERSION_FROM_SOURCE}" != "none" ]; then \
        chmod +x /tmp/reinstall-cmake.sh && /tmp/reinstall-cmake.sh ${REINSTALL_CMAKE_VERSION_FROM_SOURCE}; \
    fi \
    && rm -f /tmp/reinstall-cmake.sh

# [Optional] Uncomment this section to install additional vcpkg ports.
# RUN su vscode -c "${VCPKG_ROOT}/vcpkg install <your-port-name-here>"

# [Optional] Uncomment this section to install additional packages.
# RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
#     && apt-get -y install --no-install-recommends <your-package-list-here>
RUN apt-get update && apt-get -y install --no-install-recommends \
    libffi-dev \
    libssl-dev \
    dfu-util \
    libusb-1.0-0

COPY ./idf /opt/idf
RUN echo "source /opt/esp-idf/export.sh" >> /home/vscode/.bashrc \
    && ln -s /opt/idf /usr/bin/idf
