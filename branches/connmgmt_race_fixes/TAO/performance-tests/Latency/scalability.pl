eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../bin';
use PerlACE::Run_Test;

$status = 0;
$server_conf = PerlACE::LocalFile ("server.conf");
$client_conf = PerlACE::LocalFile ("client.conf");
$iterations = 10000;

$iorfile = PerlACE::LocalFile ("test.ior");

$JSV = new PerlACE::Process ("vbj",
                             "java_server"
                             . " -D -OAid TPool"
                             . " -D -OAthreadMax 0"
                             . " -D -OAthreadMin 16");

$JSV->IgnoreExeSubDir (1);

$SV = new PerlACE::Process ("server",
                            " -ORBSvcConf $server_conf"
                            . " -o $iorfile -n 16");


$CL = new PerlACE::Process ("client");

print STDERR "================ Java server test\n";

unlink $iorfile;

$JSV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 5) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $JSV->Kill ();
    exit 1;
}

for ($i = 1; $i < 40; $i += 4) {
    $CL->Arguments ("-ORBSvcConf $client_conf "
                    . " -k file://$iorfile "
                    . " -n $i -i $iterations -p 10 -x");

    if ($CL->SpawnWaitKill (120) != 0) {
        print STDERR "ERROR: client returned $client\n";
        $status = 1;
    }
}

$JSV->Kill ();

print STDERR "================ C++ server test\n";

unlink $iorfile;

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 5) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

for ($i = 1; $i < 40; $i += 4) {
    $CL->Arguments (" -ORBSvcConf $client_conf "
                    . " -k file://$iorfile "
                    . " -n $i -i $iterations -p 10 -x");

    if ($CL->SpawnWaitKill (120) != 0) {
        print STDERR "ERROR: client returned $client\n";
        $status = 1;
    }
}

$SV->Kill ();

unlink $iorfile;

exit $status;
