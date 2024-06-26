v80:

|   If a computer can’t compile and deploy its own software
|   then it’s not a general purpose computer, it's an appliance.

0. Usage:
================================================================================
"v80.com" is a Z80 CP/M executable that could run on real, 1985 hardware.
For assembling v80 syntax source code on PCs, ntvcm.exe is provided to
emulate CP/M. Therefore, to invoke v80 on PC, call it via ntvcm.exe:

    ntvcm.exe v80.com input.v80 [output.com]

The output file-name is optional and defaults to the input file-name
with ".com" file-extension.

To ensure that source code written in v80 syntax can be assembled on real
8-bit hardware, v80 enforces CP/M-style file-name restrictions, even on PC!

-   File-names are limited to "8.3" format; that is, up to 8 letters in the
    file-name, followed by an optional extension consisting of a dot and
    up-to 3 characters

-   FIle-names cannot contain spaces and can contain only one dot,
    specifically for the file-extension

-   Only these characters are valid in a file-name:
    a-z 0-9 " # % ' @ ^ _ ` { } ~

CP/M does not support directories so "ntvcm.exe" & "v80.com" must be in the
same directory as the source you wish to assemble and the output will be put
in the same directory. "v80.bat" is provided to assist with this, it will set
the current directory to that of v80.com before invoking it.

    v80.bat input.v80 [output.com]

This could be avoided with a native C version of v80, so if this is something
you think you could write, please see https://github.com/Kroc/v80/issues/4

This release bundle include an ntvcm executable for Windows;
for Mac, Linux & all UNIX-likes, you'll have to compile ntvcm
from source: https://github.com/davidly/ntvcm


1. Syntax:
================================================================================
1.1 Numbers:
--------------------------------------------------------------------------------
Only hexadecimal numbers are supported, prefixed with `$`, 1-4 digits.

    $8
    $ff
    $38f
    $ffff

1.2 Virtual Program-Counter:
--------------------------------------------------------------------------------
The virtual program-counter starts at $0000. A line that begins with a number
sets the virtual program-counter to that value:

    $0100                   ; sets PC to $0100

A special value, "$" without any digits, always returns the current virtual
program-counter:

    $0100                   ; sets PC to $0100
            .w  $           ; writes $00, $01 to the code-segment

To set the virtual program-counter using an expression, label or constant,
begin a line with a "$" followed by the expression / value:

    #boot   $0100           ; define #boot to equal $0100
    $       #boot           ; sets the PC to $0100

Unlike other assemblers, changing the virtual program-counter *does not*
fill the binary with padded space:

    $0100                   ; sets PC to $0100
            .b  $1 $2 $3    ; writes 3 bytes to the code-segment
    $4000                   ; sets PC to $4000
            .w  $code       ; writes bytes 4 & 5 of code-segment!

The align keyword, ".a" can be used to pad the assembled binary instead:

    .a  $100                ; align to the next page
    .a  $ + $80             ; pad exactly $80 bytes

1.3 Labels:
--------------------------------------------------------------------------------
Labels begin with a colon:

    :label

Labels can begin with a number; in fact, almost any character is game as only
whitespace is considered the end of a label name, although you should avoid
going too far. a-z, 0-9 and _ are recommended.

    :1

A line that 'begins' with a label name, that is, before any keyword, defines
the label as having the current virtual program-counter.

Labels cannot be redefined.
All labels must be unique.

1.4 Constants:
--------------------------------------------------------------------------------
A constant is a reusable value given a name. A line that begins with constant
name, followed by an expression, defines a constant:

    #true   0

Constant names can begin with a number:

(the parser is very basic and considers everything between "#" and whitespace
to be the constant name, although you should try stick to a-z, 0-9 & _)

    #1      $31             ; ASCII "1"

Constants _can_ be redefined, but the expression used cannot contain a
forward-reference to a label or an undefined constant.

    :1
    #back   :1              ; OK!
    #next   #fwd            ; invalid! #fwd is currently undefined

    #fwd    :2              ; invalid! :2 is a forward-reference
    :2

1.5 Bytes / Words:
--------------------------------------------------------------------------------
Use ".b" & ".w" to write bytes and words to the assembled binary:

    .b  $AD $DE $EF $BE     ; write bytes
    .w  $DEAD $BEEF         ; write words, little-Endian

Once either keyword is encountered, expressions will be read until the end of
the line or another keyword or instruction is encountered; keywords and
instructions cannot be expressions.

    .b $1 $2 $3 .w $4 $5 $6 ; = $01 $02 $03 $0400 $0500 $0600

An expression can be described as:

|   optional unary operator(s) followed by a value,
|   optionally followed by an operator and another expression

With a "value" being any word that evaluates to a number;
i.e. number literals, labels, or constants.

1.6 Operators:
--------------------------------------------------------------------------------
THERE IS NO OPERATOR PRECEDENCE!

Expressions are evaluated left-to-right; the result is tallied after each infix
operator and before the next.

    .b  $1 + $2 * $5        ; ( $1 + $2 ) * $5 = 15! ($0f)
    .b  $1 + ( $2 * $5 )    ; = 11 ($0b) use parenthesis to force order

The unary operators come before a value:

    !  not      = flip all bits, e.g. !$ff = $00
    <  lo       = lower byte only, e.g. <$aabb = $bb
    >  hi       = upper byte only, e.g. >$aabb = $aa
    -  neg      = flip all bits and +1, e.g. -$1 = $ff

