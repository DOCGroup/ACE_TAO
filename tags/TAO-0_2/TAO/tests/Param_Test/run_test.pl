#$Id$
# -*- perl-mode -*-
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

unshift @INC, '../../../bin';
require Process;

$port = 0;
$iorfile = "theior";
$invocation = "sii";
$num = 5;
$other = "";
$debug = "";
$type = "";

sub run_test
{
  my $type = shift(@_);

  $SV = Process::Create (".".$DIR_SEPARATOR."server".$Process::EXE_EXT,
                         "$debug -ORBobjrefstyle url -ORBport $port -o ".
			 $iorfile);
  
  sleep (2);     # Give the server a chance to start up
  
  system (".".$DIR_SEPARATOR."client $debug -f $iorfile  -i $invocation -t ".
          "$type -n $num");
  
  # @@
  # Someday, a better way of doing this should be found.  Or at least 
  # something that can tell if a server is still alive.  There is kill -0 on
  # Unix, but on NT ???

  $SV->Kill (); $SV->Wait ();
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

@types = ("short", "ubstring", "bdstring", "fixed_struct", "strseq", 
          "bounded_strseq", "var_struct", "nested_struct", "struct_seq", 
	  "bounded_struct_seq", "any", "objref", "objref_sequence", 
	  "any_sequence", "short_sequence", "long_sequence", 
	  "bounded_short_sequence", "bounded_long_sequence",
	  "fixed_array", "var_array");

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
