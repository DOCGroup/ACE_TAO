eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../bin";
use PerlACE::Run_Test;

$status = 0;
$iorfile = PerlACE::LocalFile ("test.ior");

print STDERR "================ Single-threaded Latency Test\n";

unlink $iorfile;

my $type = 'octet';

for ($i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?") {
      print "Run_Test Perl script for Performance Test\n\n";
      print "run_test  [-t type] \n";
      print "\n";
      print "-t type             -- runs only one type of param test\n";
      exit 0;
    }
    elsif ($ARGV[$i] eq "-t") {
      $type = $ARGV[$i + 1];
      $i++;
    }
}

$SV = new PerlACE::Process ("server",
                            "-o $iorfile");

$CL = new PerlACE::Process ("client",
                            "-t $type -k file://$iorfile "
                            . " -i 50000");

print STDERR $CL->CommandLine () ;
$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 15) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (420);
$server = $SV->WaitKill (10);

unlink $iorfile;

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

exit $status;
