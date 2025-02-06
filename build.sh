#!/bin/sh
# v80, (C) Kroc Camen 2023-2025, MIT License
#
# halt on any error
set -e

# binaries we'll be using
#
ntvcm=build/ntvcm/ntvcm
wla_z80=build/wla-dx/binaries/wla-z80
wla_6502=build/wla-dx/binaries/wla-6502
wla_link=build/wla-dx/binaries/wlalink
runcpm=build/runcpm/RunCPM/RunCPM


# download and build NTVCM, if not present
#
if ! [ -d build/ntvcm ]; then
        echo Cloning NTVCM...
        git clone --recurse-submodules \
		https://github.com/davidly/ntvcm.git build/ntvcm
fi
if ! [ -f $ntvcm ]; then
        echo Building NTVCM...
        cd build/ntvcm
        ./mr.sh
        cd ../..
fi

# download and build WLA-DX, if not present
#
if ! [ -d build/wla-dx ]; then
        echo Cloning WLA-DX...
        git clone --recurse-submodules \
                https://github.com/vhelin/wla-dx.git build/wla-dx
fi
# WLA-DX uses cmake so if the makefile is not present, generate it:
if ! [ -f build/wla-dx/Makefile ]; then
        echo Generating WLA-DX Makefiles...
        cd build/wla-dx
        # (from INSTALL in build/wla-dx)
        cmake -G "Unix Makefiles" .
        cd ../..
fi
# lastly if the binary doesn't exist, compile it
if ! [ -f $wla_link ]; then
        echo Building WLA-DX...
        pwd
        cd build/wla-dx
        make wla-z80 wla-6502 wlalink
        cd ../..
fi

# download and build RunCPM, if not present
#
if ! [ -d build/runcpm ]; then
        echo Cloning RunCPM...
        git clone --recurse-submodules \
                https://github.com/MockbaTheBorg/RunCPM.git build/runcpm
fi
if ! [ -f $runcpm ]; then
        echo Building RunCPM...
        cd build/runcpm/RunCPM
        # NOTE: Mac requires "macosx" instead of "posix" here
        make posix build
        cd ../../..
fi
# delete previous build files (but not the sub-directories)
echo Clean build files...
rm -f build/*.*

# ensure the virtual floppy-disk folder exists
mkdir -p build/runcpm/RunCPM/A/0
# clear RunCPM temp files
rm -f build/runcpm/RunCPM/A/0/*.*

# build v80 assembler [v0] from WLA-DX source:
# ==============================================================================
# this builds "v80_wla.com", the WLA version of v80

# assemble,
$wla_z80 -v -I "v0" -o "build/v80_wla.o" "v80.wla"
# and link
$wla_link -v -b "v0/link.ini" "build/v80_wla.com"

echo

# build and run tests...
# ------------------------------------------------------------------------------
# function: assemble .wla source with WLA-DX-Z80
wla_z80() {
        echo "* wla-z80 $1.wla"

        # assmeble file to test.o, regardless of input name
        $wla_z80 -I "test" -o "build/test.o" "test/$1.wla"
        # link test.o to input-named *_wla.com file
        $wla_link -b "test/link.ini" "build/$1_wla.com"
}

# function: assemble a .v80 file using the WLA-built [v0] version of v80
v80_wla() {
        echo "* v80_wla.com $1 $2"
        cd build
        ./ntvcm/ntvcm -l v80_wla.com "$1" "$2" # > "${1%.*}.sym"
        cd ..
}

# function: run a comparison between WLA-DX-Z80 [PC] & v80_wla [v0]
run_test_wla() {
        # build "$1.wla" with WLA-DX [PC],
        wla_z80 "$1"
        # and "$1.v80" with v80_wla [v0]
        v80_wla "$1"  
}

# copy test v80/v65 files to build directory for running tests
cp test/*.v?? build

# copy the COM files into the RunCPM disk directory
cp build/*.com build/runcpm/RunCPM/A/0
# copy test v80/v65 files
cp test/*.v?? build/runcpm/RunCPM/A/0

run_test_wla "z80"

echo
echo OK.
exit 0

