# v80

_v80_ is a (very) small, (very) fast Z80 assembler that runs on Z80 for bootstrapping larger projects.

It currently runs on CP/M with plans to make it self-hosting (can assemble itself) with ports to [ZealOS] and [Agon Light].

[ZealOS]: https://github.com/Zeal-Operating-System/ZealOS
[Agon Light]: https://www.thebyteattic.com/p/agon.html

## ~~Features~~

- No Macros! Cut + Paste
- No linker. You don't need one
- No decimal numbers. Let's face it, you think in hexadecimal anyway

## Syntax

v80 uses a non-standard syntax designed for parsing simplicity / speed, not compatibility with existing source code.

```
; v80: ................ zilog:

  adc                   adc
  adc.a                 adc A
  adc.hl+bc             adc HL, BC
  adc*hl                adc [HL]      ; "*" like a pointer
  adc*ix FF             adc [IX+$FF]
  bit7.a                bit 7, A
  bit7*hl               bit 7, [HL]
  call FFFF             call $FFFF
  call?nz FFFF          call nz $FFFF
```