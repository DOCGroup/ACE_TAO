#$Id$
# -*- perl -*-
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

unshift @INC, '../../../bin';
require ACEutils;

$iorfile = "server.ior";
$SV = Process::Create ($EXEPREFIX."server$Process::EXE_EXT",
		       " -o $iorfile -ORBsvcconf server.conf");

ACE::waitforfile ($iorfile);

$status  = system ($EXEPREFIX."client$Process::EXE_EXT "
		   . "-k file://$iorfile -n 10 -i 1000");

$SV->Kill (); $SV->Wait ();

unlink $iorfile;

exit $status;
