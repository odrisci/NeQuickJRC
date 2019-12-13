#########################################
## NeQuickG_JRC test makefile
##
## Author: Angela Aragon-Angel
## Copyright: Joint Research Centre (JRC), 2019
## This software has been released as free and open source software
## under the terms of the European Union Public Licence (EUPL), version 1.
## Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
#########################################

# macros for tools used in this makefile and
# its dependencies
#(please respect alphabetical order)
AWK = awk
CAT = cat
DIFF = diff
ECHO = echo
GREP = grep
PR = pr
PRINTF = printf
RM = rm -f
SED = sed
SHELL = /bin/bash
SORT = sort
DOXYGEN = doxygen

# other macros
include ./../macros.mak

# this makefile needs make > 3.80
need := 3.80
ok := $(filter $(need),$(firstword $(sort $(MAKE_VERSION) \
                                           $(need))))
ifeq (, $(ok))
  $(error needs makefile > 3,80)
endif

# Target list: clean_all, clean, debug and release
#########################################
# clean target
CLEAN_TARGET := clean

# clean all targets
CLEAN_ALL_TARGET := clean_all

# the debug target
DEBUG_TARGET := debug

# the release target
RELEASE_TARGET := release

DOC := doxygen
#########################################

# the root path for the NeQuick G project
# relative to the makefile
NEQUICK_ROOT_PATH = ./../..

# the NeQuick G source (*.c) path
C_SEARCH_PATH = $(NEQUICK_ROOT_PATH)/src

# the NeQuick G header (*.c) path
H_SEARCH_PATH = $(NEQUICK_ROOT_PATH)/src

# the binary directory (where the binary will be generated)
# the path depends of the compiler selected (variable CC)
BIN_FOLDER = $(NEQUICK_ROOT_PATH)/bin/$(CC)/

# the tool prefix name
NEQUICK_G_TOOL_PREFIX = NeQuickG_JRC

# the release binary in <bin directory>/
NEQUICK_TOOL_RELEASE = $(BIN_FOLDER)$(NEQUICK_G_TOOL_PREFIX)

# the object directory for the release target
# where intermediate files generated during compilation, test are
# stored
OBJ_FOLDER_RELEASE = ./release/

# debug targets adds a D to the prefix to generate the tool name
NEQUICK_TOOL_DEBUG = $(BIN_FOLDER)$(NEQUICK_G_TOOL_PREFIX)D

# the object directory for the debug target
# where intermediate files generated during compilation and test are
# stored
OBJ_FOLDER_DEBUG = ./debug/

# is this the debug target?
# true if IS_DEBUG_TARGET_IF_NON_EMPTY is non empty: ifneq (, $(IS_DEBUG_TARGET_IF_NON_EMPTY))
# Make will set the special variable MAKECMDGOALS to the list of goals
# specified on the command line
# filter removes from MAKECMDGOALS the the targets that do not match debug
IS_DEBUG_TARGET_IF_NON_EMPTY := $(filter $(DEBUG_TARGET), $(MAKECMDGOALS))

# set binary and object directory as a function of debug/release
ifneq (, $(IS_DEBUG_TARGET_IF_NON_EMPTY))
  NEQUICK_TOOL = $(NEQUICK_TOOL_DEBUG)
  OBJ_FOLDER = $(OBJ_FOLDER_DEBUG)
else
  NEQUICK_TOOL = $(NEQUICK_TOOL_RELEASE)
  OBJ_FOLDER = $(OBJ_FOLDER_RELEASE)
endif

# type definitions for C, object, header and dependencies
.TYPE.C := c
.TYPE.OBJ := o
.TYPE.H := h
.TYPE.DEPENDENCIES := d

# include the NeQuick library
include $(C_SEARCH_PATH)/lib/makefile.mak

# compiler flags
CPPFLAGS += $(foreach d, $(H_SEARCH_PATH), -I$d)

