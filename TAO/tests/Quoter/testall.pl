# This is a Perl script that runs the client and all the other servers that
# are needed

$n = 1;
$leave = 0;
$ior = 0;
$done = "";
$debug = "";
$mt = "";
$other = "";

pick_resource ();

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
#  if ($resource ne "tss")
#  {
    $concurrency = "thread-per-connection";
    pick_mult_orbs ();
#  }
}

sub pick_mult_orbs
{
  if ($resource ne "global")
  {
    $multorb = "yes";
    pick_c_resource ();
  }
  $multorb = "no";
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
    $c_multorb = "yes";
    do_work ();
  }
  $c_multorb = "no";
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
  print "Server: $resource $poalock $poa $concurrency $multorb\n";
  print "Client: $c_resource $c_multorb\n";

  if ($multorb eq "yes") { $mt = "-sm"; } else { $mt = ""; }
  if ($c_multorb eq "yes") { $cmt = "-cm"; } else { $cmt = ""; }

  system ("perl run_test.pl $mt $cmt -customconf");
}

