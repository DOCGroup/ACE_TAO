eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;
use lib "$ENV{'ACE_ROOT'}/bin";
use PerlACE::Run_Test;

$DAnCE = "$ENV{'ACE_ROOT'}/TAO/CIAO/DAnCE";
$CIAO_ROOT = "$ENV{'CIAO_ROOT'}";
$TAO_ROOT = "$ENV{'TAO_ROOT'}";
$HOST = "$ENV{'HOSTNAME'}";
$PORT = 60003;

$ns_running = 0;
$nsior = PerlACE::LocalFile ("ns.ior");

unlink $nsior;

# Invoke naming service

$NS = new PerlACE::Process ("$TAO_ROOT/orbsvcs/Naming_Service/Naming_Service", "-m 0 -ORBEndpoint iiop://$HOST:$PORT -o ns.ior");

$NS->Spawn ();

print STDERR "Starting Naming Service\n";

if (PerlACE::waitforfile_timed ($nsior, 10) == -1)
{
    print STDERR "ERROR: cannot find naming service IOR file\n";
    $NS->Kill ();
    exit 1;
}
print STDERR "Naming Service Started at $HOST:$PORT\n";

sleep (99999999999);
