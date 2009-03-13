
SHELL = /bin/sh

# config.mk
# Cross compiler config

OS		 := linux
#OS		 := win32

PROC	 := i386
#PROC	 := arm

TOOLS	 := local
#TOOLS	 := debian
#TOOLS	 := codesourcery
#TOOLS	 := snapgear
#TOOLS	 := mingw32


ifeq ($(OS),win32)
	LIBLINK	 := static
else
	LIBLINK := shared
endif

#CFG_CURDIR := `pwd`
CFG_ROOT := $(PRJ_LIBROOT)/../..
CFG_TOOLROOT := $(PRJ_LIBROOT)/../../tools
CFG_LIBROOT  := $(PRJ_LIBROOT)/..

ifdef UNICODE
CFG_CEXTRA := $(CFG_CEXTRA) -DUNICODE -D_UNICODE
endif

ifeq ($(OS),win32)

	PLATFORM := windows

	ifdef DBG
		ifeq ($(LIBLINK),static)	
			CFG_CEXTRA	 := /DDEBUG /D_DEBUG /D_MT /MTd /Zi $(CFG_CEXTRA)
		else
			ifeq ($(PRJ_TYPE),dll)
				CFG_CEXTRA	 := /DDEBUG /D_DEBUG /D_MT /MDd /Zi $(CFG_CEXTRA)
			else
				CFG_CEXTRA	 := /DDEBUG /D_DEBUG /D_MT /MDd /Zi $(CFG_CEXTRA)
			endif
		endif
		ifeq ($(LIBLINK),static)	
			CFG_LEXTRA	 := /DEBUG
		else
			CFG_LEXTRA	 := /DEBUG /NODEFAULTLIB:libcmtd
		endif
		CFG_DPOSTFIX := _d
		CFG_STDLIBS	 := ole32.lib user32.lib
	else
		ifeq ($(LIBLINK),static)	
			CFG_CEXTRA	 := /MT /O2 $(CFG_CEXTRA)
		else
			CFG_CEXTRA	 := /MD /O2 $(CFG_CEXTRA)
		endif
		CFG_LEXTRA	 := 
		CFG_STDLIBS	 := ole32.lib user32.lib
	endif


	# Tools
	CFG_PP := cl /nologo /DWIN32 /wd4996
	CFG_LD := link /NOLOGO
	CFG_CC := cl /nologo /DWIN32 /wd4996
	CFG_AR := lib /nologo
	CFG_DP := makedepend
	CFG_MD := md
	CFG_RM := rmdir /s /q
	CFG_DEL:= del /f /q

	CFG_CC_OUT := /Fo
	CFG_LD_OUT := /OUT:
	CFG_AR_OUT := /OUT:
	CFG_CC_INC := /I
	
	CFG_FLAG_EXPORT := /EXPORT:

	CFG_CFLAGS := /EHsc /c $(CFG_CEXTRA)
	CFG_LFLAGS := $(CFG_LEXTRA)
	
	ifeq ($(PRJ_TYPE),dll)
		CFG_LFLAGS := $(CFG_LFLAGS) /DLL
	endif

	CFG_OBJ_EXT := obj
	CFG_DEP_EXT := d
	CFG_CUR_ROOT := $(shell cd)

	CFG_LIB_PRE	 :=
	CFG_LIB_POST := .lib
	CFG_EXE_POST := .exe
	CFG_DLL_POST := .dll
	
	ifdef PRJ_DEFS
		CFG_DEFS := $(foreach def,$(PRJ_DEFS),/D$(def) )
	endif	
	
