#########################################
## NeQuickG_JRC test makefile
##
## Author: Angela Aragon-Angel
## Copyright: Joint Research Centre (JRC), 2019
## This software has been released as free and open source software
## under the terms of the European Union Public Licence (EUPL), version 1.
## Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
#########################################
LINT_TOOL_DIR = ./../../../../Tools/PC-lint\ 9.0/
LINT_TOOL = lint-nt.exe
LINT_INCLUDE_DIR = $(LINT_TOOL_DIR)

LINT_CPPFLAGS += $(foreach d, $(H_SEARCH_PATH), -i$d)
LINT_CPPFLAGS += $(foreach d, $(C_SEARCH_PATH), -i$d)

ifneq (, $(IS_DEBUG_TARGET_IF_NON_EMPTY))
LINT_CFLAGS += -DFTR_UNIT_TEST
endif

ifeq ($(FTR_MODIP_CCIR_AS_CONSTANTS),1)
LINT_CFLAGS += -DFTR_MODIP_CCIR_AS_CONSTANTS
endif

.PHONY: lint
lint: $(NEQUICK_TOOL)
	@$(LINT_TOOL_DIR)$(LINT_TOOL) -i$(LINT_TOOL_DIR)/lnt $(LINT_TOOL_DIR)/lnt/co-gcc.lnt $(LINT_CPPFLAGS) $(LINT_CFLAGS) -e451 -e537 -e793 -esym\(129,_Noreturn\) $(SOURCEFILES)
