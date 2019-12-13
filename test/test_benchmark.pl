use strict;

use constant NL => "\n";
use constant CMD_PIPE => ' 2>&1 | ';

sub usage();
# logprint - print string to screen and log file
sub logprint(@);
# executes the command, p0=command, p1=output_string_reference
sub execute($$);

### MAIN #######################################################################

my $g_log_file;
my $error_flag;

#0. create log file
{
  open($g_log_file, '>>'.$0.".log");
  if (not defined($g_log_file)) {
    die "Error opening file: $!".NL;
  }
}

# get the NeQuick binary from the environment
my $nequick_tool_path;
if (defined $ARGV[0]) {
  $nequick_tool_path = $ARGV[0];
} else {
  usage();
}

my $modip_file_path;
if (defined $ARGV[1]) {
  $modip_file_path = $ARGV[1];
} else {
  usage();
}

my $ccir_folder;
if (defined $ARGV[2]) {
  $ccir_folder = $ARGV[2];
} else {
  usage();
}

my $benchmark_folder;
if (defined $ARGV[3]) {
  $benchmark_folder = $ARGV[3];
} else {
  usage();
}

opendir my $dir, $benchmark_folder or die "Cannot open directory: $!".NL;
my @benchmark_files = readdir $dir;
closedir $dir;

my $nequick_cmd_line_prefix = $nequick_tool_path." ";

if(not defined $ENV{"FTR_MODIP_CCIR_AS_CONSTANTS"}) {
  $nequick_cmd_line_prefix .= $modip_file_path." ".$ccir_folder." ";
}

$nequick_cmd_line_prefix .= "-j ";

foreach my $benchmark_file (@benchmark_files) {

  my $filename = $benchmark_folder.$benchmark_file;
  if(-e $filename && -f _ && -r _ ) {
    my $nequick_cmd_line = $nequick_cmd_line_prefix.$filename;
    my $out = '';

    logprint("Test of benchmark file:".$filename.NL);
    if (execute($nequick_cmd_line, \$out) != 0) {
      logprint "Error during execution of NeQuick tool".NL;
      $error_flag = 1;
    }
  }
}

close($g_log_file);

exit -1 if($error_flag != 0);
exit 0;
### END OF MAIN #######################################################################

sub execute($$) {
  my ($strCommand, $refOutput) = @_;
  open(CMD, $strCommand.CMD_PIPE) or die(' Cannot execute '. $strCommand . $! .NL);
  select((select(STDOUT), $| = 1)[0]); # autoflush
  my $bufsize = 8192; # typical size for i/o buffers
  my ( $databuf, $readbuf, $nread );
  while (($nread = sysread(CMD, $readbuf, $bufsize )) > 0) {
    logprint($readbuf);
    $$refOutput .= $_;
  }
  # the code below suffers from buffering
  # it won't update a progress bar from the tool until a
  # new line is printed (even with the autoflush option)
  # so we must use the slow sysread method above.
  # select((select(STDOUT), $| = 1)[0]); # autoflush
  # while(<CMD>) { # read command's output from the pipe
  #  logprint($_);
  #  $$refOutput .= $_;
  # }
  close(CMD);
  # append error text
  $$refOutput .= $!;
  # return error value
  return ($?/256); # Convert Perl -> DOS return value
}

sub usage() {
  logprint("usage: ".$0." <nequick_tool_path> <modip_file> <ccir_directory <benchmark_folder>");
  exit -1;
}

# logprint - print string to screen and log file
sub logprint(@) {
  print @_;
  print $g_log_file @_;
  return;
}
