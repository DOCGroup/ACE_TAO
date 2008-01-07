eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

$NS_HOST = "roon.dre.vanderbilt.edu";
$NS_PORT = 40000;
$HOST = "$ENV{'HOSTNAME'}";
$PORT1 = 30000;
$PORT2 = 30001;
use lib "$ENV{'ACE_ROOT'}/bin";
use PerlACE::Run_Test;

$CIAO_ROOT=$ENV{'CIAO_ROOT'};


# Set up NamingService environment
$ENV{"NameServiceIOR"} = "corbaloc:iiop:$NS_HOST:$NS_PORT/NameService";


$SV1 = new PerlACE::Process ("$CIAO_ROOT/DAnCE/NodeManager/NodeManager",
                             "-ORBEndpoint iiop://$HOST:$PORT1 -ORBInitRef NameService=corbaloc:iiop:$NS_HOST:$NS_PORT/NameService -c svc_node_manager.conf -s $CIAO_ROOT/DAnCE/NodeApplication/NodeApplication -d 10");

$SV2 = new PerlACE::Process ("$CIAO_ROOT/DAnCE/NodeManager/NodeManager",
                             "-ORBEndpoint iiop://$HOST:$PORT2 -ORBInitRef NameService=corbaloc:iiop:$NS_HOST:$NS_PORT/NameService -c svc_node_manager.conf -s $CIAO_ROOT/DAnCE/NodeApplication/NodeApplication -d 10");

$SV1->Spawn ();
$SV2->Spawn ();

sleep (99999999999);
