CC 				= cc
POLYFILL_FLAGS	= -DNO_CTYPE_C -DNO_LIBGEN_H -DNO_STRING_H -DNO_SYS_PARAM_H -DNO_SYS_STAT_H
C89_FLAGS 		= -std=c89 -pedantic -D_POSIX_C_SOURCE=1
C99_FLAGS 		= -std=c99 -D_POSIX_C_SOURCE=201109L

INCLUDES 		= 
MACOS_FLAGS 	= -D_DARWIN_C_SOURCE

C80_SRCS = c80.c error.c file.c parser.c symtab.c token.c

POLYFILL_HDRS = 			\
	polyfill/assert.h		\
	polyfill/ctype.h		\
	polyfill/libgen.h		\
	polyfill/limits.h		\
	polyfill/stdio.h		\
	polyfill/stdlib.h		\
	polyfill/string.h		\
	polyfill/sys/param.h	\
	polyfill/sys/stat.h		\
	$(NONE)

c89: $(C80_SRCS)
	$(CC) $(C89_FLAGS) $(POLYFILL_FLAGS) $(CPPFLAGS) $(INCLUDES) $(CFLAGS) -o c80 $^

macos-c99: $(C80_SRCS)
	$(CC) $(C99_FLAGS) $(CPPFLAGS) $(MACOS_FLAGS) -DNDEBUG $(INCLUDES) $(CFLAGS) -o c80 $^

$(C80_SRCS): c80.h $(POLYFILL_HDRS)

