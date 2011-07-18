# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use Env (ACE_ROOT);
use lib "$ACE_ROOT/bin";
use PerlACE::Run_Test;

$emior= PerlACE::LocalFile ("em.ior");
unlink $emior;
$plior= PerlACE::LocalFile ("pl.ior");
unlink $plior;

if (defined $ENV{'CIAO_ROOT'}) {
  $CIAO_ROOT = $ENV{'CIAO_ROOT'};
}
else {
  $CIAO_ROOT = "$ACE_ROOT/TAO/CIAO";
}

if (defined $ENV{'DANCE_ROOT'}) {
  $DANCE_ROOT = $ENV{'DANCE_ROOT'};
}
else {
  $DANCE_ROOT = "$ACE_ROOT/TAO/CIAO/DAnCE";
}


$NA = new PerlACE::Process ("$CIAO_ROOT/bin/ciao_componentserver");
$NA_cmd = $NA->Executable ();

$NA1 = new PerlACE::Process ("$DANCE_ROOT/bin/dance_node_manager",
                             "-ORBEndpoint iiop://localhost:11000 -s $NA_cmd -n Administrator_Node");

$NA2 = new PerlACE::Process ("$DANCE_ROOT/bin/dance_node_manager",
                             "-ORBEndpoint iiop://localhost:22000 -s $NA_cmd -n First_Receiver_Node");

$NA3 = new PerlACE::Process ("$DANCE_ROOT/bin/dance_node_manager",
                             "-ORBEndpoint iiop://localhost:33000 -s $NA_cmd -n Second_Receiver_Node");

$NA4 = new PerlACE::Process ("$DANCE_ROOT/bin/dance_node_manager",
                             "-ORBEndpoint iiop://localhost:44000 -s $NA_cmd -n Messenger_Node");

# Each Node Manager lauches a Node Application process.
open(STDIN, "<admin.dat");
$Ret1 = $NA1->Spawn ();
if($Ret1 == -1) {
  print STDERR "ERROR: Administrator returned <$Ret1>\n"
}

$Ret2 = $NA2->Spawn ();
if($Ret2 == -1) {
  print STDERR "ERROR: Receiver 1 returned <$Ret2>\n"
}

$Ret3 =  $NA3->Spawn ();
if($Ret3 == -1) {
  print STDERR "ERROR: Receiver 2 returned <$Ret3>\n"
}

$Ret4 =  $NA4->Spawn ();
if($Ret4 == -1) {
  print STDERR "ERROR: Messenger returned <$Ret4>\n"
}

#Start an Execution Manager
$EM = new PerlACE::Process ("$DANCE_ROOT/bin/dance_execution_manager", "-e$emior --node-map ApplicationNodeMap.dat");

$Ret5 = $EM->Spawn ();
if($Ret5 == -1) {
  print STDERR "ERROR: Execution Manager returned <$Ret5>\n"
}

if (PerlACE::waitforfile_timed ($emior, 5) == -1) {
  print STDERR "ERROR: cannot find file <$emior>\n";
  $EM->Kill();
  unlink $emior;
  exit 1;
}

sleep(5);
#Start the plan  launcher
$EX = new PerlACE::Process ("$DANCE_ROOT/bin/dance_plan_launcher",
                            "-x Application-flattened_Unhomed.cdp -k file://$emior ");

#Stop the plan launcher
$EX2 = new PerlACE::Process ("$DANCE_ROOT/bin/dance_plan_launcher",
                             "-k file://$emior -x Application-flattened_Unhomed.cdp");

$Ret6 = $EX->Spawn ();
if($Ret6 == -1) {
  print STDERR "ERROR: plan launcher returned <$Ret6>\n"
}

sleep(10);

$Ret7 = $EX2->Spawn ();
if($Ret7 == -1) {
  print STDERR "ERROR: plan launcher (shutdown) returned <$Ret7>\n"
}

sleep(5);

$NA1->Kill();
$NA2->Kill();
$NA3->Kill();
$NA4->Kill();
$EX->Kill();
$EX2->Kill();
$EM->Kill();

unlink $emior;
unlink $plior;

exit 0;
