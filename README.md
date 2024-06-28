# v80

>   If a computer can’t compile and deploy its own software then it’s not a general purpose computer, it's an appliance.

_v80_ is a very small (8KB), very simple Z80 assembler written in Z80 that runs on real 8-bit hardware, but also on PC (via [NTVCM] or [RunCPM]). _v80_'s goal is to ensure reproducability of 8-bit software on real 8-bit hardware, both modern and retro, whilst never excluding PC-based development.

[NTVCM]:  https://github.com/davidly/ntvcm
[RunCPM]: https://github.com/MockbaTheBorg/RunCPM

It currently runs on CP/M with plans to make it self-hosting (can assemble itself) with ports to [Agon Light], [Zeal 8-bit OS] and other Z80-based systems. In the future _v80_ will support other CPUs and cross-assembly (assembling 6502 on Z80 etc).

[Zeal 8-bit OS]: https://github.com/zeal8bit/Zeal-8-bit-OS
[Agon Light]:    https://www.thebyteattic.com/p/agon.html

If you feel like you could write a C version of _v80_, [your help](https://github.com/Kroc/v80/issues/4) would be appreciated as it would remove the need to simulate CP/M to assemble on PC, with the no-directories limit that imposes.

## ~~Features~~

- No linker, we have linker at home → includes
- No Macros! Copy + Paste is your friend or re-use includes
- No decimal numbers. Let's face it, you think in hexadecimal anyway
- $FFFF is the largest number in the universe
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
    .a  $100    ; align to next page

    ; to skip a specific number of bytes,
    ; count from the current program-counter!
    ;
    .a  $ + $80 ; skip exactly 128 bytes

A full guide on the syntax is included in ["release/readme.txt"](/release/readme.txt).

## Building v80

If you just want to use _v80_ to write and assemble Z80 software, just download a [release](https://github.com/Kroc/v80/releases). If you want to build v80 from source, everything needed to build _v80_ on Windows is included in the repository. Building on Mac, Linux and UNIX-likes can be done, but will require you to source / compile your own binaries of [WLA-DX] & [NTVCM].

[WLA-DX]: https://github.com/vhelin/wla-dx

Just run "build.bat" to assemble _v80_ -- the binary is placed in the "releases" folder.

### Running on Real 8-bit, CP/M Hardware:

_v80_ is assembled as a generic CP/M binary that should run on any Z80-based, CP/M v2.2 (or above) system -- no hardware or system-specific calls are used. For now, _v80_ does not provide any system-specific floppy-disk images for loading _v80_ on to real hardware, but you can use tools like [Disk Image Manager] (Amstrad / Sinclair) or [cpmtools] to make disk-images. If you would like a specific system to have pre-loaded _v80_ disk images, please consider submitting an issue.

[Disk Image Manager]: https://github.com/damieng/DiskImageManager
[cpmtools]: http://www.moria.de/~michael/cpmtools/

## Change History

### 2024/6/28: v0.1 alpha

- Local labels (`_`)

### 2024/6/27: v0.0

First release, _v80_ includes:

- Z80, CP/M v2.2. binary < 8KB
- Constants (`#`) and labels (`:`)
- Expression parsing with deferred expressions for forward-references to labels
- Conditional assembly markers
- Alignment / padding of binary
- Z80 instruction set