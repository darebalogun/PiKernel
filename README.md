# PiKernel
A bootable 64bit kernel for the raspberry pi 3.

## Getting Started
Follow the instructions below to install this kernel on your physical Raspberry Pi 3(RPi3) device or alternatively emulate it using Qemu.
As of the time of writing this Qemu still does not support certain RPi3 peripherals like the system timer within the GPU so certain features will not work.

### Prerequisites
1. A Raspberry Pi 3 with memory card with [NOOBS](https://www.raspberrypi.org/downloads/noobs/)
2. A monitor and HDMI cable
3. UART to USB adapter
4. Toolchain: Aarch64 ELF baremetal target cross compiler available [here](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-a/downloads)
5. [Qemu Emulator](https://www.qemu.org/download/) (optional)

### Installation Instructions
1. Clone this repository
```
$ git clone https://github.com/darebalogun/PiKernel.git
```
2. cd into the repository folder
```
cd PiKernel
```
3. cd into the build folder
```
cd build
```
4. Make clean
```
make clean
```
5. If running on Qemu: make run
```
make run
```
6. If running on hardware: make
```
make
```
7. Insert memory card into your PC and copy 'kernel8.img' onto the boot folder on the memory card
8. Disable device tree by adding the following line to config.txt
```
device_tree=
```
9. Replace the memory card in the RPi and power on to boot

## Contributing
Contributions welcome through pull requests

## Acknowledgements
