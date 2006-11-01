eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# run_test.pl,v 1.5 2006/01/31 21:54:08 wotte Exp
# -*- perl -*-

use lib "$ENV{'ACE_ROOT'}/bin";
use PerlACE::Run_Test;

$DAnCE = "$ENV{'CIAO_ROOT'}/DAnCE";
$CIAO_ROOT = "$ENV{'CIAO_ROOT'}";
$TAO_ROOT = "$ENV{'TAO_ROOT'}";

$iiop = "iiop://localhost:50002";
$node_app = "$DAnCE/NodeApplication/NodeApplication";
$d_cmd = "$DAnCE/NodeManager/NodeManager";
$iorfile = "Groung.ior";
$d_param = "-n -ORBEndpoint $iiop -s $node_app -o $iorfile -d 60";
$space = new PerlACE::Process ($d_cmd, $d_param);
$space->SpawnWaitKill (5000);

exit 0;
