
PRJ_RELROOT := ../..
include $(PRJ_RELROOT)/buildpath.mk

default_target: all

#-------------------------------------------------------------------
# Project
#-------------------------------------------------------------------
PRJ_NAME := sqmod_live555
PRJ_DEPS := live555
PRJ_TYPE := dll
PRJ_INCS := winglib/lib/oexlib winglib/lib/sqbind SqPlus/include SqPlus/sqplus \
			live555/liveMedia/include live555/UsageEnvironment/include \
			live555/BasicUsageEnvironment/include live555/groupsock/include
PRJ_LIBS := sqbind oexlib sqplus sqstdlib squirrel cximage jpeg png tiff zlib live555
PRJ_DEFS := 
PRJ_LIBP :=
PRJ_OSLB :=
 
PRJ_SUBROOT := _sqmod

PRJ_EXPORTS := SRV_GetModuleInfo SRV_Start SRV_Stop SQBIND_Export_Symbols

#-------------------------------------------------------------------
# Configure build
#-------------------------------------------------------------------
include $(PRJ_LIBROOT)/config.mk

#-------------------------------------------------------------------
# File locations
#-------------------------------------------------------------------
export LOC_TAG := def
include $(PRJ_LIBROOT)/build.mk

#-------------------------------------------------------------------
# Execute the build
#-------------------------------------------------------------------
include $(PRJ_LIBROOT)/go.mk

