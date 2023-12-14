# v80

_v80_ is a (very) small, (very) fast Z80 assembler that runs on Z80 for bootstrapping larger projects.

It currently runs on CP/M with plans to make it self-hosting (can assemble itself) with ports to [Zeal-8-bit-OS] and [Agon Light].

[Zeal-8-bit-OS]: https://github.com/zeal8bit/Zeal-8-bit-OS
[Agon Light]: https://www.thebyteattic.com/p/agon.html

## ~~Features~~

- No Macros! Copy + Paste is your friend
- No linker. You don't need one
- No decimal numbers. Let's face it, you think in hexadecimal anyway
- No floating point numbers. Have you _tried_ coding floats on an 8-bit CPU??
- No negative numbers. It's all 1s in 2s compliment
- No shift operators. Multiply/divide by powers of 2  
  (honestly the reason for this is just because the parser is limited to single-character operators)

## Syntax

v80 uses a non-standard syntax designed for parsing simplicity / speed, not compatibility with existing source code.


    v80: ................ zilog:

    adc                   adc
    adc.a                 adc A
    adc.hl+bc             adc HL, BC
    adc*hl                adc [HL]      ; "*" like a pointer
    adc*ix  $FF           adc [IX+$FF]
    bit7.a                bit 7,  A
    bit7*hl               bit 7,  [HL]
    call    $FFFF         call    $FFFF
    call?nz $FFFF         call nz $FFFF

### Labels:

Labels begin with a colon.

    :label

### Operators:

The unary operators come before a value:

    !  not
    <  lo-byte
    >  hi-byte

Standard operators come between values:

    +  add
    -  subtract
    *  multiply
    /  divide (integer)
    &  and
    |  or
    ^  xor
    %  modulo

There are no shift operators, simply because the parser can only handle single-character operators and couldn't distinguish `>` and `>>`. Multiply or divide by powers of 2 to do shifts; e.g. `* 2` = `<< 1`, `/ 8` = `>> 3`. Also, there's no power/exponentiation operator :P
