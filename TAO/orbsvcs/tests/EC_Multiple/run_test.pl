eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../../bin';
require Process;

$NS_ior = "NameService.ior";
$sleeptime = 3;

$NS = Process::Create ("../../Naming_Service/Naming_Service".
                       $Process::EXE_EXT." -o $NS_ior ");

sleep $sleeptime;

# This is a very simple test, no multiple consumers and no gateways.
$status = system ($EXEPREFIX."EC_Multiple".$Process::EXE_EXT.
                  " -s local");

$NS->Kill ();

unlink $NS_ior;

# @@ Capture any errors from the server too.
exit $status;
