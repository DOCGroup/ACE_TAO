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

$prefix = "." . $DIR_SEPARATOR;
$status = 0;

print STDERR "\n\nThroughput/Latency single threaded configuration\n";
$T = Process::Create ($prefix . "Throughput".$EXE_EXT,
		      " -ORBsvcconf ec.st.conf "
		      . "-burstsize 100000 -burstcount 1");
if ($T->TimedWait (60) == -1) {
  print STDERR "ERROR: Test timedout\n";
  $status = 1;
  $T->Kill (); $T->TimedWait (1);
}


print STDERR "\n\nThroughput/Latency MT-safe configuration\n";
$T = Process::Create ($prefix . "Throughput".$EXE_EXT,
		      " -burstsize 100000"
		      ." -burstcount 1");
if ($T->TimedWait (60) == -1) {
  print STDERR "ERROR: Test timedout\n";
  $status = 1;
  $T->Kill (); $T->TimedWait (1);
}


print STDERR "\n\nThroughput/Latency MT-safe configuration, 4 consumers\n";
$T = Process::Create ($prefix . "Throughput".$EXE_EXT,
		      " -burstsize 100000"
		      ." -burstcount 1 -consumers 4");
if ($T->TimedWait (60) == -1) {
  print STDERR "ERROR: Test timedout\n";
  $status = 1;
  $T->Kill (); $T->TimedWait (1);
}


print STDERR "\n\nThroughput/Latency MT-safe configuration,",
  " 4 consumers 4 suppliers\n";
$T = Process::Create ($prefix . "Throughput".$EXE_EXT,
		      " -burstsize 100000"
		      ." -burstcount 1 -consumers 4 -suppliers 4");
if ($T->TimedWait (60) == -1) {
  print STDERR "ERROR: Test timedout\n";
  $status = 1;
  $T->Kill (); $T->TimedWait (1);
}


print STDERR "\n\nThroughput/Latency MT-safe configuration,",
  " 4 consumers 4 suppliers\n";
$T = Process::Create ($prefix . "Throughput".$EXE_EXT,
		      " -burstsize 100000"
		      ." -burstcount 1 -consumers 4 -suppliers 4"
		      ." -consumers_tshift 0 -suppliers_tshift 0");
if ($T->TimedWait (60) == -1) {
  print STDERR "ERROR: Test timedout\n";
  $status = 1;
  $T->Kill (); $T->TimedWait (1);
}

print STDERR "\n\nConnection and disconnection time,",
  " 100 consumers 100 suppliers\n";
$T = Process::Create ($prefix . "Connect".$EXE_EXT,
		      " -consumers 100 -suppliers 100"
		      ." -connection_order interleaved");
if ($T->TimedWait (60) == -1) {
  print STDERR "ERROR: Test timedout\n";
  $status = 1;
  $T->Kill (); $T->TimedWait (1);
}


print STDERR "\n\nConnection and disconnection time,",
  " 500 consumers 500 suppliers\n";
$T = Process::Create ($prefix . "Connect".$EXE_EXT,
		      " -consumers 500 -suppliers 500"
		      ." -connection_order interleaved");
if ($T->TimedWait (60) == -1) {
  print STDERR "ERROR: Test timedout\n";
  $status = 1;
  $T->Kill (); $T->TimedWait (1);
}

exit $status;
