eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../../bin';
require Process;
require ACEutils;

$file="test.ior";

$SV = Process::Create ($EXEPREFIX."server".$Process::EXE_EXT,
                       "-o $file");

ACE::waitforfile ($file);

$status  = system ($EXEPREFIX."client".$Process::EXE_EXT.
                   "-x -k file://$file");

$SV->Wait ();

$SV = Process::Create ($EXEPREFIX."server".$Process::EXE_EXT,
                       "-o $file");

ACE::waitforfile ($file);

$status  = system ($EXEPREFIX."mt_client".$Process::EXE_EXT.
                   "-x -k file://$file -ORBsvcconf svc.mt.conf");

$SV->Wait ();

# @@ Capture any errors from the server too.
exit $status;
