eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../bin";

require ACEutils;

#event count
$ev_count = 2;
$status = 0;

# setup CosEC params such that..
# cos event service name = "cosec1"
# for ConsumerQOS: EventID = 21, SourceID = 6
# for SupplierQOS: EventID = 20, SourceID = 5.
$CosEC1_params = "-n cosec1 -e 21 -o 6 -p \"5 20\"";

# setup another CosEC params such that..
# cos event service name = "cosec2"
# for ConsumerQOS: EventID = 20, SourceID = 5
# for SupplierQOS: EventID = 21, SourceID = 6.
$CosEC2_params = "-n cosec2 -e 20 -o 5 -p \"6 21\"";

sub cosec_multiple_test1
{
    # first start the Naming service..
    $SV1 = Process::Create ("..".$DIR_SEPARATOR
			    ."..".$DIR_SEPARATOR
			    ."Naming_Server".$DIR_SEPARATOR
			    ."Naming_Server".$EXE_EXT, "");

    sleep 10;

    # now start the Rt EC..
    $SV2 = Process::Create ("..".$DIR_SEPARATOR
			    ."..".$DIR_SEPARATOR
			    ."Event_Service".$DIR_SEPARATOR
			    ."Event_Service".$EXE_EXT, "-t new");

    sleep 10;

    # now start the CosEC1..
    $SV3 = Process::Create ("..".$DIR_SEPARATOR
			    ."..".$DIR_SEPARATOR
			    ."CosEvent_Service".$DIR_SEPARATOR
			    ."CosEvent_Service".$EXE_EXT, $CosEC1_params);

    sleep 10;

    # now start the CosEC2..
    $SV4 = Process::Create ("..".$DIR_SEPARATOR
			    ."..".$DIR_SEPARATOR
			    ."CosEvent_Service".$DIR_SEPARATOR
			    ."CosEvent_Service".$EXE_EXT, $CosEC2_params);

    sleep 10;

    #start 1 consumer that uses CosEC1 to receive events
    $CONS = Process::Create ($EXEPREFIX."consumer".$EXE_EXT, "-n cosec1 -c $ev_count");

    sleep 10;

    #start 1 supplier  that uses CosEC2 to send events
    $SUPP = Process::Create ($EXEPREFIX."supplier".$EXE_EXT, "-n cosec2 -c $ev_count");

    sleep 10;

    #wait for the supplier to finish
    if ($SUPP->TimedWait (60) == -1) {
      print STDERR "ERROR: supplier timedout\n";
      $status = 1;
      $SUPP->Kill (); $SUPP->TimedWait (1);
    }


    #wait for the consumer to finish
    if ($CONS->TimedWait (60) == -1) {
      print STDERR "ERROR: consumer timedout\n";
      $status = 1;
      $CONS->Kill (); $CONS->TimedWait (1);
    }

    #----------
 #start 1 consumer that uses CosEC1 to receive events
    $CONS2 = Process::Create ($EXEPREFIX."consumer".$EXE_EXT, "-n cosec2 -c $ev_count");

    sleep 10;

    #start 1 supplier  that uses CosEC2 to send events
    $SUPP2 = Process::Create ($EXEPREFIX."supplier".$EXE_EXT, "-n cosec1 -c $ev_count");

    sleep 10;

    #wait for the supplier to finish
    if ($SUPP2->TimedWait (60) == -1) {
      print STDERR "ERROR: supplier2 timedout\n";
      $status = 1;
      $SUPP2->Kill (); $SUPP2->TimedWait (1);
    }


    #wait for the consumer to finish
    if ($CONS2->TimedWait (60) == -1) {
      print STDERR "ERROR: consumer2 timedout\n";
      $status = 1;
      $CONS2->Kill (); $CONS2->TimedWait (1);
    }

    #----------
    # cleanup..
    $SV1->Terminate ();
    $SV2->Terminate ();
    $SV3->Terminate ();
    $SV4->Terminate ();

    if ($SV1->TimedWait (5) == -1 ||
        $SV2->TimedWait (5) == -1 ||
        $SV3->TimedWait (5) == -1 ||
        $SV4->TimedWait (5)) {
      $SV1->Kill ();
      $SV2->Kill ();
      $SV3->Kill ();
      $SV4->Kill ();
      $SV1->TimedWait (1);
      $SV2->TimedWait (1);
      $SV3->TimedWait (1);
      $SV4->TimedWait (1);
      print STDERR "ERROR: couldn't terminate servers nicely\n";
      $status = 1;
    }
}

# Parse the arguments

for ($i = 0; $i <= $#ARGV; $i++)
{
  SWITCH:
  {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?")
    {
        print "usage: run_test.pl -e event_count -h help\n";
        exit;
    }
    if ($ARGV[$i] eq "-e")
    {
        $ev_count = $ARGV[$i + 1];
        $i++;
        last SWITCH;
    }
}
}

cosec_multiple_test1 ();

exit $status;
