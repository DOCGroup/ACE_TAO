# $Id$
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# This is a Perl script that runs the client and all the other servers that
# are needed

$n = 1;
$leave = 0;
$ior = 0;
$done = "";
$debug = "";
$other = "";
$runonce = 0;

# configuration variables

$resource = "global";
$poalock = "thread";
$poa = "global";
$concurrency = "reactive";
$mult = "no";
$c_resource = "global";
$c_mult = "no";

for ($i = 0; $i <= $#ARGV; $i++)
{
  SWITCH: 
  {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?")
    {
      print "testall\n";
      print "  -resource {global, tss}\n";
      print "  -poalock {thread, null}\n";
      print "  -poa {global, tss}\n";
      print "  -concurrency {reactive, thread-per-connection}\n";
      print "  -mult {yes, no}\n";
      print "  -c_resource {global, tss}\n";
      print "  -c_mult {yes, no}\n";
      exit;
    }
    if ($ARGV[$i] eq "-resource")
    {
      $runonce = 1;
      $resource = $ARGV[$i + 1];
      $i++;
    }
    if ($ARGV[$i] eq "-poalock")
    {
      $runonce = 1;
      $poalock = $ARGV[$i + 1];
      $i++;
    }
    if ($ARGV[$i] eq "-poa")
    {
      $runonce = 1;
      $poa = $ARGV[$i + 1];
      $i++;
    }
    if ($ARGV[$i] eq "-concurrency")
    {
      $runonce = 1;
      $concurrency = $ARGV[$i + 1];
      $i++;
    }
    if ($ARGV[$i] eq "-mult")
    {
      $runonce = 1;
      $mult = $ARGV[$i + 1];
      $i++;
    }
    if ($ARGV[$i] eq "-c_resource")
    {
      $runonce = 1;
      $c_resource = $ARGV[$i + 1];
      $i++;
    }
    if ($ARGV[$i] eq "-c_mult")
    {
      $runonce = 1;
      $c_mult = $ARGV[$i + 1];
      $i++;
    }
  }
}

if ($runonce > 0)
{
  do_work ();
}
else
{
  pick_resource ();
}

sub pick_resource
{
  $resource = "global";
  pick_poalock ();
  $resource = "tss";
  pick_poalock ();
}

sub pick_poalock
{
  $poalock = "thread";
  pick_poa ();
  $poalock = "null";
  pick_poa ();
}

sub pick_poa 
{
  $poa = "global";
  pick_concurrency ();

  if ($resource ne "global")
  {
    $poa = "tss";
    pick_concurrency ();
  }
}

sub pick_concurrency
{
  $concurrency = "reactive";
  pick_mult_orbs ();
  $concurrency = "thread-per-connection";
  pick_mult_orbs ();
}

sub pick_mult_orbs
{
  if ($resource ne "global")
  {
    $mult = "yes";
    pick_c_resource ();
  }
  $mult = "no";
  pick_c_resource ();
}

sub pick_c_resource
{
  $c_resource = "global";
  pick_c_mult_orbs ();
  $c_resource = "tss";
  pick_c_mult_orbs ();
}

sub pick_c_mult_orbs
{
  if ($c_resource ne "global")
  {
    $c_mult = "yes";
    do_work ();
  }
  $c_mult = "no";
  do_work ();
}

sub make_s_conf
{
  open (FH, ">s.conf");
   
  select (FH);

  print "dynamic Resource_Factory Service_Object * TAO:_make_TAO_Resource_Factory() \"-ORBresources ".$resource." -ORBpoa $poa\"\n";
  print "dynamic Client_Strategy_Factory Service_Object * TAO:_make_TAO_Default_Client_Strategy_Factory()\n";
  print "dynamic Server_Strategy_Factory Service_Object * TAO:_make_TAO_Default_Server_Strategy_Factory() \"-ORBconcurrency $concurrency -ORBpoalock ".$poalock." -ORBdemuxstrategy dynamic -ORBtablesize 128\"\n";

  select (STDOUT);
  close (FH);
}

sub make_c_conf
{
  open (FH, ">c.conf");
   
  select (FH);

  print "dynamic Resource_Factory Service_Object * TAO:_make_TAO_Resource_Factory() \"-ORBresources $c_resource\"\n";
  print "dynamic Client_Strategy_Factory Service_Object * TAO:_make_TAO_Default_Client_Strategy_Factory()\n";
  print "dynamic Server_Strategy_Factory Service_Object * TAO:_make_TAO_Default_Server_Strategy_Factory() \"-ORBconcurrency reactive -ORBdemuxstrategy dynamic -ORBtablesize 128\"\n";

  select (STDOUT);
  close (FH);
}


sub do_work
{
  make_s_conf ();
  make_c_conf ();

  print "-------------------------------------------\n";
  print "Server: $resource $poalock $poa $concurrency $mult\n";
  print "Client: $c_resource $c_mult\n";

  if ($mult eq "yes") { $mt = "-sm"; } else { $mt = ""; }
  if ($c_mult eq "yes") { $cmt = "-cm"; } else { $cmt = ""; }

  system ("perl run_test.pl $other $mt $cmt -onewin -customconf");
}

