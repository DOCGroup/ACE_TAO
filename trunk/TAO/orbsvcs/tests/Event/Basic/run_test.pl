eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a Perl script that runs the client and all the other servers that
# are needed

unshift @INC, '../../../../../bin';
require Process;
require Uniqueid;

print STDERR "\n\nReconnect suppliers and consumers,",
  " using disconnect/connect calls\n";
system ("Reconnect -suppliers 100 -consumers 100 -d 100");

print STDERR "\n\nReconnect suppliers and consumers, using connect calls\n";
system ("Reconnect -suppliers 100 -consumers 100 -d 100 -s -c");

print STDERR "\n\nShutdown EC with clients still attached\n";
system ("Shutdown -verbose -suppliers 5 -consumers 5");

print STDERR "\n\nComplex event channel test,",
  "multiple ECs connected through gateways\n";
system ("Observer -ORBsvcconf observer.conf"
	." -consumer_tshift 0 -consumers 5"
	." -supplier_tshift 0 -suppliers 2"
	." -burstsize 10 -burstcount 10 -burstpause 0"
	." -busyhwm 1024 -maxwritedelay 1024");

exit 0;
