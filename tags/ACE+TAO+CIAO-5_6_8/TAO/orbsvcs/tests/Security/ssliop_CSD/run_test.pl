eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use Env (ACE_ROOT);
use lib "$ACE_ROOT/bin";
use PerlACE::Run_Test;

my $options = "csd|ssliop|ssliop-csd";
my $ssliop_conf = "ssliop.conf";
my $csd_conf = "csd.conf";
my $ssliop_csd_conf = "ssliop-csd.conf";
my $client_conf = "client1.conf";

my $serv_opts = "-ORBSvcConf $ssliop_conf";
my $client_opts = "-ORBSvcConf $client_conf";

$args = @ARGV;
if ($args > 0) {
    if (($ARGV[0] =~ /\-h/) || ($ARGV[0] =~ /\-\?/)) {
	print $0. " $options\n";
    } elsif ($ARGV[0] =~ /ssliop\-csd/) {
	$serv_opts = "-ORBSvcConf $ssliop_csd_conf";
    } elsif ($ARGV[0] =~ /csd/) {
	$serv_opts = "-ORBSvcConf $csd_conf";
	$client_opts = "";
    } else {
	print "Using default ssliop configuration.\n";
    }
} else {
    print "Using default ssliop configuration.\n";
}

my $file = PerlACE::LocalFile("Messenger.ior");

unlink $file;

$ENV{'SSL_CERT_FILE'} = 'cacert.pem';

# start MessengerServer
$S = new PerlACE::Process("MessengerServer",
                          "$serv_opts");

print STDERR "\n\nSecurity Unaware Application Examples\n";
print STDERR     "-------------------------------------\n";
print STDERR     "Starting Messenger Server...\n\n";

print $S->CommandLine(). "\n";
$S->Spawn();
if (PerlACE::waitforfile_timed ($file, 5) == -1) {
    print STDERR "ERROR: cannot find file <$file>\n";
    $SV->Kill ();
    exit 1;
}

# start MessengerClient
$C2 = new PerlACE::Process("MessengerClient",
                           "$client_opts");

print $C2->CommandLine(). "\n";
print STDERR     "\nStarting MessengerClient, example 3...\n\n";
if ($C2->SpawnWaitKill(10) != 0) {
     $S->Kill();
     exit (1);
}

$ENV{'SSL_CERT_FILE'} = '';

# clean-up
$S->Kill();

exit 0;