Unary operators apply to the immediate right-hand value, before operators, i.e.

    .b  >:label + $0100     ; error! = ( >:label ) + $0100, 2-bytes!
    .b  >( :label + $0100 ) ; right! 1 byte
    .b  :label + <:future   ; lo-byte of :future is calculated before addition

Standard operators come between values:

    +  add
    -  subtract
    *  multiply
    /  divide (integer)
    \  modulo (remainder)
    &  bitwise and
    |  bitwise or
    ^  bitwise xor

There are no power / exponentiation operators. There are no shift operators.
To do shifts, multiply or divide by powers of 2, e.g. * 2 = << 1, / 8 = >> 3.
Powers of 2 are up to you, I would be surprised if you don't have them
memorised already, say with me: 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, ...

Both operators and parenthesis require spacing to avoid being mistaken
as part of a symbol name or number, i.e.

    .w  $0100+:boot         ; wrong: "$0100+:boot" is treated as one number
    .w  $0100 + :boot       ; OK
    .w  (:boot + $0100)     ; wrong: "$0100)" parentheses is treated as a digit
    .w  ( :boot + $0100 )   ; OK -- use internal spaces for parenthesis

Opening and closing parentheses can be placed together, however:

    .b  (( $1 + ( $2 * $3 )) - $4 )

All numbers are 16-bit internally in v80. For negative numbers, this means that
the upper-byte will be non-zero, i.e. -$1 = $FFFF, and therefore won't fit in a
byte:

    .b  -$1                 ; ERROR! number too large for byte
    .b  <-$1                ; use lo-byte operator to get lower byte only

1.7 Strings:
--------------------------------------------------------------------------------
Strings are a series of bytes:

    .b  "Hello, World!"

Strings cannot be used with the ".w" keyword, which is for 16-bit words only.

    .w  "Error! .w cannot take strings!"

There are no string esacpes (like "\n" in many languages); break the string
with numbers to insert ASCII codes manually:

    .b  "line 1" $0a "line 2"

As speech marks are used to denote strings, they cannot be included directly
in a string. Use the number $22 for ASCII speech marks:

    .b  $22 "Hello, World!" $22

1.8 Includes:
--------------------------------------------------------------------------------
Use ".i" to include another v80 source file.

    .i  "include.v80"

An include file can be reused multiple times. Each time, it will be re-parsed
meaning that constant redefines will be applied, allowing for macro-like
application: (but be aware that labels cannot be redefined)

    #count  #count + 1      ; increment a counter
            .b  #count      ; a different number each time included

Includes can be nested but it's recommended to not exceed 4 levels as each
level pushes a large number of bytes to the stack. No sanity checks are done
for recursive includes. Don't do that.

1.9 Alignment:
--------------------------------------------------------------------------------
Changing the virtual program-counter does not pad the binary! Only emitting
bytes using ".b" & ".w" keywords or instructions adds to the binary.

The align keyword pads the binary with null bytes until the virtual program-
counter modulo the parameter equals zero. That is, bytes are added until the
program-counter divides evenly with the given parameter with no remainder.
If the current program-counter already divides evenly,
then no bytes are emitted.

    .a  $100                ; align to next page
    .a  $c130               ; pad to specific desired program-counter
    .a  $ + $80             ; pad a specific number of bytes

1.10 Conditions:
--------------------------------------------------------------------------------
Portions of code can be conditionally included or excluded from assembly using
condition markers. Unlike if-statements in many programming languages, there
are no comparison operators, rather comparison is done against zero; if you
want to check if a constant is a particular value, you test for this by
subtracting the value to equal zero, e.g.

    ?=  #magic_number - 7
        ; if condition does not match,
        ; all indented code is skipped

    ; (end of condition block)

There are four condition markers to vary condition requirements:

    ?= <expr>   ; only 0 passes
    ?! <expr>   ; only not-0 passes
    ?+ <expr>   ; only positive (sign+) passes
    ?- <expr>   ; only negative (sign-) passes

These allow for "greater than" and "less than" checks, although you should
be wary that a "positive" number is limited from $0000 to $7FFF (hi-bit clear)
and a "negative" number from $8000 to $FFFF (hi-bit set)

    ?+  $4000 - $
        ; is program-counter <= $4000?
        ; e.g. $4000 - $2000 = $2000 (hi-bit clear, is positive)
        ;      $4000 - $8000 = $c000 (hi-bit set, is negative)
        ;      $4000 - $F000 = $5000 (hi-bit clear, mistaken as positive!)

    ?-  #magic_number - 7
        ; is #magic_number > 7?

When a condition matches, parsing continues as normal and the following lines
are included. When a condition fails, parsing skips any line that is indented
*more* than the condition marker:

    ?!  $ffff   ; condition fails, $ffff != $0
        nop
        ?=  $0000
            ; this condition *would* pass, if the above one did too, but none
            ; of these lines are included due to the condition-fail above
    
    nop ; <- condition ends, this line is included

There is no support for else" branches like if-statements in many programming
languages -- try repeating the conditional with reverse a condition:

    ?=  $0000
        ; "true" branch

    ?!  $0000
        ; "false" branch


2. Instructions:
================================================================================
