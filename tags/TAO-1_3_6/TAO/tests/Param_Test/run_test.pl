eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../bin";
use PerlACE::Run_Test;

$iorfile = PerlACE::LocalFile ("server.ior");

$invocation = "sii";
$num = 5;
$debug = "";
$status = 0;

unlink $iorfile;

# Parse the arguments

@types = ("short", "ulonglong", "ubstring", "bdstring",
          "ubwstring", "bdwstring", "fixed_struct",
          "ub_strseq", "bd_strseq", "ub_wstrseq", "bd_wstrseq",
          "var_struct", "nested_struct", "recursive_struct",
          "ub_struct_seq", "bd_struct_seq",
          "ub_array_seq", "bd_array_seq", "multdim_array",
          "any", "objref", "objref_sequence", "objref_struct",
          "any_sequence",
          "ub_short_sequence", "ub_long_sequence",
          "bd_short_sequence", "bd_long_sequence",
          "fixed_array", "var_array", "typecode", "exception",
          "big_union", "small_union", "recursive_union", "complex_any");


for ($i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?") {
      print "Run_Test Perl script for TAO Param Test\n\n";
      print "run_test [-n num] [-d] [-onewin] [-h] [-t type] [-i (dii|sii)] [-chorus <target>]\n";
      print "\n";
      print "-n num              -- runs the client num times\n";
      print "-d                  -- runs each in debug mode\n";
      print "-h                  -- prints this information\n";
      print "-t type             -- runs only one type of param test\n";
      print "-i (dii|sii)        -- Changes the type of invocation\n";
      print "-chorus <target>    -- Run tests on chorus target\n";
      exit 0;
    }
    elsif ($ARGV[$i] eq "-n") {
      $num = $ARGV[$i + 1];
      $i++;
    }
    elsif ($ARGV[$i] eq "-d") {
      $debug = $debug." -d";
    }
    elsif ($ARGV[$i] eq "-t") {
      @types = split (',', $ARGV[$i + 1]);
      $i++;
    }
    elsif ($ARGV[$i] eq "-i") {
      $invocation = $ARGV[$i + 1];
      $i++;
    }
}

$SV = new PerlACE::Process ("server", "$debug -o $iorfile");
$CL = new PerlACE::Process ("client");

foreach $type (@types) {
    unlink $iorfile; # Ignore errors

    print STDERR "==== Testing $type === wait....\n";

    $SV->Spawn ();
    
    if (PerlACE::waitforfile_timed ($iorfile, 15) == -1) {
        print STDERR "ERROR: cannot find file <$iorfile>\n";
        $SV->Kill (); 
        exit 1;
    }

    $CL->Arguments ("$debug -f $iorfile  -i $invocation -t $type -n $num -x");

    $client = $CL->SpawnWaitKill (60);
    
    if ($client != 0) {
        print STDERR "ERROR: client returned $client\n";
        $status = 1;
    }

    $server = $SV->WaitKill (10);
    
    if ($server != 0) {
        print STDERR "ERROR: server returned $server\n";
        $status = 1;
    }
    
    unlink $iorfile;

    print STDERR "==== Test for $type finished ===\n";
}

unlink $iorfile;

exit $status;