else

	# --with-sysroot
	# --with-headers

	PLATFORM := posix

	ifdef DBG
		CFG_CEXTRA	 := -g -DDEBUG -D_DEBUG $(CFG_CEXTRA) 
		CFG_LEXTRA	 := -g
		CFG_DPOSTFIX := _d
	else
		CFG_CEXTRA	 := -O2 -s $(CFG_CEXTRA) 
		ifneq ($(PRJ_TYPE),dll)
			CFG_LEXTRA	 := -s
		endif
	endif
	
	ifeq ($(PRJ_TYPE),dll)
		CFG_LEXTRA := $(CFG_LEXTRA) -shared
	else	
		ifeq ($(LIBLINK),static)
			CFG_LEXTRA := $(CFG_LEXTRA) -static
		endif
	endif
	
	# Arm compiler
	ifeq ($(PROC),arm)

		ifeq ($(TOOLS),snapgear)
		
			# Snapgear
			CFG_TOOLPREFIX := $(CFG_TOOLROOT)/$(TOOLS)/usr/local/bin/arm-linux-

			CFG_STDLIB := -lrt -pthread
			CFG_LFLAGS := $(CFG_LEXTRA)
			CFG_CFLAGS := $(CFG_CEXTRA) -c -MMD -DOEX_ARM -DOEX_LOWRAM -DOEX_NOSTRUCTINIT -DOEX_PACKBROKEN -DOEX_NOSHM
			CFG_SFLAGS := $(CFG_CFLAGS) -S -MMD
			CFG_AFLAGS := cq

		endif
		ifeq ($(TOOLS),codesourcery)

			# Google Android
			CFG_TOOLPREFIX := $(CFG_TOOLROOT)/$(TOOLS)/bin/arm-none-linux-gnueabi-

			CFG_STDLIB := -lrt -pthread
			CFG_LFLAGS := $(CFG_LEXTRA)
			CFG_CFLAGS := $(CFG_CEXTRA) -c -MMD -DOEX_ARM -DOEX_LOWRAM -DOEX_NOSHM
			CFG_SFLAGS := $(CFG_CFLAGS) -S -MMD
			CFG_AFLAGS := cq

		endif
		ifeq ($(TOOLS),nihilism)

			# nihilism
			CFG_TOOLPREFIX := $(CFG_TOOLROOT)/$(TOOLS)/bin/arm-unknown-linux-gnu-

			CFG_STDLIB := -lrt -pthread
			CFG_LFLAGS := $(CFG_LEXTRA)
			CFG_CFLAGS := $(CFG_CEXTRA) -c -MMD -DOEX_ARM -DOEX_LOWRAM -DOEX_NOSHM -DOEX_NOSTRUCTINIT -DOEX_PACKBROKEN -DOEX_NOVIDEO
			CFG_SFLAGS := $(CFG_CFLAGS) -S -MMD
			CFG_AFLAGS := cq

		endif
		ifeq ($(TOOLS),uclinux)

			# uclinux
			CFG_TOOLPREFIX := $(CFG_TOOLROOT)/$(TOOLS)/bin/arm-linux-

			CFG_STDLIB := -lrt -pthread
			CFG_LFLAGS := $(CFG_LEXTRA)
			CFG_CFLAGS := $(CFG_CEXTRA) -c -MMD -DOEX_ARM -DOEX_LOWRAM -DOEX_NOSHM -DOEX_NOSTRUCTINIT -DOEX_PACKBROKEN -DOEX_NOVIDEO
			CFG_SFLAGS := $(CFG_CFLAGS) -S -MMD
			CFG_AFLAGS := cq
			
		endif
		ifeq ($(TOOLS),openmoko)

			# openmoko
			CFG_TOOLPREFIX := $(CFG_TOOLROOT)/$(TOOLS)/arm/bin/arm-angstrom-linux-gnueabi-

			CFG_STDLIB := -lrt -pthread
			CFG_LFLAGS := $(CFG_LEXTRA)
			CFG_CFLAGS := $(CFG_CEXTRA) -c -MMD -DOEX_ARM -DOEX_LOWRAM -DOEX_NOSHM -DOEX_NOSTRUCTINIT -DOEX_PACKBROKEN -DOEX_NOVIDEO
			CFG_SFLAGS := $(CFG_CFLAGS) -S -MMD
			CFG_AFLAGS := cq

		else
	
			# Custom tools
			CFG_TOOLPREFIX := $(CFG_TOOLROOT)/$(TOOLS)/bin/$(TOOLS)-
			override TOOLS := custom

			CFG_STDLIB := -lrt -pthread
			CFG_LFLAGS := $(CFG_LEXTRA)
			CFG_CFLAGS := $(CFG_CEXTRA) -c -fexceptions -MMD -DOEX_ARM -DOEX_LOWRAM -DOEX_NOSTRUCTINIT -DOEX_PACKBROKEN -DOEX_NOSHM
			CFG_SFLAGS := $(CFG_CFLAGS) -S -MMD
			CFG_AFLAGS := cq
			
		endif

	else
	
		ifeq ($(TOOLS),mingw32)

			# Cross compile for windows
			CFG_TOOLPREFIX := i586-mingw32msvc- 

			CFG_STDLIB := -lrt -pthread
			CFG_LFLAGS := $(CFG_LEXTRA)
			CFG_CFLAGS := $(CFG_CEXTRA) -c -MMD
			CFG_SFLAGS := $(CFG_CFLAGS) -S -MMD
			CFG_AFLAGS := cq

		else
		
			# Local platform
			CFG_TOOLPREFIX := 

			# -lregex -lpng -ljpeg -lzlib -ltiff -lstdc++ -lgcc -lodbc32 -lwsock32 -lwinspool -lwinmm -lshell32 -lcomctl32 -lctl3d32 -lodbc32 -ladvapi32 -lodbc32 -lwsock32 -lopengl32 -lglu32 -lole32 -loleaut32
			
			CFG_STDLIB := -lrt -pthread
			CFG_LFLAGS := $(CFG_LEXTRA) -rdynamic -Wl,-E -Wl,--export-dynamic
			CFG_CFLAGS := $(CFG_CEXTRA) -c -MMD
			CFG_SFLAGS := $(CFG_CFLAGS) -S -MMD
			CFG_AFLAGS := cq

		endif
		
		# you can't use dlopen() [-ldl] with static linking!
		# http://www.qnx.com/developers/docs/6.3.2/neutrino/lib_ref/d/dlopen.html
		ifeq ($(LIBLINK),shared)
			CFG_STDLIB := $(CFG_STDLIB) -ldl
		endif		
		
	endif
	
