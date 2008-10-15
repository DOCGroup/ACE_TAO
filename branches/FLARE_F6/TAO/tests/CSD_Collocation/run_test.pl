eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("Collocation",  "-ORBSvcConf svc.conf.csd");
}
else {
    $SV = new PerlACE::Process ("Collocation", "-ORBSvcConf svc.conf.csd");
}

$server = $SV->SpawnWaitKill (60);

if ($server != 0) {
    print STDERR "ERROR: CSD Collocation test returned $server \n";
    $status = 1;
}

exit $status;
