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

print STDERR "\n\nThroughput/Latency single threaded configuration\n";
system ("Throughput -burstsize 100000 -burstcount 1");

print STDERR "\n\nThroughput/Latency MT-safe configuration\n";
system ("Throughput -ORBsvcconf ec.st.conf -burstsize 100000"
	." -burstcount 1");

print STDERR "\n\nThroughput/Latency MT-safe configuration, 4 consumers\n";
system ("Throughput -ORBsvcconf ec.st.conf -burstsize 100000"
	." -burstcount 1 -consumers 4");

print STDERR "\n\nThroughput/Latency MT-safe configuration,",
  " 4 consumers 4 suppliers\n";
system ("Throughput -ORBsvcconf ec.st.conf -burstsize 100000"
	." -burstcount 1 -consumers 4 -suppliers 4");

print STDERR "\n\nThroughput/Latency MT-safe configuration,",
  " 4 consumers 4 suppliers\n";
system ("Throughput -ORBsvcconf ec.st.conf -burstsize 100000"
	." -burstcount 1 -consumers 4 -suppliers 4"
	." -consumers_tshift 0 -suppliers_tshift 0");

print STDERR "\n\nConnection and disconnection time,",
  " 100 consumers 100 suppliers\n";
system ("Connect -ORBsvcconf ec.st.conf -consumers 100 -suppliers 100"
	." -connection_order interleaved");

print STDERR "\n\nConnection and disconnection time,",
  " 500 consumers 500 suppliers\n";
system ("Connect -ORBsvcconf ec.st.conf -consumers 500 -suppliers 500"
	." -connection_order interleaved");

exit 0;