#	ifeq ($(PRJ_TYPE),dll)
#		CFG_LD := $(CFG_TOOLPREFIX)ld -E --export-dynamic
#	else
#		ifeq ($(PRJ_TYPE),lib)
#			CFG_LD := $(CFG_TOOLPREFIX)ld -E --export-dynamic
#		else
#			CFG_LD := $(CFG_TOOLPREFIX)g++ -rdynamic -Wl,-E -Wl,--export-dynamic
#		endif
#	endif
	
	# Tools
	CFG_PP := $(CFG_TOOLPREFIX)g++
	CFG_LD := $(CFG_TOOLPREFIX)g++
	CFG_CC := $(CFG_TOOLPREFIX)gcc
	CFG_AR := $(CFG_TOOLPREFIX)ar
	CFG_DP := $(CFG_TOOLPREFIX)makedepend
	CFG_MD := mkdir -p
	CFG_RM := rm -rf
	CFG_DEL:= rm -f
	
	CFG_CC_OUT := -o $(nullstring)
	CFG_LD_OUT := -o $(nullstring)

	CFG_OBJ_EXT := o
	CFG_DEP_EXT := d
	CFG_CUR_ROOT := $(shell pwd)
	
	CFG_CC_INC := -I
	
	CFG_LIB_PRE	 := lib
	CFG_LIB_POST := .a
	CFG_DLL_PRE	 := lib
	CFG_DLL_POST := .so
	
	ifdef PRJ_DEFS
		CFG_DEFS := $(foreach def,$(PRJ_DEFS),-D$(def) )
	endif	

endif

CFG_BUILD_TYPE := $(PLATFORM)-$(OS)-$(PROC)-$(TOOLS)

ifdef UNICODE
CFG_BUILD_TYPE := $(CFG_BUILD_TYPE)-unicode
endif

ifdef DBG
CFG_BUILD_TYPE := $(CFG_BUILD_TYPE)-debug
endif

ifeq ($(LIBLINK),static)
CFG_BUILD_TYPE := $(CFG_BUILD_TYPE)-static
else
CFG_BUILD_TYPE := $(CFG_BUILD_TYPE)-shared
endif

CFG_BINROOT  := $(CFG_LIBROOT)/../bin/$(CFG_BUILD_TYPE)

ifdef PRJ_BINROOT
CFG_OUTROOT := $(PRJ_BINROOT)/bin/$(CFG_BUILD_TYPE)
else
CFG_OUTROOT  := $(CFG_BINROOT)
endif


