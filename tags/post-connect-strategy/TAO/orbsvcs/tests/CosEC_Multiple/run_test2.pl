eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../bin";

require ACEutils;

$con_count = 10;
$sup_count = 10;
$ev_count = 8;
$status = 0;

sub cosec_multiple_test2
{
    # first start the Naming service..
    $SV1 = Process::Create ($EXEPREFIX."../../Naming_Service/Naming_Server".$EXE_EXT, "");

    sleep 10;

    # now start the Rt EC..
    $SV2 = Process::Create ($EXEPREFIX."../../Event_Service/Event_Service".$EXE_EXT, "");

    sleep 10;

    # now start the CosEC1..
    $SV3 = Process::Create ($EXEPREFIX."../../CosEvent_Service/CosEvent_Service".$EXE_EXT, "");

    sleep 10;

    # now start the consumers
    for ($cntr1 = 0; $cntr1 < $con_count ; $cntr1+=1)
    {
        print "creating consumer# $cntr1\n";
        $CONS = Process::Create ($EXEPREFIX."consumer".$EXE_EXT,
                                 "-c $ev_count");
        sleep 10;
    }


    # now start the suppliers
    for ($cntr2 = 0; $cntr2 < $con_count ; $cntr2+=1)
    {
        print "creating supplier# $cntr2\n";
        $SUPP = Process::Create ($EXEPREFIX."supplier".$EXE_EXT,
                                 "-c $ev_count");
        sleep 10;
    }

    print "waiting for the last supplier to finish\n";
    if ($SUPP->TimedWait (60) == -1) {
      print STDERR "ERROR: supplier timedout\n";
      $status = 1;
      $SUPP->Kill (); $SUPP->TimedWait (1);
    }


    print "waiting for the last consumer to finish\n";
    if ($CONS->TimedWait (60) == -1) {
      print STDERR "ERROR: consumer timedout\n";
      $status = 1;
      $CONS->Kill (); $CONS->TimedWait (1);
    }


    print "cleanup...\n";
    $SV1->Kill ();
    $SV2->Kill ();
    $SV3->Kill ();

    $SV1->TimedWait (1);
    $SV2->TimedWait (1);
    $SV3->TimedWait (1);

    print "done!.\n";
}

# Parse the arguments

for ($i = 0; $i <= $#ARGV; $i++)
{
  SWITCH:
  {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?")
    {
        print "usage: run_test2.pl -e event_count -c consumers -s suppliers -h help\n";
        exit;
    }
    if ($ARGV[$i] eq "-e")
    {
        $ev_count = $ARGV[$i + 1];
        $i++;
        last SWITCH;
    }
    if ($ARGV[$i] eq "-c")
    {
        $con_count = $ARGV[$i + 1];
        $i++;
        last SWITCH;
    }
    if ($ARGV[$i] eq "-s")
    {
        $sup_count = $ARGV[$i + 1];
        $i++;
        last SWITCH;
    }
}
}


cosec_multiple_test2 ();

exit $status;
