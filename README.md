# v80

>   If a computer can’t compile and deploy its own software then it’s not a general purpose computer, it's an appliance.

_v80_ is an 8-bit native, multi-platform, multi-CPU cross-assembler that runs on real 8-bit hardware, but also on PC (via [NTVCM] or [RunCPM]). _v80_'s goal is to ensure reproducability of 8-bit software on real 8-bit hardware, both modern and retro, whilst never excluding PC-based development.

[NTVCM]:  https://github.com/davidly/ntvcm
[RunCPM]: https://github.com/MockbaTheBorg/RunCPM

_"v80"_ refers to the Z80 assembler, but versions that target different CPUs
and/or run on different host CPUs are available, or are planned:

    v80         assembles Z80 code ".v80"
    v65         assembles 6502 code ".v65", currently running on Z80

    v69*        assembles 6809 code ".v69"
    v83*        assembles GameBoy (SM83) code ".v83"

    *(coming in a future release)

If you would like to see _v80_ on your favourite CPU / system
consider raising an issue at https://github.com/kroc/v80

It currently runs on CP/M and can assemble itself with plans to port it to [Agon Light], [Zeal 8-bit OS] and other Z80-based systems. _v80_ can assemble 6502 code via its pluggable ISA support and a native 6502 port is planned.

[Zeal 8-bit OS]: https://github.com/zeal8bit/Zeal-8-bit-OS
[Agon Light]:    https://www.thebyteattic.com/p/agon.html

