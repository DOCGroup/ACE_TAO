eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../bin";
require ACEutils;
require Process;

$iorfile = "server.ior";
$invocation = "sii";
$num = 5;
$other = "";
$debug = "";
$type = "";

unlink $iorfile;

sub run_test
{
  my $type = shift(@_);

  unlink $iorfile; # Ignore errors
  $SV = Process::Create ($EXEPREFIX."server".$Process::EXE_EXT,
                         "$debug -o $iorfile");

  if (ACE::waitforfile_timed ($iorfile, 5) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
  }

  $CL = Process::Create ($EXEPREFIX."client",
			 " $debug -f $iorfile  -i $invocation -t ".
			 "$type -n $num -x");

  $client = $CL->TimedWait (60);
  if ($client == -1) {
    print STDERR "ERROR: client timedout\n";
    $CL->Kill (); $CL->TimedWait (1);
  }

  $server = $SV->TimedWait (2);
  if ($server == -1) {
    print STDERR "ERROR: server timedout\n";
    $SV->Kill (); $SV->TimedWait (1);
  }
  unlink $iorfile;
}

# Parse the arguments

for ($i = 0; $i <= $#ARGV; $i++)
{
  SWITCH:
  {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?")
    {
      print "Run_Test Perl script for TAO Param Test\n\n";
      print "run_test [-n num] [-d] [-onewin] [-h] [-t type] [-i (dii|sii)]\n";
      print "\n";
      print "-n num              -- runs the client num times\n";
      print "-d                  -- runs each in debug mode\n";
      print "-onewin             -- keeps all tests in one window on NT\n";
      print "-h                  -- prints this information\n";
      print "-t type             -- runs only one type of param test\n";
      print "-i (dii|sii)        -- Changes the type of invocation\n";
      exit;
    }
    if ($ARGV[$i] eq "-n")
    {
      $num = $ARGV[$i + 1];
      $i++;
      last SWITCH;
    }
    if ($ARGV[$i] eq "-d")
    {
      $debug = $debug." -d";
      last SWITCH;
    }
    if ($ARGV[$i] eq "-onewin")
    {
      if ($^O eq "MSWin32")
      {
        $Process::newwindow = "no";
      }
      last SWITCH;
    }
    if ($ARGV[$i] eq "-t")
    {
      $type = $ARGV[$i + 1];
      $i++;
      last SWITCH;
    }
    if ($ARGV[$i] eq "-i")
    {
      $invocation = $ARGV[$i + 1];
      $i++;
      last SWITCH;
    }
    $other .= $ARGV[$i];
  }
}

@types = ("short", "ulonglong", "ubstring", "bdstring", "fixed_struct",
          "ub_strseq", "bd_strseq",
          "var_struct", "nested_struct", "recursive_struct",
          "ub_struct_seq", "bd_struct_seq",
          "any", "objref", "objref_sequence", "objref_struct",
          "any_sequence",
          "ub_short_sequence", "ub_long_sequence",
          "bd_short_sequence", "bd_long_sequence",
          "fixed_array", "var_array", "typecode", "exception",
	  "big_union", "complex_any");

if ($type ne "")
{
  run_test ($type);
}
else
{
  foreach $type (@types)
  {
    run_test ($type);
  }
}

unlink $iorfile;
