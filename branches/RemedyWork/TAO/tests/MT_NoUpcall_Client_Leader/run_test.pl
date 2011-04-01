eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$client_conf = PerlACE::LocalFile ("mt_noupcall$PerlACE::svcconf_ext");
$server_conf = PerlACE::LocalFile ("mt_noupcall$PerlACE::svcconf_ext");

$debug_level = '';

foreach $i (@ARGV) {
    if ($i eq '-d') {
        $debug_level = '-ORBdebuglevel 9';
    } 
}

$server_iorfile = PerlACE::LocalFile ("server.ior");
$client_iorfile = PerlACE::LocalFile ("client.ior");

unlink $server_iorfile;
unlink $client_iorfile;

$SV = new PerlACE::Process ("server", 
                              "$debug_level "
                            . "-ORBSvcConf $server_conf ");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($server_iorfile, 15) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
  
$SV->TimedWait (2);

$CL = new PerlACE::Process ("client", "$debug_level ");

$client = $CL->Spawn ();

if (PerlACE::waitforfile_timed ($client_iorfile, 15) == -1) {
    print STDERR "ERROR: cannot find file <$client_iorfile>\n";
    $CL->Kill (); $CL->TimedWait (1);
    exit 1;
} 

print "INFO: Awaiting server ...\n";
$server = $SV->Wait ();
$SV->TimedWait (1);

print "INFO: Awaiting client ...\n";
$client = $CL->Wait ();
$CL->TimedWait (1);

print "INFO: Clean up\n";
unlink $server_iorfile;
unlink $client_iorfile;

if ($server != 0 || $client != 0) {
    print STDERR "ERROR: Test failed\n";
    exit 1;
}
 
print "INFO: Test succeded\n";
exit 0;
 
