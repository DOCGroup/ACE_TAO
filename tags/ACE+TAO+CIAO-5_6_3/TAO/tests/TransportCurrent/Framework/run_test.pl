eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use Config;
use PerlACE::Run_Test;

PerlACE::add_lib_path ('../lib');

sub add_path {
    my($name)  = shift;
    my($value) = shift;
    if (defined $ENV{$name}) {
        $ENV{$name} .= $Config{'path_sep'} . $value
        }
    else {
        $ENV{$name} = $value;
    }
}

my $status = 0;
my $confmod = "";

my $mode = shift (@ARGV);
if ( $mode =~ /-dynamic/) {
}
elsif  ( $mode =~ /-static/) {
    $confmod = "-static";
}
else {
    print STDERR "Unknown $mode. Specify -static or -dynamic\n";
    exit 1;
}

my $iorbasefile = "server.ior";
my $iorfile = PerlACE::LocalFile ("$iorbasefile");
my $confserverbase = "server$confmod.conf";
my $confserver = PerlACE::LocalFile ("$confserverbase");
my $confclient = PerlACE::LocalFile ("client$confmod.conf");
unlink $iorfile;

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server",
                                  "@ARGV -ORBSvcConf $confserverbase -o $iorbasefile");
}
else {
    $SV = new PerlACE::Process ("server",
                                "@ARGV -ORBSvcConf $confserver -o $iorfile");
}

$CL = new PerlACE::Process ("client",
                            "@ARGV -n 1 -ORBSvcConf $confclient -k file://$iorfile");

print STDERR $SV->CommandLine()."\n";
$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "$0: ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

print STDERR $CL->CommandLine()."\n";
$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "$0: ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (15);

if ($server != 0) {
    print STDERR "$0: ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