# common flags for debug/release
# for the complete set of compilation flags,
# check the makefile for the compiler used
# -std=c11 using C standard ISO/IEC 9899:2011
# -pipe (to speed up build process)
#   Use pipes rather than temporary files for communication
#   between the various stages of compilation.
#   This fails to work on some systems where the assembler
#   is unable to read from a pipe;
#   but the GNU assembler has no trouble.
# -pedantic-errors
#    Like -pedantic, except that errors are produced rather
#    than warnings.
#    where -pedantic
#      Issue all the warnings demanded by strict ISO C;
#      reject all programs that use forbidden extensions,
#      and some other programs that do not follow ISO C.
#      For ISO C, follows the version of the ISO C standard
#      specified by any -std option used.
CFLAGS += -std=c11 -pipe -pedantic-errors

ifneq (, $(IS_DEBUG_TARGET_IF_NON_EMPTY))
# flags for debug
# -Oo Do not optimize (needed for proper debug)
# -g Produce debugging information in the operating system's native format
# -pg Generate extra code to write profile information suitable for the analysis program gprof.
# -DFTR_UNIT_TEST include unit test sources
# options used for test coverage:
# --coverage tells the compiler to generate additional information needed by gcov
#  (basically a flow graph of the program) and also includes additional code in the object files
#  for generating the extra profiling information needed by gcov.
#  These additional files are placed in the directory where the object file is located.
# -fsanitize=address (not supported for cygwin)
#  Enables AddressSanitizer, a fast memory error detector.
#   Memory access instructions are instrumented to detect out-of-bounds and use-after-free bugs.
  CFLAGS += -O0 -g -pg
  CFLAGS += --coverage
ifneq ($(OS),Windows_NT)
  CFLAGS += -fsanitize=address
endif
  CFLAGS += -DFTR_UNIT_TEST
  LFLAGS = -pg --coverage
ifneq ($(OS),Windows_NT)
  LFLAGS += -fsanitize=address
endif
else
# flags for release
# -O3 Full optimization (increases performance of the generated code and compilation time)
# -march=native tells the compiler to detect the hardware architecture and
#   to enable hardware-dependent optimizations
# -DNDEBUG do not include asserts
  CFLAGS += -O3 -march=native -DNDEBUG
endif

# build and test with modip grid and
# CCIR coefficients for the foF2 and M(3000)F2 models
# as preloaded constants in the tool
# i.e. no need to pass these files as external parameters
ifeq ($(FTR_MODIP_CCIR_AS_CONSTANTS),1)
  CFLAGS += -DFTR_MODIP_CCIR_AS_CONSTANTS
endif

# tells the compiler where the headers can be found
CFLAGS += $(CPPFLAGS) -I/usr/include/

# the programs list to be generated: NeQuickG tool
PROGS = $(NEQUICK_TOOL)

# the NeQuickG sources
# main + data streams + library + exception
# MUST NOT be included in the public sources
SOURCEFILES += \
  NeQuickG_JRC_exception \
  NeQuickG_JRC_input_data_std_stream \
  NeQuickG_JRC_input_data_stream \
  NeQuickG_JRC_main

# create objects list by adding OBJ_FOLDER and the suffix TYPE.OBJ
# to each source in the list SOURCEFILES
# result OBJS = <object_path>/source_1.obj, <object_path>/source_2.obj, ..., <object_path>/source_n.obj
OBJS = $(addprefix $(OBJ_FOLDER),$(addsuffix .$(.TYPE.OBJ),$(SOURCEFILES)))
# Dependencies: if a header is modified all its dependencies must be rebuilt
# create dependencies list by adding OBJ_FOLDER and the suffix TYPE.DEPENDENCIES
# to each source in the list SOURCEFILES
# result DEPENDENCIES = <object_path>/source_1.d, <object_path>/source_2.d, ..., <object_path>/source_n.d
DEPENDENCIES = $(addprefix $(OBJ_FOLDER),$(addsuffix .$(.TYPE.DEPENDENCIES),$(SOURCEFILES)))

