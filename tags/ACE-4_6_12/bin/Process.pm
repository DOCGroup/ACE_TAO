# $Id$
$inc = "Process_Unix.pm";
$DIR_SEPARATOR = "/";

if ($^O eq "MSWin32")
{
	$inc = "Process_Win32.pm";
	$DIR_SEPARATOR = "\\";
}

require $inc;

1;