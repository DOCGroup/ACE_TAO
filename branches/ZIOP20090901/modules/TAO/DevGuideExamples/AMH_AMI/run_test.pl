# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use Env (ACE_ROOT);
use lib "$ACE_ROOT/bin";
use PerlACE::Run_Test;

$inner_ior = PerlACE::LocalFile ("inner.ior");
$middle_ior =  PerlACE::LocalFile ("middle.ior");
unlink $inner_ior;
unlink $middle_ior;

# start inner_server

$IS = new PerlACE::Process("inner_server");
$IS->Spawn();

if (PerlACE::waitforfile_timed ($inner_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$inner_ior>\n";
    $IS->Kill();
    unlink $inner_ior;
    exit 1;
}


# start middle_server

$MS = new PerlACE::Process("middle_server");
$MS->Spawn();

if (PerlACE::waitforfile_timed ($middle_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$middle_ior>\n";
    $MS->Kill();
    unlink $middle_ior;
    exit 1;
}

# start client

$C = new PerlACE::Process("client");
$C->Spawn();

$CRET = $C->WaitKill(45);
$IS->Kill();
$MS->Kill();

# clean-up

unlink $inner_ior;
unlink $middle_ior;

if ($CRET != 0) {
    print STDERR "ERROR: Client returned <$CRET>\n";
    exit 1 ;
}

exit 0;
