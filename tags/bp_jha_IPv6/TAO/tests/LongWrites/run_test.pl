eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib '../../../bin';
use PerlACE::Run_Test;
use Cwd;

my $iorfile = PerlACE::LocalFile ("server.ior");
unlink $iorfile;

my $SV = new PerlACE::Process ("server", "-o $iorfile");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 5) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
} 

$CL1 = new PerlACE::Process ("client", " -k file://$iorfile ");
$CL2 = new PerlACE::Process ("client", " -k file://$iorfile ");
$CL3 = new PerlACE::Process ("client", " -k file://$iorfile ");

$CL1->Spawn (60);
$CL2->Spawn (60);
$CL3->Spawn (60);

$client1 = $CL1->WaitKill (60);
$client2 = $CL2->WaitKill (60);
$client3 = $CL3->WaitKill (60);
$server = $SV->WaitKill (5);

unlink $iorfile;
  
if ($server != 0
    || $client1 != 0 || $client2 != 0 || $client3 != 0) {
  print "ERROR: non-zero status returned by the server or clients\n";
  exit 1;
}

exit 0;
