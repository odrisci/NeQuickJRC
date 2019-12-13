#########################################
## NeQuickG_JRC test makefile
##
## Author: Angela Aragon-Angel
## Copyright: Joint Research Centre (JRC), 2019
## This software has been released as free and open source software
## under the terms of the European Union Public Licence (EUPL), version 1.
## Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
#########################################

THIS_CLANG_DIRECTORY := $(dir $(realpath $(firstword $(MAKEFILE_LIST))))

#compiler
CC = clang
CFLAGS = -Weverything -Wno-padded \
  -Wno-documentation -Wno-double-promotion -Wno-format-nonliteral
  
ifeq ($(FTR_MODIP_CCIR_AS_CONSTANTS),1)
#because the trick to support this is done using the
#preprocessor we have to disable this one. 
  CFLAGS += -Wno-disabled-macro-expansion -Wno-strict-prototypes
#complains with __llvm_internal_gcov_emit_arcs_args.0, must be a bug
  export ASAN_OPTIONS=detect_odr_violation=0
endif

#linker
LINK = clang
LFLAGS =

ifneq (, $(filter debug, $(MAKECMDGOALS)))
  REQUIRED_TOOLS += llvm-cov
  CODE_COVERAGE_OPTION_TOOLCHAIN_TOOL = --gcov-tool $(THIS_CLANG_DIRECTORY)/llvm-gcov.sh
endif

include ./../makefile.mak
