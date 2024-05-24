# v80

_v80_ is a (very) small, (very) fast Z80 assembler that runs on Z80 for bootstrapping larger projects.

It currently runs on CP/M with plans to make it self-hosting (can assemble itself) with ports to [Zeal 8-bit OS] and [Agon Light].

[Zeal 8-bit OS]: https://github.com/zeal8bit/Zeal-8-bit-OS
[Agon Light]: https://www.thebyteattic.com/p/agon.html

## ~~Features~~

- No linker, we have linker at home &rarr; includes
- No Macros! Copy + Paste is your friend or re-use includes
- No decimal numbers. Let's face it, you think in hexadecimal anyway
- No floating point numbers. Have you _tried_ coding floats on an 8-bit CPU??
- No negative numbers. It's all 1s in 2s compliment
- No shift operators. Multiply/divide by powers of 2
- true = 0, like how a CPU _actually_ works.  
  search your heart, you know it to be true

## Syntax

v80 uses a non-standard syntax designed for parsing simplicity / speed, not compatibility with existing source code.

The basic principle is that v80 can only recognise a word by the first character, so everything must be prefixed with a sigil or otherwise be easily categorizable, e.g. `a`-`z` = instruction.

    ; v80: ................ zilog:
    ;
    adc   $ff               ; adc $ff
    adc.a                   ; adc A,      A
    adc.hl.bc               ; adc HL,     BC
    adc*hl                  ; adc A,      [HL]
    adc*ix  $00             ; adc [IX+$00]
    bit7.a                  ; bit 7,      A
    bit7*hl                 ; bit 7,      [HL]
    call    $FFFF           ; call        $FFFF
    call?nz $FFFF           ; call nz,    $FFFF
    ; ...
    ; (see "src/test_z80.v80" for full list)

    #true   $1              ; constant define

    ; constants can be redefined, but the value
    ; must be constant (no forward-references)
    ;
    #true   $0

    ; label define
    ;
    :label

    ; a hex number at the start of a line
    ; sets the program-counter
    ;
    $c000

    ; or use `$` to set program-counter
    ; using an expression
    ;
    $   #base + $0100

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
    ; <         = lo
    ; >         = hi
    ; !         = not (flip all bits)
    ; -         = neg (flip all bits and +1)

    ; infix operatosr:
    ;
    ; + - * /   = add sub mul div
    ; ^ & | \   = xor and or  mod

    ; file includes
    ;
    .i "file.v80"

    ; if statements: the expression is evaluated and
    ; the if passes / skips based on equality with 0
    ;
    ?= <expr>   ; only 0 passes
    ?! <expr>   ; only not-0 passes
    ?+ <expt>   ; only positive (sign+) passes
    ?- <expr>   ; only negative (sign-) passes
        ; if the condition fails, indented lines are
        ; skipped until first line of equal indent
    ;   <- end of if statement

    ; align: skip bytes until the program-counter
    ; modulo the expression is zero
    ;
    .a  $100    ; align to next page

    ; to skip a specific number of bytes,
    ; count from the current program-counter!
    ;
    .a  $ + $80 ; skip exactly 128 bytes

### Labels:

Labels begin with a colon.

    :label

Labels can begin with a number; in fact, almost any character is game as only whitespace is considered the end of a label name, although you should avoid going too far. `a`-`z`, `0`-`9` and `_` are recommended.

    :1

A line that 'begins' with a label name, that is, before any keyword, defines the label as having the current Program Counter.

Labels cannot be redefined. All labels must be unique.

### Constants:

A constant is a reusable value given a name.
A line that begins with constant name defines a constant:

    #true   0

Constant names can begin with a number:

(the parser is very basic and considers everything between `#` and whitespace to be the constant name, although you should try stick to `a`-`z`, `0`-`9` & `_`)

    #1      $31         ; ASCII "1"

Constants _can_ be redefined, but their _value_ must be constant!  
That is, a constant cannot use a forward-reference or undefined constant.

    :1
    #back   :1          ; OK!

    :2
    #back   :2          ; OK!

    #fwd    :3          ; invalid! `:3` is unknown
    :3

### keywords:

Use `.b` & `.w` to write bytes and words to the assembled binary.

    .b  $AD $DE $EF $BE     ; write bytes
    .w  $DEAD $BEEF         ; write words

Once either keyword is encountered, expressions will be read until the end of the line or another keyword is encountered; keywords cannot be expressions.

    .b $1 $2 $3 .w $4 $5 $6 ; = $01 $02 $03 $0400 $0500 $0600

An expression can be described as:

>   an optional unary operator followed by a value,  
>   optionally followed by an operator and another expression

With a _value_ being any word that evaluates to a number;  
i.e. number literals, labels, or constants.

### Operators:

The unary operators come before a value:

    !  not
    <  lo-byte
    >  hi-byte
    -  negate

Standard operators come between values:

    +  add
    -  subtract
    *  multiply
    /  divide (integer)
    &  and
    |  or
    ^  xor
    %  modulo

There are no shift operators, simply because the parser can only handle single-character operators and couldn't distinguish `>` and `>>`. To do shifts, multiply or divide by powers of 2, e.g. `* 2` = `<< 1`, `/ 8` = `>> 3`. Also, there's no power / exponentiation operator :P
