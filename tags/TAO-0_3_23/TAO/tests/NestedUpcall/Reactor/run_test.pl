eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../../bin';
require Process;
require ACEutils;

$SV = Process::Create ($EXEPREFIX."server".$Process::EXE_EXT,
                       " -ORBobjrefstyle url");

sleep ($ACE::sleeptime);

$status  = system ($EXEPREFIX."client".$Process::EXE_EXT.
                   " -x ");

$SV->Wait ();

# @@ Capture any errors from the server too.
exit $status;
