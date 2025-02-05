#!/bin/sh
# v80, (C) Kroc Camen 2023-2025, MIT License
#
# halt on any error
set -e

# binaries we'll be using
#
ntvcm=build/ntvcm/ntvcm
wladxz80=build/wla-dx/binaries/wla-z80
wladx6502=build/wla-dx/binaries/wla-6502
wlalink=build/wla-dx/binaries/wlalink
runcpm=build/runcpm/RunCPM/RunCPM


# download and build NTVCM, if not present
#
if ! [ -e build/ntvcm ]; then
        echo Cloning NTVCM...
        git clone --recurse-submodules \
		https://github.com/davidly/ntvcm.git build/ntvcm
fi
if ! [ -e $ntvcm ]; then
        echo Building NTVCM...
        cd build/ntvcm
        ./mr.sh
        cd ../..
fi

# download and build WLA-DX, if not present
#
if ! [ -e build/wla-dx ]; then
        echo Cloning WLA-DX...
        git clone --recurse-submodules \
                https://github.com/vhelin/wla-dx.git build/wla-dx
fi
# WLA-DX uses cmake so if the makefile is not present, generate it:
if ! [ -e build/wla-dx/Makefile ]; then
        echo Generating WLA-DX Makefiles...
        cd build/wla-dx
        # (from INSTALL in build/wla-dx)
        cmake -G "Unix Makefiles" .
        cd ../..
fi
# lastly if the binary doesn't exist, compile it
if ! [ -e $wlalink ]; then
        echo Building WLA-DX...
        pwd
        cd build/wla-dx
        make wla-z80 wla-6502 wlalink
        cd ../..
fi

# download and build RunCPM, if not present
#
if ! [ -e build/runcpm ]; then
        echo Cloning RunCPM...
        git clone --recurse-submodules \
                https://github.com/MockbaTheBorg/RunCPM.git build/runcpm
fi
if ! [ -e $runcpm ]; then
        echo Building RunCPM...
        cd build/runcpm/RunCPM
        make posix build
        cd ../../..
fi

# delete previous build files (but not the sub-directories)
echo Clean build files...
rm -f build/*.*
 
echo
echo OK.