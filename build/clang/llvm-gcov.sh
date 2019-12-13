#########################################
## NeQuickG_JRC test makefile
##
## Author: Angela Aragon-Angel
## Copyright: Joint Research Centre (JRC), 2019
## This software has been released as free and open source software
## under the terms of the European Union Public Licence (EUPL), version 1.
## Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
#########################################
#!/bin/bash
exec llvm-cov gcov "$@"
