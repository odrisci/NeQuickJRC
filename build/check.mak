#########################################
## NeQuickG_JRC test makefile
##
## Author: Angela Aragon-Angel
## Copyright: Joint Research Centre (JRC), 2019
## This software has been released as free and open source software
## under the terms of the European Union Public Licence (EUPL), version 1.
## Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
#########################################

# the modip file
MODIP_FILE = $(NEQUICK_ROOT_PATH)/modip/modip2001_wrapped.asc

# the CCIR file
CCIR_FOLDER = $(NEQUICK_ROOT_PATH)/ccir/

# the test directory: where scripts and test data can be found
TEST_FOLDER = $(NEQUICK_ROOT_PATH)/test

# the benchmark test directory
BENCHMARK_FOLDER = $(TEST_FOLDER)/benchmark/

# the ESA test directory
ESA_TEST_FOLDER = $(TEST_FOLDER)/ESA/

# test coverage file
LCOV_OUT_PUT_FILE = $(BIN_FOLDER)/$(NEQUICK_TOOL_DEBUG).info

# HTML test coverage report directory
COVERAGE_REPORT_FOLDER = $(BIN_FOLDER)/coverage_report/

# the test coverage tool
COVERAGE_TOOL = lcov

# the test coverage HTML report generator
COVERAGE_TOOL_HTML_REPORT_TOOL = genhtml

# rule to create the HTML test coverage directory: invoke mkdir
$(COVERAGE_REPORT_FOLDER):
	@mkdir -p $@

# check tools exists: the test script needs perl
REQUIRED_TOOLS += perl
# and for the debug version the coverage tools
ifneq (, $(IS_DEBUG_TARGET_IF_NON_EMPTY))
REQUIRED_TOOLS += $(COVERAGE_TOOL) $(COVERAGE_TOOL_HTML_REPORT_TOOL)
endif

# function to check that the tools for test are present
define check_check_tools
  $(foreach bin,$(1),\
  $(if $(shell command -v $(bin) 2> /dev/null),,$(error Please install `$(bin)`)))
endef

# the test target
# | $(COVERAGE_REPORT_FOLDER) tells make file to create the folder
#  if not already created
.PHONY: check
check: $(NEQUICK_TOOL) | $(COVERAGE_REPORT_FOLDER)
	$(call check_check_tools, $(REQUIRED_TOOLS))
ifneq (, $(IS_DEBUG_TARGET_IF_NON_EMPTY))
	@$(RM)r $(COVERAGE_REPORT_FOLDER)
# if debug target initiate test coverage support, discard standard output (>/dev/null)
	@$(COVERAGE_TOOL) --directory $(OBJ_FOLDER) $(CODE_COVERAGE_OPTION_TOOLCHAIN_TOOL) --zerocounters -q >/dev/null
# invoke unit test (not available for release), discard error output (2>/dev/null)
# the unit test also test bad cases, this avoid printing all the errors to the console
ifeq ($(FTR_MODIP_CCIR_AS_CONSTANTS),1)
	@./$< -u 2>/dev/null
else
	@./$< $(MODIP_FILE) $(CCIR_FOLDER) -u 2>/dev/null
endif
endif
# run the benchmark and ESA tests
	@$(TEST_FOLDER)/test.sh ./$< $(MODIP_FILE) $(CCIR_FOLDER) $(BENCHMARK_FOLDER) $(ESA_TEST_FOLDER)
ifneq (, $(IS_DEBUG_TARGET_IF_NON_EMPTY))
# if debug target generate test coverage report
	@$(COVERAGE_TOOL) --directory $(OBJ_FOLDER) $(CODE_COVERAGE_OPTION_TOOLCHAIN_TOOL) -c -o $(LCOV_OUT_PUT_FILE) >/dev/null
	@$(COVERAGE_TOOL_HTML_REPORT_TOOL) $(LCOV_OUT_PUT_FILE) --output-directory $(COVERAGE_REPORT_FOLDER) >/dev/null
	@gprof ./$< > $(BIN_FOLDER)/profile_analysis.txt
endif
