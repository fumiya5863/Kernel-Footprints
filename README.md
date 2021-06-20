# Kernel-Footprints

## Overview
いろいろなカーネルモジュール

## Requirement
- Ubuntu20.04.2
- kernel5.8.0-53
- GCC

## How to install

HTTP
```shell
git clone https://github.com/fumiya5863/ImmatureShell.git
```

SSH
```shell
git clone git@github.com:fumiya5863/Kernel-Footprints.git
```

## How to use(HelloWorld)

```shell
sudo apt install linux-headers-$(uname -r)
cd Kernel-Footprints
cd HelloWorld
make
sudo insmod hello.ko
dmesg
sudo rmmod hello
make clean
```

## step
- [1.HelloWorld](https://github.com/fumiya5863/Kernel-Footprints/tree/main/HelloWorld)
- [2.Param](https://github.com/fumiya5863/Kernel-Footprints/tree/main/Param)
- [3.CharacterDeviceDriver1](https://github.com/fumiya5863/Kernel-Footprints/tree/main/CharacterDeviceDriver1)
- [4.CharacterDeviceDriver2](https://github.com/fumiya5863/Kernel-Footprints/tree/main/CharacterDeviceDriver2)
- [5.Ioctl](https://github.com/fumiya5863/Kernel-Footprints/tree/main/Ioctl)

## Author
[fumiya5863](https://github.com/fumiya5863)
