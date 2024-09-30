v80:

    If a computer can’t compile and deploy its own software
    then it’s not a general purpose computer, it's an appliance.

v80 is an 8-bit native, multi-platform, multi-CPU cross-assembler.

"v80" refers to the Z80 port, but versions that target different CPUs
and/or run on different host CPUs are available, or are planned:

    v80         assembles Z80 code ".v80"
    v65         assembles 6502 code ".v65", currently running on Z80

    v69*        assembles 6809 code ".v69"
    v83*        assembles GameBoy (SM83) code ".v83"

    *(coming in a future release)

If you would like to see v80 on your favourite CPU / system
consider raising an issue at https://github.com/kroc/v80

v80's purpose is to be as small and broadly portable as possible, providing a
self-bootstrapping base for all 8-bit software, without absolute dependence on
PCs for 8-bit systems to do useful, creative, meaningful work.


0. Usage:
================================================================================
"v80.com" is a Z80 CP/M executable that assembles Z80 source code (in v80
syntax). "v65" is a cross-assembler version of v80 that runs on Z80 CP/M
but assembles 6502 source code.

Since "v80.com" is a Z80 executable that could run on real 1985 hardware,
for assembling v80 syntax source code on PCs "ntvcm.exe" is provided to
emulate CP/M. Therefore, to invoke v80/v65 on PC, call it via ntvcm.exe:

|   ntvcm.exe v80.com input.v80 [output.com]
|   ntvcm.exe v65.com input.v65 [output.prg]

The output file-name is optional and defaults to the input file-name
with ".com" (v80) or ".prg" (v65) file-extension.

To ensure that source code written in v80 syntax can be assembled on real
8-bit hardware, v80 enforces CP/M-style file-name restrictions, even on PC!

-   File-names are limited to "8.3" format; that is, up to 8 letters in the
    file-name, followed by an optional extension consisting of a dot and
    up-to 3 characters

-   File-names cannot contain spaces and may contain only one dot,
    specifically for the file-extension

-   Only these characters are valid in a file-name:
    a-z 0-9 " # % ' @ ^ _ ` { } ~

CP/M does not support directories so "ntvcm.exe" & "v80.com" must be in the
same directory as the source you wish to assemble and the output will be put
in the same directory. "v80.bat" is provided to assist with this, it will set
the current directory to that of v80.com before invoking it.

|   v80.bat input.v80 [output.com]

This could be avoided with a native C version of v80, so if this is something
you think you could write, please see https://github.com/Kroc/v80/issues/4

This release bundle includes an NTVCM executable for Windows;
for Mac, Linux & all UNIX-likes, you'll have to compile NTVCM
from source: https://github.com/davidly/ntvcm


1. Syntax:
================================================================================
1.1 Numbers:
--------------------------------------------------------------------------------
Hexadecimal number are prefixed with `$`, 1-4 digits.

|   .b  $8
|   .b  $ff
|   .w  $38f
|   .w  $ffff

Binary numbers are prefixed with `%`, 1-16 digits.

|   .w  %1111111100000000

Decimal numbers are limited to 0 through 65535 (i.e. $FFFF)

|   .w  0 256 512 1024 2048 4096 8192 16384 32768 65535

Note that negative numbers do not exist in practice; the negate operator just
flips all the bits and adds one, so be aware that any negative number greater
than 32768 is positive:

|   .w  -32768              ; = $8000 %1000000000000000 or +32768
|   .w  -32769              ; = $7FFF $0111111111111111 or +32767!

Character literals produce the character code of the character following
the sigil:

|   .b  'a                  ; $61 in ASCII
|   .b  'A                  ; $41 in ASCII
|   .b  ''                  ; $27 in ASCII

Whitespace cannot be used following the sigil,
use number literals for those.

NOTE:   the value of a character literal is based on the encoding of the input
        file, NOT the encoding of the system, which might not be ASCII! (C64)
        Use number literals when you need to guarantee system-specific
        character codes like PETSCII

1.2 Virtual Program-Counter:
--------------------------------------------------------------------------------
The virtual program-counter starts at $0000. A line that begins with a number
sets the virtual program-counter to that value:

|   $0100                   ; sets PC to $0100

A special value, "$" without any digits, always returns the current virtual
program-counter:

|   $0100                   ; sets PC to $0100
|           .w  $           ; writes $00, $01 to the code-segment

To set the virtual program-counter using an expression, label or constant,
begin a line with a "$" followed by the expression / value:

|   #boot   $0100           ; define #boot to equal $0100
|   $       #boot           ; sets the PC to $0100

