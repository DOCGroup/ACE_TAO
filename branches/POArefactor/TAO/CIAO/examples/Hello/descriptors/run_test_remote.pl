#$Id$
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a Perl script that runs the Hello example.

use lib "$ENV{'ACE_ROOT'}/bin";
use PerlACE::Run_Test;

$status = 0;

$ACE_ROOT=$ENV{'ACE_ROOT'};
$CIAO_ROOT=$ENV{'CIAO_ROOT'};
# amount of delay between running the servers
$sleeptime = 1;
$longsleeptime = 3;
$DM1_args = "-ORBEndpoint iiop://localhost:20000 -n ${CIAO_ROOT}/tools/ComponentServer/ComponentServer";
$DM1 = new PerlACE::Process ("${CIAO_ROOT}/tools/Daemon/CIAO_Daemon", "$DM1_args");

$DM2_args = "-ORBEndpoint iiop://localhost:12000 -n ${CIAO_ROOT}/tools/ComponentServer/ComponentServer";
$DM2 = new PerlACE::Process ("${CIAO_ROOT}/tools/Daemon/CIAO_Daemon", "$DM2_args");

$AM_args = "-o ior -c test.dat";
$AM = new PerlACE::Process ("${CIAO_ROOT}/tools/Assembly_Deployer/Assembly_Manager", "$AM_args");

$AD_args = "-k file://ior -a remote.cad -o cookie -d cookie";
$AD = new PerlACE::Process ("${CIAO_ROOT}/tools/Assembly_Deployer/Assembly_Deployer", "$AD_args");

$ST = new PerlACE::Process ("${CIAO_ROOT}/examples/Hello/Sender/starter");

# Start the daemons.
$DM1->Spawn ();
$DM2->Spawn ();
# Give the service time to settle
sleep $sleeptime;

# Start the Assembly_Manager
$AM->Spawn ();

# Sait till the Manager finishes writing the IOR
sleep $longsleeptime;

# Start the Assembly_Deployer
$AD->Spawn ();

# This might take a while
sleep $longsleeptime;


#Now start the starter for 1 times.

$ST->SpawnWaitKill (2);

$ST->SpawnWaitKill (2);

$ST->SpawnWaitKill (2);

sleep 2;

exit $status;
