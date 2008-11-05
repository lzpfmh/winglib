
#-------------------------------------------------------------------
# Vars
#-------------------------------------------------------------------

ifndef FILE_EXE

ifeq ($(TYPE_PRJ),lib)
FILE_EXE := lib$(NAME_PRJ)$(DPOSTFIX).a
else
FILE_EXE := $(NAME_PRJ)$(DPOSTFIX)
endif
endif

ifndef LOC_CXX_$(LOC_TAG)
	LOC_CXX_$(LOC_TAG) := cpp
endif
ifndef LOC_H_$(LOC_TAG)
	LOC_H_$(LOC_TAG) := h
endif

ifeq ($(LOC_CXX_$(LOC_TAG)),c)
COMPILER := $(CC)
else
COMPILER := $(PP)
endif

ifdef LOC_SRC_$(LOC_TAG)
PATH_SRC_$(LOC_TAG) := $(CUR_ROOT)/$(LOC_SRC_$(LOC_TAG))
else
PATH_SRC_$(LOC_TAG) := $(CUR_ROOT)
endif

ifdef LOC_INC_$(LOC_TAG)
PATH_INC_$(LOC_TAG) := $(CUR_ROOT)/$(LOC_INC_$(LOC_TAG))
else
PATH_INC_$(LOC_TAG) := $(CUR_ROOT)
endif

ifdef LOC_OUT_$(LOC_TAG)
PATH_BIN_$(LOC_TAG) := $(BINROOT)/_$(NAME_PRJ)/$(LOC_OUT_$(LOC_TAG))
else
PATH_BIN_$(LOC_TAG) := $(BINROOT)/_$(NAME_PRJ)
endif

PATH_OBJ_$(LOC_TAG) := $(PATH_BIN_$(LOC_TAG))/obj
PATH_EXE_$(LOC_TAG) := $(BINROOT)/$(FILE_EXE)
PATH_INS_$(LOC_TAG) := /usr/share/$(NAME_PRJ)
PATH_LNK_$(LOC_TAG) := /usr/bin


#-------------------------------------------------------------------
# Sources
#-------------------------------------------------------------------
HEADERS_$(LOC_TAG) 	:= $(wildcard $(PATH_INC_$(LOC_TAG))/*.$(LOC_H_$(LOC_TAG)))
SOURCES_$(LOC_TAG) 	:= $(wildcard $(PATH_SRC_$(LOC_TAG))/*.$(LOC_CXX_$(LOC_TAG)))
OBJECTS_$(LOC_TAG) 	:= $(subst $(PATH_SRC_$(LOC_TAG))/,$(PATH_OBJ_$(LOC_TAG))/, $(SOURCES_$(LOC_TAG):.$(LOC_CXX_$(LOC_TAG))=.o) )

include $(wildcard $(PATH_OBJ_$(LOC_TAG))/*.d)

#-------------------------------------------------------------------
# Options
#-------------------------------------------------------------------

all_$(LOC_TAG): setup_$(LOC_TAG)

rebuild_$(LOC_TAG): clean_$(LOC_TAG) all_$(LOC_TAG)

setup_$(LOC_TAG): $(PATH_OBJ_$(LOC_TAG))

$(PATH_OBJ_$(LOC_TAG)):
	mkdir -p $@

clean_$(LOC_TAG):
	@rm -rf $(PATH_OUT_$(LOC_TAG))
	
#-------------------------------------------------------------------
# Build
#-------------------------------------------------------------------

$(PATH_OBJ_$(LOC_TAG))/%.o : $(PATH_SRC_$(LOC_TAG))/%.$(LOC_CXX_$(LOC_TAG))
	$(COMPILER) $(CFLAGS) $< -o $@
#	- $(COMPILER) $(SFLAGS) $< -o $(subst .o,.asm, $@)


