# PPL All-in-One (AIO)

## Platforms Supported

* Generic Linux 
* Nvidia Jetson Orin 
* Android

## Setups

### Android

```bash
xmake f -p android --ndk=~/Android/Sdk/ndk/28.0.12433566/ --android_sdk=~/Android/Sdk/ --ndk_sdkver=24 -c
```

the `--ndk_sdkver=24` is the magic line for building google `benchmark` library

For GPU, we use `Vulkan`

```bash
sudo apt update
sudo apt install glslang-tools

# check installation is correct
glslangValidator --version
```
#### on Ubuntu 24.04

Basically you want to install vulkan SDK on the system, follow
 https://vulkan.lunarg.com/sdk/home#android

```bash
wget -qO - https://packages.lunarg.com/lunarg-signing-key-pub.asc | sudo apt-key add -
sudo wget -qO /etc/apt/sources.list.d/lunarg-vulkan-1.3.296-noble.list https://packages.lunarg.com/vulkan/1.3.296/lunarg-vulkan-1.3.296-noble.list
sudo apt update
sudo apt install vulkan-sdk
```

#### volk.h

To make Vulkan easily work on Android. We use `volk` library. 

