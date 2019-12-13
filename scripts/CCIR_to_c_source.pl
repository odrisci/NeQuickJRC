#########################################
## NeQuickG_JRC test makefile
##
## Author: Angela Aragon-Angel
## Copyright: Joint Research Centre (JRC), 2019
## This software has been released as free and open source software
## under the terms of the European Union Public Licence (EUPL), version 1.
## Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
#########################################
use strict;
use File::Basename;

use constant NL => "\n";
use constant SOLAR_ACTIVITY_CONDITIONS  => 2;
use constant F2_COEFF_MAX_DEGREE  => 76;
use constant F2_COEFF_MAX_ORDER  => 13;
use constant FM3_COEFF_MAX_DEGREE  => 49;
use constant FM3_COEFF_MAX_ORDER  => 9;

sub usage();
# logprint - print string to screen and log file
sub logprint(@);
# executes the command, p0=command, p1=output_string_reference
sub execute($$);

### MAIN #######################################################################

my $error_flag;

# get the ccir file from the environment
my $ccir_file_path;
if (defined $ARGV[0]) {
  $ccir_file_path = $ARGV[0];
} else {
  usage();
}

my $g_CCIR_source_file;
#0. create CCIR source file
{
  open($g_CCIR_source_file, '>'.$ccir_file_path.".c");
  if (not defined($g_CCIR_source_file)) {
    die "Error opening file: $!".NL;
  }
}

#open ccir file
open(IN_VER, '< '.$ccir_file_path) or die('Cannot open '.$ccir_file_path.NL);
my $line;

my @entries;

while(defined($line = <IN_VER>)) {
  $line =~ s/\r?\n$//;
  my @_entries = split('\s+', $line);
  for (my $index = 0; $index < scalar @_entries; $index++) {
    if (@_entries[$index] != "") {
      push(@entries, @_entries[$index]);
    }
  }
}

close(IN_VER);

if (scalar @entries != SOLAR_ACTIVITY_CONDITIONS*(F2_COEFF_MAX_DEGREE*F2_COEFF_MAX_ORDER + FM3_COEFF_MAX_DEGREE*FM3_COEFF_MAX_ORDER)) {
  die "File not valid".NL;
}

my($filename, $directories, $suffix) = fileparse($ccir_file_path);

print $g_CCIR_source_file "#include <math.h>".NL.NL;
print $g_CCIR_source_file "#include \"NeQuickG_JRC_iono_F2_layer_fourier_coefficients.h\"".NL.NL;

print $g_CCIR_source_file "const F2_coefficient_array_t g_NeQuickG_".$filename."_F2[ITU_F2_LAYER_SOLAR_ACTIVITY_CONDITIONS_COUNT] = {\n";

my $solar_activity_index;
my $total_index = 0;

for ($solar_activity_index = 0; $solar_activity_index < SOLAR_ACTIVITY_CONDITIONS; $solar_activity_index++) {
  print $g_CCIR_source_file "  {\n";
  my $degree;
  for ($degree = 0; $degree < F2_COEFF_MAX_DEGREE; $degree++) {
    print $g_CCIR_source_file "    {\n";
    print $g_CCIR_source_file "      ";
    my $order;
    for ($order = 0; $order < F2_COEFF_MAX_ORDER; $order++) {
      print $g_CCIR_source_file @entries[$total_index++].", ";
    }
    print $g_CCIR_source_file "\n";
    print $g_CCIR_source_file "    },\n";
  }
  print $g_CCIR_source_file "  },\n";
}
print $g_CCIR_source_file "};\n";

print $g_CCIR_source_file "\n";

print $g_CCIR_source_file "const Fm3_coefficient_array_t g_NeQuickG_".$filename."_Fm3[ITU_F2_LAYER_SOLAR_ACTIVITY_CONDITIONS_COUNT] = {\n";

for ($solar_activity_index = 0; $solar_activity_index < SOLAR_ACTIVITY_CONDITIONS; $solar_activity_index++) {
  print $g_CCIR_source_file "  {\n";
  my $degree;
  for ($degree = 0; $degree < FM3_COEFF_MAX_DEGREE; $degree++) {
    print $g_CCIR_source_file "    {\n";
    print $g_CCIR_source_file "      ";
    my $order;
    for ($order = 0; $order < FM3_COEFF_MAX_ORDER; $order++) {
      print $g_CCIR_source_file @entries[$total_index++].", ";
    }
    print $g_CCIR_source_file "\n";
    print $g_CCIR_source_file "    },\n";
  }
  print $g_CCIR_source_file "  },\n";
}
print $g_CCIR_source_file "};\n";

close($g_CCIR_source_file);