If you feel like you could write a C version of _v80_, [your help](https://github.com/Kroc/v80/issues/4) would be appreciated as it would remove the need to simulate CP/M to assemble on PC, with the no-directories limit that imposes.

## ~~Features~~

- No linker, we have linker at home → includes  
  (the stdout of _v80_ is a symbol file!)
- No Macros! Copy + Paste is your friend or re-use includes
- $FFFF/65'535 is the largest number in the universe
- No floating point numbers. Have you _tried_ coding floats on an 8-bit CPU??
- No shift operators. Multiply/divide by powers of 2
- No comparison operators -- we use the zero-flag here
- No commas. Commas were a mistake

## Syntax Sample:

_v80_ uses a non-standard syntax designed for parsing simplicity / speed, not compatibility with existing source code.

The basic principle is that _v80_ can only recognise a word by the first character, so everything must be prefixed with a sigil or otherwise be easily categorizable, e.g. `a`-`z` = instruction.

    ; v80: ................ ; zilog:
    ;                       ;
    add   $ff               ; add A,    $ff
    adc.c                   ; adc A,    C
    sub*hl                  ; sub A,    [HL]
    sbc.hl.bc               ; sbc HL,   BC
    ld.a*ix $00             ; ld  A,    [IX+$00]
    bit7.a                  ; bit 7,    A
    jr?nz   :label          ; jr  nz,   :label
    ; ...
    ; (see "release/z80.txt" for full list)

    #true   $1              ; constant define

    ; constants can be redefined, but the value
    ; must be constant (no forward-references)
    ;
    #true   $0

    ; a hex number at the start of a line
    ; sets the program-counter
    ;
    $c000

    ; or use `$` to set program-counter
    ; using an expression
    ;
    $   #base + $0100

    ; other number types supported:
    ;
    #bin    %10000001       ; binary (1-16 digits)
    #dec    65535           ; decimal
    #chr    'A              ; character literals

    :label                  ; label define

    ; local labels are appended to the
    ; last defined non-local label
    ;
    _local                  ; i.e. :label_local

    ; keywords begin with `.`
    ;
    .b <bytes>
    .w <words>

    ; in expressions, `$` returns
    ; current program-counter
    ;
    .b :label - $

    ; no string escapes, just use numbers
    ;
    .b "line 1" $0a "line 2"

    ; unary operators:
    ; (before value)
    ;
    ; <         = lo (bottom byte of word)
    ; >         = hi (top byte of word)
    ; !         = not (flip all bits)
    ; -         = neg (flip all bits and +1)

    ; infix operators:
    ;
    ; + - * /   = add sub mul div
    ; ^ & | \   = xor and or  mod

    ; file includes
    ;
    .i "file.v80"

    ; condition markers: the expression is evaluated and
    ; the condition passes / skips based on equality with 0
    ;
    ?= <expr>   ; only 0 passes
    ?! <expr>   ; only not-0 passes
    ?+ <expr>   ; only positive (sign+) passes
    ?- <expr>   ; only negative (sign-) passes
        ; if the condition fails,
        ; indented lines are skipped
        ?!      <expr>
                ; this allows for nested conditions

    ; align: skip bytes until the program-counter
    ; modulo the expression is zero
    ;
    .a  $100                ; align to next page

    ; or fill a specific number of bytes,
    ; with a given value
    ;
    .f  '!  10              ; emit "!", 10 times

A full guide on the syntax is included in ["release/readme.txt"](/release/readme.txt). If you're using VSCode, there's an extension for _v80_ syntax highlighting in the marketplace under [v80-vscode](https://marketplace.visualstudio.com/items?itemName=KrocCamen.vscode-v80).

## Building _v80_

If you just want to use _v80_ to write and assemble Z80 (or 6502) software, just download a [release](https://github.com/Kroc/v80/releases). If you want to build _v80_ from source, everything needed to build _v80_ on Windows is included in the repository. Building on Mac, Linux and UNIX-likes can be done, but will require you to source / compile your own binaries of [WLA-DX] & [NTVCM].

[WLA-DX]: https://github.com/vhelin/wla-dx

Just run "build.bat" to assemble _v80_ and related cross-assemblers -- the binary is placed in the "releases" folder.

### Running on Real 8-bit, CP/M Hardware:

_v80_ is assembled as a generic CP/M binary that should run on any Z80-based, CP/M v2.2 (or above) system -- no hardware or system-specific calls are used. For now, _v80_ does not provide any system-specific floppy-disk images for loading _v80_ on to real hardware, but you can use tools like [Disk Image Manager] (Amstrad / Sinclair) or [cpmtools] to make disk-images. If you would like a specific system to have pre-loaded _v80_ disk images, please consider submitting an issue.

[Disk Image Manager]: https://github.com/damieng/DiskImageManager
[cpmtools]: http://www.moria.de/~michael/cpmtools/

## Change History

### 2024/7/?: v1.0.0-dev

- _v80_ can assemble itself via a _v80_-syntax version of _v80_!
- _v65_ is a 6502 cross-assembler version of _v80_, self-assembled by _v80_ using pluggable ISAs. Support for more CPU ISAs will come in the future

### 2024/7/1: v0.1.3

In a spectacular case of not seeing the woods for the trees, the expression parser wasn't evaluating anything more than a single operator and ignoring the rest of the expression. Unary operators were also not working due to expected registers changing at some point.

_v80_ now assembles itself byte-for-byte compared to the WLA-DX version meaning there should be no more surprise bug-fix releases, and a _v80_ version of _v80_ will be coming with v1.0!

### 2024/7/1: v0.1.2

- Square brackets allowed as alternatives to parenthesis

Many fixes realised whilst working on _v80_ assembling itself:

- Expression parsing was sometimes returning carry-set if not deferred
- Multiplication was straight-up broken!
- Decimals not being recognised in `.b` / `.w` lists
- 2nd pass not reporting the correct line/col number for errors

### 2024/6/30: v0.1.1

- Fixed defining labels after setting program-counter with expression not working as intended and an issue with closing parenthesis

### 2024/6/30: v0.1

- Decimal numbers!
- Binary literals (`%`)
- Character literals (`'`)
- Local labels (`_`)
- Fill keyword (`.f`)
- Disk full error (CP/M)

### 2024/6/27: v0.0

First release, _v80_ includes:

- Z80, CP/M v2.2. binary < 8KB
- Constants (`#`) and labels (`:`)
- Expression parsing with deferred expressions for forward-references to labels
- Conditional assembly markers
- Alignment / padding of binary
- Z80 instruction set