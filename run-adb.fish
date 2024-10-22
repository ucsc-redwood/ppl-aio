#!/usr/bin/env fish

# get executable name from command line
if test (count $argv) -lt 1
    echo "Usage: $argv[0] <executable-name>"
    exit 1
end
set executable $argv[1]

# or 'arm64-v8a'
set local_executable_path "./build/android/armeabi-v7a/release/$executable"
set executable_path "/data/local/tmp/$executable"
set vulkan_shader_path /data/local/tmp/

# Function to run the executable on a specific device
function run_on_device
    set device $argv[1]
    adb -s $device push ./shaders/compiled_shaders/*.spv $vulkan_shader_path
    adb -s $device push ./$local_executable_path /data/local/tmp/
    adb -s $device shell chmod +x $executable_path
    adb -s $device shell $executable_path

end

# Main task: find all connected devices and run the executable on each
set devices (adb devices | awk 'NR>1 && $1!="list" && $1!="" {print $1}')

if test -z "$devices"
    echo "No devices connected."
else
    for device in $devices
        echo "Running on device: $device"
        run_on_device $device
    end
end
