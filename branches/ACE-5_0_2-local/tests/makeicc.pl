# $Id$

@ls = `ls -1 *.cpp`;
$allicc = "icc.bat";

foreach $tmp (@ls)
{
	($a1, $a2) = split("cpp",$tmp);
	$tmp2 = "$a1\icc";

open(T,"> $a1\icc");
	print T "option\n";
        print T "    define (\"ACE_HAS_WINSOCK2\",\"1\"),\n";
        print T "    define (\"__ACE_INLINE__\",\"0\"),\n";
        print T "    define (\"WIN32\",\"1\"),\n";
        print T "    define (\"_WINDOWS\",\"1\"),\n";
        print T "    define(\"ACE_HAS_DLL\",\"0\"),\n";

        print T "    link(libSearchPath,'H:\\ACE_wrappers\\ace'),\n";
	print T "    incl(searchPath,'H:\\ACE_wrappers'),\n";
	print T "    link(linkWithMultiThreadLib,yes)\n";
	print T "     {\n";
	chop($a1);
	print T "	target \"$a1\.exe\"\n";
	print T "	{\n";


	print T "	   source type (cpp) \"$a1\.cpp\"\n\n";
	print T "	   source type (lib) \"aced.lib\"\n";
	print T "   	   source type (lib) \"advapi32.lib\"\n";
	print T "	   source type (lib) \"user32.lib\"\n";
	print T "	   source type (lib) \"ws2_32.lib\"\n";
	print T "	   source type (lib) \"wsock32.lib\"\n";
	print T "	   source type (lib) \"mswsock.lib\"\n";
	print T "	}\n";
	print T "   }\n";
close T;

open(ALLICC,">> $allicc");
	print ALLICC "vacbld -SEV=E $a1\.icc\n";
close ALLICC;
	print "$a1\n";
}

