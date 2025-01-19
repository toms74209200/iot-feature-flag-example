## Environment

- ESP-IDF v5.3.2
- Visual Studio Code(Dev Containers)
- [usbipd-win](https://github.com/dorssel/usbipd-win)(Windows)

## Usage

### Connect ESP32 device

If you want to connect and flash ESP32 via USB, attach and mount the USB device to the container.
Change to uncomment the following line in devcontainer.json.

[**devcontainer.json**](.devcontainer/devcontainer.json)

```diff
--	// "runArgs": ["--device=/dev/ttyUSB0:/dev/ttyUSB0"],
++	"runArgs": ["--device=/dev/ttyUSB0:/dev/ttyUSB0"],
```

"Reopen in Container" in Visual Studio Code.

### Setup build configuration

Setup `sdkconfig` to use `menuconfig`.

- Wi-Fi Configuration
- MQTT Configuration
- Conpiler options > [Enable C++ run-tme type info (RTTI)](https://docs.espressif.com/projects/esp-idf/en/v5.3.2/esp32c6/api-guides/cplusplus.html#runtime-type-information-rtti)

```bash
$ idf menuconfig
```

### Flash device

```bash
$ idf flash monitor
```
