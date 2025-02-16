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
#runcpm=build/runcpm/RunCPM/RunCPM


# download and build NTVCM, if not present:
# <https://github.com/davidly/ntvcm>
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

# download and build WLA-DX, if not present:
# <https://github.com/vhelin/wla-dx>
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

## download and build RunCPM, if not present:
## <https://github.com/MockbaTheBorg/RunCPM>
##
#if ! [ -d build/runcpm ]; then
#        echo Cloning RunCPM...
#        git clone --recurse-submodules \
#                https://github.com/MockbaTheBorg/RunCPM.git build/runcpm
#fi
#if ! [ -f $runcpm ]; then
#        echo Building RunCPM...
#        cd build/runcpm/RunCPM
#        # NOTE: Mac requires "macosx" instead of "posix" here
#        make posix build
#        cd ../../..
#fi

# delete previous build files (but not the sub-directories)
echo Clean build files...
rm -f build/*.*

## ensure the virtual floppy-disk folder exists
#mkdir -p build/runcpm/RunCPM/A/0
## clear RunCPM temp files
#rm -f build/runcpm/RunCPM/A/0/*.*


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
#           note: "_wla" is appended to the generated .com filename
wla_z80() {
        echo "* wla-z80 $1.wla"

        # assmeble file to test.o, regardless of input name
        $wla_z80 -I "test" -o "build/test.o" "test/$1.wla"
        # link test.o to input-named *_wla.com file
        $wla_link -b "test/link.ini" "build/${1}_wla.com"
}

# function: assemble a .v80 file using the WLA-built [v0] version of v80
#           note: "_wla" is appended to the generated .com filename
v80_wla() {
        echo "* v80_wla.com $1 $2"
        cd build
        # we want to capture the NTVCM return value and check for HALT status
        # but `set -e` will normally terminate the script. `set -e` will not 
        # trigger if a `&&` or `||` chain is used. by appending `&& true` we
        # retain the return value if it is non-zero. technique adapted from:
        # <https://stackoverflow.com/a/11231970>
        #
        ./ntvcm/ntvcm -l -p v80_wla.com "$1" "$2" > "${1%.*}.sym" && true
        err=$?
        if ! [ "$err" -eq "0" ]
        then
                # when an error occurs, print the log
                # that should display the assembly error
                cat "${1%.*}.sym"
                exit $err
        fi
        cd ..
}

# function: run a comparison between WLA-DX-Z80 [PC] & v80_wla [v0]
run_test_wla() {
        # assemble "$1.wla" with WLA-DX [PC]
        # (generates "$1_wla.com")
        wla_z80 "$1"
        # and "$1.v80" with v80_wla [v0]
        # (generates "$1.com")
        v80_wla "$1.v80"
        # compare binaries
        diff --brief --side-by-side "build/${1}_wla.com" "build/$1.com"
}

# copy test v80 files to build directory for running tests
cp test/*.v80 build

## copy the COM files into the RunCPM disk directory
#cp build/*.com build/runcpm/RunCPM/A/0
## and the test v80 files
#cp test/*.v80 build/runcpm/RunCPM/A/0

# TEST: build "z80.wla" & "z80.v80" and compare binaries
run_test_wla "z80"
# TEST: relative jump distances
run_test_wla "jr"


# build v80 [v1] using v80 [v0]!
# ==============================================================================
# copy v80 [v1] source into build directory
cp v1/*.v?? build
## and to RunCPM
#cp v1/*.v?? build/runcpm/RunCPM/A/0

# do a 1st-generation build of v80 [v1] using v80 [v0]!
v80_wla "cpm_z80.v80" "v80.com"

# build and run tests...
# ------------------------------------------------------------------------------
# function: assemble a .v80 file using v80 [v1]
v80() {
        echo "* v80.com $1 $2"
        cd build
        # we want to capture the NTVCM return value and check for HALT status
        # but `set -e` will normally terminate the script. `set -e` will not 
        # trigger if a `&&` or `||` chain is used. by appending `&& true` we
        # retain the return value if it is non-zero. technique adapted from:
        # <https://stackoverflow.com/a/11231970>
        #
        ./ntvcm/ntvcm -l -p v80.com "$1" "$2" > "${1%.*}.sym" && true; err=$?
        if ! [ "$err" -eq "0" ]
        then
                # when an error occurs, print the log
                # that should display the assembly error
                cat "${1%.*}.sym"
                exit $err
        fi
        cd ..
}

# function: run a comparison between WLA-DX-Z80 [PC] & v80 [v1]
run_test_z80() {
        # assemble "$1.wla" with WLA-DX [PC]
        # (generates "$1_wla.com")
        wla_z80 "$1"
        # and "$1.v80" with v80 [v1]
        # (generates "$1.com")
        v80 "$1.v80"
        # compare binaries
        diff --brief --side-by-side "build/$1_wla.com" "build/$1.com"
}

# TEST: build "z80.wla" & "z80.v80" and compare binaries
run_test_z80 "z80"

# build v65:
# ==============================================================================
# use v80 to assemble a version of v80 that assembles 6502 code...
v80 "cpm_6502.v80" "v65.com"

## copy binary to RunCPM directory
#cp build/v65.com build/runcpm/RunCPM/A/0

# build and run tests...
# ------------------------------------------------------------------------------
# function: assemble .wla source with WLA-DX-6502
#           note: "_wla" is appended to the generated .prg filename
wla_6502() {
        echo "* wla-6502 $1.wla"

        # assmeble file to test.o, regardless of input name
        $wla_6502 -I "test" -o "build/test.o" "test/$1.wla"
        # link test.o to input-named *_wla.com file
        $wla_link -b "test/link.ini" "build/${1}_wla.prg"
}

# function: assemble a file with v65
v65() {
        echo "* v65.com $1 $2"
        cd build
        # assemble the source file with v65 and write stdout to symbol file
        #
        # we want to capture the NTVCM return value and check for HALT status
        # but `set -e` will normally terminate the script. `set -e` will not 
        # trigger if a `&&` or `||` chain is used. by appending `&& true` we
        # retain the return value if it is non-zero. technique adapted from:
        # <https://stackoverflow.com/a/11231970>
        #
        ./ntvcm/ntvcm -l -p v65.com "$1" "$2" > "${1%.*}.sym" && true; err=$?
        if ! [ "$err" -eq "0" ]
        then
                # when an error occurs, print the log
                # that should display the assembly error
                cat "${1%.*}.sym"
                ## check for error caused by the Z80 HALT instruction
                #if [ "$err" -eq "255" ]; then
                #        # rerun the assembly with RunCPM to allow for debugging
                #        # FIXME: RunCPM doesn't support lowercase
                #        # <https://github.com/MockbaTheBorg/RunCPM/issues/204>
                #        echo v65.com "$1" "$2" > "runcpm/RunCPM/AUTOEXEC.TXT"
                #        cd runcpm/RunCPM
                #        ./RunCPM
                #fi
                exit $err
        fi
        cd ..
}

# function: run a comparison between WLA-DX-6502 [PC] & v65
run_test_6502() {
        # assemble "$1.wla" with WLA-DX [PC]
        # (generates "$1_wla.prg")
        wla_6502 "$1"
        # and "$1.v65" with v65 [v1]
        # (generates "$1.prg")
        v65 "$1.v65"
        # compare binaries
        diff --brief --side-by-side "build/${1}_wla.prg" "build/$1.prg"
}

# copy test v65 files to build directory for running tests
cp test/*.v65 build

# TEST: verify all 6502 instructions
run_test_6502 "6502"


# copy binaries to release folder:
# ==============================================================================
echo "* Populate release folder..."
cp build/v*.com release/

echo
echo OK.
exit 0
