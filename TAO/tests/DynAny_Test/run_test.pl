eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../bin";
require ACEutils;

$type = "";

sub run_test
{
  my $type = shift(@_);

  system ($EXEPREFIX."basic_test -t $type");

}

# Parse the arguments

for ($i = 0; $i <= $#ARGV; $i++)
{
  SWITCH:
  {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?")
    {
      print "Run_Test Perl script for TAO DynAny Test\n\n";
      print "run_test [-t type]\n";
      print "\n";
      print "-t type             -- runs only one type of dynany test\n";
      exit;
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


