#########################################
## NeQuickG_JRC test makefile
##
## Author: Angela Aragon-Angel
## Copyright: Joint Research Centre (JRC), 2019
## This software has been released as free and open source software
## under the terms of the European Union Public Licence (EUPL), version 1.
## Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
#########################################
#compiler
CC = gcc

# compiler flags:
# -Wall
# This enables all the warnings about constructions that some
# users consider questionable, and that are easy to avoid
# (or modify to prevent the warning)
# -Wextra
# This enables some extra warning flags that are not enabled by -Wall
# -Wmissing-prototypes
# Warn if a global function is defined without a previous prototype declaration. 
# -Wfloat-equal
# Warn if floating-point values are used in equality comparisons.
# -Wshadow
# Warn whenever a local variable or type declaration shadows another variable,
# parameter, type or whenever a built-in function is shadowed. 
# -Wpedantic
# Issue all the warnings demanded by strict ISO C;
# reject all programs that use forbidden extensions
# -Wconversion
# Warn for implicit conversions that may alter a value.
# This includes conversions between real and integer
# -Wcast-qual
# Warn whenever a pointer is cast so as to remove a type qualifier
# from the target type.
# For example, warn if a const char * is cast to an ordinary char *. 
CFLAGS = -Wall -Wextra -Wmissing-prototypes \
  -Wfloat-equal -Wshadow -Wpedantic \
  -Wconversion -Wcast-qual

#linker
LINK = gcc
LFLAGS =

include ./../makefile.mak
include lint.mak
