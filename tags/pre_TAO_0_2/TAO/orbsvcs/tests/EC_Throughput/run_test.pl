# -*- perl-mode -*-
# $Id$
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

unshift @INC, '../../../../bin';
require Process;

$NS_ior = "NameService.ior";
$sleeptime = 3;

$NS = Process::Create ("../../Naming_Service/Naming_Service".
		       $Process::EXE_EXT." -o $NS_ior ");
sleep $sleeptime;

$ES = Process::Create ("../../Event_Service/Event_Service".$Process::EXE_EXT);

sleep $sleeptime;

$C = Process::Create ("ECT_Consumer".$Process::EXE_EXT);
$S = Process::Create ("ECT_Supplier".$Process::EXE_EXT);

$C->Wait ();
$S->Wait ();

$NS->Kill ();
$ES->Kill ();

unlink $NS_ior;

# @@ Capture the errors from the processes.
exit 0;