Unlike other assemblers, changing the virtual program-counter *does not*
fill the binary with padded space:

|   $0100                   ; sets PC to $0100
|           .b  $1 $2 $3    ; writes 3 bytes to the code-segment
|   $4000                   ; sets PC to $4000
|           .w  $code       ; writes bytes 4 & 5 of code-segment!

The fill (".f") or align (".a") keywords can be used to pad the assembled
binary instead:

|   .f  $ab 26              ; pad with exactly 26 bytes of $ab
|   .a  $100                ; align to the next page

1.3 Labels:
--------------------------------------------------------------------------------
Labels begin with a colon:

|   :label

Labels can begin with a number; in fact, almost any character is game as only
whitespace is considered the end of a label name, although you should avoid
going too far. a-z, 0-9 and _ are recommended.

|   :1

A line that 'begins' with a label name, that is, before any keyword, defines
the label as having the current virtual program-counter.

Labels cannot be redefined.
All labels must be unique.

1.4 Local Labels:
--------------------------------------------------------------------------------
Local labels can be "reused", as they automatically append themselves to
the last defined, non-local, label:

|   _local                  ; error: local label without label!
|
|   :label1
|   _first                  ; defines :label1_first
|   _second     jr _first   ; defines :label1_second, jumps to :label1_first
|
|   :label2
|   _first                  ; defines :label2_first
|   _second     jr _first   ; defines :label2_second, jumps to :label2_first

Note that the combined length of the local label name and its parent must not
exceed 31 characters, including label sigil:

|   :2345678901234567890    ; 20 chars
|   _234567890              ; 30 chars - OK
|   _23456789012            ; 32 chars - invalid symbol error!

1.5 Constants:
--------------------------------------------------------------------------------
A constant is a reusable value given a name. A line that begins with constant
name, followed by an expression, defines a constant:

|   #true   0

Constant names can begin with a number:

(the parser is very basic and considers everything between "#" and whitespace
to be the constant name, although you should try stick to a-z, 0-9 & _)

|   #1      $31             ; ASCII "1"

Constants _can_ be redefined, but the expression used cannot contain a
forward-reference to a label or an undefined constant.

|   :1
|   #back   :1              ; OK!
|   #next   #fwd            ; invalid! #fwd is currently undefined
|   
|   #fwd    :2              ; invalid! :2 is a forward-reference
|   :2

1.6 Bytes / Words:
--------------------------------------------------------------------------------
Use ".b" & ".w" to write bytes and words to the assembled binary:

|   .b  $AD $DE $EF $BE     ; write bytes
|   .w  $DEAD $BEEF         ; write words, little-Endian

Once either keyword is encountered, expressions will be read until the end of
the line or another ".b" / ".w" keyword or instruction is encountered:

|   .b 1 2 3 .w 4 5 6       ; = $01 $02 $03 $0400 $0500 $0600

An expression can be described as:

    optional unary operator(s) followed by a value,
    optionally followed by an operator and another expression

With a "value" being any word that evaluates to a number;
i.e. number literals, labels, or constants.

1.7 Expressions:
--------------------------------------------------------------------------------
THERE IS NO OPERATOR PRECEDENCE!

Expressions are evaluated left-to-right; the result is tallied after each infix
operator and before the next.

|   .b  $1 + $2 * $5        ; ( $1 + $2 ) * $5 = 15! ($0f)
|   .b  $1 + ( $2 * $5 )    ; = 11 ($0b) use parenthesis to force order

The unary operators come before a value:

-   ! = not: flip all bits, e.g. !$ff = $00
-   < = lo: lower byte only, e.g. <$aabb = $bb
-   > = hi: upper byte only, e.g. >$aabb = $aa
-   - = neg: flip all bits and +1, e.g. -$1 = $ff

Unary operators apply to the immediate right-hand value, before operators, i.e.

|   .b  >:label + $0100     ; error! = ( >:label ) + $0100, 2-bytes!
|   .b  >( :label + $0100 ) ; right! 1 byte
|   .b  :label + <:future   ; lo-byte of :future is calculated before addition

Standard operators come between values:

-   + = add
-   - = subtract
-   * = multiply
-   / = divide (integer)
-   \ = modulo (remainder)
-   & = bitwise and
-   | = bitwise or
-   ^ = bitwise xor

There are no power / exponentiation operators. There are no shift operators.
To do shifts, multiply or divide by powers of 2, e.g. * 2 = << 1, / 8 = >> 3.
Powers of 2 are up to you, I would be surprised if you don't have them
memorised already, say with me: 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, ...

