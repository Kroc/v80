On Linux, compiled with C89 and minimal library functions:
   $ cc -std=c89 -pedantic -O2 -D_POSIX_C_SOURCE=1 -DNDEBUG -I. -o v80 main.c

On macOS, compiled with C99 and supporting library functions:
   $ cc -std=c99 -O2 -D_POSIX_C_SOURCE=200809L -D_DARWIN_C_SOURCE -DNDEBUG -I. -o v80 main.c

Other architectures pending - code is C89 compliant!

Add zero or more of these defines to use custom implementations if you have a
constrained libc missing any of these associated APIs:

   -DNO_CTYPE_H         if <ctype.h> is missing isalnum, isprint and isspace
   -DNO_GETLINE         if <stdlib.h> or libc don't implement getline()
   -DNO_LIBGEN_H        if <libgen.h> or libc don't implement dirname()
   -DNO_SIZE_T          if your environment has no size_t type definition
   -DNO_STRDUP          if <string.h> or libc don't implement strdup()
   -DNO_STRING_H        if <string.h> or missing c89 str* APIs
   -DNO_STRLCAT         if <string.h> or libc don't implement strlcat()
   -DNO_STRLCPY         if <string.h> or libc don't implement strlcpy()
   -DNO_STRNDUP         if <string.h> or libc don't implement strndup()
   -DNO_STRTOUL         if <stdlib.h> or libc don't implement strtoul()
   -DNO_SYS_PARAM_H     if <sys/param.h> or missing MAXPATHLEN definition
   -DNO_SYS_STAT_H      if <sys/stat.h> or missing lstat() API

Once compiled, this version of v80 loads architecture instructions from an
additional `tbl_*.v80` file containing macro definitions.  For example to
assemble the CP/M Z80 assembler:

   $ ./v80 -i ./tbl_z80.v80 ../v1/cpm_z80.v80
   
Will cross assemble a CP/M Z80 binary at `../v1/cpm_z80.com`.

