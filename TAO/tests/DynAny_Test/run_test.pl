eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../bin";
require ACEutils;
require Process;
use Cwd;

$cwd  = getcwd();
$type = "";


sub run_test
{
  my $type = shift(@_);

  $BT = Process::Create ($EXEPREFIX."basic_test".$EXE_EXT,
			 " -t $type");

  $test = $BT->TimedWait (10);
  if ($test == -1) {
    print STDERR "ERROR: test timedout\n";
    $BT->Kill (); $BT->TimedWait (1);
  }

}

# Parse the arguments

for ($i = 0; $i <= $#ARGV; $i++)
{
  SWITCH:
  {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?")
    {
      print "Run_Test Perl script for TAO DynAny Test\n\n";
      print "run_test [-chorus <target>] [-t type]\n";
      print "\n";
      print "-t type             -- runs only one type of dynany test\n";
      exit;
    }
    if ($ARGV[$i] eq '-chorus') {
      $i++;
      if (defined $ARGV[$i]) {
        $EXEPREFIX = "rsh $ARGV[$i] arun $cwd$DIR_SEPARATOR";
      }
      else {
        print STDERR "The -chorus option requires the hostname of the target\n";
        exit(1);
      }
    }
    if ($ARGV[$i] eq "-t")
    {
      $type = $ARGV[$i + 1];
      $i++;
      last SWITCH;
    }
  }
}

@types = ("dynany", "dynarray", "dynenum", "dynsequence", "dynstruct",
          "dynunion");



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


