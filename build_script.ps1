# Preset Selection
param (

    #Telling user mandatory input for preset

    [switch]$c=$false, # Clean Build
    [switch]$r=$false, # Release Build
    [switch]$d=$false # Debug Normal Build

    #Clean build remove the build directory, and rebuild the thing. While release preset is build normally for embedded system. if no input, generate in normal debug mode
)



#Clean Build
if ( $c )
{
    echo "CLEAN BUILD..."
    rm -R -Force build #Recurse that delete everything including subfolder
}

$mode="Debug"

if ( $r )
{   
    echo "RELEASE BUILD..."
    $mode="Release"
}

#NORMAL
if ( $d )
{   
    echo "DEBUG BUILD..."
}

cmake -S . -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE="$PWD\cmake\gcc-arm-none-eabi.cmake" -DCMAKE_BUILD_TYPE="$mode"
cmake --build build

#Execution Policy of your machine may stop build script, run "Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass"