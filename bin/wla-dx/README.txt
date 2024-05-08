wla-dx v10.6 documentation

* 1. Introduction

* 2. Quickstart

* 3. Assembler Directives

  * 3.1. ".16BIT"

  * 3.2. ".24BIT"

  * 3.3. ".8BIT"

  * 3.4. ".ACCU 8"

  * 3.5. ".ADDR 16000, main, 255"

  * 3.6. ".ALIGN 4"

  * 3.7. ".ARRAYDB NAME MyArray INDICES '0', 0, 1"

  * 3.8. ".ARRAYDD NAME MyArray INDICES '0', 0, 1"

  * 3.9. ".ARRAYDEF NAME MyArray SIZE 256"

  * 3.10. ".ARRAYDEFINE NAME MyArray SIZE 256"

  * 3.11. ".ARRAYDL NAME MyArray INDICES '0', 0, 1"

  * 3.12. ".ARRAYDW NAME MyArray INDICES '0', 0, 1"

  * 3.13. ".ARRAYIN NAME MyArray INDEX 0 VALUE 10"

  * 3.14. ".ARRAYOUT NAME MyArray INDEX 0 DEFINITION ArrayOut"

  * 3.15. ".ASC "HELLO WORLD!""

  * 3.16. ".ASCIITABLE"

  * 3.17. ".ASCSTR "HELLO WORLD!", $A"

  * 3.18. ".ASCTABLE"

  * 3.19. ".ASM"

  * 3.20. ".ASSERT VALUE_1 == 1"

  * 3.21. ".BACKGROUND "parallax.gb""

  * 3.22. ".BANK 0 SLOT 1"

  * 3.23. ".BASE $80"

  * 3.24. ".BITS 4 DATA %1011, %0100, %1010, %0101"

  * 3.25. ".BLOCK "Block1""

  * 3.26. ".BR"

  * 3.27. ".BREAK"

  * 3.28. ".BREAKPOINT"

  * 3.29. ".BYT 100, $30, %1000, "HELLO WORLD!""

  * 3.30. ".CARTRIDGETYPE 1"

  * 3.31. ".COMPUTEGBCHECKSUM"

  * 3.32. ".COMPUTEGBCOMPLEMENTCHECK"

  * 3.33. ".COMPUTESMDCHECKSUM"

  * 3.34. ".COMPUTESMSCHECKSUM"

  * 3.35. ".COMPUTESNESCHECKSUM"

  * 3.36. ".CONTINUE"

  * 3.37. ".COUNTRYCODE 1"

  * 3.38. ".DATA $ff00, 2"

  * 3.39. ".DB 100, $30, %1000, "HELLO WORLD!""

  * 3.40. ".DBCOS 0.2, 10, 3.2, 120, 1.3"

  * 3.41. ".DBM filtermacro 1, 2, "encrypt me""

  * 3.42. ".DBRND 20, 0, 10"

  * 3.43. ".DBSIN 0.2, 10, 3.2, 120, 1.3"

  * 3.44. ".DD $1ffffff, $2000000"

  * 3.45. ".DDM filtermacro 1, 2, 3"

  * 3.46. ".DEF IF $FF0F"

  * 3.47. ".DEFINE IF $FF0F"

  * 3.48. ".DESTINATIONCODE 1"

  * 3.49. ".DL $102030, $405060"

  * 3.50. ".DLM filtermacro 1, 2, 3"

  * 3.51. ".DS 256, $10"

  * 3.52. ".DSB 256, $10"

  * 3.53. ".DSD 256, $1ffffff"

  * 3.54. ".DSL 16, $102030"

  * 3.55. ".DSTRUCT waterdrop INSTANCEOF water VALUES"

  * 3.56. ".DSW 128, 20"

  * 3.57. ".DW 16000, 10, 255"

  * 3.58. ".DWCOS 0.2, 10, 3.2, 1024, 1.3"

  * 3.59. ".DWM filtermacro 1, 2, 3"

  * 3.60. ".DWRND 20, 0, 10"

  * 3.61. ".DWSIN 0.2, 10, 3.2, 1024, 1.3"

  * 3.62. ".ELIF defined(DEBUG) && VERSION > 110"

  * 3.63. ".ELSE"

  * 3.64. ".EMPTYFILL $C9"

  * 3.65. ".ENDASM"

  * 3.66. ".ENDA"

  * 3.67. ".ENDB"

  * 3.68. ".ENDBITS"

  * 3.69. ".ENDEMUVECTOR"

  * 3.70. ".ENDE"

  * 3.71. ".ENDIF"

  * 3.72. ".ENDME"

  * 3.73. ".ENDM"

  * 3.74. ".ENDNATIVEVECTOR"

  * 3.75. ".ENDRO"

  * 3.76. ".ENDR"

  * 3.77. ".ENDSNES"

  * 3.78. ".ENDST"

  * 3.79. ".ENDS"

  * 3.80. ".ENDU"

  * 3.81. ".ENUM $C000"

  * 3.82. ".ENUMID ID_1 0"

  * 3.83. ".EQU IF $FF0F"

  * 3.84. ".EXHIROM"

  * 3.85. ".EXPORT work_x"

  * 3.86. ".FAIL "THE EYE OF MORDOR HAS SEEN US!""

  * 3.87. ".FARADDR main, irq_1"

  * 3.88. ".FASTROM"

  * 3.89. ".FCLOSE FP_DATABIN"

  * 3.90. ".FILTER filtermacro 1, 2, "encrypt me""

  * 3.91. ".FOPEN "data.bin" FP_DATABIN"

  * 3.92. ".FREAD FP_DATABIN DATA"

  * 3.93. ".FSEEK FP_DATABIN 10 START"

  * 3.94. ".FSIZE FP_DATABIN SIZE"

  * 3.95. ".FTELL FP_DATABIN POSITION"

  * 3.96. ".FUNCTION SUM_AB(varA,varB)"

  * 3.97. ".GBHEADER"

  * 3.98. ".HEX "a0A0ffDE""

  * 3.99. ".HIROM"

  * 3.100. ".IF DEBUG == 2"

  * 3.101. ".IFDEF IF"

  * 3.102. ".IFDEFM \2"

  * 3.103. ".IFEQ DEBUG 2"

  * 3.104. ".IFEXISTS "main.s""

  * 3.105. ".IFGR DEBUG 2"

  * 3.106. ".IFGREQ DEBUG 2"

  * 3.107. ".IFLE DEBUG 2"

  * 3.108. ".IFLEEQ DEBUG 2"

  * 3.109. ".IFNDEF IF"

  * 3.110. ".IFNDEFM \2"

  * 3.111. ".IFNEQ DEBUG 2"

  * 3.112. ".INC "cgb_hardware.i""

  * 3.113. ".INCBIN "sorority.bin""

  * 3.114. ".INCDIR "/usr/programming/gb/include/""

  * 3.115. ".INCLUDE "cgb_hardware.i""

  * 3.116. ".INDEX 8"

  * 3.117. ".INPUT NAME"

  * 3.118. ".LICENSEECODENEW "1A""

  * 3.119. ".LICENSEECODEOLD $1A"

  * 3.120. ".LONG $102030, $405060"

  * 3.121. ".LOROM"

  * 3.122. ".MACRO TEST"

  * 3.123. ".MEMORYMAP"

  * 3.124. ".NAME "NAME OF THE ROM""

  * 3.125. ".NEXTU name"

  * 3.126. ".NINTENDOLOGO"

  * 3.127. ".NOWDC"

  * 3.128. ".ORG $150"

  * 3.129. ".ORGA $150"

  * 3.130. ".OUTNAME "other.o""

  * 3.131. ".PRINT "Numbers 1 and 10: ", DEC 1, " $", HEX 10, "\n""

  * 3.132. ".PRINTT "Here we are...\n""

  * 3.133. ".PRINTV DEC DEBUG+1"

  * 3.134. ".RAMSECTION "Vars" BASE $7E BANK 0 SLOT 1 ALIGN 256 OFFSET
    32"

  * 3.135. ".RAMSIZE 0"

  * 3.136. ".REDEF IF $0F"

  * 3.137. ".REDEFINE IF $0F"

  * 3.138. ".REPEAT 6"

  * 3.139. ".REPT 6"

  * 3.140. ".ROMBANKMAP"

  * 3.141. ".ROMBANKS 2"

  * 3.142. ".ROMBANKSIZE $4000"

  * 3.143. ".ROMDMG"

  * 3.144. ".ROMGBCONLY"

  * 3.145. ".ROMGBC"

  * 3.146. ".ROMSGB"

  * 3.147. ".ROMSIZE 1"

  * 3.148. ".ROW $ff00, 1, "3""

  * 3.149. ".SDSCTAG 1.0, "DUNGEON MAN", "A wild dungeon exploration
    game", "Ville Helin""

  * 3.150. ".SECTION "Init" FORCE"

  * 3.151. ".SEED 123"

  * 3.152. ".SEEDRANDOM"

  * 3.153. ".SHIFT"

  * 3.154. ".SLOT 1"

  * 3.155. ".SLOWROM"

  * 3.156. ".SMC"

  * 3.157. ".SMDHEADER"

  * 3.158. ".SMSHEADER"

  * 3.159. ".SMSTAG"

  * 3.160. ".SNESEMUVECTOR"

  * 3.161. ".SNESHEADER"

  * 3.162. ".SNESNATIVEVECTOR"

  * 3.163. ".STRINGMAP script "Hello\n""

  * 3.164. ".STRINGMAPTABLE script "script.tbl""

  * 3.165. ".STRUCT enemy_object"

  * 3.166. ".SYM SAUSAGE"

  * 3.167. ".SYMBOL SAUSAGE"

  * 3.168. ".TABLE byte, word, byte"

  * 3.169. ".UNBACKGROUND $1000 $1FFF"

  * 3.170. ".UNDEF DEBUG"

  * 3.171. ".UNDEFINE DEBUG"

  * 3.172. ".UNION name"

  * 3.173. ".VERSION 1"

  * 3.174. ".WDC"

  * 3.175. ".WHILE COUNTER > 0"

  * 3.176. ".WORD 16000, 10, 255"

* 4. Assembler Syntax

  * 4.1. Case Sensitivity

  * 4.2. Comments

  * 4.3. Line splitting

  * 4.4. Using Commas

  * 4.5. Labels

  * 4.6. Number Types

  * 4.7. Strings

  * 4.8. Substitution

  * 4.9. Mnemonics

  * 4.10. Brackets?

* 5. Error Messages

* 6. Supported ROM/RAM/Cartridge Types (WLA-GB)

  * 6.1. ROM Size

  * 6.2. RAM Size

  * 6.3. Cartridge Type

* 7. Bugs

* 8. Files

  * 8.1. "tests"

  * 8.2. "tests/gb-z80/lib"

  * 8.3. "memorymaps"

* 9. Functions

  * 9.1. User defined functions

  * 9.2. Built-in functions

  * 9.3. Examples of functions

* 10. Temporary Files

* 11. Compiling

  * 11.1. Compiling Object Files

  * 11.2. Compiling Library Files

* 12. Linking

* 13. Arithmetics

* 14. Binary to DB Conversion

* 15. Things you should know about coding for…

  * 15.1. Z80

  * 15.2. 6502

  * 15.3. 65C02

  * 15.4. 65CE02

  * 15.5. 65816

  * 15.6. HUC6280

  * 15.7. SPC-700

  * 15.8. Pocket Voice (GB-Z80)

  * 15.9. GB-Z80

* 16. WLA Flags

* 17. Extra compile time definitions

* 18. Good things to know about WLA

* 19. WLA DX’s architectural overview

  * 19.1. WLA

  * 19.2. WLALINK

* 20. WLA Symbols

  * 20.1. WLA Symbol Version History

  * 20.2. Information For Emulator Developers

    * 20.2.1. [information]

    * 20.2.2. [labels]

    * 20.2.3. [definitions]

    * 20.2.4. [breakpoints]

    * 20.2.5. [symbols]

    * 20.2.6. [source files v2]

    * 20.2.7. [rom checksum]

    * 20.2.8. [addr-to-line mapping v2]

    * 20.2.9. [sections]

    * 20.2.10. [ramsections]

* 21. Legal Note

* 1. Manpage: WLA-LINK

* 2. Manpage: WLA-CPU

* 3. Manpage: WLAB


Introduction
************

The history behind WLA DX, from the original author, Ville Helin:

   I wrote this because I had never written an assembler before and I
   really needed a macro assembler which could compile the GB-Z80 code
   I wrote. ;) Gaelan Griffin needed real Z80 support for his SMS
   projects so I thought I could write WLA to be a little more open
   and nowadays it supports all the Z80 systems you can think of.
   You’ll just have to define the memorymap of the destination machine
   for your project. After fixing some bugs I thought I could add
   support for 6502 systems so all NES-people would get their share of
   WLA as well. After finishing that few people said they’d like 65816
   support (they had SNES developing in mind) so I added support for
   that. And then I thought I should write a 6510 version of WLA as
   well…

   This is my ideal GB-Z80 macro assembler (not in final form, not
   yet). ;) Tastes differ. Thus WLA! Notice that WLA was initially
   made for Game Boy developers so the GB-Z80 version and the rest
   differ a little.

Good to know about WLA DX:

Almost all rules that apply to Z80 source code processing with WLA DX
apply also to 6502, 65C02, 65CE02, 65816, 6800, 6801, 6809, 8008,
8080, HUC6280, SPC-700 and SuperFX.

About the names: WLA DX means all the tools covered in this
documentation. So WLA DX includes WLA
GB-Z80/Z80/6502/65C02/65CE02/65816/6800/6801/6809/
8008/8080/HUC6280/SPC-700/SuperFX macro assembler (what a horribly
long name), WLAB, and WLALINK
GB-Z80/Z80/6502/65C02/65CE02/65816/6800/6801/6809/8008/8080/
HUC6280/SPC-700/SuperFX linker. We use plain WLA to refer to the macro
assembler in this document.

There was WLAD, an GB-Z80 dissassembler, but it has been discontinued
and removed from the project and the documentation.

Currently WLA can also be used as a patch tool. Just include the
original ROM image into the project with ".BACKGROUND" and insert
e.g., "OVERWRITE .SECTION" s to patch the desired areas. Output the
data into a new ROM image and there you have it. 100% readable (asm
coded) patches are reality!

Note that you can directly compile only object and library files. You
must use WLALINK to link these (or only one, if you must) into a
ROM/program file.

   WLA DX’s old homepage: http://www.villehelin.com/wla.html
   WLA DX’s new homepage: https://github.com/vhelin/wla-dx


Quickstart
**********

Every assembly file needs to begin with the definition of ".MEMORYMAP"
and ".ROMBANKMAP". You can put these inside a separate file that is
included at the beginning of the assembly files. Here’s an example:

   .MEMORYMAP
   DEFAULTSLOT 1
   SLOT 0 START $0000 SIZE $2000
   SLOT 1 START $2000 SIZE $2000
   .ENDME

   .ROMBANKMAP
   BANKSTOTAL 2
   BANKSIZE $2000
   BANKS 8
   .ENDRO

Right after these, before any code is written, you should define bank,
slot and org:

   .BANK 0 SLOT 0
   .ORGA $0000

Now you are ready to start programming!

The next step would be creating a linkfile for the linker as after the
assembly files go through the assembler they need to be linked. Here
is a minimal example of a linkfile when your project has just main.s
(that has been assembled into main.o):

   [objects]
   main.o

Give this to the linker and you’ll get the final binaries.


Assembler Directives
********************

Here’s the order in which the data is placed into the output:

1. Data and group 3 directives outside sections.

2. Group 2 directives.

3. Data and group 3 directives inside sections.

4. Group 1 directives.

+-----+------------------------------------------------------------------+
| ALL | All, GB-Z80, Z80, 6502, 65C02, 65CE02, 65816, HUC6280, SPC-700,  |
|     | 68000, 6800, 6801, 6809, 8008, 8080 and SuperFX versions apply.  |
+-----+------------------------------------------------------------------+
| GB  | Only the GB-Z80 version applies.                                 |
+-----+------------------------------------------------------------------+
| GB8 | Only the GB-Z80 and 65816 versions apply.                        |
+-----+------------------------------------------------------------------+
| Z80 | Only the Z80 version applies.                                    |
+-----+------------------------------------------------------------------+
| 658 | Only the 65816 version applies.                                  |
+-----+------------------------------------------------------------------+
| 68K | Only the 68000 version applies.                                  |
+-----+------------------------------------------------------------------+
| 680 | Only the 6800, 6801 and 6809 versions apply.                     |
+-----+------------------------------------------------------------------+
| 800 | Only the 8008 version applies.                                   |
+-----+------------------------------------------------------------------+
| 808 | Only the 8080 version applies.                                   |
+-----+------------------------------------------------------------------+
| SPC | Only the SPC-700 version applies.                                |
+-----+------------------------------------------------------------------+
| SFX | Only the SuperFX version applies.                                |
+-----+------------------------------------------------------------------+
| 65x | Only the 6502, 65C02, 65CE02, 65816 and HUC6280 versions apply.  |
+-----+------------------------------------------------------------------+
| !GB | All but the GB-Z80 versions apply.                               |
+-----+------------------------------------------------------------------+

Group 1:

+-----+------------------------------------------------------------------------------------+
| GB  | ".COMPUTEGBCHECKSUM"                                                               |
+-----+------------------------------------------------------------------------------------+
| 68K | ".COMPUTESMDCHECKSUM"                                                              |
+-----+------------------------------------------------------------------------------------+
| Z80 | ".COMPUTESMSCHECKSUM"                                                              |
+-----+------------------------------------------------------------------------------------+
| 658 | ".COMPUTESNESCHECKSUM"                                                             |
+-----+------------------------------------------------------------------------------------+
| Z80 | ".SDSCTAG 1.0, "DUNGEON MAN", "A wild dungeon exploration game", "Ville Helin""    |
+-----+------------------------------------------------------------------------------------+
| Z80 | ".SMSTAG"                                                                          |
+-----+------------------------------------------------------------------------------------+

Group 2:

+-----+---------------------------------+
| GB  | ".CARTRIDGETYPE 1"              |
+-----+---------------------------------+
| GB  | ".COMPUTEGBCOMPLEMENTCHECK"     |
+-----+---------------------------------+
| GB  | ".COUNTRYCODE 1"                |
+-----+---------------------------------+
| GB  | ".DESTINATIONCODE 1"            |
+-----+---------------------------------+
| ALL | ".EMPTYFILL $C9"                |
+-----+---------------------------------+
| 658 | ".ENDEMUVECTOR"                 |
+-----+---------------------------------+
| 658 | ".ENDNATIVEVECTOR"              |
+-----+---------------------------------+
| 658 | ".ENDSNES"                      |
+-----+---------------------------------+
| 658 | ".EXHIROM"                      |
+-----+---------------------------------+
| ALL | ".EXPORT work_x"                |
+-----+---------------------------------+
| 658 | ".FASTROM"                      |
+-----+---------------------------------+
| GB  | ".GBHEADER"                     |
+-----+---------------------------------+
| 658 | ".HIROM"                        |
+-----+---------------------------------+
| GB  | ".LICENSEECODENEW "1A""         |
+-----+---------------------------------+
| GB  | ".LICENSEECODEOLD $1A"          |
+-----+---------------------------------+
| 658 | ".LOROM"                        |
+-----+---------------------------------+
| GB8 | ".NAME "NAME OF THE ROM""       |
+-----+---------------------------------+
| GB  | ".NINTENDOLOGO"                 |
+-----+---------------------------------+
| ALL | ".OUTNAME "other.o""            |
+-----+---------------------------------+
| GB  | ".RAMSIZE 0"                    |
+-----+---------------------------------+
| GB  | ".ROMDMG"                       |
+-----+---------------------------------+
| GB  | ".ROMGBC"                       |
+-----+---------------------------------+
| GB  | ".ROMGBCONLY"                   |
+-----+---------------------------------+
| GB  | ".ROMSGB"                       |
+-----+---------------------------------+
| GB  | ".ROMSIZE 1"                    |
+-----+---------------------------------+
| 658 | ".SLOWROM"                      |
+-----+---------------------------------+
| 658 | ".SMC"                          |
+-----+---------------------------------+
| 68K | ".SMDHEADER"                    |
+-----+---------------------------------+
| Z80 | ".SMSHEADER"                    |
+-----+---------------------------------+
| 658 | ".SNESEMUVECTOR"                |
+-----+---------------------------------+
| 658 | ".SNESHEADER"                   |
+-----+---------------------------------+
| 658 | ".SNESNATIVEVECTOR"             |
+-----+---------------------------------+
| GB  | ".VERSION 1"                    |
+-----+---------------------------------+

Group 3:

+-----+--------------------------------------------------------------------+
| 65x | ".16BIT"                                                           |
+-----+--------------------------------------------------------------------+
| 658 | ".24BIT"                                                           |
+-----+--------------------------------------------------------------------+
| 65x | ".8BIT"                                                            |
+-----+--------------------------------------------------------------------+
| 658 | ".ACCU 8"                                                          |
+-----+--------------------------------------------------------------------+
| ALL | ".ADDR 16000, main, 255"                                           |
+-----+--------------------------------------------------------------------+
| ALL | ".ALIGN 4"                                                         |
+-----+--------------------------------------------------------------------+
| ALL | ".ARRAYDB NAME MyArray INDICES '0', 0, 1"                          |
+-----+--------------------------------------------------------------------+
| ALL | ".ARRAYDD NAME MyArray INDICES '0', 0, 1"                          |
+-----+--------------------------------------------------------------------+
| ALL | ".ARRAYDEF NAME MyArray SIZE 256"                                  |
+-----+--------------------------------------------------------------------+
| ALL | ".ARRAYDEFINE NAME MyArray SIZE 256"                               |
+-----+--------------------------------------------------------------------+
| ALL | ".ARRAYDL NAME MyArray INDICES '0', 0, 1"                          |
+-----+--------------------------------------------------------------------+
| ALL | ".ARRAYDW NAME MyArray INDICES '0', 0, 1"                          |
+-----+--------------------------------------------------------------------+
| ALL | ".ARRAYIN NAME MyArray INDEX 0 VALUE 10"                           |
+-----+--------------------------------------------------------------------+
| ALL | ".ARRAYOUT NAME MyArray INDEX 0 DEFINITION ArrayOut"               |
+-----+--------------------------------------------------------------------+
| ALL | ".ASC "HELLO WORLD!""                                              |
+-----+--------------------------------------------------------------------+
| ALL | ".ASCIITABLE"                                                      |
+-----+--------------------------------------------------------------------+
| ALL | ".ASCSTR "HELLO WORLD!", $A"                                       |
+-----+--------------------------------------------------------------------+
| ALL | ".ASCTABLE"                                                        |
+-----+--------------------------------------------------------------------+
| ALL | ".ASM"                                                             |
+-----+--------------------------------------------------------------------+
| ALL | ".ASSERT VALUE_1 == 1"                                             |
+-----+--------------------------------------------------------------------+
| ALL | ".BACKGROUND "parallax.gb""                                        |
+-----+--------------------------------------------------------------------+
| ALL | ".BANK 0 SLOT 1"                                                   |
+-----+--------------------------------------------------------------------+
| ALL | ".BASE $80"                                                        |
+-----+--------------------------------------------------------------------+
| ALL | ".BITS 4 DATA %1011, %0100, %1010, %0101"                          |
+-----+--------------------------------------------------------------------+
| ALL | ".BLOCK "Block1""                                                  |
+-----+--------------------------------------------------------------------+
| ALL | ".BR"                                                              |
+-----+--------------------------------------------------------------------+
| ALL | ".BREAK"                                                           |
+-----+--------------------------------------------------------------------+
| ALL | ".BREAKPOINT"                                                      |
+-----+--------------------------------------------------------------------+
| ALL | ".BYT 100, $30, %1000, "HELLO WORLD!""                             |
+-----+--------------------------------------------------------------------+
| ALL | ".CONTINUE"                                                        |
+-----+--------------------------------------------------------------------+
| ALL | ".DATA $ff00, 2"                                                   |
+-----+--------------------------------------------------------------------+
| ALL | ".DB 100, $30, %1000, "HELLO WORLD!""                              |
+-----+--------------------------------------------------------------------+
| ALL | ".DBCOS 0.2, 10, 3.2, 120, 1.3"                                    |
+-----+--------------------------------------------------------------------+
| ALL | ".DBM filtermacro 1, 2, "encrypt me""                              |
+-----+--------------------------------------------------------------------+
| ALL | ".DBRND 20, 0, 10"                                                 |
+-----+--------------------------------------------------------------------+
| ALL | ".DBSIN 0.2, 10, 3.2, 120, 1.3"                                    |
+-----+--------------------------------------------------------------------+
| ALL | ".DD $1ffffff, $2000000"                                           |
+-----+--------------------------------------------------------------------+
| ALL | ".DDM filtermacro 1, 2, 3"                                         |
+-----+--------------------------------------------------------------------+
| ALL | ".DEF IF $FF0F"                                                    |
+-----+--------------------------------------------------------------------+
| ALL | ".DEFINE IF $FF0F"                                                 |
+-----+--------------------------------------------------------------------+
| ALL | ".DL $102030, $405060"                                             |
+-----+--------------------------------------------------------------------+
| ALL | ".DLM filtermacro 1, 2, 3"                                         |
+-----+--------------------------------------------------------------------+
| ALL | ".DS 256, $10"                                                     |
+-----+--------------------------------------------------------------------+
| ALL | ".DSB 256, $10"                                                    |
+-----+--------------------------------------------------------------------+
| ALL | ".DSD 256, $1ffffff"                                               |
+-----+--------------------------------------------------------------------+
| ALL | ".DSL 16, $102030"                                                 |
+-----+--------------------------------------------------------------------+
| ALL | ".DSTRUCT waterdrop INSTANCEOF water DATA "tingle", 40, 120"       |
+-----+--------------------------------------------------------------------+
| ALL | ".DSW 128, 20"                                                     |
+-----+--------------------------------------------------------------------+
| ALL | ".DW 16000, 10, 255"                                               |
+-----+--------------------------------------------------------------------+
| ALL | ".DWCOS 0.2, 10, 3.2, 1024, 1.3"                                   |
+-----+--------------------------------------------------------------------+
| ALL | ".DWM filtermacro 1, 2, 3"                                         |
+-----+--------------------------------------------------------------------+
| ALL | ".DWRND 20, 0, 10"                                                 |
+-----+--------------------------------------------------------------------+
| ALL | ".DWSIN 0.2, 10, 3.2, 1024, 1.3"                                   |
+-----+--------------------------------------------------------------------+
| ALL | ".ELIF defined(DEBUG) && VERSION > 110"                            |
+-----+--------------------------------------------------------------------+
| ALL | ".ELSE"                                                            |
+-----+--------------------------------------------------------------------+
| ALL | ".ENDA"                                                            |
+-----+--------------------------------------------------------------------+
| ALL | ".ENDASM"                                                          |
+-----+--------------------------------------------------------------------+
| ALL | ".ENDB"                                                            |
+-----+--------------------------------------------------------------------+
| ALL | ".ENDBITS"                                                         |
+-----+--------------------------------------------------------------------+
| ALL | ".ENDE"                                                            |
+-----+--------------------------------------------------------------------+
| ALL | ".ENDIF"                                                           |
+-----+--------------------------------------------------------------------+
| ALL | ".ENDM"                                                            |
+-----+--------------------------------------------------------------------+
| ALL | ".ENDME"                                                           |
+-----+--------------------------------------------------------------------+
| ALL | ".ENDR"                                                            |
+-----+--------------------------------------------------------------------+
| ALL | ".ENDRO"                                                           |
+-----+--------------------------------------------------------------------+
| ALL | ".ENDS"                                                            |
+-----+--------------------------------------------------------------------+
| ALL | ".ENDST"                                                           |
+-----+--------------------------------------------------------------------+
| ALL | ".ENDU"                                                            |
+-----+--------------------------------------------------------------------+
| ALL | ".ENUM $C000"                                                      |
+-----+--------------------------------------------------------------------+
| ALL | ".ENUMID ID_1 0"                                                   |
+-----+--------------------------------------------------------------------+
| ALL | ".EQU IF $FF0F"                                                    |
+-----+--------------------------------------------------------------------+
| ALL | ".FAIL "THE EYE OF MORDOR HAS SEEN US!""                           |
+-----+--------------------------------------------------------------------+
| ALL | ".FARADDR main, irq_1"                                             |
+-----+--------------------------------------------------------------------+
| ALL | ".FCLOSE FP_DATABIN"                                               |
+-----+--------------------------------------------------------------------+
| ALL | ".FILTER filtermacro 1, 2, "encrypt me""                           |
+-----+--------------------------------------------------------------------+
| ALL | ".FOPEN "data.bin" FP_DATABIN"                                     |
+-----+--------------------------------------------------------------------+
| ALL | ".FREAD FP_DATABIN DATA"                                           |
+-----+--------------------------------------------------------------------+
| ALL | ".FSEEK FP_DATABIN 10 START"                                       |
+-----+--------------------------------------------------------------------+
| ALL | ".FSIZE FP_DATABIN SIZE"                                           |
+-----+--------------------------------------------------------------------+
| ALL | ".FTELL FP_DATABIN POSITION"                                       |
+-----+--------------------------------------------------------------------+
| ALL | ".FUNCTION SUM_AB(varA,varB)"                                      |
+-----+--------------------------------------------------------------------+
| ALL | ".HEX "a0A0ffDE""                                                  |
+-----+--------------------------------------------------------------------+
| ALL | ".IF DEBUG == 2"                                                   |
+-----+--------------------------------------------------------------------+
| ALL | ".IFDEF IF"                                                        |
+-----+--------------------------------------------------------------------+
| ALL | ".IFDEFM \2"                                                       |
+-----+--------------------------------------------------------------------+
| ALL | ".IFEQ DEBUG 2"                                                    |
+-----+--------------------------------------------------------------------+
| ALL | ".IFEXISTS "main.s""                                               |
+-----+--------------------------------------------------------------------+
| ALL | ".IFGR DEBUG 2"                                                    |
+-----+--------------------------------------------------------------------+
| ALL | ".IFGREQ DEBUG 1"                                                  |
+-----+--------------------------------------------------------------------+
| ALL | ".IFLE DEBUG 2"                                                    |
+-----+--------------------------------------------------------------------+
| ALL | ".IFLEEQ DEBUG 1"                                                  |
+-----+--------------------------------------------------------------------+
| ALL | ".IFNDEF IF"                                                       |
+-----+--------------------------------------------------------------------+
| ALL | ".IFNDEFM \2"                                                      |
+-----+--------------------------------------------------------------------+
| ALL | ".IFNEQ DEBUG 2"                                                   |
+-----+--------------------------------------------------------------------+
| ALL | ".INC "cgb_hardware.i""                                            |
+-----+--------------------------------------------------------------------+
| ALL | ".INCBIN "sorority.bin""                                           |
+-----+--------------------------------------------------------------------+
| ALL | ".INCDIR "/usr/programming/gb/include/""                           |
+-----+--------------------------------------------------------------------+
| ALL | ".INCLUDE "cgb_hardware.i""                                        |
+-----+--------------------------------------------------------------------+
| 658 | ".INDEX 8"                                                         |
+-----+--------------------------------------------------------------------+
| ALL | ".INPUT NAME"                                                      |
+-----+--------------------------------------------------------------------+
| ALL | ".LONG $102030, $405060"                                           |
+-----+--------------------------------------------------------------------+
| ALL | ".MACRO TEST"                                                      |
+-----+--------------------------------------------------------------------+
| ALL | ".MEMORYMAP"                                                       |
+-----+--------------------------------------------------------------------+
| ALL | ".NEXTU name"                                                      |
+-----+--------------------------------------------------------------------+
| 658 | ".NOWDC"                                                           |
+-----+--------------------------------------------------------------------+
| ALL | ".ORG $150"                                                        |
+-----+--------------------------------------------------------------------+
| ALL | ".ORGA $150"                                                       |
+-----+--------------------------------------------------------------------+
| ALL | ".PRINT "Numbers 1 and 10: ", DEC 1, " $", HEX 10, "\n""           |
+-----+--------------------------------------------------------------------+
| ALL | ".PRINTT "Here we are...\n""                                       |
+-----+--------------------------------------------------------------------+
| ALL | ".PRINTV DEC DEBUG+1"                                              |
+-----+--------------------------------------------------------------------+
| ALL | ".RAMSECTION "Vars" BASE $7E BANK 0 SLOT 1 ALIGN 256 OFFSET 32"    |
+-----+--------------------------------------------------------------------+
| ALL | ".REDEF IF $F"                                                     |
+-----+--------------------------------------------------------------------+
| ALL | ".REDEFINE IF $F"                                                  |
+-----+--------------------------------------------------------------------+
| ALL | ".REPEAT 6"                                                        |
+-----+--------------------------------------------------------------------+
| ALL | ".REPT 6"                                                          |
+-----+--------------------------------------------------------------------+
| ALL | ".ROMBANKMAP"                                                      |
+-----+--------------------------------------------------------------------+
| ALL | ".ROMBANKS 2"                                                      |
+-----+--------------------------------------------------------------------+
| ALL | ".ROMBANKSIZE $4000"                                               |
+-----+--------------------------------------------------------------------+
| ALL | ".ROW $ff00, 1, "3""                                               |
+-----+--------------------------------------------------------------------+
| ALL | ".SECTION "Init" FORCE"                                            |
+-----+--------------------------------------------------------------------+
| ALL | ".SEED 123"                                                        |
+-----+--------------------------------------------------------------------+
| ALL | ".SEEDRANDOM"                                                      |
+-----+--------------------------------------------------------------------+
| ALL | ".SHIFT"                                                           |
+-----+--------------------------------------------------------------------+
| ALL | ".SLOT 1"                                                          |
+-----+--------------------------------------------------------------------+
| ALL | ".STRINGMAP script "Hello\n""                                      |
+-----+--------------------------------------------------------------------+
| ALL | ".STRINGMAPTABLE script "script.tbl""                              |
+-----+--------------------------------------------------------------------+
| ALL | ".STRUCT enemy_object"                                             |
+-----+--------------------------------------------------------------------+
| ALL | ".SYM SAUSAGE"                                                     |
+-----+--------------------------------------------------------------------+
| ALL | ".SYMBOL SAUSAGE"                                                  |
+-----+--------------------------------------------------------------------+
| ALL | ".TABLE byte, word, byte"                                          |
+-----+--------------------------------------------------------------------+
| ALL | ".UNBACKGROUND $1000 $1FFF"                                        |
+-----+--------------------------------------------------------------------+
| ALL | ".UNDEF DEBUG"                                                     |
+-----+--------------------------------------------------------------------+
| ALL | ".UNDEFINE DEBUG"                                                  |
+-----+--------------------------------------------------------------------+
| ALL | ".UNION name"                                                      |
+-----+--------------------------------------------------------------------+
| 658 | ".WDC"                                                             |
+-----+--------------------------------------------------------------------+
| ALL | ".WHILE COUNTER > 0"                                               |
+-----+--------------------------------------------------------------------+
| ALL | ".WORD 16000, 10, 255"                                             |
+-----+--------------------------------------------------------------------+

Descriptions:


".16BIT"
========

Analogous to ".8BIT". ".16BIT" forces all addresses and immediate
values to be expanded into 16-bit range, when possible, that is:

   LSR 11       ; $46 $0B

That would be the case, normally, but after ".16BIT" it becomes:

   LSR 11       ; $4E $0B $00

This is not a compulsory directive.


".24BIT"
========

Analogous to ".8BIT" and ".16BIT". ".24BIT" forces all addresses to be
expanded into 24-bit range, when possible, that is:

   AND $11       ; $25 $11

That would be the case, normally, but after ".24BIT" it becomes:

   AND $11       ; $2F $11 $00 $00

If it is not possible to expand the address into ".24BIT" range, then
WLA tries to expand it into 16-bit range.

This is not a compulsory directive.


".8BIT"
=======

There are a few mnemonics that look identical, but take different
sized arguments. Here’s a list of such 6502 mnemonics:

ADC, AND, ASL, BIT, CMP, CPX, CPY, DEC, EOR, INC, LDA, LDX, LDY, ORA,
ROL, SBC, STA, STX and STY.

For example:

   LSR 11       ; $46 $0B
   LSR $A000    ; $4E $00 $A0

The first one could also be:

   LSR 11       ; $4E $0B $00

".8BIT" is here to help WLA to decide to choose which one of the
opcodes it selects. When you give ".8BIT" (default) no 8-bit
address/value is expanded to 16-bits.

By default WLA uses the smallest possible size. This is true also when
WLA finds a computation it can’t solve right away. WLA assumes the
result will be inside the smallest possible bounds, which depends on
the type of the mnemonic.

You can also use the fixed argument size versions of such mnemonics by
giving the size with the operand (i.e., operand hinting). Here are few
examples:

   LSR 11.B   ; $46 $0B
   LSR 11.W   ; $4E $0B $00

In WLA-65816 ".ACCU" / ".INDEX" / "SEP" / "REP" override ".8BIT" /
".16BIT"/".24BIT" when considering the immediate values, so be
careful. Still, operand hints override all of these, so use them to be
sure.

This is not a compulsory directive.


".ACCU 8"
=========

Forces WLA to override the accumulator size given with "SEP" / "REP".
".ACCU" doesn’t produce any code, it only affects the way WLA
interprets the immediate values (8 for 8 bit operands, 16 for 16 bit
operands) for opcodes dealing with the accumulator.

So after giving ".ACCU 8":

   AND #6

will produce "$29 $06", and after giving ".ACCU 16":

   AND #6

will yield "$29 $00 $06".

Note that "SEP" / "REP" again will in turn reset the accumulator/index
register size.

This is not a compulsory directive.


".ADDR 16000, main, 255"
========================

".ADDR" is an alias for ".DW".

This is not a compulsory directive.


".ALIGN 4"
==========

Makes it so that on the next line the address is a multiple of the
supplied value. Currently this directive can only be given outside
".SECTION" s or inside FORCE ".SECTION" s or inside ".SECTION" s that
have ALIGN that is a multiple of the ".ALIGN" here.

This is not a compulsory directive.


".ARRAYDB NAME MyArray INDICES '0', 0, 1"
=========================================

This is the same as ".DB", but defines bytes by reading indexed values
from the given array. In the example the indices are ‘0’ (48), 0 and
1.

"NAME" and "INDICES" are optional so this works also:

   .ARRAYDB MyArray '0', 0, 1

If you supply ".ARRAYDB" a string as indices, each character is used
as an index:

   .ARRAYDB NAME MyArray INDICES "MAP THIS!"

This is not a compulsory directive.


".ARRAYDD NAME MyArray INDICES '0', 0, 1"
=========================================

".ARRAYDD" works the same way as ".ARRAYDB", but defines 32-bit double
words.

This is not a compulsory directive.


".ARRAYDEF NAME MyArray SIZE 256"
=================================

".ARRAYDEF" is an alias for ".ARRAYDEFINE".

This is not a compulsory directive.


".ARRAYDEFINE NAME MyArray SIZE 256"
====================================

Defines an array called MyArray, and its initial size is 256 items.
Each item is an ANSI C89 int (32-bit). The array can be written into
using directive ".ARRAYIN" and it can be read from using directive
".ARRAYOUT". This array exists only in WLA’s memory and during
assembling, but it can be used for e.g., mapping parts of ASCII table
into e.g., 4 bits:

   // define a too small array for mapping "0123456789" -> 4-bits
   // it gets enlarged by out-of-bounds .ARRAYINs later...
   .ARRAYDEFINE NAME MyArray SIZE 4

   // define the mapping
   .ARRAYIN NAME MyArray INDEX '0' VALUE %0000
   .ARRAYIN NAME MyArray INDEX '1' VALUE %0001
   .ARRAYIN NAME MyArray INDEX '2' VALUE %0010
   .ARRAYIN NAME MyArray INDEX '3' VALUE %0011
   .ARRAYIN NAME MyArray INDEX '4' VALUE %0100
   .ARRAYIN NAME MyArray INDEX '5' VALUE %0101
   .ARRAYIN NAME MyArray INDEX '6' VALUE %0110
   .ARRAYIN NAME MyArray INDEX '7' VALUE %0111
   .ARRAYIN NAME MyArray INDEX '8' VALUE %1000
   .ARRAYIN NAME MyArray INDEX '9' VALUE %1001

   // map!
   .ARRAYOUT NAME MyArray INDEX '6' DEFINITION Mapping
   .DB Mapping
   .ARRAYOUT NAME MyArray INDEX '6' DEFINITION Mapping
   .DB Mapping
   .ARRAYOUT NAME MyArray INDEX '8' DEFINITION Mapping
   .DB Mapping
   .ARRAYOUT NAME MyArray INDEX '2' DEFINITION Mapping
   .DB Mapping
   .ARRAYOUT NAME MyArray INDEX '7' DEFINITION Mapping
   .DB Mapping
   .ARRAYOUT NAME MyArray INDEX '5' DEFINITION Mapping
   .DB Mapping

You can also do the mapping using e.g., ".ARRAYDB":

   .ARRAYDB NAME MyArray INDICES '6', '6', '8', '2', '7', '5'
   .ARRAYDB NAME MyArray INDICES "668275"

And create the mapping using only one ".ARRAYIN":

   .ARRAYIN NAME MyArray INDEX '0' VALUES %0000, %0001, \
       %0010, %0011, %0100, %0101, %0110, %0111, %1000, \
       %1001

Note that keywords NAME and SIZE are optional, so this works also:

   .ARRAYDEFINE MyArray 4

This is not a compulsory directive.


".ARRAYDL NAME MyArray INDICES '0', 0, 1"
=========================================

".ARRAYDL" works the same way as ".ARRAYDB", but defines 24-bit long
words.

This is not a compulsory directive.


".ARRAYDW NAME MyArray INDICES '0', 0, 1"
=========================================

".ARRAYDW" works the same way as ".ARRAYDB", but defines 16-bit words.

This is not a compulsory directive.


".ARRAYIN NAME MyArray INDEX 0 VALUE 10"
========================================

Writes a value into an array defined using ".ARRAYDEFINE". Check out
".ARRAYDEFINE" for a nice example. The value needs to be known at the
time the assembler is parsing through the code.

Keywords NAME, INDEX and VALUE are optional so this works also:

   .ARRAYIN MyArray 0 10

This is not a compulsory directive.


".ARRAYOUT NAME MyArray INDEX 0 DEFINITION ArrayOut"
====================================================

Reads a value from an array defined using ".ARRAYDEFINE". Check out
".ARRAYDEFINE" for a nice example. The value is stored in definition
"ArrayOut" in the example.

Keywords NAME, INDEX and DEFINITION are optional so this works also:

   .ARRAYOUT MyArray 0 ArrayOut

This is not a compulsory directive.


".ASC "HELLO WORLD!""
=====================

".ASC" is an alias for ".DB", but if you use ".ASC" it will remap the
characters using the mapping given via ".ASCIITABLE".

You can also use ASC(‘?’) to map individual characters in the code

   .DB ASC('A'), ASC('B'), ASC(10), ASC('\r')

and

   LD A, ASC(‘A’)

This is not a compulsory directive.


".ASCIITABLE"
=============

".ASCIITABLE"’s only purpose is to provide character mapping for
".ASC" and "ASC('?')". Take a look at the example:

   .ASCIITABLE
   MAP "A" TO "Z" = 0
   MAP "!" = 90
   .ENDA

Here we set such a mapping that character "A" is equal to "0", "B" is
equal to "1", "C" is equal to "2", and so on, and "!" is equal to
"90".

After you’ve given the ".ASCIITABLE", use ".ASC" to define bytes using
this mapping (".ASC" is an alias for ".DB", but with ".ASCIITABLE"
mapping). For example, ".ASC "ABZ"" would define bytes "0", "1" and
"25" in our previous example.

Note that the following works as well:

   .ASCIITABLE
   MAP 'A' TO 'Z' = 0
   MAP 65 = 90          ; 65 is the decimal for ASCII 'A'
   .ENDA

Also note that the characters that are not given any mapping in
".ASCIITABLE" map to themselves (i.e., "2" maps to "2" in our previous
example, etc.).

This is not a compulsory directive.


".ASCSTR "HELLO WORLD!", $A"
============================

".ASCSTR" is the same as ".ASC", but it maps only supplied strings.
All given bytes are not touched.:

   .ASCSTR "HELLO WORLD!", $A

In this example the string “HELLO WORLD!” is mapped using the mapping
given via ".ASCIITABLE", but the last byte $A is left as it is.

This is not a compulsory directive.


".ASCTABLE"
===========

".ASCTABLE" is an alias for ".ASCIITABLE".

This is not a compulsory directive.


".ASM"
======

Tells WLA to start assembling. Use ".ASM" to continue the work which
has been disabled with ".ENDASM". ".ASM" and ".ENDASM" can be used to
mask away big blocks of code. This is analogous to the ANSI C
-comments ("/*...*/"), but ".ASM" and ".ENDASM" can be nested, unlike
the ANSI C -counterpart.

This is not a compulsory directive.


".ASSERT VALUE_1 == 1"
======================

".ASSERT" takes a condition, and if it’s evaluated to be true, nothing
happens. If it’s false, then assembling ends right there in an error.

This is not a compulsory directive.


".BACKGROUND "parallax.gb""
===========================

This chooses an existing ROM image ("parallax.gb" in this case) as a
background data for the project. You can overwrite the data with
"OVERWRITE" sections only, unless you first clear memory blocks with
".UNBACKGROUND" after which there’s room for other sections as well.

Note that ".BACKGROUND" can be used only when compiling an object
file.

".BACKGROUND" is useful if you wish to patch an existing ROM image
with new code or data.

This is not a compulsory directive.


".BANK 0 SLOT 1"
================

Defines the ROM bank and the slot it is inserted into in the memory.
You can also type the following:

   .BANK 0

This tells WLA to move into BANK 0 which will be put into the
"DEFAULTSLOT" of ".MEMORYMAP".

Every time you use ".BANK", supply ".ORG" / ".ORGA" as well, just to
make sure WLA calculates addresses correctly.

This is a compulsory directive.


".BASE $80"
===========

Defines the base value for the bank number (used only in 24-bit
addresses and when getting a label’s bank number with ":"). Here are
few examples of how to use ".BASE" (both examples assume the label
resides in the first ROM bank):

   .BASE $00
   label1:
   .BASE $80
   label2:

     JSL label1   ; if label1 address is $1234, this will assemble into
                  ; JSL $001234
     JSL label2   ; label2 is also $1234, but this time the result will be
                  ; JSL $801234

".BASE" defaults to "$00". Note that the address of the label will
also contribute to the bank number (bank number == ".BASE" + ROM bank
of the label).

On 65816, use ".LOROM", ".HIROM" or ".EXHIROM" to define the ROM mode.

This is not a compulsory directive.


".BITS 4 DATA %1011, %0100, %1010, %0101"
=========================================

This is the same as ".DB", but defines bits (1-32). Consecutive
".BITS" will supply bits to the same bitstream, so don’t do any stream
breaking ".DB" calls or anything that defines data. "DATA" is
optional. Give

   .BITS START

to start a new bitstream.

Here’s an example of how to define two bytes worth of bits:

   .BITS 6 CABBAGE, %011110    ; CABBAGE == %110011
   .BITS 4 8+2                 ; 8 + 2 == %1010
   .BITS 4 %1011
   .ENDBITS                    ; writes the final byte in the bitstream
                               ; and resets the counters

If your ".BITS" bitstream doesn’t define exactly a multiple of 8 bits,
the remaining bits are set to zero. Remember to issue ".ENDBITS" after
the last ".BITS".

Currently the bits are written from most significant bit to the least
significant bit, so our previous example would give us (consecutive)
bytes %11001101, %11101010 and %10110000 ($CD, $EA and $B0).

This is not a compulsory directive.


".BLOCK "Block1""
=================

Begins a block (called "Block1" in the example). These blocks have
only one function: to display the number of bytes they contain. When
you embed such a block into your code, WLA displays its size when it
assembles the source file.

Use ".ENDB" to terminate a ".BLOCK". Note that you can nest ".BLOCK"
s.

This is not a compulsory directive.


".BR"
=====

Inserts a breakpoint that behaves like a ".SYM" without a name.
Breakpoints can only be seen in WLALINK’s symbol file.

This is not a compulsory directive.


".BREAK"
========

Exits the active ".REPEAT" or ".WHILE".

This is not a compulsory directive.


".BREAKPOINT"
=============

".BREAKPOINT" is an alias for ".BR".

This is not a compulsory directive.


".BYT 100, $30, %1000, "HELLO WORLD!""
======================================

".BYT" is an alias for ".DB".

This is not a compulsory directive.


".CARTRIDGETYPE 1"
==================

Indicates the type of the cartridge (mapper and so on). This is a
standard Gameboy cartridge type indicator value found at "$147" in a
Gameboy ROM, and there this one is put to also.

This is not a compulsory directive.


".COMPUTEGBCHECKSUM"
====================

When this directive is used WLA computes the ROM checksum found at
"$14E" and "$14F" in a Gameboy ROM. Note that this directive can only
be used with WLA-GB.

Note that you can also write ".COMPUTECHECKSUM" (the old name for this
directive), but it’s not recommended.

This is not a compulsory directive.


".COMPUTEGBCOMPLEMENTCHECK"
===========================

When this directive is used WLA computes the ROM complement check
found at "$14D" in a Gameboy ROM.

Note that you can still use ".COMPUTECOMPLEMENTCHECK" (the old name
for this directive), but it’s not recommended.

This is not a compulsory directive.


".COMPUTESMDCHECKSUM"
=====================

When this directive is used WLA computes the Sega Mega Drive ROM
checksum found at "$18E". Note that this directive works only with
WLA-68000.

This is not a compulsory directive.


".COMPUTESMSCHECKSUM"
=====================

When this directive is used WLA computes the ROM checksum found at
"$7FFA" and "$7FFB" (or "$3FFA" - "$3FFB" is the ROM is 16KBs, or
"$1FFA" - "$1FFB" for 8KB ROMs) in a SMS/GG ROM. Note that this
directive can only be used with WLA-z80. Also note that the ROM size
must be at least 8KBs. The checksum is calculated using bytes "0x0000"
- "0x1FEF" / "0x3FEF" / "0x7FEF".

This is not a compulsory directive.


".COMPUTESNESCHECKSUM"
======================

When this directive is used WLA computes the SNES ROM checksum and
inverse checksum found at "$7FDC" - "$7FDF" (LoROM), "$FFDC" - "$FFDF"
(HiROM) or "$40FFDC" - "$40FFDF" and "$FFDC" - "$FFDF" (ExHiROM). Note
that this directive can only be used with WLA-65816. Also note that
the ROM size must be at least 32KB for LoROM images, 64KB for HiROM
images and 32.5MBit for ExHiROM.

".LOROM", ".HIROM" or ".EXHIROM" must be issued before
".COMPUTESNESCHECKSUM".

This is not a compulsory directive.


".CONTINUE"
===========

Jumps to the beginning of an active ".REPEAT" or ".WHILE".

This is not a compulsory directive.


".COUNTRYCODE 1"
================

Indicates the country code located at "$14A" of a Gameboy ROM.

This is not a compulsory directive.


".DATA $ff00, 2"
================

Defines bytes after a .TABLE has been used to define the format. An
alternative way of defining bytes to .DB/.DW.

Note that when you use .DATA you can give as many items .TABLE
defines. The next time you’ll use .DATA you’ll continue from the point
the previous .DATA ended.

Examples:

   .TABLE dsw 2, dsb 2

This defines two rows worth of bytes:

   .DATA $ff00, $aabb, $10, $20, $1020, $3040, $50, $60

This does the same:

   .DATA $ff00, $aabb
   .DATA $10, $20
   .DATA $1020, $3040
   .DATA $50, $60

This is not a compulsory directive.


".DB 100, $30, %1000, "HELLO WORLD!""
=====================================

Defines bytes.

This is not a compulsory directive.


".DBCOS 0.2, 10, 3.2, 120, 1.3"
===============================

Defines bytes just like ".DSB" does, only this time they are filled
with cosine data. ".DBCOS" takes five arguments.

The first argument is the starting angle. Angle value ranges from "0"
to "359.999"…, but you can supply WLA with values that are out of the
range - WLA fixes them ok. The value can be integer or float.

The second argument descibes the amount of additional angles. The
example will define 11 angles.

The third argument is the adder value which is added to the angle
value when next angle is calculated. The value can be integer or
float.

The fourth and fifth arguments can be seen from the pseudo code below,
which also describes how ".DBCOS" works. The values can be integer or
float.

Remember that "cos" (and "sin") here returns values ranging from "-1"
to "1":

   .DBCOS A, B, C, D, E

   for (B++; B > 0; B--) {
     output_data((D * cos(A)) + E)
     A = keep_in_range(A + C)
   }

This is not a compulsory directive.


".DBM filtermacro 1, 2, "encrypt me""
=====================================

Defines bytes using a filter macro. All the data is passed to
"filtermacro" in the first argument, one byte at a time, and the byte
that actually gets defined is the value of definition "_OUT" ("_out"
works as well). The second macro argument holds the offset from the
beginning (the first byte) in bytes (the series being "0", "1", "2",
"3", …).

Here’s an example of a filter macro that increments all the bytes by
one:

   .macro increment
   .redefine _out \1+1
   .endm

This is not a compulsory directive.


".DBRND 20, 0, 10"
==================

Defines bytes, just like ".DSB" does, only this time they are filled
with (pseudo) random numbers. We use the integrated Mersenne Twister
to generate the random numbers. If you want to seed the random number
generator, use ".SEED".

The first parameter ("20" in the example) defines the number of random
numbers we want to generate. The next two tell the range of the random
numbers, i.e. min and max.

Here’s how it works:

   .DBRND A, B, C

   for (i = 0; i < A; i++)
     output_data((rand() % (C-B+1)) + B);

You can also use the following keywords to make the code clearer:

   .DBRND COUNT A MIN B MAX C

This is not a compulsory directive.


".DBSIN 0.2, 10, 3.2, 120, 1.3"
===============================

Analogous to ".DBCOS", but does "sin()" instead of "cos()".

This is not a compulsory directive.


".DD $1ffffff, $2000000"
========================

Defines double words (four bytes each). ".DD" takes only numbers,
labels and characters as input, not strings.

This is not a compulsory directive.


".DDM filtermacro 1, 2, 3"
==========================

Defines 32-bit words using a filter macro. Works just like ".DBM",
".DWM" and ".DLM".

This is not a compulsory directive.


".DEF IF $FF0F"
===============

".DEF" is an alias for ".DEFINE".

This is not a compulsory directive.


".DEFINE IF $FF0F"
==================

Assigns a number or a string to a definition label.

By default all defines are local to the file where they are presented.
If you want to make the definition visible to all the files in the
project, use ".EXPORT" or add EXPORT to the end of .DEFINE:

   .DEFINE ID_0 0 EXPORT

WARNING: Please declare your definition lexically before using it as
otherwise the assembler might make incorrect assumptions about its
value and size and choose e.g. wrong opcodes and generate binary that
doesn’t run properly.

Here are some examples:

   .DEFINE X 1000
   .DEFINE FILE "level01.bin"
   .DEFINE TXT1 "hello and welcome", 1, "to a new world...", 0
   .DEFINE BYTES 1, 2, 3, 4, 5
   .DEFINE COMPUTATION X+1
   .DEFINE DEFAULTV

All definitions with multiple values are marked as data strings, and
".DB" is about the only place where you can later on use them:

   .DEFINE BYTES 1, 2, 3, 4, 5
   .DB 0, BYTES, 6

is the same as:

   .DB 0, 1, 2, 3, 4, 5, 6

If you omit the definition value (in our example "DEFAULTV"), WLA will
default to "0".

Note that you must do your definition before you use it, otherwise WLA
will use the final value of the definition. Here’s an example of this:

   .DEFINE AAA 10
   .DB AAA            ; will be 10.
   .REDEFINE AAA 11

but:

   .DB AAA            ; will be 11.
   .DEFINE AAA 10
   .REDEFINE AAA 11

You can also create definitions on the command line. Here’s an example
of this:

   wla-gb -vl -DMOON -DNAME=john -DPRICE=100 -DADDRESS=$100 math.s

"MOON"’s value will be "0", "NAME" is a string definition with value
"john", "PRICE"’s value will be "100", and "ADDRESS"’s value will be
"$100".

Note that:

   .DEFINE AAA = 10   ; the same as ".DEFINE AAA 10".

works as well. And this works also:

   AAA = 10

This is not a compulsory directive.


".DESTINATIONCODE 1"
====================

".DESTINATIONCODE" is an alias for ".COUNTRYCODE".

This is not a compulsory directive.


".DL $102030, $405060"
======================

Defines long words (three bytes each). ".DL" takes only numbers,
labels and characters as input, not strings.

This is not a compulsory directive.


".DLM filtermacro 1, 2, 3"
==========================

Defines 24-bit words using a filter macro. Works just like ".DBM",
".DWM" and ".DDM".

This is not a compulsory directive.


".DS 256, $10"
==============

".DS" is an alias for ".DSB".

This is not a compulsory directive.


".DSB 256, $10"
===============

Defines "256" bytes of "$10".

This is not a compulsory directive.


".DSD 256, $1ffffff"
====================

Defines "256" double words (four bytes) of "$1ffffff".

This is not a compulsory directive.


".DSL 16, $102030"
==================

Defines "16" long words (three bytes) of "$102030".

This is not a compulsory directive.


".DSTRUCT waterdrop INSTANCEOF water VALUES"
============================================

Defines an instance of ".STRUCT" water, called waterdrop, and fills it
with the given data. Before calling ".DSTRUCT" we must have defined
the structure, and in this example it could be like:

   .STRUCT water
       name   ds 8
       age    db
       weight dw
   .ENDST

There are two syntaxes for ".DSTRUCT"; the new and legacy versions. To
use the new syntax, put the keyword "VALUES" at the end of the first
line. The old syntax uses the keyword "DATA" or none at all.

The new syntax looks like this:

   .DSTRUCT waterdrop INSTANCEOF water VALUES
       name:   .db "tingle"
       age:    .db 40
       weight: .dw 120
   .ENDST

The fields can be put in any order. Any omitted fields are set to the
".EMPTYFILL" value ("$00" by default). Any data-defining directive can
be used within ".DSTRUCT", as long as it does not exceed the size of
the data it is being defined for. The only exception is ".DSTRUCT"
itself, which cannot be nested.

The old syntax looks like this:

   .DSTRUCT waterdrop INSTANCEOF water DATA "tingle", 40, 120

The "DATA" and "INSTANCEOF" keywords are optional. This will assign
data for each field of the struct in the order they were defined.

In either example you would get the following labels:

   waterdrop
   waterdrop.name
   waterdrop.age
   waterdrop.weight
   _sizeof_waterdrop        = 11
   _sizeof_waterdrop.name   = 8
   _sizeof_waterdrop.age    = 1
   _sizeof_waterdrop.weight = 2

The legacy syntax does not support unions; it will give an error if
you attempt to define data for a union.

For the new syntax, nested structs are supported like so (assume the
"water" struct is also defined:

   .STRUCT drop_pair
       waterdrops: instanceof water 2
   .ENDST

   .DSTRUCT drops INSTANCEOF drop_pair VALUES
       waterdrops.1:        .db "qwertyui" 40
                            .dw 120
       waterdrops.2.name:   .db "tingle"
       waterdrops.2.age:    .db 40
       waterdrops.2.weight: .dw 12
   .ENDST

In this case, the properties of "waterdrops.1" were defined
implicitly; 8 bytes for the name, followed by a byte for the age,
followed by a word for the weight. The values for "waterdrops.2" were
defined in a more clear way.

In this case, "waterdrops" and "waterdrops.1" are equivalent.
"waterdrops.1.name" is different, even though its address is the same,
because it has a size of 8. If you attempted to do this:

   .DSTRUCT drops INSTANCEOF drop_pair VALUES
       waterdrops.1.name:   .db "qwertyui" 40
                            .dw 120
   .ENDST

It would fail, because only the 8 name bytes are available to be
defined in this context, as opposed to the 11 bytes for the entire
"waterdrops.1" structure.

Named unions can be assigned to in a similar way, by writing its full
name with a "." separating the union name and the field name.

The struct can be defined namelessly:

   .DSTRUCT INSTANCEOF drop_pair VALUES
       ...
   .ENDST

You can use "SIZE" to specify the size of the instance. The additional
bytes are filled with ".EMPTYFILL":

   .DSTRUCT INSTANCEOF drop_pair SIZE 128 VALUES
       ...
   .ENDST

If you don’t want to generate labels use "NOLABELS":

   .DSTRUCT INSTANCEOF drop_pair NOLABELS VALUES
       ...
   .ENDST

Here’s another example using the legacy syntax:

   .DSTRUCT INSTANCEOF water SIZE 32 NOLABELS DATA "Ocean", 100, 16384

This is not a compulsory directive.


".DSW 128, 20"
==============

Defines "128" words (two bytes) of "20".

This is not a compulsory directive.


".DW 16000, 10, 255"
====================

Defines words (two bytes each). ".DW" takes only numbers, labels and
characters as input, not strings.

This is not a compulsory directive.


".DWCOS 0.2, 10, 3.2, 1024, 1.3"
================================

Analogous to ".DBCOS" (but defines 16-bit words).

This is not a compulsory directive.


".DWM filtermacro 1, 2, 3"
==========================

Defines 16-bit words using a filter macro. Works just like ".DBM",
".DLM" and  ".DDM".

This is not a compulsory directive.


".DWRND 20, 0, 10"
==================

Analogous to ".DBRND" (but defines words).

This is not a compulsory directive.


".DWSIN 0.2, 10, 3.2, 1024, 1.3"
================================

Analogous to ".DBCOS" (but defines 16-bit words and does "sin()"
instead of "cos()").

This is not a compulsory directive.


".ELIF defined(DEBUG) && VERSION > 110"
=======================================

".ELIF" means "ELSE IF". Can be used after an ".IF" and the likes in
following fashion

   .IF VERSION == 101
     .db 1
   .ELIF VERSION == 102
     .db 2
   .ELIF VERSION == 103
     .db 3
   .ELSE
     .db $ff
   .ENDIF

This is not a compulsory directive.


".ELSE"
=======

If the previous ".IFxxx" failed then the following text until ".ENDIF"
is acknowledged.

This is not a compulsory directive.


".EMPTYFILL $C9"
================

This byte is used in filling the unused areas of the ROM file.
"EMPTYFILL" defaults to "$00".

This is not a compulsory directive.


".ENDASM"
=========

Tells WLA to stop assembling. Use ".ASM" to continue the work.

This is not a compulsory directive.


".ENDA"
=======

Ends the ASCII table.

This is not a compulsory directive, but when ".ASCIITABLE" or
".ASCTABLE" are used this one is required to terminate them.


".ENDB"
=======

Terminates ".BLOCK".

This is not a compulsory directive, but when ".BLOCK" is used this one
is required to terminate it.


".ENDBITS"
==========

Terminates ".BITS".

This is not a compulsory directive, but when ".BITS" is used this one
is required to terminate it.


".ENDEMUVECTOR"
===============

Ends definition of the emulation mode interrupt vector table.

This is not a compulsory directive, but when ".SNESEMUVECTOR" is used
this one is required to terminate it.


".ENDE"
=======

Ends the enumeration.

This is not a compulsory directive, but when ".ENUM" is used this one
is required to terminate it.


".ENDIF"
========

This terminates any ".IFxxx" directive.

This is not a compulsory directive, but if you use any ".IFxxx" then
you need also to apply this.


".ENDME"
========

Terminates ".MEMORYMAP".

This is not a compulsory directive, but when ".MEMORYMAP" is used this
one is required to terminate it.


".ENDM"
=======

Ends a ".MACRO".

This is not a compulsory directive, but when ".MACRO" is used this one
is required to terminate it.


".ENDNATIVEVECTOR"
==================

Ends definition of the native mode interrupt vector table.

This is not a compulsory directive, but when ".SNESNATIVEVECTOR" is
used this one is required to terminate it.


".ENDRO"
========

Ends the rom bank map.

This is not a compulsory directive, but when ".ROMBANKMAP" is used
this one is required to terminate it.


".ENDR"
=======

Ends the ".REPEAT" or ".WHILE".

This is not a compulsory directive, but when ".REPEAT" or ".WHILE" is
used this one is required to terminate it.


".ENDSNES"
==========

This ends the SNES header definition.

This is not a compulsory directive, but when ".SNESHEADER" is used
this one is required to terminate it.


".ENDST"
========

Ends the structure definition.

This is not a compulsory directive, but when ".STRUCT" is used this
one is required to terminate it.


".ENDS"
=======

Ends the section.

This is not a compulsory directive, but when ".SECTION" or
".RAMSECTION" is used this one is required to terminate it.


".ENDU"
=======

Ends the current union.


".ENUM $C000"
=============

Starts enumeration from "$C000". Very useful for defining variables.

To start a descending enumeration, put "DESC" after the starting
value. WLA defaults to "ASC" (ascending enumeration).

You can also add "EXPORT" after these if you want to export all the
generated definitions automatically.

Here’s an example of ".ENUM":

   .STRUCT mon                ; check out the documentation on
   name ds 2                  ; .STRUCT
   age  db
   .ENDST

   .ENUM $A000
   _scroll_x DB               ; db  - define byte (byt and byte work also)
   _scroll_y DB
   player_x: DW               ; dw  - define word (word works also)
   player_y: DW
   map_01:   DS  16           ; ds  - define size (bytes)
   map_02    DSB 16           ; dsb - define size (bytes)
   map_03    DSW  8           ; dsw - define size (words)
   monster   INSTANCEOF mon 3 ; three instances of structure mon
   dragon    INSTANCEOF mon   ; one mon
   .ENDE

Previous example transforms into following definitions:

   .DEFINE _scroll_x      $A000
   .DEFINE _scroll_y      $A001
   .DEFINE player_x       $A002
   .DEFINE player_y       $A004
   .DEFINE map_01         $A006
   .DEFINE map_02         $A016
   .DEFINE map_03         $A026
   .DEFINE monster        $A036
   .DEFINE monster.1      $A036
   .DEFINE monster.1.name $A036
   .DEFINE monster.1.age  $A038
   .DEFINE monster.2      $A039
   .DEFINE monster.2.name $A039
   .DEFINE monster.2.age  $A03B
   .DEFINE monster.3      $A03C
   .DEFINE monster.3.name $A03C
   .DEFINE monster.3.age  $A03E
   .DEFINE dragon         $A03F
   .DEFINE dragon.name    $A03F
   .DEFINE dragon.age     $A041

"DB", "DW", "DS", "DSB", "DSW" and "INSTANCEOF" can also be in
lowercase. You can also use a dotted version of the symbols, but it
doesn’t advance the memory address. Here’s an example:

   .ENUM $C000 DESC EXPORT
   bigapple_h db
   bigapple_l db
   bigapple:  .dw
   .ENDE

And this is what is generated:

   .DEFINE bigapple_h $BFFF
   .DEFINE bigapple_l $BFFE
   .DEFINE bigapple   $BFFE
   .EXPORT bigapple, bigapple_l, bigapple_h

This way you can generate a 16-bit variable address along with
pointers to its parts.

Here’s another example with a nameless INSTANCEOF:

   .STRUCT position_t
   pos_x  DW
   pos_y  DW
   .ENDST

   .STRUCT enemy_t
   id     DW
          INSTANCEOF position_t ; here we import fields from position_t
   health DW
   .ENDST

   .ENUM $A000
   nemesis INSTANCEOF enemy_t
   .ENDE

Regarding nemesis, you’ll get these definitions:

   .DEFINE nemesis        $A000
   .DEFINE nemesis.id     $A000
   .DEFINE nemesis.pos_x  $A002
   .DEFINE nemesis.pos_y  $A004
   .DEFINE nemesis.health $A006

If you want more flexible variable positioning, take a look at
".RAMSECTION" s.

You can also specify the size of an instantiated struct (padding added
at the end) using the keyword "SIZE". Also use keyword "COUNT" to make
things more clear:

   .STRUCT mon                            ; the size of this .STRUCT is 3 (bytes)
   name ds 2
   age  db
   .ENDST

   .ENUM $A000
   monsters INSTANCEOF mon SIZE 4 COUNT 2 ; two instances of structure mon.
   .ENDE                                  ; each instance is padded to 4 bytes.

Note that in the previous example we’ll also get extra definitions

   _paddingof_monsters.1   (== 1) _paddingof_monsters.2   (== 1)

This is not a compulsory directive.


".ENUMID ID_1 0"
================

".ENUMID" will create definitions with an autoincrementing value. For
example:

   .ENUMID 0
   .ENUMID ID_1
   .ENUMID ID_2
   .ENUMID ID_3

… will create the following definitions:

   ID_1 = 0
   ID_2 = 1
   ID_3 = 2

You can also specify the adder:

   .ENUMID 0 STEP 2
   .ENUMID MONSTER_ID_1
   .ENUMID MONSTER_ID_2
   .ENUMID MONSTER_ID_3

… to create definitions:

   MONSTER_ID_1 = 0
   MONSTER_ID_2 = 2
   MONSTER_ID_3 = 4

If you wish to export the definitions automatically, use EXPORT:

   .ENUMID 16 STEP 2 EXPORT
   .ENUMID MUSIC_1
   .ENUMID MUSIC_2
   .ENUMID MUSIC_3

… will create the following definitions and export them all:

   MUSIC_1 = 16
   MUSIC_2 = 18
   MUSIC_3 = 20

This is not a compulsory directive.


".EQU IF $FF0F"
===============

".EQU" is an alias for ".DEFINE".

This is not a compulsory directive.


".EXHIROM"
==========

With this directive you can define the SNES ROM mode to be ExHiROM.
Issuing ".EXHIROM" will override the user’s ROM bank map when WLALINK
computes 24-bit addresses and bank references. If no ".HIROM",
".LOROM" or ".EXHIROM" are given then WLALINK obeys the banking
defined in ".ROMBANKMAP".

".EXHIROM" also sets the ROM mode bit in "$40FFD5" (mirrored in
"$FFD5").

This is not a compulsory directive.


".EXPORT work_x"
================

Exports the definition "work_x" to outside world. Exported definitions
are visible to all object files and libraries in the linking
procedure. Note that you can only export value definitions, not string
definitions.

You can export as many definitions as you wish with one ".EXPORT":

   .EXPORT NUMBER, NAME, ADDRESS, COUNTRY
   .EXPORT NAME, AGE

This is not a compulsory directive.


".FAIL "THE EYE OF MORDOR HAS SEEN US!""
========================================

Terminates the compiling process. You can also specify the error code:

   .FAIL 2

These work as well:

   .FAIL
   .FAIL "EXIT CODE IS 1"
   .FAIL "UH OH..." 3

By default, if you don’t specify the error code, it’ll be 1.

This is not a compulsory directive.


".FARADDR main, irq_1"
======================

".FARADDR" is an alias for ".DL".

This is not a compulsory directive.


".FASTROM"
==========

Sets the ROM memory speed bit in "$FFD5" (".HIROM"), "$7FD5"
(".LOROM") or "$FFD5" and "$40FFD5" (".EXHIROM") to indicate that the
SNES ROM chips are 120ns chips.

This is not a compulsory directive.


".FCLOSE FP_DATABIN"
====================

Closes the filehandle "FP_DATABIN".

This is not a compulsory directive.


".FILTER filtermacro 1, 2, "encrypt me""
========================================

Runs the supplied data, in bytes, through a filter macro. All the data
is passed to "filtermacro" in the first argument, one byte at a time.
The second macro argument holds the offset from the beginning (the
first byte) in bytes (the series being "0", "1", "2", "3", …).

Here’s an example of a filter macro that defines bits (four per byte):

   .macro increment
   .bits 4 \1
   .endm

Here’s a bigger example where we map some ASCII characters into 4 bits
per char:

   // define an array for mapping ASCII values into less bits
   .ARRAYDEFINE NAME MapArray SIZE 4

   .ARRAYIN NAME MapArray INDEX 'A' VALUES %0000, %0001, %0010, \
       %0011, %0100, %0101, %0110 // defines mappings for A-G
   .ARRAYIN NAME MapArray INDEX  0  VALUE %1111

   .MACRO MapInto4Bits
   .ARRAYOUT NAME MapArray INDEX \1 DEFINITION MAPPING
   .BITS 4 MAPPING
   .IF \1 == 0
       .ENDBITS
   .ENDIF
   .ENDM

   .FILTER MapInto4Bits "BAGED", 0

This is not a compulsory directive.


".FOPEN "data.bin" FP_DATABIN"
==============================

Opens the file "data.bin" for reading and associates the filehandle
with name "FP_DATABIN".

This is not a compulsory directive.


".FREAD FP_DATABIN DATA"
========================

Reads one byte from "FP_DATABIN" and creates a definition called
"DATA" to hold it. "DATA" is an ordinary definition label, so you can
".UNDEFINE" it.

Here’s an example on how to use ".FREAD":

   .fopen "data.bin" fp
   .fsize fp t
   .repeat t
   .fread fp d
   .db d+26
   .endr
   .undefine t, d

This is not a compulsory directive.


".FSEEK FP_DATABIN 10 START"
============================

Sets the file position of the given file pointer. There are three
modes:

   .FSEEK FP_DATABIN 10 START   ; 10 bytes from the beginning of the file
   .FSEEK FP_DATABIN -10 END    ; 10 bytes before the end of the file
   .FSEEK FP_DATABIN 10 CURRENT ; 10 bytes forward from the current
                                ; position

This is not a compulsory directive.


".FSIZE FP_DATABIN SIZE"
========================

Creates a definition called "SIZE", which holds the size of the file
associated with the filehandle "FP_DATABIN". "SIZE" is an ordinary
definition label, so you can ".UNDEFINE" it.

This is not a compulsory directive.


".FTELL FP_DATABIN POSITION"
============================

Creates a definition called "POSITION", which holds the file position
of the file associated with the filehandle "FP_DATABIN". "POSITION" is
an ordinary definition label, so you can ".UNDEFINE" it.

This is not a compulsory directive.


".FUNCTION SUM_AB(varA,varB)"
=============================

Creates a function called "SUM_AB". Here are some examples:

   .FUNCTION SUM_AB(varA, varB) (varA + varB)
   .FUNCTION SUB_A_6(varA) varA-6
   .FUNCTION SUM_ABC(varA, varB, varC) (SUM_AB(varA. varB) + varC)
   .FUNCTION CONSTANT_1() 1

".FUNCTION" can be used anywhere values are expected:

   LDA SUM_AB(1, 2)
   .DEFINE SUM = 0 + 1 + SUM_AB(2, 3) + 4 + 5

This is not a compulsory directive.


".GBHEADER"
===========

This begins the GB header definition, and automatically defines
".COMPUTEGBCHECKSUM". End the header definition with .ENDGB. Here’s an
example:

   .GBHEADER
       NAME "TANKBOMBPANIC"  ; identical to a freestanding .NAME.
       LICENSEECODEOLD $34   ; identical to a freestanding .LICENSEECODEOLD.
       LICENSEECODENEW "HI"  ; identical to a freestanding .LICENSEECODENEW.
       CARTRIDGETYPE $00     ; identical to a freestanding .CARTRIDGETYPE.
       RAMSIZE $09           ; identical to a freestanding .RAMSIZE.
       ROMSIZE               ; identical to a freestanding .ROMSIZE.
       COUNTRYCODE $01       ; identical to a freestanding .COUNTRYCODE/DESTINATIONCODE.
       DESTINATIONCODE $01   ; identical to a freestanding .DESTINATIONCODE/COUNTRYCODE.
       NINTENDOLOGO          ; identical to a freestanding .NINTENDOLOGO.
       VERSION $01           ; identical to a freestanding .VERSION.
       ROMDMG                ; identical to a freestanding .ROMDMG.
                             ; Alternatively, ROMGBC or ROMGBCONLY can be used
   .ENDGB

This is not a compulsory directive.


".HEX "a0A0ffDE""
=================

Defines bytes using the supplied string that contains the bytes in
hexadecimal format. For example, the same result can be obtained using
".DB"

   .DB $a0, $A0, $ff, $DE

".HEX" can also be used in the following ways:

   .HEX 01 AA 02 BB 03 CC    ; -> .DB $01, $AA, $02, $BB, $03, $CC
   .HEX BLOCK
        01 02 03 04 05 06    ; -> .DB $01, $02, $03, $04, $05, $06
        07 08 09 0A 0B 0C    ; -> .DB $07, $08, $09, $0A, $0B, $0C
   .ENDHEX

This is not a compulsory directive.


".HIROM"
========

With this directive you can define the SNES ROM mode to be HiROM.
Issuing ".HIROM" will override the user’s ROM bank map when WLALINK
computes 24-bit addresses and bank references. If no ".HIROM",
".LOROM" or ".EXHIROM" are given then WLALINK obeys the banking
defined in ".ROMBANKMAP".

".HIROM" also sets the ROM mode bit in "$FFD5".

This is not a compulsory directive.


".IF DEBUG == 2"
================

If the condition is fulfilled the following piece of code is
acknowledged until ".ENDIF" / ".ELSE" / ".ELIF" occurs in the text,
otherwise it is skipped. Operands must be immediate values or strings.

The following operators are supported:

+---------+-----------------------+
| "!"     | not                   |
+---------+-----------------------+
| "<"     | less than             |
+---------+-----------------------+
| "<="    | less or equal to      |
+---------+-----------------------+
| ">"     | greater than          |
+---------+-----------------------+
| ">="    | greater or equal to   |
+---------+-----------------------+
| "=="    | equals to             |
+---------+-----------------------+
| "!="    | doesn’t equal to      |
+---------+-----------------------+
| "||"    | logical or            |
+---------+-----------------------+
| "&&"    | logical and           |
+---------+-----------------------+

All "IF" directives (yes, including ".IFDEF", ".IFNDEF", etc) can be
nested. They can also be used within "ENUM" s, "RAMSECTION" s,
"STRUCT" s, "ROMBANKMAP" s, and most other directives that occupy
multiple lines.

Note that complex conditions are also possible

   .IF DEBUG == 2 && defined(HELLO) && exists("main.s")

Here defined() and exists() both return 1 of they are true, and 0 if
false. In fact in conditions 0 is false and anything else is
considered to be true.

This is not a compulsory directive.


".IFDEF IF"
===========

If "IF" is defined, then the following piece of code is acknowledged
until ".ENDIF" / ".ELSE" occurs in the text, otherwise it is skipped.

This is not a compulsory directive.


".IFDEFM \2"
============

If the specified argument is defined (argument number two, in the
example), then the following piece of code is acknowledged until
".ENDIF" / ".ELSE" occurs in the macro, otherwise it is skipped.

This is not a compulsory directive. ".IFDEFM" works only inside a
macro.


".IFEQ DEBUG 2"
===============

If the value of "DEBUG" equals to "2", then the following piece of
code is acknowledged until ".ENDIF" / ".ELSE" occurs in the text,
otherwise it is skipped. Both arguments can be computations, defines
or immediate values.

This is not a compulsory directive.


".IFEXISTS "main.s""
====================

If "main.s" file can be found, then the following piece of code is
acknowledged until ".ENDIF" / ".ELSE" occurs in the text, otherwise it
is skipped.

By writing the following few lines you can include a file if it exists
without breaking the compiling loop if it doesn’t exist:

   .IFEXISTS FILE
   .INCLUDE FILE
   .ENDIF

This is not a compulsory directive.


".IFGR DEBUG 2"
===============

If the value of "DEBUG" is greater than "2", then the following piece
of code is acknowledged until ".ENDIF" / ".ELSE" occurs in the text,
otherwise it is skipped. Both arguments can be computations, defines
or immediate values.

This is not a compulsory directive.


".IFGREQ DEBUG 2"
=================

If the value of "DEBUG" is greater or equal to "2", then the following
pieceof code is acknowledged until ".ENDIF" / ".ELSE" occurs in the
text, otherwise it is skipped. Both arguments can be computations,
defines or immediate values.

This is not a compulsory directive.


".IFLE DEBUG 2"
===============

If the value of "DEBUG" is less than "2", then the following piece of
code is acknowledged until ".ENDIF" / ".ELSE" occurs in the text,
otherwise it is skipped. Both arguments can be computations, defines
or immediate values.

This is not a compulsory directive.


".IFLEEQ DEBUG 2"
=================

If the value of "DEBUG" is less or equal to "2", then the following
piece of code is acknowledged until ".ENDIF" / ".ELSE" occurs in the
text, otherwise it is skipped. Both arguments can be computations,
defines or immediate values.

This is not a compulsory directive.


".IFNDEF IF"
============

If "IF" is not defined, then the following piece of code is
acknowledged until ".ENDIF" / ".ELSE" occurs in the text, otherwise it
is skipped.

This is not a compulsory directive.


".IFNDEFM \2"
=============

If the specified argument is not defined, then the following piece of
code is acknowledged until ".ENDIF" / ".ELSE" occurs in the macro,
otherwise it is skipped.

This is not a compulsory directive. ".IFNDEFM" works only inside a
macro.


".IFNEQ DEBUG 2"
================

If the value of "DEBUG" doesn’t equal to "2", then the following piece
of code is acknowledged until ".ENDIF" / ".ELSE" occurs in the text,
otherwise it is skipped. Both arguments can be computations, defines
or immediate values.

This is not a compulsory directive.


".INC "cgb_hardware.i""
=======================

"INC" is an alias for "INCLUDE".

This is not a compulsory directive.


".INCBIN "sorority.bin""
========================

Includes the specified data file into the source file. ".INCBIN"
caches all files into memory, so you can ".INCBIN" any data file
millions of times, but it is loaded from hard drive only once.

You can optionally use "SWAP" after the file name, e.g.,

   .INCBIN "kitten.bin" SWAP

".INCBIN" data is divided into blocks of two bytes, and inside every
block the bytes are exchanged (like "SWAP r" does to nibbles). This
requires that the size of the read data is even.

You can also force WLA to skip n bytes from the beginning of the file
by writing for example:

   .INCBIN "kitten.bin" SKIP 4

Four bytes are skipped from the beginning of "kitten.bin" and the rest
is incbinned.

It is also possible to incbin only n bytes from a file:

   .INCBIN "kitten.bin" READ 10 FREADSIZE bytesRead

Will read ten bytes from the beginning of "kitten.bin" and create a
definition called "bytesRead" with value 10. If you make "READ"
negative, like:

   .INCBIN "kitten.bin" READ -2

all bytes except the last two are read. To extend this:

   .INCBIN "kitten.bin" SKIP 1 READ -2

and one byte will be skipped at the beginning of the file and two at
the end.

You can also force WLA to create a definition holding the size of the
file:

   .INCBIN "kitten.bin" FSIZE size_of_kitten

Want to circulate all the included bytes through a filter macro? Do
this:

   .INCBIN "kitten.bin" FILTER filtermacro

The filter macro is executed for each byte of the included data, data
byte being the first argument, and offset from the beginning being the
second parameter, just like in the case of ".DBM", ".DWM", ".DLM" and
".DDM".

And you can combine all these four commands:

   .INCBIN "kitten.bin" SKIP 10 READ 8 SWAP FSIZE size_of_kitten FILTER filtermacro

This example shows how to incbin eight bytes (swapped) after skipping
10 bytes from the beginning of file "kitten.bin", and how to get the
size of the file into a definition label "size_of_kitten". All the
data bytes are circulated through a filter macro.

Here’s an example of a filter macro that increments all the bytes by
one:

   .macro filtermacro    ; the input byte is \1, the output byte is in "_out"
   .redefine _out \1+1   ; \2 is the index of the element
   .endm

Instead of passing just one byte at a time to the filter macro, you
can specify the chunk size as follows:

   .INCBIN "kitten.bin" FILTER filtermacro FILTERSIZE 4

"FILTERSIZE" specifies the chunk size of the number of bytes (read) in
1 in the filter macro. 2 in the macro specifies the index of the chunk
and 3 specifies the size of the chunk (same as "FILTERSIZE"). You can
still use "SWAP" to change the order of the bytes in 1.

If the file’s not found in the ".INCDIR" directory, WLA tries to find
it in the current working directory. If the "INCDIR" is specified in
the command line, WLA will first search for the file in that
directory. If not found, it will then proceed as aforementioned.

This is not a compulsory directive.


".INCDIR "/usr/programming/gb/include/""
========================================

Changes the current include root directory. Use this to specify main
directory for the following ".INCLUDE", ".INCBIN" and
".STRINGMAPTABLE" directives. If you want to change to the current
working directory (WLA also defaults to this), use:

   .INCDIR ""

If the "INCDIR" is specified in the command line, that directory will
be searched before the ".INCDIR" in the file. If the file is not
found, WLA will then silently search the specified ".INCDIR".

This is not a compulsory directive.


".INCLUDE "cgb_hardware.i""
===========================

Includes the specified file to the source file. If the file’s not
found in the ".INCDIR" directory, WLA tries to find it in the current
working directory. If the "INCDIR" is specified in the command line,
WLA will first try to find the file specified in that directory. Then
proceed as mentioned before if it is not found.

If you want to prefix all labels inside the included file with
something, use:

   .INCLUDE "music_player.s" NAMESPACE "musicplayer"

In the case of this example, all sections, macros, labels and
references to those labels inside the included file are prefixed with
“musicplayer.”, though there are a couple of exceptions. If a
".SECTION" inside the included file has its own namespace, the
".INCLUDE" ‘s namespace doesn’t affect it. If a ".SECTION" inside the
included file uses "APPENDTO" with a section name that starts with
""*:"", that "APPENDTO" is considered to belong to the global
namespace and we won’t prefix it with the ".INCLUDE" ‘s namespace.

To add the namespace prefix to everything including ".DEFINE" s use
the keyword "ISOLATED":

   .INCLUDE "music_player.s" NAMESPACE "musicplayer" ISOLATED

Note that a dot is the namespace separator so your namespace cannot
contain a dot.

Note that you can create the file name from pieces:

   .INCLUDE ROOTDIR, SUBDIR, "cthulhu.s" NAMESPACE "cthulhu"

This might end up looking for a file “root/subdir/cthulhu.s”,
depending on the definitions.

If you are using the ".INCLUDE" inside a ".MACRO" and want to have the
file included only once, use the keyword "ONCE":

   .INCLUDE "include_one.s" NAMESPACE "once" ONCE

This is not a compulsory directive.


".INDEX 8"
==========

Forces WLA to override the index ("X" / "Y") register size given with
"SEP" / "REP". ".INDEX" doesn’t produce any code, it only affects the
way WLA interprets the immediate values ("8" for 8 bit operands, "16"
for 16 bit operands) for opcodes dealing with the index registers.

So after giving ".INDEX 8"

   CPX #10

will produce "$E0 $A0", and after giving ".INDEX 16"

   CPX #10

will yield "$E0 $00 $A0".

Note that "SEP" / "REP" again will in turn reset the accumulator/index
register size.

This is not a compulsory directive.


".INPUT NAME"
=============

".INPUT" is much like any Basic-language input: ".INPUT" asks the user
for a value or string. After ".INPUT" is the variable name used to
store the data.

".INPUT" works like ".REDEFINE", but the user gets to type in the
data.

Here are few examples how to use input:

   .PRINTT "The name of the ROM? "
   .INPUT NAME
   .NAME NAME

   ...

   .PRINTT "Give the .DB amount.\n"
   .INPUT S
   .PRINTT "Give .DB data one at a time.\n"
   .REPEAT S
     .INPUT B
     .DB B
   .ENDR

   ...

This is not a compulsory directive.


".LICENSEECODENEW "1A""
=======================

This is a standard new licensee code found at "$144" and "$145" in a
Gameboy ROM, and there this one is put to also. ".LICENSEECODENEW"
cannot be defined with .LICENSEECODEOLD. "$33" is inserted into
"$14B", as well.

This is not a compulsory directive.


".LICENSEECODEOLD $1A"
======================

This is a standard old licensee code found at "$14B" in a Gameboy ROM,
and there this one is put to also. ".LICENSEECODEOLD" cannot be
defined with ".LICENSEECODENEW".

This is not a compulsory directive.


".LONG $102030, $405060"
========================

".LONG" is an alias for ".DL".

This is not a compulsory directive.


".LOROM"
========

With this directive you can define the SNES ROM mode to be LoROM.
Issuing ".LOROM" will override the user’s ROM bank map when WLALINK
computes 24-bit addresses and bank references. If no ".HIROM",
".LOROM" or ".EXHIROM" are given then WLALINK obeys the banking
defined in ".ROMBANKMAP".

WLA defaults to ".LOROM".

This is not a compulsory directive.


".MACRO TEST"
=============

Begins a macro called "TEST".

You can use "\@" inside a macro to e.g., separate a label from the
other macro "TEST" occurrences. "\@" is replaced with an integer
number indicating the amount of times the macro has been called
previously so it is unique to every macro call. "\@" can also be used
inside strings inside a macro or just as a plain value. Look at the
following examples for more information.

You can also type "\!" to get the name of the source file currently
being parsed. "\." can be used the same way to get the name of the
macro.

Also, if you want to use macro arguments in e.g., calculation, you can
type "\X" where "X" is the number of the argument. Another way to
refer to the arguments is to use their names given in the definition
of the macro (see the examples for this).

Remember to use ".ENDM" to finish the macro definition. Note that you
cannot use ".INCLUDE" inside a macro. Note that WLA’s macros are in
fact more like procedures than real macros, because WLA doesn’t
substitute macro calls with macro data. Instead WLA jumps to the macro
when it encounters a macro call at compile time.

You can call macros from inside a macro. Note that the preprocessor
does not expand the macros. WLA traverses through the code according
to the macro calls.

Here are some examples:

   .MACRO NOPMONSTER
       .REPT 32         ; gives us 32 NOPs
       NOP
       .ENDR
   .ENDM

   .MACRO LOAD_ABCD
       LD A, \1
       LD B, \2
       LD C, \3
       LD D, :\4        ; load the bank number of \4 into register D.
       NOPMONSTER       ; note that \4 must be a label or ROM address
       LD HL, 1<<\1     ; for this to work...
   .INCBIN \5
   .ENDM

   .MACRO QUEEN

   QUEEN\@:
       LD   A, \1
       LD   B, \1
       CALL QUEEN\@

       .DB  "\@", 0     ; will translate into a zero terminated string
                        ; holding the amount of macro QUEEN calls.
       .DB  "\\@", 0    ; will translate into a string containing
                        ; \@.
       .DB  \@          ; will translate into a number indicating
                        ; the amount of macro QUEEN calls.

   .ENDM

   .MACRO LOAD_ABCD_2 ARGS ONE, TWO, THREE, FOUR, FIVE
       LD A, ONE        ; note! ONE, TWO, THREE, FOUR and FIVE
       LD B, TWO        ; here are actually definitions that
       LD C, THREE      ; exist as long as the .MACRO is alive
       LD D, FOUR       ; so be careful when using named args...
       NOPMONSTER
       LD HL, 1<<ONE
   .INCBIN FIVE
   .ENDM

And here’s how they can be used:

   NOPMONSTER
   LOAD_ABCD $10, $20, $30, XYZ, "merman.bin"
   QUEEN 123
   LOAD_ABCD_2 $10, $20, $30, XYZ, "merman.bin"

Note that arguments can be optionally wrapped inside parentheses:

   NOPMONSTER()
   LOAD_ABCD($10, $20, $30, XYZ, "merman.bin")
   QUEEN(123)
   LOAD_ABCD_2($10, $20, $30, XYZ, "merman.bin")

Note that you must separate the arguments with commas.

Note that the following works as well:

   .DEF prev_test $0000

   .MACRO test ARGS str
   __\._{\@+1}:                   ; this will become __test_1 during
       .PRINT __\._{\@+1}, "\n"   ; the first call, __test_2 during the
       .WORD  prev_test           ; second call...
       .REDEF prev_test __\._{\@+1}
       .BYTE  str.length, str, 0
   .ENDM

If you want to give names to the macro’s arguments you can do that by
listing them in order after supplying ARGS after the macro’s name.

Every time a macro is called a definition "NARGS" is created. It shows
only inside the macro and holds the number of arguments the macro was
called with. So don’t have your own definition called "NARGS". Here’s
an example:

   .MACRO LUPIN
     .IF NARGS != 1
       .FAIL
     .ENDIF

     .PRINTT "Totsan! Ogenki ka?\n"
   .ENDM

You can also use "\?" to ask for the type of the argument in the
following fashion:

   .macro differentThings
     .if \?1 == ARG_IMMEDIATE
       .db \1
     .elif \?1 == ARG_NUMBER
       .db 1
     .elif \?1 == ARG_STRING
       .db 2
     .elif \?1 == ARG_LABEL
       .db 3
     .elif \?1 == ARG_PENDING_CALCULATION
       .db 4
     .endif
   .endm

   .section "TestingDifferentThings"
   TDT1:
       differentThings 100
       differentThings "HELLO"
       differentThings TDT1
       differentThings TDT1+1
       differentThings #0
   .ends

The previous example will result in .db 1, 2, 3, 4, 0

Here’s another useful example:

   .DEFINE DEFINITION_A 1

   .MACRO REDEFINER
   .REDEFINE \1 = ?1 + 1      ; \1 here is the definition's name,
   .ENDM                      ; and ?1 is its value.

   REDEFINER &DEFINITION_A    ; here we feed the definition's name
                              ; as first argument, not it's value

Another useful example:

   .MACRO LOOP ISOLATED
      LD A, 10
   -  DEC A                   ; B
      JP NZ, -
   .ENDM

   ...
      LD B, 20
   -  LOOP                    ; C
      DEC B
      JP NZ, -                ; A
   ...

Here we use the keyword "ISOLATED" to make un-named labels inside the
macro to be isolated from the outside world. Without it the jump in A
would jump to B, but now it jumps to C.

Using the keyword "ISOLATED" we would also make the macro to have its
own child label stack:

           .macro MACROM
   AA03:   .db 0
   @child: .db 1          ; A
           .dw @child     ; B
           .endm

   AA00:   .db "25>"
   @child: MACROM         ; C
           .dw @child     ; D
           .db "<25"

In this case B points to A and D points to A. If you add keyword
"ISOLATED" to ".MACRO" MACROM then B still points to A, but A doesn’t
leak out of MACROM and D points to C. Exiting a ".MACRO" that uses
keyword "ISOLATED" restores the child label stack.

One example more, but this time with local labels:

           .macro LOCALS isolated
   _hello: .db 0            ; A
           .dw _hello + 1   ; B
           .endm

   _hello: .db "27>"        ; C
           .db 0, 1, 2
           LOCALS
           .dw _hello + 2   ; D
           .db "<27"

Normally this would create the local label "_hello" twice and it would
not work. Adding the keyword "ISOLATED" to ".MACRO" makes the local
labels unique and D points to C and B points to A.

To enable only local label isolation use the keyword "ISOLATELOCAL"
instead of "ISOLATED" and to enable only the isolation of un-named
labels use the keyword "ISOLATEUNNAMED".

Note that there is an alternative way of defining a ".MACRO":

   .macro DBSUMOFTWOVALUES(v1,v2) isolated
   .db v1+v2
   .endm

This is not a compulsory directive.


".MEMORYMAP"
============

Begins the memory map definition. Using ".MEMORYMAP" you must first
describe the target system’s memory architecture to WLA before it can
start to compile the code. ".MEMORYMAP" gives you the freedom to use
WLA to compile data for numerous different real systems.

Examples:

   .MEMORYMAP
   DEFAULTSLOT 0
   SLOTSIZE $4000
   SLOT 0 $0000
   SLOT 1 $4000
   .ENDME

   .MEMORYMAP
   DEFAULTSLOT 0
   SLOT 0 $0000 $4000 "ROMSlot"
   SLOT 1 $4000 $4000 "RAMSlot"
   .ENDME

   .MEMORYMAP
   DEFAULTSLOT 0
   SLOT 0 START $0000 SIZE $4000 NAME "ROMSlot"
   SLOT 1 START $4000 SIZE $4000 NAME "RAMSlot"
   .ENDME

   .MEMORYMAP
   DEFAULTSLOT 1
   SLOTSIZE $6000
   SLOT 0 $0000
   SLOTSIZE $2000
   SLOT 1 $6000
   SLOT 2 $8000
   .ENDME

Here’s a real life example from Adam Klotblixt. It should be
interesting for all the ZX81 coders:

   ...

   .MEMORYMAP
   DEFAULTSLOT 1
   SLOTSIZE $2000
   SLOT 0 $0000
   SLOTSIZE $6000
   SLOT 1 $2000
   .ENDME

   .ROMBANKMAP
   BANKSTOTAL 2
   BANKSIZE $2000
   BANKS 1
   BANKSIZE $6000
   BANKS 1
   .ENDRO

   .BANK 1 SLOT 1
   .ORGA $2000

   ...

"SLOTSIZE" defines the size of the following slots, unless you
explicitly specify the size of the slot, like in the second and third
examples. You can redefine "SLOTSIZE" as many times as you wish.

"DEFAULTSLOT" describes the default slot for banks which aren’t
explicitly inserted anywhere. Check ".BANK" definition for more
information.

"SLOT" defines a slot and its starting address. "SLOT" numbering
starts at "0" and ends to "255" so you have 256 slots at your
disposal.

This is a compulsory directive, and make sure all the object files
share the same ".MEMORYMAP" or you can’t link them together.


".NAME "NAME OF THE ROM""
=========================

If ".NAME" is used with WLA-GB then the 16 bytes ranging from "$0134"
to "$0143" are filled with the provided string. WLA-65816 fills the 21
bytes from "$FFC0" to "$FFD4" in HiROM and from "$7FC0" to "$7FD4" in
LoROM mode with the name string (SNES ROM title). For ExHiROM the
ranges are from "$40FFC0" to "$40FFD4" and from "$FFC0" to "$FFD4"
(mirrored).

If the string is shorter than 16/21 bytes the remaining space is
filled with "$00".

This is not a compulsory directive.


".NEXTU name"
=============

Proceeds to the next entry in a union.


".NINTENDOLOGO"
===============

Places the required Nintendo logo into the Gameboy ROM at "$104".

This is not a compulsory directive.


".NOWDC"
========

Turns WLA-65816 into a mode where it accepts its default syntax
assembly code, which doesn’t support WDC standard. This is the default
mode for WLA-65816.

This is not a compulsory directive.


".ORG $150"
===========

Defines the starting address. The value supplied here is relative to
the ROM bank given with ".BANK".

When WLA starts to parse a source file, ".ORG" is set to "$0", but
it’s always a good idea to explicitly use ".ORG", for clarity.

This is a compulsory directive.


".ORGA $150"
============

Defines the starting address. The value supplied here is absolute and
used directly in address computations. WLA computes the right position
in ROM file. By using ".ORGA" you can instantly see from the source
file where the following code is located in the 16-bit memory.

Here’s an example:

   .MEMORYMAP
   SLOTSIZE $4000
   DEFAULTSLOT 0
   SLOT 0 $0000
   SLOT 1 $4000
   .ENDME

   .ROMBANKMAP
   BANKSTOTAL 2
   BANKSIZE $4000
   BANKS 2
   .ENDRO

   .BANK 0 SLOT 1
   .ORGA $4000

   MAIN:       JP      MAIN

Here "MAIN" is at "$0000" in the ROM file, but the address for label
"MAIN" is "$4000". By using ".ORGA" instead of ".ORG", you can
directly see from the value the address where you want the code to be
as ".ORG" is just an offset to the "SLOT".


".OUTNAME "other.o""
====================

Changes the name of the output file. Here’s an example:

   wla-gb -o test.o test.s

would normally output "test.o", but if you had written:

   .OUTNAME "new.o"

somewhere in the code WLA would write the output to "new.o" instead.

This is not a compulsory directive.


".PRINT "Numbers 1 and 10: ", DEC 1, " $", HEX 10, "\n""
========================================================

Prints strings and numbers to stdout. A combination and a more usable
version of .PRINTT and .PRINTV. Useful for debugging.

Optional: Give "DEC" (decimal) or "HEX" (hexadecimal) before the value
you want to print.

This is not a compulsory directive.


".PRINTT "Here we are...\n""
============================

Prints the given text into stdout. Good for debugging stuff. "PRINTT"
takes only a string as argument, and the only supported formatting
symbol is "\n" (line feed).

This is not a compulsory directive.


".PRINTV DEC DEBUG+1"
=====================

Prints the value of the supplied definition or computation into
stdout. Computation must be solvable at the time of printing (just
like definitions values). "PRINTV" takes max two parameters. The first
describes the type of the print output. "DEC" means decimal, "HEX"
means hexadecimal. This is optional. Default is "DEC".

Use "PRINTV" with "PRINTT" as "PRINTV" doesn’t print linefeeds, only
the result. Here’s an example:

   .PRINTT "Value of \"DEBUG\" = $"
   .PRINTV HEX DEBUG
   .PRINTT "\n"

This is not a compulsory directive.


".RAMSECTION "Vars" BASE $7E BANK 0 SLOT 1 ALIGN 256 OFFSET 32"
===============================================================

"RAMSECTION" s accept only variable labels and variable sizes, and the
syntax to define these is identical to ".ENUM" (all the syntax rules
that apply to ".ENUM" apply also to ".RAMSECTION"). Additionally you
can embed structures (".STRUCT") into a "RAMSECTION". Here’s an
example:

   .RAMSECTION "Some of my variables" BANK 0 SLOT 1 RETURNORG PRIORITY 100
   vbi_counter:   db
   player_lives:  db
   .ENDS

By default "RAMSECTION" s behave like "FREE" sections, but instead of
filling any banks RAM sections will occupy RAM banks inside slots. You
can fill different slots with different variable labels. It’s
recommend that you create separate slots for holding variables (as ROM
and RAM don’t usually overlap).

If you want that WLA returns the "ORG" to what it was before issuing
the "RAMSECTION", use the keyword "RETURNORG".

Keyword "PRIORITY" means just the same as "PRIORITY" of a ".SECTION",
it is used to prioritize some sections when placing them in the output
ROM/PRG. The "RAMSECTION" s with higher "PRIORITY" are placed first in
the output, and if the priorities match, then bigger "RAMSECTION" s
are placed first.

NOTE! Currently WLA-DX assumes that there are 256 RAM banks available
for each slot in the memory map. There is no other way to limit this
number at the moment than manually keep the "BANK" number inside real
limits.

Anyway, here’s another example:

   .MEMORYMAP
   SLOTSIZE $4000
   DEFAULTSLOT 0
   SLOT 0 $0000           ; ROM slot 0.
   SLOT 1 $4000           ; ROM slot 1.
   SLOT 2 $A000 "RAMSlot" ; variable RAM is here!
   .ENDME

   .STRUCT game_object
   x DB
   y DB
   .ENDST

   .RAMSECTION "vars 1" BANK 0 SLOT 2
   moomin1   DW
   phantom   DB
   nyanko    DB
   enemy     INSTANCEOF game_object
   .ENDS

   .RAMSECTION "vars 2" BANK 1 SLOT "RAMSlot"  ; Here we use slot 2
   moomin2   DW
   .ENDS

   .RAMSECTION "vars 3" BANK 1 SLOT $A000      ; Slot 2 here as well...
   moomin3_all .DSB 3
   moomin3_a    DB
   moomin3_b    DB
   moomin3_c    DB
   .ENDS

   .RAMSECTION "vars 4" BANK 1 SLOT $A000
   enemies      INSTANCEOF game_object 2 STARTFROM 0 ; If you leave away "STARTFROM 0" the indexing will start from 1
   .ENDS

If no other RAM sections are used, then this is what you will get:

   .DEFINE moomin1     $A000
   .DEFINE phantom     $A002
   .DEFINE nyanko      $A003
   .DEFINE enemy       $A004
   .DEFINE enemy.x     $A004
   .DEFINE enemy.y     $A005
   .DEFINE moomin2     $A000
   .DEFINE moomin3_all $A002
   .DEFINE moomin3_a   $A002
   .DEFINE moomin3_b   $A003
   .DEFINE moomin3_c   $A004
   .DEFINE enemies     $A005
   .DEFINE enemies.0   $A005
   .DEFINE enemies.0.x $A005
   .DEFINE enemies.0.y $A006
   .DEFINE enemies.1   $A007
   .DEFINE enemies.1.x $A007
   .DEFINE enemies.1.y $A008

"BANK" in ".RAMSECTION" is optional so you can leave it away if you
don’t switch RAM banks, or the target doesn’t have them (defaults to
0).

NOTE! The generated _sizeof_ labels for ".RAMSECTION" “vars 3” will
be:

   _sizeof_moomin3_all (== 3)
   _sizeof_moomin3_a   (== 1)
   _sizeof_moomin3_b   (== 1)
   _sizeof_moomin3_c   (== 1)

Going back to the previous example, if you wanted to make the size of
all instances of "game_object" to be 8 (bytes) in "enemies":

   .RAMSECTION "vars 4" BANK 1 SLOT $A000
   enemies      INSTANCEOF game_object SIZE 8 COUNT 2 STARTFROM 0
   .ENDS

Use the keyword "SIZE" to do that. Also note that the keyword "COUNT"
is optional, and recommended.

It is also possible to merge two or more sections using "APPENDTO":

   .RAMSECTION "RAMSection1" BANK 0 SLOT 0
   label1    DB
   .ENDS

   .RAMSECTION "RAMSection2" APPENDTO "RAMSection1"
   label2    DB
   .ENDS

NOTE! The "APPENDTO" ".SECTION" s are appended in the order the linker
sorts them. So first "PRIORITY" is considered (0 by default, the
bigger the value the more important it is) and then the size of the
".SECTION" is considered, bigger ".SECTION" s are more important than
smaller.

If you wist to skip some bytes without giving them labels, use "." as
a label:

   .RAMSECTION "ZERO_PAGE" BANK 0 SLOT 0
   UsingThisByte1: DB
   .               DB ; RESERVED
   .               DB ; RESERVED
   UsingThisByte2: DB
   .               DB ; RESERVED
   UsingThisByte3: DB
   .ENDS

If you want to use "FORCE" RAMSECTIONs that are fixed to a specified
address, do as follows:

   .RAMSECTION "FixedRAMSection" BANK 0 SLOT 0 ORGA $0 FORCE
   .               DB ; SYSTEM RESERVED
   .               DB ; SYSTEM RESERVED
   PlayerX         DB
   PlayerY         DB
   .ENDS

Other types that are supported: "SEMIFREE" and "SEMISUBFREE".

Note that ".ALIGN" also works inside a ".RAMSECTION", but there are
limitations (see ".ALIGN"). Here’s an example:

   .RAMSECTION "AlignTest" BANK 0 SLOT 1 ALIGN 8
   Objects INSTANCEOF game_object COUNT 2
   .ALIGN 8
   Byte1 DB
   Byte2 DB
   .ALIGN 4
   Checksum DW
   .ENDS

Here’s the order in which WLA writes the RAM sections:

1. "FORCE"

2. "SEMISUBFREE"

3. "SEMIFREE"

4. "FREE"

You can change this order using "[ramsectionwriteorder]" in a link
file.

NOTE: You can use "ORGA" to specify the fixed address for a "FORCE"
"RAMSECTION". "ORG" is also supported.

NOTE: When you have "RAMSECTION" s inside libraries, you must give
them BANKs and SLOTs in the linkfile, under [ramsections].

NOTE: "WINDOW" and "BITWINDOW" work also with ".RAMSECTION" s.

This is not a compulsory directive.


".RAMSIZE 0"
============

Indicates the size of the RAM. This is a standard Gameboy RAM size
indicator value found at "$149" in a Gameboy ROM, and there this one
is put to also.

This is not a compulsory directive.


".REDEF IF $0F"
===============

".REDEF" is an alias for ".REDEFINE".

This is not a compulsory directive.


".REDEFINE IF $0F"
==================

Assigns a new value or a string to an old definition. If the
definition doesn’t exist, ".REDEFINE" performs ".DEFINE"’s work.

When used with ".REPT" "REDEFINE" helps creating tables:

   .DEFINE CNT 0

   .REPT 256
   .DB CNT
   .REDEFINE CNT CNT+1
   .ENDR

This is not a compulsory directive.


".REPEAT 6"
===========

Repeats the text enclosed between ".REPEAT x" and ".ENDR" "x" times
("6" in this example). You can use ".REPEAT" s inside ".REPEAT" s. "x"
must be bigger or equal than "0".

It’s also possible to have the repeat counter/index in a definition:

   .REPEAT 6 INDEX COUNT
   .DB COUNT
   .ENDR

This would define bytes "0", "1", "2", "3", "4" and "5".

This is not a compulsory directive.


".REPT 6"
=========

".REPT" is an alias for ".REPEAT".

This is not a compulsory directive.


".ROMBANKMAP"
=============

Begins the ROM bank map definition. You can use this directive to
define the project’s ROM banks. Use ".ROMBANKMAP" when not all the ROM
banks are of equal size. Note that you can use ".ROMBANKSIZE" and
".ROMBANKS" instead of ".ROMBANKMAP", but that’s only when the ROM
banks are equal in size.

Examples:

   .ROMBANKMAP
   BANKSTOTAL 16
   BANKSIZE $4000
   BANKS 16
   .ENDRO

   .ROMBANKMAP
   BANKSTOTAL 510
   BANKSIZE $6000
   BANKS 1
   BANKSIZE $2000
   BANKS 509
   .ENDRO

The first one describes an ordinary ROM image of 16 equal sized banks.
The second one defines a 4MB Pocket Voice ROM image. In the PV ROM
image the first bank is "$6000" bytes and the remaining "509" banks
are smaller ones, "$2000" bytes each.

"BANKSTOTAL" tells the total amount of ROM banks. It must be defined
prior to anything else.

"BANKSIZE" tells the size of the following ROM banks. You can supply
WLA with "BANKSIZE" as many times as you wish.

"BANKS" tells the amount of banks that follow and that are of the size
"BANKSIZE" which has been previously defined.

This is not a compulsory directive when ".ROMBANKSIZE" and ".ROMBANKS"
are defined.

You can redefine ".ROMBANKMAP" as many times as you wish as long as
the old and the new ROM bank maps match as much as possible. This way
you can enlarge the size of the project on the fly.


".ROMBANKS 2"
=============

Indicates the size of the ROM in rombanks.

This is a compulsory directive unless ".ROMBANKMAP" is defined.

You can redefine ".ROMBANKS" as many times as you wish as long as the
old and the new ROM bank maps match as much as possible. This way you
can enlarge the size of the project on the fly.


".ROMBANKSIZE $4000"
====================

Defines the ROM bank size. Old syntax is ".BANKSIZE x".

This is a compulsory directive unless ".ROMBANKMAP" is defined.


".ROMDMG"
=========

Inserts data into the specific ROM location to mark the ROM as a DMG
(Gameboy) ROM ("$00" -> "$0146"). It will only run in DMG mode.

This is not a compulsory directive. ".ROMDMG" cannot be used with
".ROMSGB".


".ROMGBCONLY"
=============

Inserts data into the specific ROM location to mark the ROM as a
Gameboy Color ROM ("$C0" -> "$0143", so ROM name is max. 15 characters
long). It will only run in GBC mode.

This is not a compulsory directive.


".ROMGBC"
=========

Inserts data into the specific ROM location to mark the ROM as a dual-
mode ROM ("$80" -> "$0143", so ROM name is max. 15 characters long).
It will run in either DMG or GBC mode.

This is not a compulsory directive.


".ROMSGB"
=========

Inserts data into the specific ROM location to mark the ROM as a Super
Gameboy enhanced ROM ("$03" -> "$0146").

This is not a compulsory directive. ".ROMSGB" cannot be used with
".ROMDMG".


".ROMSIZE 1"
============

This is a standard Gameboy ROM size indicator value found at "$148" in
a Gameboy ROM, and there this one is put to also. If you don’t specify
a value then WLA-GB tries to calculate it based on ".ROMBANKS" /
".ROMBANKMAP".

This is not a compulsory directive.


".ROW $ff00, 1, "3""
====================

Defines bytes after a .TABLE has been used to define the format. An
alternative way of defining bytes to .DB/.DW.

Note that when you use .ROW you’ll need to give all the items .TABLE
defines, i.e. one full row. To give more or less bytes use .DATA.

Example:

   .TABLE word, byte, word
   .ROW $aabb, "H", $ddee

This is the same as

   .DW $aabb .DB “H” .DW $ddee

This is not a compulsory directive.


".SDSCTAG 1.0, "DUNGEON MAN", "A wild dungeon exploration game", "Ville Helin""
===============================================================================

".SDSCTAG" adds SDSC tag to your SMS/GG ROM file. The ROM size must be
at least 8KB just like with ".COMPUTESMSCHECKSUM" and ".SMSTAG". For
more information about this header take a look at
http://www.smspower.org/dev/sdsc/. Here’s an explanation of the
arguments:

   .SDSCTAG {version number}, {program name}, {program release notes}, {program author}

Note that program name, release notes and program author can also be
pointers to strings instead of being only strings (which WLA
terminates with zero, and places them into suitable locations inside
the ROM file). So:

   .SDSCTAG 0.8, PRGNAME, PRGNOTES, PRGAUTHOR
   ...
   PRGNAME:  .DB "DUNGEON MAN", 0
   PRGNOTES: .DB "A wild and totally crazy dungeon exploration game", 0
   PRGAUTHOR:.DB "Ville Helin", 0

works also. All strings supplied explicitly to ".SDSCTAG" are placed
somewhere in ".BANK 0 SLOT 0".:

   .SDSCTAG 1.0, "", "", ""
   .SDSCTAG 1.0, 0, 0, 0

are also valid, here "0" and """" mean the user doesn’t want to use
any descriptive strings. Version number can also be given as an
integer, but then the minor version number defaults to zero.

".SDSCTAG" also defines ".SMSTAG" (as it’s part of the SDSC ROM tag
specification).

This is not a compulsory directive.


".SECTION "Init" FORCE"
=======================

Section is a continuous area of data which is placed into the output
file according to the section type and ".BANK" and ".ORG" directive
values.

The example begins a section called "Init". Before a section can be
declared, ".BANK" and ".ORG" should be used unless WLA is in library
file output mode. Library file’s sections must all be "FREE" ones.
".BANK" tells the bank number where this section will be later
relocated into. ".ORG" tells the offset for the relocation from the
beginning of ".BANK".

It is also possible to supply "BANK", "SLOT", "BASE" and "ORG" or
"ORGA" to ".SECTION" as follows:

   .SECTION "NoInheritedParameters" BASE $70 BANK 0 SLOT 1 ORGA $1000

You can put sections inside a namespace. For instance, if you put a
section into a namespace called "bank0", then labels in that section
can be accessed with "bank0.label". This is not necessary inside the
section itself. The namespace directive should immediately follow the
name:

   .SECTION "Init" NAMESPACE "bank0"

You can give the size of the section, if you wish to force the section
to some specific size, the following way:

   .SECTION "Init" SIZE 100 FREE

It’s possible to force WLALINK to align the sections by giving the
alignment as follows:

   .SECTION "Init" SIZE 100 ALIGN 4 FREE

If you need an offset from the alignment, use OFFSET:

   .SECTION "Init" SIZE 10 ALIGN 256 OFFSET 32 FREE

And if you want that WLA returns the "ORG" to what it was before
issuing the section, put "RETURNORG" at the end of the parameter list:

   .SECTION "Init" SIZE 100 ALIGN 4 FREE RETURNORG

By default WLA advances the "ORG", so, for example, if your "ORG" was
"$0" before a section of 16 bytes, then the "ORG" will be "16" after
the section.

Note also that if your section name begins with double underlines
(e.g., "__UNIQUE_SECTION!!!") the section will be unique in the sense
that when WLALINK recieves files containing sections which share the
same name, WLALINK will save only the first of them for further
processing, all others are deleted from memory with corresponding
labels, references and calculations.

If a section name begins with an exclamation mark ("!") it tells
WLALINK to not to drop it, even if you use WLALINK’s ability to
discard all unreferenced sections and there are no references to the
section. You can achieve the same effect by adding "KEEP" to the end
of the list:

   .SECTION "Init" SIZE 100 ALIGN 4 FREE RETURNORG KEEP

"FORCE" after the name of the section tells WLA that the section
*must* be inserted so it starts at ".ORG". "FORCE" can be replaced
with "FREE" which means that the section can be inserted somewhere in
the defined bank, where there is room. You can also use "OVERWRITE" to
insert the section into the memory regardless of data collisions.
Using "OVERWRITE" you can easily patch an existing ROM image just by
".BACKGROUND"’ing the ROM image and inserting "OVERWRITE" sections
into it. "SEMIFREE" sections are also possible and they behave much
like "FREE" sections. The only difference is that they are positioned
somewhere in the bank starting from ".ORG". "SEMISUBFREE" sections on
the other hand are positioned somewhere in the bank starting from "$0"
and ending to ".ORG".

"SUPERFREE" sections are also available, and they will be positioned
into the first suitable place inside the first suitable bank
(candidates for these suitable banks have the same size with the slot
of the section, no other banks are considered). You can also leave
away the type specifier as the default type for the section is "FREE".

If you wish to specify the banks where the section could be inserted
into, use "SEMISUPERFREE" (and "BANKS" to specify the banks list):

   .SECTION "IAmABankedSection" SEMISUPERFREE BANKS 15-13/10/6-9/3/1

The banks list in the example unrolls into this: [ 15, 14, 13, 10, 6,
7, 8, 9, 3, 1 ]. The banks are inspected for free space in the given
order.

You can name the sections as you wish, but there is one special name.
A section called "BANKHEADER" is placed in the front of the bank where
it is defined. These sections contain data that is not in the memory
map of the machine, so you can’t refer to the data of a "BANKHEADER"
section, but you can write references to outside. So no labels inside
"BANKHEADER" sections. These special sections are useful when writing
e.g., MSX programs. Note that library files don’t take "BANKHEADER"
sections.

Here’s an example of a "BANKHEADER" section:

   .BANK 0
   .ORG 0
   .SECTION "BANKHEADER"
       .DW MAIN
       .DW VBI
   .ENDS

   .SECTION "Program"
   MAIN: CALL MONTY_ON_THE_RUN
   VBI:  PUSH HL
         ...
         POP HL
         RETI
   .ENDS

Here’s an example of an ordinary section:

   .BANK 0
   .ORG $150
   .SECTION "Init" FREE PRIORITY 1000
           DI
           LD  SP, $FFFE
           SUB A
           LD  ($FF00+R_IE), A
   .ENDS

This tells WLA that a "FREE" section called "Init" must be located
somewhere in bank "0" and it has a sorting "PRIORITY" of 1000. If you
replace "FREE" with "SEMIFREE" the section will be inserted somewhere
in the bank "0", but not in the "$0" - "$14F" area. If you replace
"FREE" with "SUPERFREE" the section will be inserted somewhere in any
bank with the same size as bank "0".

Here’s the order in which WLALINK writes the sections:

1. "FORCE"

2. "SEMISUPERFREE"

3. "SEMISUBFREE"

4. "SEMIFREE"

5. "FREE"

6. "SUPERFREE"

7. "OVERWRITE"

You can change this order using "[sectionwriteorder]" in a link file.

Before the sections are inserted into the output file, they are sorted
by priorities, so that the section with the highest priority is
processed first. If priorities are the same, then the size of the
section matters, and bigger sections are processed before smaller
ones. The default "PRIORITY", when not explicitly given, is 0. Note
that "PRIORITY" accepts negative values as well.

You can use "AUTOPRIORITY" instead of "PRIORITY" when you want to
assign descending priority to sections. Using this you can make it so
that e.g., "APPENDTO" sections are appended in the lexical parsing
order. "AUTOPRIORITY" starts from 65535 and is subtracted by one every
time it’s used.

You can also create a RAM section. For more information about them,
please read the ".RAMSECTION" directive explanation.

It is also possible to merge two or more sections using "APPENDTO":

   .SECTION "Base"
   .DB 0
   .ENDS

   .SECTION "AppendToBase" FREE RETURNORG APPENDTO "Base"
   .DB 1
   .ENDS

And you can force a section to be placed after another section, with
an offset:

   .SECTION "Follower" OFFSET 32 AFTER "Base"
   .DB 111
   .ENDS

If you want to force WLALINK to place a section say between $0100 and
$0200 in the address space, use "WINDOW" (note that ".SLOT" must be
used to make this placement possible, have the ".SECTION" in the
correct slot):

   .SECTION "SpecialStuff" FREE WINDOW $0100 $0200
   NOP
   .ENDS

If you want to position a ".SECTION" so that it is placed in memory in
a spot where e.g., only the least 8 bits of the address change (the
".SECTION" must thus be less than 256 bytes in size), use "BITWINDOW":

   .SECTION "PageX" FREE BITWINDOW 8
   NOP
   .ENDS

This is not a compulsory directive.


".SEED 123"
===========

Seeds the random number generator.

The random number generator is initially seeded with the output of
"time()", which is, according to the manual, *the time since the Epoch
(00:00:00 UTC, January 1, 1970), measured in seconds*. So if you don’t
".SEED" the random number generator yourself with a constant value,
".DBRND" and ".DWRND" give you different values every time you run
WLA.

In WLA DX 9.4a and before we used the stdlib’s "srand()" and "rand()"
functions making the output differ on different platforms. Since v9.4
WLA DX contains its own Mersenne Twister pseudo random number
generator.

This is not a compulsory directive.


".SEEDRANDOM"
=============

Seeds the random number generator with the output of "time()", which
is, according to the manual, *the time since the Epoch (00:00:00 UTC,
January 1, 1970), measured in seconds*.

By default the (pseudo) random number generator is seeded with
"time()", so you don’t have to do it yourself, but just in the case
you want to do it somewhere in the source code, use this.

This is not a compulsory directive.


".SHIFT"
========

Shifts the macro arguments one down ("\2" becomes "\1", "\3" becomes
"\2", etc.). ".SHIFT" can thus only be used inside a ".MACRO".

This is not a compulsory directive.


".SLOT 1"
=========

Changes the currently active memory slot. This directive is meant to
be used with "SUPERFREE" sections, where only the slot number is
constant when placing the sections.

You can use the number, address or name of the slot here:

   .SLOT 1           ; Use slot 1.
   .SLOT $2000       ; Use a slot with starting address of $2000.
   .SLOT "SlotOne"   ; Use a slot with a name "SlotOne"

This is not a compulsory directive.


".SLOWROM"
==========

Clears the ROM memory speed bit in "$FFD5" (".HIROM"), "$7FD5"
(".LOROM") or "$FFD5" and "$40FFD5" (".EXHIROM") to indicate that the
SNES ROM chips are 200ns chips.

This is not a compulsory directive.


".SMC"
======

Forces WLALINK to compute a proper SMC header for the ROM file.

SMC header is a chunk of 512 bytes. WLALINK touches only its first
three bytes, and sets the rest to zeroes. Here’s what will be inside
the first three bytes:

+--------+---------------------------------------------------------------------+
| Byte   | Description                                                         |
+--------+---------------------------------------------------------------------+
| "0"    | low byte of 8KB page count.                                         |
+--------+---------------------------------------------------------------------+
| "1"    | high byte of 8KB page count.                                        |
+--------+---------------------------------------------------------------------+
| "2"    | * Bit "7": "0"  * Bit "6": "0"  * Bit "5": "0" = LoROM, "1" = HiROM |
|        | * Bit "4": "0" = LoROM, "1" = HiROM  * Bit "3" and "2": SRAM size   |
|        | ("00" = 256Kb, "01" = 64Kb, "10" = 16Kb, "11" = 0Kb)  * Bit "1":    |
|        | "0"  * Bit "0": "0"                                                 |
+--------+---------------------------------------------------------------------+

This is not a compulsory directive.


".SMDHEADER"
============

Defines the Sega Mega Drive ROM header in "$100-$1FF". All the fields
in ".SMDHEADER" are optional. Here are the default values:

   .SMDHEADER
       SYSTEMTYPE "SEGA MEGA DRIVE "    ; 16 bytes
       COPYRIGHT  "                "    ; 16 bytes
       TITLEDOMESTIC "             "    ; 48 bytes (all spaces)
       TITLEOVERSEAS "             "    ; 48 bytes (all spaces)
       SERIALNUMBER  "             "    ; 14 bytes (all spaces)
       DEVICESUPPORT "J            "    ; 16 bytes ('J' and the rest are spaces)
       ROMADDRESSRANGE $0, -1           ;  8 bytes (-1 is turned into ROM size minus one)
       RAMADDRESSRANGE $FF0000, $FFFFFF ;  8 bytes
       EXTRAMEMORY "RA", $A0, $20, S, E ; 12 bytes (S and E and start and end, both 0)
       MODEMSUPPORT "            "      ; 12 bytes (all spaces)
       REGIONSUPPORT "JUE"              ;  3 bytes
   .ENDSMD

When ".SMDHEADER" is defined, also the ROM checksum is calculated.

See https://plutiedev.com/rom-header for more information about the
SMD header.

This is not a compulsory directive.


".SMSHEADER"
============

All the fields in ".SMSHEADER" are optional and PRODUCTCODE, VERSION,
REGIONCODE and RESERVEDSPACE default to zero. If ROMSIZE is not
specified it will be calculated automatically:

   .SMSHEADER
       PRODUCTCODE 26, 70, 2 ; 2.5 bytes
       VERSION 1             ; 0-15
       REGIONCODE 4          ; 3-7
       RESERVEDSPACE 0, 0    ; 2 bytes
       ROMSIZE 0             ; 0-15
       CHECKSUMSIZE 32*1024  ; Uses the first this-many bytes in checksum
                             ;   calculations (excluding header area)
       FORCECHECKSUM $1234   ; Forces the checksum to be this value
       BASEADDRESS $1FF0     ; Write the header at this address
   .ENDSMS

The "REGIONCODE" also defines the system:

+---------+--------------------+
| "3"     | SMS Japan          |
+---------+--------------------+
| "4"     | SMS Export         |
+---------+--------------------+
| "5"     | GG Japan           |
+---------+--------------------+
| "6"     | GG Export          |
+---------+--------------------+
| "7"     | GG International   |
+---------+--------------------+

When ".SMSHEADER" is defined, also the checksum is calculated, and TMR
SEGA, two reserved bytes and ROM size are defined.

See http://www.smspower.org/Development/ROMHeader for more information
about SMS header.

This is not a compulsory directive.


".SMSTAG"
=========

".SMSTAG" forces WLA to write an ordinary SMS/GG ROM tag to the ROM
file. Currently only the string "TMR SEGA" and ROM checksum are
written (meaning that ".SMSTAG" also defines ".COMPUTESMSCHECKSUM").
The ROM size must be at least 8KBs.

This is not a compulsory directive.


".SNESEMUVECTOR"
================

Begins definition of the emulation mode interrupt vector table:

   .SNESEMUVECTOR
   COP    COPHandler
   UNUSED $0000
   ABORT  BRKHandler
   NMI    VBlank
   RESET  Main
   IRQBRK IRQBRKHandler
   .ENDEMUVECTOR

These can be defined in any order, but they will be placed into memory
starting at "$7FF4" ("$FFF4" in HiROM, "$40FFF4" and "$FFF4" in
ExHiROM) in the order listed above. All the vectors default to
"$0000".

This is not a compulsory directive.


".SNESHEADER"
=============

This begins the SNES header definition, and automatically defines
".COMPUTESNESCHECKSUM". From here you may define any of the following:

* "ID "ABCD"" - inserts a one to four letter string starting at
  "$7FB2" (lorom) or "$FFB2" (hirom).

* "NAME "Hello World!"" - identical to a freestanding ".NAME".

* "LOROM" - identical to a freestanding ".LOROM".

* "HIROM" - identical to a freestanding ".HIROM".

* "EXHIROM" - identical to a freestanding ".EXHIROM".

* "SLOWROM" - identical to a freestanding ".SLOWROM".

* "FASTROM" - identical to a freestanding ".FASTROM".

* "CARTRIDGETYPE $00" - Places the given 8-bit value in "$7FD6"
  ("$FFD6" in HiROM, "$40FFD6" and "$FFD6" in ExHiROM). Some possible
  values I’ve come across but cannot guarantee the accuracy of:

     +----------+--------+--------+------------+
     | "$00"    | ROM    |        |            |
     +----------+--------+--------+------------+
     | "$01"    | ROM    | RAM    |            |
     +----------+--------+--------+------------+
     | "$02"    | ROM    | SRAM   |            |
     +----------+--------+--------+------------+
     | "$03"    | ROM    |        | DSP1       |
     +----------+--------+--------+------------+
     | "$04"    | ROM    | RAM    | DSP1       |
     +----------+--------+--------+------------+
     | "$05"    | ROM    | SRAM   | DSP1       |
     +----------+--------+--------+------------+
     | "$13"    | ROM    |        | Super FX   |
     +----------+--------+--------+------------+

* "ROMSIZE $09" - Places the given 8-bit value in "$7FD7" ("$FFD7" in
  HiROM, "$40FFD7" and "$FFD7" in ExHiROM). Possible values include
  (but may not be limited to):

     +----------+---------------+
     | "$08"    | 2 Megabits    |
     +----------+---------------+
     | "$09"    | 4 Megabits    |
     +----------+---------------+
     | "$0A"    | 8 Megabits    |
     +----------+---------------+
     | "$0B"    | 16 Megabits   |
     +----------+---------------+
     | "$0C"    | 32 Megabits   |
     +----------+---------------+

* "SRAMSIZE $01" - Places the given 2-bit value into "$7FD8" ("$FFD8"
  in HiROM, "$40FFD8" and "$FFD8" in ExHiROM). I believe these are the
  only possible values:

     +----------+---------------+
     | "$00"    | 0 kilobits    |
     +----------+---------------+
     | "$01"    | 16 kilobits   |
     +----------+---------------+
     | "$02"    | 32 kilobits   |
     +----------+---------------+
     | "$03"    | 64 kilobits   |
     +----------+---------------+

* "COUNTRY $00" - Places the given 8-bit value into "$7FD9" ("$FFD9"
  in HiROM, "$40FFD9" and "$FFD9" in ExHiROM). "$00" is Japan and
  "$01" is the United States, and there several more for other regions
  that I cannot recall off the top of my head.

* "LICENSEECODE $00" - Places the given 8-bit value into "$7FDA"
  ("$FFDA" in HiROM, "$40FFDA" and "$FFDA" in ExHiROM). You must find
  the legal values yourself as there are plenty of them. ;)

* "VERSION $01" - Places the given 8-bit value into "$7FDB" ("$FFDB"
  in HiROM, "$40FFDB" and "$FFDB" in ExHiROM). This is supposedly
  interpreted as version 1.byte, so a "$01" here would be version
  1.01.

This is not a compulsory directive.


".SNESNATIVEVECTOR"
===================

Begins definition of the native mode interrupt vector table:

   .SNESNATIVEVECTOR
   COP    COPHandler
   BRK    BRKHandler
   ABORT  ABORTHandler
   NMI    VBlank
   UNUSED $0000
   IRQ    IRQHandler
   .ENDNATIVEVECTOR

These can be defined in any order, but they will be placed into memory
starting at "$7FE4" ("$FFE4" in HiROM, "$40FFE4" and "$FFE4" in
ExHiROM) in the order listed above. All the vectors default to
"$0000".

This is not a compulsory directive.


".STRINGMAP script "Hello\n""
=============================

After you’ve given the ".STRINGMAPTABLE", use ".STRINGMAP" to define
bytes using the mapping in ".STRINGMAPTABLE". For example:

   .STRINGMAP script, "いうえA\n"

".STRINGMAP" with ".STRINGMAPTABLE" is an alternative way of mapping
characters to ".ASC" and ".ASCIITABLE". Also note that here the result
and the source of the mapping can be more than just one byte.

This is not a compulsory directive.


".STRINGMAPTABLE script "script.tbl""
=====================================

".STRINGMAPTABLE"’s only purpose is to provide string mapping for
".STRINGMAP". Take a look at the example:

   .STRINGMAPTABLE script "script.tbl"

This will load the file “script.tbl” and define a new string mapping
called “script”. This file is in the “table file” format commonly used
for game translations; take a look at an example of one:

   00=A
   01=B
   ; This is a comment
   ff01=あ
   ff02=いうえ
   fe=\n

The values to the left of the ‘=’ are a variable number of bytes
expressed in hex, which map to the text value on the right. Note that
depending on the text encoding of the file, this may be a variable
number of bytes too. Thus this is a more flexible version of
".ASCIITABLE".

After you’ve given the ".STRINGMAPTABLE", use ".STRINGMAP" to define
bytes using this mapping. For example:

   .STRINGMAP script, "いうえA\n"

This will map to the byte values "FF 02 00 FE", provided the source
file and TBL file use the same string encoding - use of UTF-8 is
advised.

Note that all characters must be defined in the mapping - there is no
fallback to ASCII encoding. You also cannot mix in byte values like
with ".DB" and ".ASC".

You can define multiple named string map tables.

This is not a compulsory directive.


".STRUCT enemy_object"
======================

Begins the definition of a structure. These structures can be placed
inside "RAMSECTION" s and "ENUM" s. Here’s an example:

   .STRUCT enemy_object
   id      dw             ; the insides of a .STRUCT are 1:1 like in .ENUM
   x       db             ; except that no structs inside structs are
   y       db             ; allowed.
   data    ds  10
   info    dsb 16
   stats   dsw  4
   .ENDST

This also creates a definition "_sizeof_[struct name]", in our example
this would be "_sizeof_enemy_object", and the value of this definition
is the size of the object, in bytes (2+1+1+10+16+4*2 = 38 in the
example).

You’ll get the following definitions as well:

   enemy_object.id    (== 0)
   enemy_object.x     (== 2)
   enemy_object.y     (== 3)
   enemy_object.data  (== 4)
   enemy_object.info  (== 14)
   enemy_object.stats (== 30)

After defining a ".STRUCT" you can create an instance of it in a
".RAMSECTION" / ".ENUM" by typing:

   <instance name> INSTANCEOF <struct name> [optional, the number of structures]

Here’s an example:

   .RAMSECTION "enemies" BANK 4 SLOT 4
   enemies   INSTANCEOF enemy_object 4
   enemyman  INSTANCEOF enemy_object
   enemyboss INSTANCEOF enemy_object
   .ENDS

This will create definitions like "enemies", "enemies.1.id",
"enemies.1.x", "enemies.1.y" and so on. Definition "enemies" is
followed by four "enemy_object" instances. After those four come
"enemyman" and "enemyboss" instances, but as they are single
instances, their definitions lack the index: "enemyman",
"enemyman.id", "enemyman.x", "enemyman.y" and so on.

Take a look at the documentation on ".RAMSECTION" & ".ENUM", they have
more examples of how you can use ".STRUCT" s.

**A WORD OF WARNING**: Don’t use labels "b", "B", "w" and "W" inside a
structure as e.g., WLA sees "enemy.b" as a byte sized reference to
enemy. All other labels should be safe:

   lda enemy1.b  ; load a byte from zeropage address enemy1 or from the address
                 ; of enemy1.b??? i can't tell you, and WLA can't tell you...

It’s possible to explicitly define the size of the ".STRUCT" by using
keyword "SIZE":

   .STRUCT PaddedStruct SIZE 8
   posX  DW
   posY  DW
   .ENDST

Normally this ".STRUCT" would define four bytes, but by using keyword
"SIZE" its size is now eight bytes. The extra padding, put after the
last item in the ".STRUCT", will contain ".EMPTYFILL" bytes when used
with ".DSTRUCT".

Note that if we ".DSTRUCT" “PaddedStruct” and name it PS1 we’ll also
get a definition

   _paddingof_PS1   (== 4)

This is not a compulsory directive.


".SYM SAUSAGE"
==============

WLA treats symbols ("SAUSAGE" in this example) like labels, but they
only appear in the symbol files WLALINK outputs. Useful for finding
out the location where WLALINK puts data.

This is not a compulsory directive.


".SYMBOL SAUSAGE"
=================

".SYMBOL" is an alias for ".SYM".

This is not a compulsory directive.


".TABLE byte, word, byte"
=========================

Defines table’s columns. With .DATA and .ROW you can define data much
like using .DB or .DW, but .TABLE makes it convenient to feed big
amounts of data in mixed format.

For example:

   .TABLE byte, word, byte

After the columns have been defined, you can define rows using e.g.,

   .ROW $01, $0302, $04

This is the same as:

   .DB $01
   .DW $0302
   .DB $04

Note that .DATA can also be used instead of .ROW, if one wants to give
the data in pieces.

All supported column formats:
   * DB, BYT, BYTE

   * DW, WORD, ADDR

   * DL, LONG, FARADDR

   * DD

   * DS, DSB

   * DSW

   * DSL

   * DSD

This is not a compulsory directive.


".UNBACKGROUND $1000 $1FFF"
===========================

After issuing ".BACKGROUND" you might want to free some parts of the
backgrounded ROM image for e.g., "FREE" sections. With ".UNBACKGROUND"
you can define such regions. In the example a block starting at
"$1000" and ending at "$1FFF" was released (both ends included). You
can issue ".UNBACKGROUND" as many times as you wish.

This is not a compulsory directive.


".UNDEF DEBUG"
==============

".UNDEF" is an alias for ".UNDEFINE".

This is not a compulsory directive.


".UNDEFINE DEBUG"
=================

Removes the supplied definition label from system. If there is no such
label as given no error is displayed as the result would be the same.

You can undefine as many definitions as you wish with one ".UNDEFINE":

   .UNDEFINE NUMBER, NAME, ADDRESS, COUNTRY
   .UNDEFINE NAME, AGE

This is not a compulsory directive.


".UNION name"
=============

Begins a “union”. This can only be used in ".ENUM" s, ".RAMSECTION" s
and ".STRUCT" s.

When entering a union, the current address in the enum is saved, and
the following data is processed as normal. When the ".NEXTU" directive
is encountered, the address is reverted back to the start of the
union. This allows one to assign an area of memory to multiple labels:

   .ENUM $C000
       .UNION
           pos_lowbyte:  db
           pos_highbyte: db
           extra_word:   dw
       .NEXTU
           pos:          dw
       .ENDU
       after: db
   .ENDE

This example is equivalent to:

   .DEFINE pos_lowbyte  $c000
   .DEFINE pos_highbyte $c001
   .DEFINE extra_word   $c002
   .DEFINE pos          $c000
   .DEFINE after        $c004

The ".UNION" and ".NEXTU" commands can be given an argument to assign
a prefix to the labels that follow:

   .ENUM $C000
       .UNION union1
           byte1: db
           byte2: db
       .NEXTU union2
           word1: dw
       .ENDU
   .ENDE

This example is equivalent to:

   .DEFINE union1.byte1 $c000
   .DEFINE union1.byte2 $c001
   .DEFINE union2.word1 $c000

Unions can be nested.


".VERSION 1"
============

Indicates the Mask ROM version number located at "$14C" of a Gameboy
ROM.

This is not a compulsory directive.


".WDC"
======

Turns WLA-65816 into a mode where it accepts WDC standard assembly
code, in addition to WLA’s own syntax. In WDC standard mode:

   AND <x  ; 8-bit
   AND |?  ; 16-bit
   AND >&  ; 24-bit

are the same as:

   AND x.b ; 8-bit
   AND ?.w ; 16-bit
   AND &.l ; 24-bit

in WLA’s own syntax. Beware of the situations where you use ‘<’ and
‘>’ to get the low and high bytes!

This is not a compulsory directive.


".WHILE COUNTER > 0"
====================

Repeats the text enclosed between ".WHILE <CONDITION>" and ".ENDR":

   .WHILE COUNTER > 0
   .DB COUNTER
   .REDEFINE COUNTER = COUNTER - 1
   .ENDR

This is not a compulsory directive.


".WORD 16000, 10, 255"
======================

".WORD" is an alias for ".DW".

This is not a compulsory directive.


Assembler Syntax
****************


Case Sensitivity
================

WLA is case sensitive, except with directives, so be careful.


Comments
========

Comments begin with ";" or "*" and end along with the line. ";" can be
used anywhere, but "*" can be placed only at the beginning of a new
line.

WLA supports also ANSI C style commenting. This means you can start a
multiline comment with "/*" and end it with "*/".

What also is supported are C++ style comments. This means you can
start a comment with "//".

You can also use ".ASM" and ".ENDASM" directives to skip characters.
These function much like ANSI C comments, but unlike the ANSI C
comments these can be nested.


Line splitting
==============

Lines can be split using a "\" between elements. So instead of writing

   .db 1, 2, 3, 4, 5, 6, 7, 8

it’s possible to write

   .db 1, 2, 3, 4 \
      5, 6, 7, 8

Note that line splitting works only in places where WLA expects a new
label, number, calculation, etc. String splitting isn’t currently
supported.


Using Commas
============

In many places it’s possible to give parameters without commas between
them:

   .db 1 2 3 4 5 ; 01 02 03 04 05

**CAVEAT! CAVEAT! CAVEAT!**

If you specify the following

   .db 1 -2 3 -4 5 ; FF FF 05

WLA will detect and compute calculations, so to be sure, always use
commas:

   .db 1, -2, 3, -4, 5 ; 01 FE 03 FC 05


Labels
======

Labels are ordinary strings (which can also end with a ":"). Labels
starting with "_" are considered to be local labels and do not show
outside sections where they were defined, or outside object files, if
they were not defined inside a section.

Here are few examples of different labels:

   VBI_IRQ:
   VBI_IRQ2
   _VBI_LOOP:
   main:

Labels starting with "@" are considered to be child labels. They can
only be referenced within the scope of their parent labels, unless the
full name is specified. When there is more than one "@", the label is
considered to be a child of a child.

Here are some examples of child labels:

   PARENT1:
   @CHILD:
   @@SUBCHILD

   PARENT2:
   @CHILD:

This is legal, since each of the "@CHILD" labels has a different
parent. You can specify a parent to be explicit, like so:

   jr PARENT1@CHILD@SUBCHILD

You can also use "__label__" to refer to the last defined parent
label:

   main:                 ; #
           nop
           nop
   @child:
           nop
           nop
   @@grandchild:
           nop
           nop
           jmp __label__ ; jump -> #
   loop:   nop           ; %
           nop
           jmp __label__ ; jump -> %

Note that when you place ":" in front of the label string when
referring to it, you’ll get the bank number of the label, instead of
the label’s address. Here’s an example:

   LD A, :LOOP
   .BANK 2 SLOT 0
   LOOP:

Here "LD A, :LOOP" will be replaced with "LD A, 2" as the label "LOOP"
is inside the bank number two.

When you are referring to a label and you are adding something to its
address (or subtracting, any arithmetics apply) the result will always
be bytes.

   .org 20
   DATA:  .dw 100, 200, 300
          ld  a, DATA+1
                 ^^^^^^ = r

So here the result "r" will be the address of "DATA" plus one, here
21. Some x86 assemblers would give here "22" as the result "r" as
"DATA" points to an array or machine words, but WLA isn’t that smart
(and some people including me think this is the better solution).

Note that each CPU WLA supports contains opcodes that either generate
an absolute reference or a relative reference to the given label. For
example,

   .org 20
   DATA:  ld  a, DATA   ; DATA becomes 20 (absolute)
          jr  DATA      ; DATA becomes -4 (relative)

Check out section 14 for the list of opcodes that generate relative
references.

You can also use "-", "--", "---", "+", "++", "+++", … as un-named
labels. Labels consisting of "-" are meant for reverse jumps and
labels consisting of "+" are meant for forward jumps. You can reuse
un-named labels as much as you wish inside your source code. Here’s an
example of this:

       dec e
       beq ++      ; jump -> ?
       dec e
       beq +       ; jump -> %
       ld d, 14
   --- ld a, 10    ; !
   --  ld b, c     ; #
   -   dec b       ; *
       jp nz, -    ; jump -> *
       dec c
       jp nz, --   ; jump -> #
       dec d
       jp nz, ---  ; jump -> !
       ld a, 20
   -   dec a       ; $
       jp nz, -    ; jump -> $
   +   halt        ; %
   ++  nop         ; ?

Note that "__" (that’s two underline characters) serves also as a un-
named label. You can refer to this label from both directions. Use
"_b" when you are jumping backwards and "_f" when you are jumping
forwards label "__".

Example:

      dec e
      jp z, _f     ; jump -> *
      dec e
   __ ldi a, (hl)  ; *
      dec e
      jp nz, _b    ; jump -> *

**CAVEAT! CAVEAT! CAVEAT!**

The following code doesn’t work as it would if WLA would determine the
distance lexically (but in practice it’s WLALINK that does all the
calculations and sees only the preprocessed output of WLA):

   .macro dummy
   -  dec a        ; #
      jp nz, -     ; jump -> #
   .endm

      ...
   -  nop          ; *
      dummy
      dec e
      jp nz, -     ; i'd like to jump to *, but i'll end up jumping
                   ; to # as it's closest to me in the output WLA produces
                   ; for WLALINK (so it's better to use \@ with labels inside
                   ; a macro).

To make un-named labels inside a ".MACRO" isolated, and the previous
example to work, use the keyword "ISOLATED"

   .macro dummy isolated
   -  dec a        ; #
      jp nz, -     ; jump -> #
   .endm

The same issue exists with child labels. See ".MACRO"’s documentation
for more details.

WLALINK will also generate "_sizeof_[label]" defines that measure the
distance between two consecutive labels. These labels have the same
scope as the labels they describe. Here is an example:

   Label1:
       .db 1, 2, 3, 4
   Label2:

In this case you’ll get a definition "_sizeof_Label1" that will have
value "4".

WLA will skip over any child labels when calculating "_sizeof". So, in
this example:

   Label1:
   .db 1, 2
   @child:
       .db 3, 4
   Label2:

The value of "_sizeof_Label1" will still have a value of "4".


Number Types
============

+----------+-------------+
| "1000"   | decimal     |
+----------+-------------+
| "$100"   | hexadecimal |
+----------+-------------+
| "100h"   | hexadecimal |
+----------+-------------+
| "0x10"   | hexadecimal |
+----------+-------------+
| "%100"   | binary      |
+----------+-------------+
| "0b10"   | binary      |
+----------+-------------+
| "'x'"    | character   |
+----------+-------------+

Remember that if you use the suffix "h" to give a hexadecimal value,
and the value begins with an alphabet, you must place a zero in front
of it so WLA knows it’s not a label (e.g., "0ah" instead of "ah").


Strings
=======

Strings begin with and end to """. Note that no "0" is inserted to
indicate the termination of the string like in e.g., ANSI C. You’ll
have to do it yourself. You can place quotation marks inside strings
the way C preprocessors accept them.

Here are some examples of strings:

   "Hello world!"
   "He said: \"Please, kiss me honey.\""


Substitution
============

It’s possible to substitute definition’s name with its value inside a
label.

Here’s an example:

   .REPEAT 10 INDEX COUNT
   Label_{COUNT}:                      ; -> Label_0, Label_1, Label_2...
   .DW Label_{COUNT}
   .ENDR

Substitution supports minimal formatting for integers:

   .DEFINE COUNT = 10
   .DEFINE UNIT = 5
   Label_{%.4x{COUNT}}:                ; -> Label_000a
   Label_{%.3X{COUNT}}_{%.3X{UNIT}}:   ; -> Label_00A_005
   Label_{%.9d{COUNT}}:                ; -> Label_000000010
   Label_{%.3i{COUNT}}:                ; -> Label_010

The examples show all the formatting symbols currently supported.

The same substitution works for strings inside quotes when the quoted
string is as follows:

   .db { "HELLO_{COUNT}" }             ; -> "HELLO_10"

Note that only WLA can do the substitution and it needs to know the
value of the definition at the time the substitution is done, i.e.,
the time a string containing a substitution is parsed.

Also note that you can embed calculations into substitutions:

   .DEFINE COUNT = 1
   Label_{COUNT+1}:                    ; -> Label_2


Mnemonics
=========

You can give the operand size with the operand itself (and this is
highly recommended) in WLA
6502/65C02/65CE02/HUC6280/65816/6800/6801/6809:

   and #20.b
   and #20.w
   bit loop.b
   bit loop.w


Brackets?
=========

You can write

   LDI (HL), A

or

   LDI [HL], A

as both mean the same thing in the syntax of most of the supported
CPUs. Yes, you could write

   LDI [HL), A

but that is not recommended.

Note that brackets have special meaning when dealing with a
65816/SPC-700 system so you can’t use

   AND [$65]

instead of

   AND ($65)

as they mean different things.


Error Messages
**************

There are quite a few of them in WLA, but most of them are not as
informative as I would like them to be. This will be fixed in the
future. Mean while, be careful. ;)


Supported ROM/RAM/Cartridge Types (WLA-GB)
******************************************


ROM Size
========

GB-Z80 version of WLA supports the following ROM bank sizes. There’s
no such limit in the Z80/6502/65C02/65CE02/65816/6800/6801/6809/8008/
8080/HUC6280/SPC-700/SuperFX version of WLA. Supply one of the
following values to ".ROMBANKS".

+---------+----------+------------+-------------+
| "$00"   | 256Kbit  | 32KByte    | 2 banks     |
+---------+----------+------------+-------------+
| "$01"   | 512Kbit  | 64KByte    | 4 banks     |
+---------+----------+------------+-------------+
| "$02"   | 1Mbit    | 128KByte   | 8 banks     |
+---------+----------+------------+-------------+
| "$03"   | 2Mbit    | 256KByte   | 16 banks    |
+---------+----------+------------+-------------+
| "$04"   | 4Mbit    | 512KByte   | 32 banks    |
+---------+----------+------------+-------------+
| "$05"   | 8Mbit    | 1MByte     | 64 banks    |
+---------+----------+------------+-------------+
| "$06"   | 16Mbit   | 2MByte     | 128 banks   |
+---------+----------+------------+-------------+
| "$07"   | 32Mbit   | 4MByte     | 256 banks   |
+---------+----------+------------+-------------+
| "$08"   | 64Mbit   | 8MByte     | 512 banks   |
+---------+----------+------------+-------------+
| "$52"   | 9Mbit    | 1.1MByte   | 72 banks    |
+---------+----------+------------+-------------+
| "$53"   | 10Mbit   | 1.2MByte   | 80 banks    |
+---------+----------+------------+-------------+
| "$54"   | 12Mbit   | 1.5MByte   | 96 banks    |
+---------+----------+------------+-------------+


RAM Size
========

Supply one of the following hex values to ".RAMSIZE" in the GB-Z80
version of WLA.

+---------+----------+------------+------------+
| "$00"   | None     | None       | None       |
+---------+----------+------------+------------+
| "$01"   | 16kbit   | 2kByte     | 1 bank     |
+---------+----------+------------+------------+
| "$02"   | 64kbit   | 8kByte     | 1 bank     |
+---------+----------+------------+------------+
| "$03"   | 256kbit  | 32kByte    | 4 banks    |
+---------+----------+------------+------------+
| "$04"   | 1Mbit    | 128kByte   | 16 banks   |
+---------+----------+------------+------------+
| "$05"   | 512kbit  | 64kByte    | 8 banks    |
+---------+----------+------------+------------+


Cartridge Type
==============

It’s up to the user to check that the cartridge type is valid and can
be used combined with the supplied ROM and RAM sizes. Give one the the
following values to ".CARTRIDGETYPE" in the GB-Z80 version of WLA.

+---------+-----------------+--------+--------+-----------+---------+----------+
| "$00"   | ROM             |        |        |           |         |          |
+---------+-----------------+--------+--------+-----------+---------+----------+
| "$01"   | ROM             | MBC1   |        |           |         |          |
+---------+-----------------+--------+--------+-----------+---------+----------+
| "$02"   | ROM             | MBC1   | RAM    |           |         |          |
+---------+-----------------+--------+--------+-----------+---------+----------+
| "$03"   | ROM             | MBC1   | RAM    | BATTERY   |         |          |
+---------+-----------------+--------+--------+-----------+---------+----------+
| "$05"   | ROM             | MBC2   |        |           |         |          |
+---------+-----------------+--------+--------+-----------+---------+----------+
| "$06"   | ROM             | MBC2   |        | BATTERY   |         |          |
+---------+-----------------+--------+--------+-----------+---------+----------+
| "$08"   | ROM             |        | RAM    |           |         |          |
+---------+-----------------+--------+--------+-----------+---------+----------+
| "$09"   | ROM             |        | RAM    | BATTERY   |         |          |
+---------+-----------------+--------+--------+-----------+---------+----------+
| "$0B"   | ROM             | MMM01  |        |           |         |          |
+---------+-----------------+--------+--------+-----------+---------+----------+
| "$0C"   | ROM             | MMM01  | SRAM   |           |         |          |
+---------+-----------------+--------+--------+-----------+---------+----------+
| "$0D"   | ROM             | MMM01  | SRAM   | BATTERY   |         |          |
+---------+-----------------+--------+--------+-----------+---------+----------+
| "$0F"   | ROM             | MBC3   |        | BATTERY   | TIMER   |          |
+---------+-----------------+--------+--------+-----------+---------+----------+
| "$10"   | ROM             | MBC3   | RAM    | BATTERY   | TIMER   |          |
+---------+-----------------+--------+--------+-----------+---------+----------+
| "$11"   | ROM             | MBC3   |        |           |         |          |
+---------+-----------------+--------+--------+-----------+---------+----------+
| "$12"   | ROM             | MBC3   | RAM    |           |         |          |
+---------+-----------------+--------+--------+-----------+---------+----------+
| "$13"   | ROM             | MBC3   | RAM    | BATTERY   |         |          |
+---------+-----------------+--------+--------+-----------+---------+----------+
| "$19"   | ROM             | MBC5   |        |           |         |          |
+---------+-----------------+--------+--------+-----------+---------+----------+
| "$1A"   | ROM             | MBC5   | RAM    |           |         |          |
+---------+-----------------+--------+--------+-----------+---------+----------+
| "$1B"   | ROM             | MBC5   | RAM    | BATTERY   |         |          |
+---------+-----------------+--------+--------+-----------+---------+----------+
| "$1C"   | ROM             | MBC5   |        |           |         | RUMBLE   |
+---------+-----------------+--------+--------+-----------+---------+----------+
| "$1D"   | ROM             | MBC5   | SRAM   |           |         | RUMBLE   |
+---------+-----------------+--------+--------+-----------+---------+----------+
| "$1E"   | ROM             | MBC5   | SRAM   | BATTERY   |         | RUMBLE   |
+---------+-----------------+--------+--------+-----------+---------+----------+
| "$20"   | MBC6            |        |        |           |         |          |
+---------+-----------------+--------+--------+-----------+---------+----------+
| "$22"   | MBC7            |        |        |           |         |          |
+---------+-----------------+--------+--------+-----------+---------+----------+
| "$BE"   | Pocket Voice    |        |        |           |         |          |
+---------+-----------------+--------+--------+-----------+---------+----------+
| "$FC"   | Pocket Camera   |        |        |           |         |          |
+---------+-----------------+--------+--------+-----------+---------+----------+
| "$FD"   | Bandai TAMA5    |        |        |           |         |          |
+---------+-----------------+--------+--------+-----------+---------+----------+
| "$FE"   | Hudson HuC-3    |        |        |           |         |          |
+---------+-----------------+--------+--------+-----------+---------+----------+
| "$FF"   | Hudson HuC-1    |        |        |           |         |          |
+---------+-----------------+--------+--------+-----------+---------+----------+


Bugs
****

If you find bugs, please let us know about them via GitHub:
https://github.com/vhelin/wla-dx/issues


Files
*****


"tests"
=======

The main purpose of the files in the "tests" directory is to test that
WLA and WLALINK can assemble and link the tiny project correctly. You
can also take a look at the code and syntax in the files, but beware:
if you run the rom files you probably don’t see anything on screen.

"include" directory under "gb-z80" could be very useful as the six
include files there have all the Game Boy hardware register address
and memory definitions you could ever need and more.


"tests/gb-z80/lib"
==================

This folder holds few very useful libraries for you to use in your
Game Boy projects. Instead of reinventing the wheel, use the stuff
found in here. Remember to compile the libraries right after you’ve
installed WLA by executing "make" in the "lib" directory.


"memorymaps"
============

Here you can find default memory maps (see ".MEMORYMAP") for various
computers and video game consoles.


Functions
*********

WLA supports functions in addition to ".MACRO" s. Functions are
different from ".MACRO" s as functions always return a value.


User defined functions
======================

Use ".FUNCTION" to create your own functions.


Built-in functions
==================

The following built-in functions can be used where ever a number is
expected:

+------------+-----------------------------------------------------------------------------------+
| abs()      | Returns the positive version of the argument                                      |
+------------+-----------------------------------------------------------------------------------+
| acos()     | The same as ANSI C90 acos()                                                       |
+------------+-----------------------------------------------------------------------------------+
| asc()      | Uses ".ASCIITABLE" to map the supplied value                                      |
+------------+-----------------------------------------------------------------------------------+
| asin()     | The same as ANSI C90 asin()                                                       |
+------------+-----------------------------------------------------------------------------------+
| atan()     | The same as ANSI C90 atan()                                                       |
+------------+-----------------------------------------------------------------------------------+
| atan2()    | The same as ANSI C90 atan2()                                                      |
+------------+-----------------------------------------------------------------------------------+
| bank()     | Returns the bank (the same as preceding ":")                                      |
+------------+-----------------------------------------------------------------------------------+
| bankbyte() | Returns the bank byte, bits 16-23                                                 |
+------------+-----------------------------------------------------------------------------------+
| ceil()     | The same as ANSI C90 ceil()                                                       |
+------------+-----------------------------------------------------------------------------------+
| clamp()    | Takes three arguments, value, min and max, clamps the value between min and max   |
+------------+-----------------------------------------------------------------------------------+
| cos()      | The same as ANSI C90 cos()                                                        |
+------------+-----------------------------------------------------------------------------------+
| cosh()     | The same as ANSI C90 cosh()                                                       |
+------------+-----------------------------------------------------------------------------------+
| defined()  | Returns 1 (true) if the supplied definition exists, 0 (false) otherwise           |
+------------+-----------------------------------------------------------------------------------+
| exists()   | Returns 1 (true) if the supplied file exists, 0 (false) otherwise                 |
+------------+-----------------------------------------------------------------------------------+
| floor()    | The same as ANSI C90 floor()                                                      |
+------------+-----------------------------------------------------------------------------------+
| hibyte()   | Returns the high byte, bits 8-15 (the same as preceding ">")                      |
+------------+-----------------------------------------------------------------------------------+
| hiword()   | Returns the high word, bits 16-31                                                 |
+------------+-----------------------------------------------------------------------------------+
| lobyte()   | Returns the low byte, bits 0-7 (the same as preceding "<")                        |
+------------+-----------------------------------------------------------------------------------+
| log()      | The same as ANSI C90 log()                                                        |
+------------+-----------------------------------------------------------------------------------+
| log10()    | The same as ANSI C90 log10()                                                      |
+------------+-----------------------------------------------------------------------------------+
| loword()   | Returns the low word, bits 0-15                                                   |
+------------+-----------------------------------------------------------------------------------+
| max()      | Takes two arguments, a and b, returns the bigger value                            |
+------------+-----------------------------------------------------------------------------------+
| min()      | Takes two arguments, a and b, returns the smaller value                           |
+------------+-----------------------------------------------------------------------------------+
| pow()      | The same as ANSI C90 pow()                                                        |
+------------+-----------------------------------------------------------------------------------+
| random()   | Takes two arguments, min and max, returns a pseudo random integer like ".DBRND"   |
+------------+-----------------------------------------------------------------------------------+
| round()    | The same as ANSI C99 round()                                                      |
+------------+-----------------------------------------------------------------------------------+
| sign()     | Return 0 if the supplied value is 0, -1 if negative and 1 if positive             |
+------------+-----------------------------------------------------------------------------------+
| sin()      | The same as ANSI C90 sin()                                                        |
+------------+-----------------------------------------------------------------------------------+
| sinh()     | The same as ANSI C90 sinh()                                                       |
+------------+-----------------------------------------------------------------------------------+
| sqrt()     | Returns the square root of the supplied value                                     |
+------------+-----------------------------------------------------------------------------------+
| tan()      | The same as ANSI C90 tan()                                                        |
+------------+-----------------------------------------------------------------------------------+
| tanh()     | The same as ANSI C90 tanh()                                                       |
+------------+-----------------------------------------------------------------------------------+

Note! Use bankbyte() with WLA-65816 as on that platform the bank (+
base) bits are 16-23. On other platforms bank() works better.


Examples of functions
=====================

Here’s an example about how these functions can be used

   .IF defined(USE_DEBUG) && defined(DEBUG_SHOW) && min(VALUE_A, VALUE_B) > 10

   LDX #loword(CPU_ADDR)           ; instead of (CPU_ADDR & $00FFFF)
   LDA #bankbyte(CPU_ADDR)         ; instead of :CPU_ADDR
   .DB random(0, 10)               ; defines a byte with value 0-10

   .ENDIF

NOTE: random() needs immediate min and max values.


Temporary Files
***************

Note that WLA will generate temporary files while it works. The files
are generated using ANSI C’s tmpfile() function.

When WLA finishes its work these files are deleted as they serve of no
further use.


Compiling
*********


Compiling Object Files
======================

To compile an object file use the "-o [OUT]" option on the command
line.

These object files can be linked together (or with library files)
later with WLALINK.

Name object files so that they can be recognized as object files.
Normal suffix is ".o" (WLA default). This can also be changed with
".OUTNAME".

With object files you can reduce the amount of compiling when editing
small parts of the program. Note also the possibility of using local
labels (starting with "_").

**Note**: When you compile objects, group 1 directives are saved for
linking time, when they are all compared and if they differ, an error
message is shown. It is advisable to use something like an include
file to hold all the group 1 directives for that particular project
and include it to every object file.

If you are interested in the WLA object file format, take a look at
the file "txt/wla_file_formats.txt" which is included in the release
archive.

Here are some examples of definitions:

* "-D IEXIST"

* "-D DAY=10"

* "-D BASE = $10"

* "-D NAME=elvis"

And here’s an WLA example creating definitions on the command line:

   wla-gb -D DEBUG -D VERBOSE=5 -D NAME = "math v1.0" -o math.o math.s

"DEBUG"’s value will be "0", "VERBOSE"’s "5" and "NAME" is a string
definition with value "math v1.0". Note that "-D" always needs a space
after it, but the rest of the statement can be optionally stuck inside
one word.


Compiling Library Files
=======================

To compile a library file use the "-l [OUT]" option on the command
line.

Name these files so that they can be recognized as library files.
Normal suffix is ".lib" (WLA default).

With library files you can reduce the amount of compiling. Library
files are meant to hold general functions that can be used in
different projects. Note also the possibility of using local labels
(starting with "_"). Library files consist only of "FREE" sections.


Linking
*******

After you have produced one or more object files and perhaps some
library files, you might want to link them together to produce a ROM
image / program file. WLALINK is the program you use for that. Here’s
how you use it:

   wlalink [OPTIONS] <LINK FILE> <OUTPUT FILE>

Choose the option "-b [OUT]" for program file or "-r [OUT]" for ROM
image linking. ROM image is all the data in the ROM banks. Program
file is the data between the first used byte and the last used byte.
You can also use "-bS [START ADDRESS]" and "-bE [END ADDRESS]" to
specify the start and the end addresses of the program. Both are
optional.

Link file is a text file that contains information about the files you
want to link together. Here’s the format:

1. You must define the group for the files. Put the name of the group
   inside brackets. Valid group definitions are

      [objects]
      [libraries]
      [header]
      [footer]
      [definitions]
      [ramsections]
      [sections]
      [sectionwriteorder]
      [ramsectionwriteorder]

2. Start to list the file names.

      [objects]
      main.o
      vbi.o
      level_01.o
      ...

3. Give parameters to the library files:

      [libraries]
      bank 0 slot 1 speed.lib
      bank 4 slot 2 map_data.lib
      ...

   Here you can also use "base" to define the 65816 CPU bank number
   (like ".BASE" works in WLA):

      [libraries]
      bank 0 slot 1 base $80 speed.lib
      bank 4 slot 2 base $80 map_data.lib
      ...

   You must tell WLALINK the bank and the slot for the library files.

4. If you want to use header and/or footer in your project, you can
   type the following:

      [header]
      header.dat
      [footer]
      footer.dat

5. If you have RAMSECTIONs inside the libraries, you must place the
   sections inside BANKs and SLOTs (ORG and ORGA are optional). Note
   that you can also change the type and priority of the section, and
   can use appendto:

      [ramsections]
      bank 0 slot 3 org $0 "library 1 vars 1"
      bank 0 slot 3 orga $6100 priority 100 force "library 1 vars 2"
      bank 0 slot 3 appendto "library 1 vars 2" "library 1 vars 3"

6. If you want to relocate normal sections, do as follows (ORG, ORGA,
   KEEP, AFTER, OFFSET, PRIORITY, WINDOW, BITWINDOW and APPENDTO are
   optional, but useful):

      [sections]
      bank 0 slot 1 org $100 appendto "MusicPlayers" "MusicPlayer1"
      bank 0 slot 1 orga $2200 semisubfree priority 100 keep bitwindow 8 "EnemyAI"
      bank 0 slot 2 after "Enemies" offset 256 "Dragon"

7. If you want to make value definitions, here’s your chance:

      [definitions]
      debug 1
      max_str_len 128
      start $150
      ...

8. If you want to change the order in which the linker writes the
   sections to output:

      [sectionwriteorder]
      OVERWRITE
      FORCE
      FREE
      SEMISUPERFREE
      SEMISUBFREE
      SEMIFREE
      SUPERFREE

9. If you want to change the order in which the linker writes the RAM
   sections to output:

      [ramsectionwriteorder]
      FREE
      FORCE
      SEMISUBFREE
      SEMIFREE

Note that you have to specify all the section types here.

If flag "v" is used, WLALINK displays information about ROM file after
a succesful linking.

If flag "R" is used the file paths inside the link file are relative
to the directory where the link file is, not relative to current
working directory.

If flag "nS" is used, WLALINK doesn’t sort the sections at all, so
they are placed in the output in their order of appearance.

If flag "s" is used, WLALINK will produce a NO$GMB/NO$SNES symbol
file. It’s useful when you work under MSDOS (NO$GMB is a very good
Game Boy emulator for MSDOS/Windows) as it contains information about
the labels in your project.

If flag "S" is used, WLALINK will create a WLA symbol file, that is
much like NO$GMB symbol file, but shows also symbols, defines, and
breakpoints, not just labels.

If flag "d" is used, WLALINK discards all unreferenced "FREE",
"SEMIFREE", "SEMISUBFREE", "SUPERFREE" and "RAM" sections. This way
you can link big libraries to your project and WLALINK will choose
only the used sections, so you won’t be linking any dead code/data.

If flag "D" is used, WLALINK doesn’t create any _sizeof_* labels. Note
that to disable fully _sizeof_* label creation, you’ll also need to
give WLA the "s" flag.

If flag "pS" is used then WLALINK doesn’t use section type in writing
the ".SECTION" s, but instead uses just the "PRIORITY" (and size) when
it writes the ".SECTION" s to output.

Flag "pR" works the same as "pS" but for ".RAMSECTION" s.

If flag "t" is used with "c64PRG", WLALINK will add a two byte header
to the program file (use with flag "b"). The header contains the load
address for the PRG. Use the flag "a" to specify the load address. It
can be a value or the name of a label.

If flag "i" is given, WLALINK will write list files. Note that you
must compile the object and library files with "-i" flag as well.
Otherwise WLALINK has no extra information it needs to build list
files. Here is an example of a list file: Let’s assume you’ve compiled
a source file called "main.s" using the "i" flag. After you’ve linked
the result also with the "i" flag WLALINK has created a list file
called "main.lst". This file contains the source text and the result
data the source compiled into. List files are good for debugging.
NOTE: list file data can currently be generated only for code inside
sections. ".MACRO" calls and ".REPT" s don’t produce list file data
either.

If flag "L" is given after the above options, WLALINK will use the
directory specified after the flag for including libraries. If WLALINK
cannot find the library in the specified directory, it will then
silently search the current working directory. This is useful when
using WLA in an SDK environment where a global path is needed.

Make sure you don’t create duplicate labels in different places in the
memory map as they break the linking loop. Duplicate labels are
allowed when they overlap each other in the destination machine’s
memory. Look at the following example:

   ...
   .BANK 0
   .ORG $150

       ...
       LD      A, 1
       CALL    LOAD_LEVEL
       ...

   LOAD_LEVEL:
       LD      HL, $2000
       LD      (HL), A
       CALL    INIT_LEVEL
       RET

   .BANK 1
   .ORG 0

   INIT_LEVEL:
       ...
       RET

   .BANK 2
   .ORG $0

   INIT_LEVEL:
       ...
       RET
   ...

Here duplicate "INIT_LEVEL" labels are accepted as they both point to
the same memory address (in the program’s point of view).

Note that when you use .RAMSECTIONs, WLALINK will generate labels
RAM_USAGE_SLOT_[slot name/id]_BANK_[bank number]_START and
RAM_USAGE_SLOT_[slot name/id]_BANK_[bank number]_END that contain the
addresses of the first and last used byte in the RAM bank/slot. Note
that this only uses .RAMSECTION information to calculate the
addresses, not .ENUMs or anything else.

Examples:

   [seravy@localhost tbp]# wlalink -r linkfile testa.sfc
   [seravy@localhost tbp]# wlalink -d -i -b linkfile testb.sfc
   [seravy@localhost tbp]# wlalink -v -S -L ../../lib linkfile testc.sfc
   [seravy@localhost tbp]# wlalink -v -b -s -t c64PRG -a LOAD_ADDRESS linkfile linked.prg


Arithmetics
***********

WLA is able to solve really complex calculations like

   -((HELLO / 2) | 3)
   skeletor_end-skeletor
   10/2.5

so you can write something like

   LD HL, data_end-data
   LD A, (pointer + 1)
   CP (TEST + %100) & %10101010

WLALINK also has this ability so it can compute the pending
calculations WLA wasn’t able to solve.

NOTE! The assembler has only a limited capability to turn labels into
addresses. Often label references are left for the linker to solve.
Currently the assembler can do so when the label is outside ".SECTION"
s or inside "FORCE" or "OVERWRITE" ".SECTION" s and the label is
defined before it is referenced. Many directives like ".ASSERT"
require data that the assembler can immediately solve so you might run
into problems when feeding labels to directives.

The following operators are valid:

+--------+---------------------------------+
| "|"    | bitwise or                      |
+--------+---------------------------------+
| "&"    | bitwise and                     |
+--------+---------------------------------+
| "^"    | power                           |
+--------+---------------------------------+
| "<<"   | bitwise shift left              |
+--------+---------------------------------+
| ">>"   | bitwise shift right             |
+--------+---------------------------------+
| "+"    | plus                            |
+--------+---------------------------------+
| "-"    | minus                           |
+--------+---------------------------------+
| "#"    | modulo                          |
+--------+---------------------------------+
| "~"    | bitwise xor                     |
+--------+---------------------------------+
| "*"    | multiply                        |
+--------+---------------------------------+
| "/"    | divide                          |
+--------+---------------------------------+
| "<"    | get the low byte                |
+--------+---------------------------------+
| ">"    | get the high byte               |
+--------+---------------------------------+
| ":"    | get the bank byte of an address |
+--------+---------------------------------+

Note that you can do NOT using XOR

   - ``VALUE_A ~ $FF``   is  8-bit NOT
   - ``VALUE_B ~ $FFFF`` is 16-bit NOT

Unary XOR (e.g., "~$FF") is the same as NOT.

".IF" conditions have the following additional operators:

+--------+-----------------------------------------------------------------+
| "!"    | not                                                             |
+--------+-----------------------------------------------------------------+
| "<"    | smaller than (note that outside ".IF" this is something else)   |
+--------+-----------------------------------------------------------------+
| ">"    | larger than (note that outside ".IF" this is something else)    |
+--------+-----------------------------------------------------------------+
| "<="   | smaller or equal                                                |
+--------+-----------------------------------------------------------------+
| ">="   | larger or equal                                                 |
+--------+-----------------------------------------------------------------+
| "=="   | equal                                                           |
+--------+-----------------------------------------------------------------+
| "!="   | unequal                                                         |
+--------+-----------------------------------------------------------------+
| "||"   | logical or                                                      |
+--------+-----------------------------------------------------------------+
| "&&"   | logical and                                                     |
+--------+-----------------------------------------------------------------+

Here’s a table of the precedence of the operators in calculations and
conditions (higher priority operators come first):

+---------------------------+-----------------------------------------------+
| "(" ")"                   | expression                                    |
+---------------------------+-----------------------------------------------+
| "~" "!"                   | unary                                         |
+---------------------------+-----------------------------------------------+
| "<" ">" ":"               | low byte / high byte / bank (outside ".IF")   |
+---------------------------+-----------------------------------------------+
| "/" "*" "#" "^"           | multiplicative                                |
+---------------------------+-----------------------------------------------+
| "+" "-"                   | additive                                      |
+---------------------------+-----------------------------------------------+
| "<<" ">>"                 | bitwise shift                                 |
+---------------------------+-----------------------------------------------+
| "<" ">" "<=" ">="         | relational (only inside ".IF")                |
+---------------------------+-----------------------------------------------+
| "==" "!="                 | equality (only inside ".IF")                  |
+---------------------------+-----------------------------------------------+
| "&"                       | bitwise and                                   |
+---------------------------+-----------------------------------------------+
| "~"                       | bitwise xor                                   |
+---------------------------+-----------------------------------------------+
| "|"                       | bitwise or                                    |
+---------------------------+-----------------------------------------------+
| "&&"                      | logical and                                   |
+---------------------------+-----------------------------------------------+
| "||"                      | logical or                                    |
+---------------------------+-----------------------------------------------+

WLA computes internally with real numbers so "(5/2)*2" produces "5",
not "4".


Binary to DB Conversion
***********************

WLAB converts binary files to WLA’s byte definition strings. Here’s
how you use it:

   wlab -[ap]{bdh} <BIN FILE>

Give it the binary file and WLAB will output the WLA DB formatted data
of it into stdout. Here’s an example from real life:

   wlab -da gayskeletor.bin > gayskeletor.s

