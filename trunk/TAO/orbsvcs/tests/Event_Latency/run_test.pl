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

$ES = Process::Create ("../../Event_Service/Event_Service".$Process::EXE_EXT);

sleep $sleeptime;

$status = system ($EXEPREFIX."Event_Latency".$Process::EXE_EXT.
                  " -j -m 100");

$NS->Kill ();
$ES->Kill ();

unlink ($NS_ior);

# @@ Capture any errors from the server too.
exit $status;
