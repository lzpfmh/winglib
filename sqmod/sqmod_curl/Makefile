
default_target: all

#-------------------------------------------------------------------
# Project
#-------------------------------------------------------------------
PRJ_NAME := sqmod_curl
PRJ_TYPE := dll
PRJ_INCS := winglib/lib/oexlib winglib/lib/sqbind SqPlus/include SqPlus/sqplus curl/include
PRJ_LIBS := sqbind oexlib sqplus sqstdlib squirrel curl cximage jpeg png tiff zlib
PRJ_DEFS := HAVE_CONFIG_H CURL_STATICLIB CURL_DISABLE_LDAP
 
PRJ_EXPORTS := SRV_GetModuleInfo SRV_Start SRV_Stop SQBIND_Export_Symbols

PRJ_SUBROOT := _sqmod

PRJ_LIBROOT := ../..

#-------------------------------------------------------------------
# Configure build
#-------------------------------------------------------------------
include $(PRJ_LIBROOT)/config.mk

ifeq ($(BUILD_CURL),)
	UNSUPPORTED := Set BUILD_CURL=1 to build this project
	include $(PRJ_LIBROOT)/unsupported.mk
else

ifeq ($(PLATFORM),posix)
	PRJ_OSLB := ssl crypto idn ldap
else
	ifeq ($(BUILD),vs)
		PRJ_INCS := $(PRJ_INCS) winglib/dep/etc/vs/inc/c99
	endif
endif

#-------------------------------------------------------------------
# Source files
#-------------------------------------------------------------------

export LOC_TAG := def
include $(PRJ_LIBROOT)/build.mk

#-------------------------------------------------------------------
# Execute the build
#-------------------------------------------------------------------
include $(PRJ_LIBROOT)/go.mk

endif
