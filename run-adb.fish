#!/usr/bin/env fish

# get executable name from command line
if test (count $argv) -lt 1
    echo "Usage: $argv[0] <executable-name>"
    exit 1
end
set executable $argv[1]

set local_executable_path "./build/android/armeabi-v7a/release/$executable"
# set local_executable_path "./build/android/arm64-v8a/release/$executable"

# Function to run the executable on a specific device
function run_on_device
    set device $argv[1]
    adb -s $device push ./$local_executable_path /data/local/tmp/
    adb -s $device shell chmod +x /data/local/tmp/$executable
    adb -s $device shell /data/local/tmp/$executable
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
