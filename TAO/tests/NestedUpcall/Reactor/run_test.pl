eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../../bin';
require Process;
require ACEutils;

$file="test.ior";
$mtfile="mttest.ior";

print "\n\n==== Single-threaded test\n";

$SV = Process::Create ($EXEPREFIX."server".$Process::EXE_EXT,
                       "-o $file");
sleep 2;

ACE::waitforfile ($file);

$status  = system ($EXEPREFIX."client".$Process::EXE_EXT.
                   " -x -k file://$file");

$SV->Wait ();

unlink ($file);

print "\n\n==== Multi-threaded test\n";

sleep 5;

$SV = Process::Create ($EXEPREFIX."server".$Process::EXE_EXT,
                       " -o $mtfile");
sleep 2;

ACE::waitforfile ($mtfile);

$status  = system ($EXEPREFIX."mt_client".$Process::EXE_EXT.
                   " -x -k file://$mtfile -ORBsvcconf svc.mt.conf");

$SV->Wait ();

unlink ($mtfile);

# @@ Capture any errors from the server too.
exit $status;