#libraries: the math lib
LIBS = -lm

# tells makefile where to find sources, headers and objects
vpath %.$(.TYPE.C) $(C_SEARCH_PATH)
vpath %.$(.TYPE.H) $(H_SEARCH_PATH)
vpath %.$(.TYPE.H) /usr/include
vpath %.$(.TYPE.OBJ) $(OBJ_FOLDER)

# the default target is the release target
# must be regenerated when PROGS i.e. NEQUICK_TOOL must be regenerated
# make or make release will generate the release target
.PHONY: $(RELEASE_TARGET)
$(RELEASE_TARGET): $(PROGS)

# the debug target
# must be regenerated when PROGS i.e. NEQUICK_TOOL must be regenerated
.PHONY: $(DEBUG_TARGET)
$(DEBUG_TARGET): $(PROGS)

# the Nequick tool rule generation: invoke the linker with linker flags, libs and objects
# depends of objects
# | $(BIN_FOLDER) tells make file to create the binary folder
#  if not already created
$(NEQUICK_TOOL): $(OBJS) | $(BIN_FOLDER)
	$(LINK) $(LFLAGS) -o $@ $^ $(LIBS)

# rule to create the object folder: invoke mkdir
$(OBJ_FOLDER):
	@mkdir -p $@

# rule to create the binary folder: invoke mkdir
$(BIN_FOLDER):
	@mkdir -p $@

# the object rule generation: invoke the compiler for the source
# generates source_n.obj from source_1.c
# | $(OBJ_FOLDER) tells make file to create the object folder
#  if not already created
$(OBJ_FOLDER)%.$(.TYPE.OBJ): %.$(.TYPE.C) | $(OBJ_FOLDER)
	@$(ECHO) compiling $<
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

# clean target: delete binary, objects and dependencies
.PHONY: $(CLEAN_TARGET)
$(CLEAN_TARGET):
	@$(RM) $(PROGS) $(OBJS) $(DEPENDENCIES)

# clean all targets: delete binaries and object directories
.PHONY: $(CLEAN_ALL_TARGET)
$(CLEAN_ALL_TARGET):
	@$(RM) -r $(NEQUICK_TOOL_RELEASE) $(NEQUICK_TOOL_DEBUG) $(OBJ_FOLDER_RELEASE) $(OBJ_FOLDER_DEBUG)

.PHONY: $(DOC)
$(DOC): | $(BIN_FOLDER)
	@($(CAT) $(NEQUICK_ROOT_PATH)/doc/doxy.cfg ; \
  $(PRINTF) "OUTPUT_DIRECTORY=$(BIN_FOLDER)doxygen\nINPUT=$(C_SEARCH_PATH)\nINPUT=$(NEQUICK_ROOT_PATH)\nPROJECT_LOGO=$(NEQUICK_ROOT_PATH)/doc/EC_logo.png") | $(DOXYGEN) -

# depencies rule (generate source_n.d from source_1.c)
# see makefile manual
$(OBJ_FOLDER)%.$(.TYPE.DEPENDENCIES): %.$(.TYPE.C) | $(OBJ_FOLDER)
	@set -e; $(RM) $@; \
	$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
	$(SED) 's,\($*\)\.o[ :]*,$(OBJ_FOLDER)\1.o $@ : ,g' < $@.$$$$ > $@; \
	$(RM) $@.$$$$

# include the test makefile
include ./../check.mak
# include the help makefile
include ./../help.mak

# generate the dependencies but not for clean_all, clean, help or doc
DEPENDENCIES_EXCLUDE_TARGETS := $(CLEAN_ALL_TARGET) $(CLEAN_TARGET) $(HELP_TARGETS) $(DOC)
ifeq (, $(filter $(DEPENDENCIES_EXCLUDE_TARGETS), $(MAKECMDGOALS)))
  -include $(DEPENDENCIES)
endif
