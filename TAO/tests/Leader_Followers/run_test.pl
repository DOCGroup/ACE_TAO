eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;
$iorfilebase = "lf.ior";
$iorfile = PerlACE::LocalFile ("$iorfilebase");
$tp_conf_base = "tp$PerlACE::svcconf_ext";
$select_mt_conf_base = "select_mt$PerlACE::svcconf_ext";
$tp_conf = PerlACE::LocalFile ("$tp_conf_base");
$select_mt_conf = PerlACE::LocalFile ("$select_mt_conf_base");

if (PerlACE::is_vxworks_test()) {
    $sv_iorfile = $iorfilebase;
    $SV = new PerlACE::ProcessVX ("server");
    $tpool_reactor_directive = "-ORBsvcconf $tp_conf_base";
    $select_reactor_directive = "-ORBsvcconf $select_mt_conf_base";
}
else {
    $sv_iorfile = $iorfile;
    $SV = new PerlACE::Process ("server");
    $tpool_reactor_directive = "-ORBsvcconf $tp_conf";
    $select_reactor_directive = "-ORBsvcconf $select_mt_conf";
}
$CL = new PerlACE::Process ("client");

sub run_client ($)
{
    my $args = shift;

    $CL->Arguments ("-k file://$iorfile " . $args);

    my $client = $CL->SpawnWaitKill (200);

    if ($client != 0) {
        $time = localtime;
        print STDERR "ERROR: client returned $client at $time\n";
        $status = 1;
    }
}

sub run_clients ()
{
    print STDERR "\n\n*** No event loop threads ***\n\n\n";

    print STDERR "\nSelect Reactor\n\n";

    run_client ("$select_reactor_directive -e 0");

    print STDERR "\nTP Reactor\n\n";

    run_client ("$tpool_reactor_directive -e 0");

    print STDERR "\n\n*** Single-threaded client event loop: Select Reactor ***\n\n\n";

    print STDERR "\nSingle-threaded client running event loop for 3 seconds\n\n";

    run_client ("$select_reactor_directive -e 1 -t 3000");

    print STDERR "\nSingle-threaded client running event loop for 10 seconds\n\n";

    run_client ("$select_reactor_directive -e 1 -t 10000");

    print STDERR "\nSingle-threaded client running event loop for 20 seconds\n\n";

    run_client ("$select_reactor_directive -e 1 -t 20000");

    print STDERR "\n\n*** Single-threaded client event loop: TP Reactor ***\n\n\n";

    print STDERR "\nSingle-threaded client running event loop for 3 seconds\n\n";

    run_client ("$tpool_reactor_directive -e 1 -t 3000");

    print STDERR "\nSingle-threaded client running event loop for 10 seconds\n\n";

    run_client ("$tpool_reactor_directive -e 1 -t 10000");

    print STDERR "\nSingle-threaded client running event loop for 20 seconds\n\n";

    run_client ("$tpool_reactor_directive -e 1 -t 20000");

    print STDERR "\n\n*** Multi-threaded client event loop: TP Reactor ***\n\n\n";

    print STDERR "\nMulti-threaded client running event loop for 3 seconds\n\n";

    run_client ("$tpool_reactor_directive -e 5 -t 3000");

    print STDERR "\nMulti-threaded client running event loop for 10 seconds\n\n";

    run_client ("$tpool_reactor_directive -e 5 -t 10000");

    print STDERR "\nMulti-threaded client running event loop for 20 seconds\n\n";

    run_client ("$tpool_reactor_directive -e 5 -t 20000 -x");
}

$single = 1;
$multi = 0;

for ($i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?") {
      print "run_test [-m] -[a]\n";
      print "\n";
      print "-m  -- tests the multithreaded server (default is single)\n";
      print "-a  -- tests both multi and single threaded servers\n";
      exit 0;
    }
    elsif ($ARGV[$i] eq "-m") {
      $multi = 1;
      $single = 0;
    }
    elsif ($ARGV[$i] eq "-a") {
      $multi = 1;
      $single = 1;
    }
}

if ($single == 1) {
    unlink $iorfile;

    print STDERR "\n\n*** Single threaded server ***\n\n\n";

    $SV->Arguments ("-o $sv_iorfile $select_reactor_directive");

    $SV->Spawn ();

    if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
        print STDERR "ERROR: cannot find file <$iorfile>\n";
        $SV->Kill ();
        exit 1;
    }

    run_clients ();

    $server = $SV->WaitKill (100);
    if ($server != 0) {
        $time = localtime;
        print STDERR "ERROR: server returned $server at $time\n";
        $status = 1;
    }

    unlink $iorfile;
}

if ($multi == 1) {
    unlink $iorfile;

    print STDERR "\n\n*** Thread-Pool server ***\n\n\n";

    $SV->Arguments ("-o $sv_iorfile -e 5 $tpool_reactor_directive");

    $SV->Spawn ();

    if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
        print STDERR "ERROR: cannot find file <$iorfile>\n";
        $SV->Kill ();
        exit 1;
    }

    run_clients ();

    $server = $SV->WaitKill (10);

    if ($server != 0) {
        $time = localtime;
        print STDERR "ERROR: server returned $server at $time\n";
        $SV->Kill ();
    }

    unlink $iorfile;
}

exit $status;
