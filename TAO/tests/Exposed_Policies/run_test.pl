eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../bin";
require PerlACE::Run_Test;

$poa_file = PerlACE::LocalFile ("POA.cfg");
$obj_file = PerlACE::LocalFile ("Object.cfg");
$base_ior_file = PerlACE::LocalFile ("default.ior");
$overridden_ior_file = PerlACE::LocalFile ("overridden.ior");

$SV = new PerlACE::Process ("server", 
                            " -POAConfigFile $poa_file" .
                            " -ObjectConfigFile $obj_file" . 
                            " -BaseObjectIOR $base_ior_file" .
                            " -OverriddenIOR $overridden_ior_file");

if ($^O eq "dec_osf") {
    $poa_file = PerlACE::LocalFile ("POA.cfg.tru64");
    $obj_file = PerlACE::LocalFile ("Object.cfg.tru64");

    $SV = new PerlACE::Process ("server", 
                                " -POAConfigFile $poa_file" .
                                " -ObjectConfigFile $obj_file" .
                                " -BaseObjectIOR $base_ior_file" .
                                " -OverriddenIOR $overridden_ior_file");
}

$CL = new PerlACE::Process ("client", "-POAConfigFile $poa_file"
                            . " -ObjectConfigFile $obj_file"
                            . " -BaseObjectIOR $base_ior_file"
                            . " -OverriddenIOR $overridden_ior_file");

$status = 0;

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($base_ior_file, 30) == -1) {
    print STDERR "ERROR: cannot find file <$base_ior_file>\n";
    $SV->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->TerminateWaitKill (10);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

exit $status;