Both operators and parenthesis require spacing to avoid being mistaken
as part of a symbol name or number, i.e.

|   .w  $0100+:boot         ; wrong: "$0100+:boot" is treated as one number
|   .w  $0100 + :boot       ; OK
|   .w  (:boot + $0100)     ; wrong: "$0100)" parentheses is treated as a digit
|   .w  ( :boot + $0100 )   ; OK -- use internal spaces for parenthesis

Opening and closing parentheses can be placed together, however:

|   .b  (( $1 + ( $2 * $3 )) - $4 )

All numbers are 16-bit internally in v80. For negative numbers, this means that
the upper-byte will be non-zero, i.e. -$1 = $FFFF, and therefore won't fit in a
byte:

|   .b  -$1                 ; ERROR! number too large for byte
|   .b  <-$1                ; use lo-byte operator to get lower byte only

Square brackets can be used in place of parentheses if desired; this option
is specifically provided to indicate memory references in instructions,
if desired, e.g.:

|   ld.HL   :addr           ; = LD HL, addr
|   ld.HL*  [ :addr ]       ; = LD HL, [addr]
|   ld*$.HL [ :addr ]       ; = LD [addr], HL

1.8 Strings:
--------------------------------------------------------------------------------
Strings are a series of bytes:

|   .b  "Hello, World!"

Strings cannot be used with the ".w" keyword, which is for 16-bit words only.

|   .w  "Error! .w cannot take strings!"

There are no string esacpes (like "\n" in many languages); break the string
with numbers to insert ASCII codes manually:

|   .b  "line 1" $0a "line 2"

As speech marks are used to denote strings, they cannot be included directly
in a string. Use the number $22 for ASCII speech marks:

|   .b  $22 "Hello, World!" $22

1.9 Includes:
--------------------------------------------------------------------------------
Use ".i" to include another v80 source file.

|   .i  "include.v80"

An include file can be reused multiple times. Each time, it will be re-parsed
meaning that constant redefines will be applied, allowing for macro-like
application: (but be aware that labels cannot be redefined)

|   #count  #count + 1      ; increment a counter
|           .b  #count      ; a different number each time included

Includes can be nested but it's recommended to not exceed 4 levels as each
level pushes a large number of bytes to the stack. No sanity checks are done
for recursive includes. Don't do that.

1.10 Fill & Alignment:
--------------------------------------------------------------------------------
Changing the virtual program-counter does not pad the binary! Only emitting
bytes or instructions adds to the binary.

The fill keyword simply emits a specific number of bytes of a given value:

|   .f  $00 100             ; pad with 100 $00 bytes
|   .f  '!  10              ; emit "!" 10 times

The align keyword pads the binary with null bytes until the virtual program-
counter modulo the parameter equals zero. That is, bytes are added until the
program-counter divides evenly with the given parameter with no remainder.
If the current program-counter already divides evenly, then no bytes are
 emitted.

|   .a  $100                ; align to next page
|   .a  $c130               ; pad to specific desired program-counter
|   .a  $ + $80             ; pad a specific number of bytes
|   .f  $00 80              ; ... or use the fill keyword

1.11 Conditions:
--------------------------------------------------------------------------------
Portions of code can be conditionally included or excluded from assembly using
condition markers. Unlike if-statements in many programming languages, there
are no comparison operators, rather comparison is done against zero; if you
want to check if a constant is a particular value, you test for this by
subtracting the value to equal zero, e.g.

|   ?=  #magic_number - 7
|       ; if condition does not match,
|       ; all indented code is skipped
|       
|   ; (end of condition block)

There are four condition markers to vary condition requirements:

-   ?= <expr>   only 0 passes
-   ?! <expr>   only not-0 passes
-   ?+ <expr>   only positive (sign+) passes
-   ?- <expr>   only negative (sign-) passes

These allow for "greater than" and "less than" checks, although you should
be wary that a "positive" number is limited from $0000 to $7FFF (hi-bit clear)
and a "negative" number from $8000 to $FFFF (hi-bit set)

|   ?+  $4000 - $
|       ; is program-counter <= $4000?
|       ; e.g. $4000 - $2000 = $2000 (hi-bit clear, is positive)
|       ;      $4000 - $8000 = $c000 (hi-bit set, is negative)
|       ;      $4000 - $F000 = $5000 (hi-bit clear, mistaken as positive!)
|       
|   ?-  #magic_number - 7
|       ; is #magic_number > 7?

