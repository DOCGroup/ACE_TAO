/**

$Id$

@page Bug_3384_Regression Test README File

A regression test for a bug in (un)reliable oneways with the SYNC_NONE scope.
The test verifies if a oneway sent as the first request on a new transport
gets correctly pushed out as soon as the new transort has connected.
This is a regression test for:

http://ace.cs.wustl.edu/bugzilla/show_bug.cgi?id=3384

	To run the test use the run_test.pl script:

$ ./run_test.pl

	the script returns 0 if there are no errors.

*/
