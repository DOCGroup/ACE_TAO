#!/usr/bin/perl
#$Id$

use strict;
use lib '../../../bin';
use PerlACE::Run_Test;

my $freq = 10;
my $num_events = 20;
my $result_dir = "RTEC_RESULT";
my $TAO_ROOT=@ENV{"TAO_ROOT"};

my $i;

mkdir $result_dir unless -d $result_dir;


for ($i=0; $i< 100; ++$i) {
  chdir "$TAO_ROOT/orbsvcs/FTRT_Event_Service/$result_dir";
  my $mark =  "m$i";
  mkdir $mark unless -d $mark;

  #start Event_Service

  my $FILEPATH = "$TAO_ROOT/orbsvcs/FTRT_Event_Service/$result_dir/$mark";
  my $es_ior = "$FILEPATH/es.ior";

  unlink ($es_ior) if -e $es_ior;

  chdir "$TAO_ROOT/orbsvcs/Event_Service";
  my $ES = new PerlACE::Process ("Event_Service", "-o $es_ior");

  $ES->Spawn ();

  if (PerlACE::waitforfile_timed ("$es_ior", 5) == -1) {
    print STDERR "ERROR: cannot find file <$es_ior>\n";
    $ES->Kill ();
    exit 1;
  }

  chdir "$TAO_ROOT/orbsvcs/FTRT_Event_Service";

  #start consumer


  my $client_options = "-i file://$es_ior -f $freq -k $num_events -ORBLogFile $FILEPATH";

  my $CONSUMER = new PerlACE::Process ("rtec_consumer", "$client_options/c0.txt");
  $CONSUMER->Spawn();

  #start supplier

  my $SUPPLIER = new PerlACE::Process ("rtec_supplier", "$client_options/s0.txt");
  $SUPPLIER->Spawn();

  #wait until consumer complete

  $CONSUMER->Wait;
  $ES->TerminateWaitKill(0);
}
