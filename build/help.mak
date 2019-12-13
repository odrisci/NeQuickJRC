#########################################
## NeQuickG_JRC test makefile
##
## Author: Angela Aragon-Angel
## Copyright: Joint Research Centre (JRC), 2019
## This software has been released as free and open source software
## under the terms of the European Union Public Licence (EUPL), version 1.
## Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
#########################################
HELP_TARGET := help
NO_SUCH_TARGET := no-such-target
HELP_TARGETS := $(HELP_TARGET) $(NO_SUCH_TARGET)

.PHONY: $(HELP_TARGET)
# --question instructs make not to execute any command
# 2>/dev/null stderror to be ignored. Otherwise we get make[1]: *** No rule to make target 'no-such-target'. Stop.
# grep removes makefile name ($(firstword $(MAKEFILE_LIST)) in recursive makes) and no-such-target from the pipe
# awk removes from the pipe targets (target 1 target 2: ) starting with . or % (rules) and starting with - (libs)
#  also the annoying target !:
$(HELP_TARGET):
	@$(ECHO) targets:
	@$(MAKE) --print-data-base --question $(NO_SUCH_TARGET) 2>/dev/null | \
	$(GREP) -v -e '^$(NO_SUCH_TARGET)' -e '^$(firstword $(MAKEFILE_LIST))' | \
	$(AWK) '/^[^!.%-][-A-Za-z0-9_]*:/ \
	 {print substr($$1, 1, length($$1) - 1)} ' | \
	$(SORT) | \
	$(PR) --omit-pagination --width=80 --columns=1
	@$(ECHO)
	@$(ECHO) "configuration (make command line):"
	@$(ECHO) "FTR_MODIP_CCIR_AS_CONSTANTS=1 CCIR coefficients/modip grip. No external files, added as internal constants in the tool"