When a condition matches, parsing continues as normal and the following lines
are included. When a condition fails, parsing skips any line that is indented
*more* than the condition marker:

|   ?!  $ffff   ; condition fails, $ffff != $0
|       nop
|       ?=  $0000
|           ; this condition *would* pass, if the above one did too, but none
|           ; of these lines are included due to the condition-fail above
|   
|   nop ; <- condition ends, this line is included

There is no support for else" branches like if-statements in many programming
languages -- try repeating the conditional with a reverse condition:

|   ?=  $0000
|       ; "true" branch
|       
|   ?!  $0000
|       ; "false" branch


2. CPU Instructions:
================================================================================
v80 does not use standard instruction syntax, i.e. Intel (8080) / Zilog (Z80),
or MOS (6502). v80's syntax was chosen to be as easy and as fast as possible
to parse, but you should also find them easy to type!

See "isa_z80.txt" for a complete list of Z80 mnemonics with side-by-side Zilog
equivalents as well as cycle-counts and flag-states for the original Z80/Z80A.

- use ".v80" file-extension for Z80 source code

See "isa_6502.txt" for a complete list of 6502 mnemonics with side-by-side MOS
equivalents as well as cycle-counts and flag-states for the original 6502/6510
etc.

- use ".v65" file-extension for 6502 source code

If you would like to see v80 on your favourite CPU / system
consider raising an issue at https://github.com/kroc/v80

The following rules hold true throughout:

-   There are no spaces between instruction name and registers. Dots are used
    as separators so that the complete instruction appears as one word:

    v80:    ld.a.b              Zilog:  LD  A,  B

-   Memory references, such as "[HL]" in Zilog syntax, instead use "*" as an
    indicator instead of brackets (think "pointer to") specifically so that
    no additional parsing is required after the register name to determine
    the instruction:

    v80:    ld.a*hl             Zilog:  LD  A,  [HL]
    v80:    ld.a*ix $00         Zilog:  LD  A,  [IX+$00]

-   All numerical parameters come after the instruction -- that is, no register
    name ever comes after a parameter so that the parser instantly knows which
    opcode to use without having to read past a numeric parameter that might be
    highly complex (an expression); there is one case where this leads to
    unsual syntax, when reading into a memory location from a register:

    v80:    ld*$.hl $1234       Zilog:  LD  [$1234],    HL

-   Where a numerical parameter is part of the opcode, it is included in the
    instruction. This applies to IM and RST which don't take arbitrary numbers
    but have certain values directly encoded into them.

    v80:    im.0                Zilog:  IM  0
            im.1                        IM  1
            im.2                        IM  2
            rst.0                       RST $00
            rst.8                       RST $08
            ...                         ...
            rst.38                      RST $38

    For the bit-manipulation instructions BIT, SET & RES,
    the bit number is built into the instruction name:

    v80:    bit0.a              Zilog:  BIT 0,  A
            res1.b                      RES 1,  B
            set2.c                      SET 2,  C
            bit7*hl                     BIT 7,  [HL]

-   Condition flags for instructions (carry-flag, zero-flag etc.)
    are separated with "?":

    v80:    jr?c    $1234               JR  c,  $1234
            jp?pe   $1234               JP  pe, $1234
            ret?z   $1234               RET z,  $1234

    v80 makes an additional affordance for clarity by providing a pseudo-flag,
    "v", for overflow since the partiy and overflow flags are shared on a Z80
    and I'll be damned if I can remember if it's "pe" or "po" for overflow-set!

    v80:    jp?v    $1234       Zilog:  JP  pe, $1234
            jp?nv   $1234               JP  po, $1234

-   A number of additional pseudo-ops are provided for the author to better
    communicate intent; it can get confusing with all the register-swapping.
    The exchange DE, HL instruction can alo be written as HL, DE for those
    instances where you are 'putting aside' one register and returning to it.

    v80:    ex.hl.de            Zilog:  EX  DE, HL
            add.hl.bc                   ADD HL, BC
            ex.de.hl                    EX  DE, HL

            ex.hl*hl                    EX  [SP],   HL
            inc.hl                      INC HL
            ex*sp.hl                    EX  [SP],   HL
    
            hcf                         HALT
    
-   The Z80 shadow registers can be indicated in any instruction by appending
    an apostrophe to the register name; this replaces the separating dot where
    appropriate:

    v80:    ld.a.b'             Zilog:  LD  A,  B'
            ld.a'b                      LD  A', B
            ld.d'*hl'                   LD  D', [HL']
            ex.af.af'                   EX  AF, AF'
            ex.af'af                    EX  AF, AF'