# $Id$
# -*- perl -*-
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

unshift @INC, '../../../bin';
require ACEutils;

$client_conf="client.global.conf";
$client_process="client";
$debug_level='0';
$threads='10';

foreach $i (@ARGV) {
  if ($i eq '-tss') {
    $client_conf = "client.tss.conf";
  } elsif ($i eq '-debug') {
    $debug_level = '1';
  } elsif ($i eq '-creation') {
    $client_process = 'orb_creation';
    $threads='2';
  }
}

$iorfile = "server.ior";
$SV = Process::Create ($EXEPREFIX."server$Process::EXE_EXT ",
		       " -ORBsvcconf server.conf  -ORBdebuglevel $debug_level"
		       . " -o $iorfile");

ACE::waitforfile ($iorfile);

$status  = system ($EXEPREFIX."$client_process$Process::EXE_EXT "
		   . " -ORBsvcconf $client_conf -ORBdebuglevel $debug_level"
		   . " -k file://$iorfile "
		   . " -n $threads -i 1000");

$SV->Kill (); $SV->Wait ();

unlink $iorfile;

exit $status;
