eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;

$synch_with_server_option = "";

if ($ARGV[0] eq 'synch_with_server') { 
  $synch_with_server_option = "-s";
}
elsif ($ARGV[0] eq '') {
  #synch with transport
}
else {
  print STDERR "ERROR: invalid parameter $ARGV[0] \n";
  exit 1;
}

if (PerlACE::is_vxworks_test()) {
    $SV  = new PerlACE::ProcessVX ("server_main", "$synch_with_server_option");
}
else {
    $SV  = new PerlACE::Process ("server_main", "$synch_with_server_option");
}


$SV->Spawn ();


$server = $SV->WaitKill (60);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}


exit $status;