WLAB has three command flags of which one must be given to WLAB:

-b

Output data in binary format.

-d

Output data in decimal format.

-h

Output data in hexadecimal format.

WLAB has also two option flags:

-a

Print the address (relative to the beginning of the data).

-p

Don’t print file header.

Examples:

   [seravy@localhost src]# wlab -bap iscandar.bin > iscandar.s
   [seravy@localhost src]# wlab -h starsha.bin > starsha.s


Things you should know about coding for…
****************************************

Please be aware that the source code files in there are mainly used to
test that the compiler and linker work, they are not possibly good
examples of how you should write code using WLA DX.


Z80
===

Check the Z80 specific directives. All SMS/GG coders should find
".SMSTAG", ".SDSCTAG" and ".COMPUTESMSCHECKSUM" very useful…

There are shadow register aliases for opcodes that use registers A, F,
BC, DE and HL. The shadow register versions are just for convenience,
if the programmer wants to explicitly show that he is now using the
shadow registers. For example:

   AND A     ; (original, assembles to 0xA7) AND A’    ; (alias,
   assembles to 0xA7 and is in reality “AND A”)

Opcodes that make relative label references:

   JR *
   DJNZ


6502
====

For example mnemonics "ADC", "AND", "ASL", etc… cause problems to WLA,
because they take different sized arguments. Take a look at this:

   LSR 11       ; $46 $0B
   LSR $A000    ; $4E $00 $A0

The first one could also be

   LSR 11       ; $4E $0B $00

To really get what you want, use ".8BIT", ".16BIT" and ".24BIT"
directives. Or even better, supply WLA the size of the argument:

   LSR 11.W     ; $4E $0B $00

Opcodes that make relative label references:

   BCC
   BCS
   BEQ
   BMI
   BNE
   BPL
   BVC
   BVS


65C02
=====

Read the subsection 6502 as the information applies also to 65C02
coding…

Opcodes that make relative label references:

   BCC
   BCS
   BEQ
   BMI
   BNE
   BPL
   BVC
   BVS
   BRA
   BBR*
   BBS*


65CE02
======

Read the subsection 6502 as the information applies also to 65CE02
coding…

Opcodes that make relative label references:

   BCC
   BCS
   BEQ
   BMI
   BNE
   BPL
   BVC
   BVS
   BRA
   BSR
   BBR*
   BBS*


65816
=====

Read the subsection 6502 as the information applies also to 65816
coding…

WLA-65816 has also few SNES specific directives which are all very
helpful. Remember that when you use ".LOROM", ".HIROM", ".SLOWROM" and
".FASTROM" WLA automatically writes the information into the output.
".COMPUTESNESCHECKSUM", ".SNESHEADER" and few others could also be
useful.

Use ".BASE" to set the upmost eight bits of 24-bit addresses.

If possible, use operand hints to specify the size of the operand. WLA
is able to deduce the accumulator/index mode to some extent from
"REP"/"SEP"-mnemonics and ".ACCU" and ".INDEX"-directives, but just to
be sure, terminate the operand with ".B", ".W" or ".L".

   AND #10     ; can be two different things, depending on the size of the accu.
   AND #10.B   ; forces 8-bit immediate value.
   AND #10.W   ; forces 16-bit immediate value.

Or if you must, these work as well:

   AND.B #10   ; the same as "AND #10.B".
   AND.W #10   ; the same as "AND #10.W".

Opcodes that make relative label references:

   BCC
   BCS
   BEQ
   BMI
   BNE
   BPL
   BVC
   BVS
   BRA
   BRL
   PER

Use ".WDC" to start parsing WDC standard assembly code. ".NOWDC" sets
the parser to parse WLA syntax assembly code.

"MVN" and "MVP" work as follows:

   MVN $xx, $yy
   MVN $xxyy
   MVP $xx, $yy
   MVP $xxyy

"xx" is the source bank, "yy" is the target bank.


HUC6280
=======

Read the subsection 6502 as the information applies also to HUC6280
coding…

Opcodes that make relative label references:

   BCC
   BCS
   BEQ
   BMI
   BNE
   BPL
   BVC
   BVS
   BSR
   BBR*
   BBS*


SPC-700
=======

Note that you’ll have to put an exclamation mark before a 16-bit
value. For example,

   CALL !Main
   AND  A, !$1000

Opcodes that make relative label references:

   BCC
   BCS
   BEQ
   BMI
   BNE
   BPL
   BVC
   BVS
   BRA
   BBS
   BBC
   CBNE *
   DBNZ *


Pocket Voice (GB-Z80)
=====================

Pocket Voice uses its own MBC. You can enable Pocket Voice mode by
selecting Pocket Voice cartridge type ("$BE" in "$0147") and defining
correct ".ROMBANKMAP" and ".MEMORYMAP". In PV mode bank 0 is 24KB and
the rest are 8KB.

Note that WLA assumes that ROM offset is all the time 0. If you use
something else as the offset, make sure to compute the jumps by hand
as WLA cannot do that.

Check out "tests/gb-z80/include/pocket_voice.i" for more information.


GB-Z80
======

WLA outputs only "$10" when it decodes "STOP". Often it’s necessary to
put an extra "NOP" ("$00") after a "STOP", and sometimes something
else, but that’s left entirely to the user.

Opcodes that make relative label references:

   JR *


WLA Flags
*********

Here are short descriptions for the flags you can give to WLA:

You can supply WLA with some (or all or none) of the following option
flags:

   -c  Continue parsing after an error. Currently we can only continue after
       encountering an unknown symbol or a mistyped instruction.
   -d  Disable WLA's ability to calculate A-B where A and B are labels
   -h  Assume that all label references are 16-bit by default (size hints
       still work). Without this flag it's assumed that label references are
       8-bit unless otherwise specified.
   -i  Add list file information. Adds extra information to the output so
       WLALINK can produce list files.
   -k  Keep all empty sections. By default they are discarded.
   -M  WLA generates makefile rules describing the dependencies of the main
       source file.
   -MP Create a phony target for each dependency other than the main file,
       use this with -M.
   -MF Specify a file to write the dependencies to, use this with -M.
   -q  Quiet mode. ``.PRINT*`` -directives output nothing.
   -s  Don't create _sizeof_* and _padding_* definitions.
   -t  Test assemble. Doesn't output any files.
   -v  Verbose mode. Shows a lot of information about the compiling process.
   -v1 Verbose messages (only discard sections)
   -v2 Verbose messages (-v1 plus short summary)
   -w  Require labels to end in a colon.
   -x  Extra compile time labels and definitions. WLA does extra work by creating
       few helpful definitions, and labels SECTIONSTART_[section name] and
       SECTIONEND_[section name] at the beginning and end of a section.
   -D  Declare a definition.

One (and only one) of the following command flags must be defined.

   -l

   Output a library file.

   -o

   Output an object file.

You may also use an extra option to specify the include directory. WLA
will search this directory for included files before defaulting to the
specified ".INCDIR" or current working directory:

   -I  Directory to include files.

Examples:

   [seravy@localhost tbp]# wla -D VERSION=255 -x -v -i -o testa.o testa.s
   [seravy@localhost tbp]# wla -M testa.s
   [seravy@localhost tbp]# wla -D VERSION=$FF -D MESSAGE=\"Hello world\" -l testb.lib testb.s
   [seravy@localhost tbp]# wla -I ../../include -l testb.lib testb.s
   [seravy@localhost tbp]# wla -M -I myfiles -l testa.lib testa.s

NOTE: If you use -M and -l/-o at the same time, specify -M first on
the command line.

NOTE: The first example produces a file named "testa.o".


Extra compile time definitions
******************************

When you supply WLA with the flag "x" it will maintain few useful
definitions and labels while compiling your source codes. Please use
the enhanced error reporting engine (so don’t use flag "f") in
conjunction with flag "x" as some of the definitions require extra
information about the flow of the data which isn’t available when
using the old, crippled error reporting engine.

Here’s a list of definitions you get when you use flag "x":

+--------------------+----------------------------------------------------------+
| "WLA_FILENAME"     | A string definition holding the file name WLA is         |
|                    | currently processing.                                    |
+--------------------+----------------------------------------------------------+
| "WLA_TIME"         | A string definition holding the calendar time (obtained  |
|                    | using C’s "ctime()").                                    |
+--------------------+----------------------------------------------------------+
| "WLA_VERSION"      | A string definition holding the version number of WLA.   |
+--------------------+----------------------------------------------------------+

So you can do for example something like

   .DB WLA_TIME

to store the time when the build process started into the ROM file you
are compiling.

Definition "CADDR", which is present without supplying the flag "x",
contains the current 16-bit memory address. So

   LD HL, CADDR

will load the address of the operand data into registers "H" and "L".

**CAVEAT**: Remember when using defines that contain "CADDR" gets the
address of the place where the definition is used, not the address of
the definition, which contains the "CADDR".

Note that you’ll also get all these definitions in lower case (e.g.,
"wla_filename").

But that is not all. You will also get SECTIONSTART_[section name]
labels that are inserted into the start of every section, and
SECTIONEND_[section name] labels that are inserted into the end of
every section.


Good things to know about WLA
*****************************

* Is 511 (Amiga, MSDOS) or 2047 (other platforms) bytes too little for
  a string (file names, labels, definition labels, etc)? Check out
  "MAX_NAME_LENGTH" in "shared.h".

* Want to have more operators and operands in a calculation than 64
  (Amiga, MSDOS) or 256 (other platforms)? Check out
  "MAX_STACK_CALCULATOR_ITEMS" in "defines.h".

* WLA preprocessor doesn’t expand macros and repetitions. Those are
  actually traversed in the assembling phase.

* WLA’s source code is mainly a huge mess, but WLALINK is quite well
  structured and written. So beware!

* To get the length of a string e.g. “peasoup”, write
  “peasoup”.length.

* Do not write ".E" into your sources as WLA uses it internally to
  mark the end of a file.


WLA DX’s architectural overview
*******************************

The two most important executables inside WLA DX are WLA (the
assembler) and WLALINK (the linker).


WLA
===

WLA has four separate phases:

1. "phase_1.c": "phase_1()":

   * The biggest data processor in WLA.

   * Includes the include files: every time this happens the file is
     read in, white space is removed, lines formatted, etc.

   * Macros are processed along with directives

   * All textual data, code, etc. are transformed into WLA’s internal
     byte code that gets written into a tmp (TMP) file, and after this
     phase the assembler or the linker has no idea of target CPU’s
     opcodes - all is just pure WLA byte code.

   * The first and the only pass that handles the assembly source
     files supplied by the user.

   * The parser in this pass starts from the first byte of the first
     source file, then moves forward parsing everything that it
     encounters, but when a macro is called, the parser jumps to the
     beginning of the macro, and continues parsing from there.

2. "phase_2.c": "phase_2()":

   * If the user has issued directives like ".SDSCTAG", here we
     generate the needed data and write that into TMP.

3. "phase_3.c": "phase_3()":

   * Here we read in TMP and do some sanity checks for the data, give
     labels addresses (if possible), generate internal structures for
     labels and sections.

4. "phase_4.c": "phase_4()":

   * Again we read in TMP.

   * Now we check that if there is a reference to a calculation, and
     that calculation has been succesfully calculated, then we can
     replace the reference with the result.

   * This phase writes out object and library files, i.e., transforms
     TMP to final output files (this write out could actually be
     "pass_5")…


WLALINK
=======

WLALINK is much simpler and more straight forward than WLA; WLALINK
just reads in all the objects and library files, places the sections
along with labels into the target memory map, solves pending
calculations, calculates checksums, and writes out the final ROM/PRG
files. "wlalink/main.c:main()" should quite clearly display all the
higher level phases in the linking process.


WLA Symbols
***********

Symbols can be optionally generated as a part of the assembly and link
steps. With a compatible emulator, this can provide extra information
for debugging a ROM, or otherwise help in understanding how it
operates.

The symbols file can be generated by wlalink by adding “-S” onto the
command line. This will output labels, definitions, and some other
rudimentary data. Most prominently, this can be used to understand
where the ROM output various sections such as subroutines and data,
and be able to look that up in the emulator’s ROM or RAM space.

Extra information for address-to-line mapping can be provided by
adding the following command line arguments: - Run object generation
(e.g. “wla-65816”) with “-i” to include list data in the output obj
files - Run wlalink with “-S -A” to generate symbols with information
related to address-to-line mapping

Address-to-line mappings includes information to relate lines in the
source files to individual instructions in the generated ROM. This can
be used to provide richer disassembly in the emulator, or allow for
rich debugging in an external IDE.


WLA Symbol Version History
==========================

If you are maintaining a WLA symbol file parser, please review this
page when new versions of WLA DX are released, as the format might
have changed.

Version 1: https://github.com/vhelin/wla-dx/blob/v9.12/doc/symbols.rst

* Base version, including sections [labels], [definitions],
  [breakpoints], [symbols], [source files], [rom checksum], [addr-to-
  line mapping]

Version 2: https://github.com/vhelin/wla-dx/blob/v10.5/doc/symbols.rst

* Added [information] section

* Deprecated [source files] section, and replaced with [source files
  v2]

* Deprecated [addr-to-line mapping] section definition, and replaced
  with [addr-to-line mapping v2]

Version 3: https://github.com/vhelin/wla-
dx/blob/master/doc/symbols.rst

* Added [sections] and [ramsections] sections

* Added “wlasymbol true” under [information] section


Information For Emulator Developers
===================================

In order to properly support loading of WLA symbol files, it is
recommended to follow this specification below, especially so as to
gracefully support future additions to the symbol files.

* The file should be read one line at a time

* Any text on a line following a ";" should be ignored

* Lines matching "\[\S+\]" in regex or "[%s]" in scanf code are
  section headers, and represent a new section. Note that no section
  data will start with "[".

* Lines following the section header are the data for that section. If
  you’re acknowledging the section, utilize that section’s specific
  formatting. Read lines that match until a new section header is
  encountered.

* Unless otherwise specified, none of the data in any section should
  be assumed to be sorted in any particular way.

The following are the list of currently supported sections, what they
mean, and how their data should be interpreted.


[information]
-------------

The only fields this section has currently are “version” (and then the
version number) and “wlasymbol” (which is followed by “true”).
[information], if present, must always occur before any other section
or data, and its first line will always be the format version.


[labels]
--------

This is a list of all labels to sections of the ROM, such as
subroutine locations, or data locations. Each line lists an address in
hexadecimal (bank and offset) and a string associated with that
address. This data could be used, for example, to identify what
section a given target address is in, by searching for the label with
the closest address less than the target address.

* Regex match: "[0-9a-fA-F]{2}:[0-9a-fA-F]{4} .*"

* Format specifier: "%2x:%4x %s"


[definitions]
-------------

This is a list of various definitions provided in code - or
automatically during WLA’s processing - and values associated with
them. Most prominently, WLA outputs the size of each section of the
ROM. Each line lists an integer value in hexadecimal, and a string
(name) associated with that value.

* Regex match: "[0-9a-fA-F]{8} .*"

* Format specifier: "%8x %s"


[breakpoints]
-------------

This is a list of hexadecimal ROM addresses where the ".BREAKPOINT"
directive was used in the source assembly. Each line lists an address
in hexadecimal (bank and offset).

* Regex match: "[0-9a-fA-F]{2}:[0-9a-fA-F]{4}"

* Format specificer: "%2x:%4x"


[symbols]
---------

This is a list of hexadecimal ROM addresses where the ".SYMBOL"
directive was used in the source assembly. Each line lists an address
in hexadecimal (bank and offset) and a string associated with that
address.

* Regex match: "[0-9a-fA-F]{2}:[0-9a-fA-F]{4} .*"

* Format specifier: "%2x:%4x %s"


[source files v2]
-----------------

These are used to identify what files were used during the assembly
process, especially to map generated assembly back to source file
contents. Each line lists a hexadecimal object file index, a
hexadecimal source file index, a hexadecimal CRC32 checksum of the
file, and a file path relative to the generated ROM’s root. This could
be used to load in the contents of one of the input files when running
the ROM and verifying the file is up-to-date by checking its CRC32
checksum against the one generated during assembly.

* Regex match: "[0-9a-fA-F]{4}:[0-9a-fA-F]{4} [0-9a-fA-F]{8} .*"

* Format specifier: "%4x:%4x %8x %s"


[rom checksum]
--------------

This is just a single line identifying what the hexadecimal CRC32
checksum of the ROM file was when the symbol file was generated. This
could be used to verify that the symbol file itself is up-to-date with
the ROM in question. This checksum is calculated by reading the ROM
file’s entire binary, and not by reading any platform-specific
checksum value embedded in the ROM itself.

* Regex match:  "[0-9a-fA-F]{8}"

* Format specifier: "%8x"


[addr-to-line mapping v2]
-------------------------

This is a listing of hexadecimal ROM address, bank, ROM bank offset,
memory address, each mapped to a hexadecimal object file index, a
source file index and hexadecimal line index. The file indices refer
back to the file indices specified in the "source files" section, so
that the source file name can be discovered. This information can be
used to, for example, display source file information in line with
disassembled code, or to communicate with an external text editor the
location of the current Program Counter by specifying a source file
and line instead of some address in the binary ROM file.

* Regex match: "[0-9a-fA-F]{8} [0-9a-fA-F]{2}:[0-9a-fA-F]{4} [0-9a-
  fA-F]{4} [0-9a-fA-F]{4}:[0-9a-fA-F]{4}:[0-9a-fA-F]{8}"

* Format specifier: "%8x %2x:%4x %4x %4x:%4x:%8x"


[sections]
----------

Each line specifies a ".SECTION": hexadecimal ROM address, bank, ROM
bank offset, memory address, size and name. Use this information for
example to locate ".SECTION" data in the output.

* Regex match: "[0-9a-fA-F]{8} [0-9a-fA-F]{2}:[0-9a-fA-F]{4} [0-9a-
  fA-F]{4} [0-9a-fA-F]{8} .*"

* Format specifier: "%.8x %.2x:%.4x %.4x %.8x %s"


[ramsections]
-------------

Each line specifies a ".RAMSECTION": hexadecimal bank, RAM bank
offset, memory address, size and name. Use this information for
example to see where a ".RAMSECTION" was placed.

* Regex match: "[0-9a-fA-F]{2}:[0-9a-fA-F]{4} [0-9a-fA-F]{4} [0-9a-
  fA-F]{8} .*"

* Format specifier: "%.2x:%.4x %.4x %.8x %s"


Legal Note
**********

WLA DX (the whole package) was originally written by Ville Helin in
1998-2008. After that everybody has been able to take part in the
development of WLA DX, and recently via GitHub. The authors are not
responsible for anything the software does.

WLA DX is GPL-2.0-or-later software. For more information about
GPL-2.0-or-later, take a look at the "LICENCE" file, or visit
https://spdx.org/licenses/GPL-2.0-or-later.html

Game Boy and Game Boy Color are copyrighted by Nintendo.

Pocket Voice is copyrighted by Bung HK.


Manpage: WLA-LINK
*****************


SYNOPSIS
========

   "wlalink" ["OPTIONS"] "LINK_FILE" "OUTPUT_FILE"


OPTIONS
=======

-b

Program file output

-d

Discard unreferenced sections

-i

Write list files (Note: WLA needs "-i" as wel)

-r

ROM file output (default)

-s

Write also a NO$GMB/NO$SNES symbol file

-S

Write also a WLA symbol file

-A

Add address-to-line mapping data to WLA symbol file

-v

Verbose messages

-L LIBDIR

Look in LIBDIR for libaries before looking in CWD

-t TYPE

Output type (supported types: ‘CBMPRG’)

-a ADDR

Load address for CBM PRG

Choose one:

-b OUT

Program file linking

-r OUT

ROM image linking


DESCRIPTION
===========

*wlalink(1)* is a part of WLA-DX. It links one or more object files
(and perhaps some library files) together to produce a ROM image /
program file.

LINK_FILE is a text file that contains information about the files you
want to link together. Here’s the format:

1. You must define the group for the files. Put the name of the group
   inside brackets. Valid group definitions are

      [objects]
      [libraries]
      [header]
      [footer]
      [definitions]

2. Start to list the file names.

      [objects]
      main.o
      vbi.o
      level_01.o
      ...

3. Give parameters to the library files:

      [libraries]
      bank 0 slot 1 speed.lib
      bank 4 slot 2 map_data.lib
      ...

   Here you can also use "base" to define the 65816 CPU bank number
   (like ".BASE" works in WLA):

      [libraries]
      bank 0 slot 1 base $80 speed.lib
      bank 4 slot 2 base $80 map_data.lib
      ...

   You must tell WLALINK the bank and the slot for the library files.

4. If you want to use header and/or footer in your project, you can
   type the following:

      [header]
      header.dat
      [footer]
      footer.dat

5. If you want to make value definitions, here’s your chance:

      [definitions]
      debug 1
      max_str_len 128
      start $150
      ...

If flag "-i" is given, *wlalink(1)* will write list files. Note that
you must compile the object and library files with "-i" flag as well.
Otherwise *wlalink(1)* has no extra information it needs to build list
files.

Here is an example of a list file: Let’s assume you’ve compiled a
source file called "main.s" using the "-i" flag. After you’ve linked
the result also with the "-i" flag *wlalink(1)* has created a list
file called "main.lst". This file contains the source text and the
result data the source compiled into. List files are good for
debugging.

Make sure you don’t create duplicate labels in different places in the
memory map as they break the linking loop. Duplicate labels are
allowed when they overlap each other in the destination machine’s
memory.


EXAMPLES
========

   wlalink -r linkfile testa.sfc
   wlalink -d -i -b linkfile testb.sfc
   wlalink -v -S -L ../../lib linkfile testc.sfc


Manpage: WLA-CPU
****************


SYNOPSIS
========

   "wla-6502" ["OPTIONS"] "SRC_FILE"
   "wla-65816" ["OPTIONS"] "SRC_FILE"
   "wla-65c02" ["OPTIONS"] "SRC_FILE"
   "wla-65ce02" ["OPTIONS"] "SRC_FILE"
   "wla-6800" ["OPTIONS"] "SRC_FILE"
   "wla-6801" ["OPTIONS"] "SRC_FILE"
   "wla-6809" ["OPTIONS"] "SRC_FILE"
   "wla-8008" ["OPTIONS"] "SRC_FILE"
   "wla-8080" ["OPTIONS"] "SRC_FILE"
   "wla-gb" ["OPTIONS"] "SRC_FILE"
   "wla-huc6280" ["OPTIONS"] "SRC_FILE"
   "wla-spc700" ["OPTIONS"] "SRC_FILE"
   "wla-superfx" ["OPTIONS"] "SRC_FILE"
   "wla-z80" ["OPTIONS"] "SRC_FILE"


OPTIONS
=======

-h

Assume all label references are 16-bit by default (size hints still
work)

-i

Add list file information

-k

Keep empty sections

-M

Output makefile rules

-q

Quiet mode (".PRINT*"-directives output nothing)

-s

Don’t create _sizeof_* definitions

-t

Test compile (Don’t output any files)

-v

Verbose messages

-x

Extra compile time labels and definitions

-I DIR

Add include directory

-D DEF

Declare definition

Choose one:

-o OUT

Output an object file

-l OUT

Output an library file


DESCRIPTION
===========

Assemble a BIN_FILE to an object file ("-o") or to an library file
("-l").

These object files can be linked together (or with library files)
later with *wlalink(1)*.

Name object files so that they can be recognized as object files.
Normal suffix is ".o" (WLA default). This can also be changed with
".OUTNAME".

Name these files so that they can be recognized as library files.
Normal suffix is ".lib" (WLA default).

With object files you can reduce the amount of compiling when editing
small parts of the program. Note also the possibility of using local
labels (starting with "_").

With library files you can reduce the amount of compiling. Library
files are meant to hold general functions that can be used in
different projects. Note also the possibility of using local labels
(starting with "_"). Library files consist only of "FREE" sections.

**Note**: When you compile objects, group 1 directives are saved for
linking time, when they are all compared and if they differ, an error
message is shown. It is advisable to use something like an include
file to hold all the group 1 directives for that particular project
and include it to every object file.

If you are interested in the WLA object file format, take a look at
the file "txt/wla_file_formats.txt" which is included in the release
archive.


EXAMPLES
========

   wla-gb -D DEBUG -D VERBOSE=5 -D NAME = "math v1.0" -o math.o math.s

* "-D IEXIST"

* "-D DAY=10"

* "-D BASE = $10"

* "-D NAME=elvis"


Manpage: WLAB
*************


SYNOPSIS
========

   "wlab" -["ap"]{"bdh"} "BIN_FILE"


OPTIONS
=======

-a

Print the address (relative to the beginning of the data).

-p

Don’t print file header.

Choose one:

-b

Output data in binary format.

-d

Output data in decimal format.

-h

Output data in hexadecimal format.


DESCRIPTION
===========

*wlab(1)* converts binary files to WLA’s byte definition strings and
print it to the standard output.


EXAMPLES
========

   wlab -da gayskeletor.bin > gayskeletor.s
   wlab -bap iscandar.bin > iscandar.s
   wlab -h starsha.bin > starsha.s
