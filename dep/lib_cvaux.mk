
# cvaux

default_target: all
OFFSET_ROOT := ..
include $(OFFSET_ROOT)/config.mk

CFLAGS := $(CFLAGS) -I$(LIBROOT)/opencv/cxcore/include
CFLAGS := $(CFLAGS) -I$(LIBROOT)/opencv/cv/include
CFLAGS := $(CFLAGS) -I$(LIBROOT)/opencv/cv/src
CFLAGS := $(CFLAGS) -I$(LIBROOT)/opencv/cvaux/include

NAME_PRJ := cvaux
TYPE_PRJ := lib

export LOC_TAG := def
LOC_INC_def := $(LIBROOT)/opencv/cvaux/include
LOC_SRC_def := $(LIBROOT)/opencv/cvaux/src
include $(OFFSET_ROOT)/build.mk

all: all_def $(PATH_EXE_def) 
rebuild: rebuild_def $(PATH_EXE_def)
rebuild: clean_def

$(PATH_EXE_def): $(OBJECTS_def) $(DEPENDS)
	$(AR) $(AFLAGS) $@ $(OBJECTS_def)


