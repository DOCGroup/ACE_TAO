eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../bin';
use PerlACE::Run_Test;

my($debug) = 0;

my($iorfile1) = PerlACE::LocalFile ("test1.ior");
my($iorfile2) = PerlACE::LocalFile ("test2.ior");
unlink $iorfile1;
unlink $iorfile2;
my($status) = 0;

my($SV1) = new PerlACE::Process ("ft_replica", "-o $iorfile1");
my($SV2) = new PerlACE::Process ("ft_replica", "-o $iorfile2");
my($CL) = new PerlACE::Process ("ft_client", "-f $iorfile1,$iorfile2 -c testscript");
#my($CL) = new PerlACE::Process ("ft_client", "-f $iorfile1 -c testscript");

if ($debug) {print "starting replica 1\n";}
$SV1->Spawn ();

if ($debug) {print "waiting for replica 1's IOR\n";}

if (PerlACE::waitforfile_timed ($iorfile1, 5) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile1>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

if ($debug) {print "starting replica 2\n";}
$SV2->Spawn ();

if ($debug) {print "waiting for replica 2's IOR\n";}
if (PerlACE::waitforfile_timed ($iorfile2, 5) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile2>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
    exit 1;
}

if ($debug) {print "starting client.\n";}

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

if ($debug) {print "wait for server 1.\n";}
$server = $SV1->WaitKill (60);

if ($debug) {print "wait for server 2.\n";}
$server = $SV2->WaitKill (60);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

if ($debug) {print "releasing scratch files.\n";}
unlink $iorfile1;
unlink $iorfile2;
unlink "Persistent.dat";

exit $status;
