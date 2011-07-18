eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$iorfile = PerlACE::LocalFile ("server.ior");
unlink $iorfile;
$status = 0;

$num_clients=1;

if (PerlACE::is_vxworks_test()) {
    $SV  = new PerlACE::ProcessVX ("server_main", "-o $iorfile -n $num_clients");
}
else {
    $SV  = new PerlACE::Process ("server_main", "-o $iorfile -n $num_clients");
}

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
} 

for ($i = 0; $i < $num_clients; $i++) {

  @CLS[$i] = new PerlACE::Process ("client_main", " -i file://$iorfile");

  @CLS[$i]->Spawn ();
}

for ($i = 0; $i < $num_clients; $i++) {

  $client = @CLS[$i]->WaitKill (60);

  if ($client != 0) {
    print STDERR "ERROR: client $i returned $client\n";
    $status = 1;
  }
}


$server = $SV->WaitKill (60);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
