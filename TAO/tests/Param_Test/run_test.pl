#!/pkg/gnu/bin/perl -I../../../bin
#
# If your perl installation isn't in /pkg/gnu/bin/perl,
# please make the change accordingly
#

use Process;

#uid=`id | cut -c5-20 | cut -f1 -d"("`
$port = 30001 + $uid;
$iorfile = "theior";
$invocation = "sii";

$SV = Process::Create (".".$DIR_SEPARATOR."server".$Process::EXE_EXT, 
                       "-ORBobjrefstyle url -ORBport ".$port." -o ".$iorfile." 2");
sleep (2);     # Give the server a chance to start up

@types= ("short", "ubstring", "fixed_struct", "strseq", "var_struct", "nested_struct", "struct_seq", "objref");


foreach $type (@types)
{
	print ("BEGIN Testing type ".$type."\n");
	system (".".$DIR_SEPARATOR."client -f ".$iorfile."  -i ".$invocation." -t ".$type." 2");
	print ("END   Testing type ".$type."\n");
}

$SV->Kill ();

if ($^O eq "MSWin32")
{
  system ("del ".$iorfile);
}
else
{
  system ("rm ".$iorfile);
}