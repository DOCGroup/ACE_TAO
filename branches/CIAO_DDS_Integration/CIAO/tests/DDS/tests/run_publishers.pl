eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{'ACE_ROOT'}/bin";
use PerlACE::Run_Test;
 
$CIAO_ROOT = "$ENV{'CIAO_ROOT'}";
$TAO_ROOT = "$ENV{'TAO_ROOT'}";
$DDS_ROOT = "$ENV{'DDS_ROOT'}";
$DAnCE = "$ENV{'CIAO_ROOT'}/DAnCE";
#$debug_param = "-ORBDebug -ORBDebugLevel 100";
$irepo_running = 0;
$IREPO = 0;

$publisher_count = 1;
if (@ARGV[0]) {$publisher_count = @ARGV[0];}

# DCPS info repository parameters
$svcconf = "../descriptors/tcp.conf";
$dcpsconf = "../descriptors/dds_tcp_conf.ini";
$repaddr = "localhost:88776";
$repoior = "repo.ior";
$domainids = "../descriptors/domain_ids";

sub delete_output_files {
    unlink PerlACE::LocalFile ($repoior);
}

sub kill_open_processes {
  if ($irepo_running == 1) {
    $IREPO->Kill ();
    $IREPO->TimedWait (1);
  }
}

delete_output_files ();

$psresult = `ps axo comm= | grep DCPSInfoRepo`;
if ($psresult) {
    die "please shutdown any running DCPSInfoRepo first!\n";
}

# Invoke DCPS information repository
$IREPO = new PerlACE::Process ("$DDS_ROOT/bin/DCPSInfoRepo", 
			       "-ORBSvcConf $svcconf -ORBEndpoint iiop://$repaddr -d $domainids");
$IREPO->Spawn ();

print STDERR "Starting DCPS Info Repository\n";

if (PerlACE::waitforfile_timed ($repoior, $PerlACE::wait_interval_for_process_creation) == -1)
{
    print STDERR "ERROR: cannot find DCPS Info Repo IOR file\n";
    $IREPO->Kill ();
    exit 1;
}

$irepo_running = 1;

# invoke publishers
$pubs; # list of publisher processes
for $pub_num ( 1 .. $publisher_count) {
    print "Invoking DDS publisher $pub_num\n";
    $pubs[$pub_num] = new PerlACE::Process ("../Publisher/publisher",
				 "-ORBSvcConf $svcconf -DCPSConfigFile " .
					   "$dcpsconf -i \"Publisher No $pub_num\"");
    $pubs[$pub_num]->Spawn ();
}

print "\n*************************************************************************\n";
print "* Press any key to shutdown the test ...\n";
print "*************************************************************************\n\n";
$confirmation = <STDIN>;

# Invoke executor - stop the application -.
for $pub_num ( 1 .. $publisher_count) {
    print "killing the publisher $pub_num\n";
    $pubs[$pub_num]->Kill ();
    $pubs[$pub_num]->TimedWait (1);
}

kill_open_processes ();
delete_output_files ();

exit 0;
