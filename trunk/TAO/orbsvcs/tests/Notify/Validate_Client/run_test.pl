eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use File::Copy;

PerlACE::check_privilege_group();

$notifyior = PerlACE::LocalFile ("notify.ior");
$notify_conf = PerlACE::LocalFile ("notify$PerlACE::svcconf_ext");

$persistent_file_prefix = "persistency.notif";
$persistent_file = PerlACE::LocalFile ("persistency.notif.xml");

$status = 0;

$port = PerlACE::uniqueid () + 10001;
$TS = new PerlACE::Process ("../../../Notify_Service/Notify_Service",
                            "-NoNameSvc " .
                            "-IORoutput $notifyior -ORBSvcConf " .
                            "$notify_conf");

unlink $notifyior;
unlink <$persistent_file_prefix.*>;

$DUMMY = new PerlACE::Process ("proxy_dummy", "-o $persistent_file");
$DUMMY->Spawn();
if (PerlACE::waitforfile_timed ($persistent_file, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: waiting for the proxy_dummy to start\n";
    $DUMMY->Kill ();
    exit 1;
}


$TS->Spawn ();

if (PerlACE::waitforfile_timed ($notifyior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: waiting for the notify service to start\n";
    $TS->Kill ();
    $DUMMY->Kill ();
    exit 1;
}

sleep (10);

$found = 0 ;
open (I_FILE, "$persistent_file") ;
while( <I_FILE> )
{
   chomp ;
   if( $_ =~ /proxy/ )
   {
       $found = 1 ;
       last ;
   }
}
if( $found == 1 )
{
    print STDERR "ERROR: validate consumer/supplier test failed.\n" ;
    $status = 1 ;
}
else
{
    print STDOUT "validate consumer/supplier test succeeded.\n";
}

$TS->Kill ();
$DUMMY->Kill ();

unlink <$persistent_file_prefix.*>;
unlink $notifyior;


exit $status;

