#!/usr/bin/perl

#$Id$

use strict;
use lib '../../../bin';
use PerlACE::Run_Test;
#use Proc::Background;

my $HOSTNAME=`hostname`;

my $client_args;
my $NameServicePort=$ENV{"ORBNameServicePort"};
my $result_dir="R";
my $iterations=10;
my $num_ecs = 2;
my $num_consumers = 1;
my $num_suppliers = 1;
my $transaction_depth = 1;
my $is_AMI="NO";
my $heartbeat="NA";
my $ns_args;
my $mark;
my $freq=1;

$NameServicePort = 10000 unless defined($NameServicePort);
$ENV{"ORBNameServicePort"} = $NameServicePort;

while (defined($ARGV[0]))
{
	$_=$ARGV[0];
	if (/-num_ecs/) {
		 $num_ecs=$ARGV[1];
         $result_dir .= "_ES$num_ecs";
         shift(@ARGV);
	}
	elsif (/-num_suppliers/) {
		 $num_suppliers=$ARGV[1];
         $result_dir .= "_NS$num_suppliers";
         shift(@ARGV);
	}
	elsif (/-num_consumers/) {
		 $num_consumers=$ARGV[1];
         $result_dir .= "_NC$num_consumers";
         shift(@ARGV);
	}
	elsif (/-transaction_depth/) {
	        $transaction_depth=$ARGV[1];
		$ENV{"FTEC_TransactionDepth"}=$ARGV[1];
        $result_dir .= "_TD$ARGV[1]";
        shift(@ARGV);
	}
	elsif (/-AMI/) {
	        $is_AMI="YES";
		$ENV{"FTEC_REPLCATION_STRATEGY"}="AMI";
                $result_dir .= "_AMI";
	}
	elsif (/-hb/) {
	        $heartbeat=$ARGV[1];
		$ENV{"FTEC_DETECTOR_TRANSPORT_PROTOCOL"}="sctp";
                $ENV{"FTEC_HEART_BEAT"}=$ARGV[1];
                $ns_args = "-ORBEndPoint sciop://$HOSTNAME:$NameServicePort";
                $client_args .= " -ORBEndpoint sciop:// ";
                $result_dir .= "_HB$ARGV[1]";
                shift(@ARGV);
	}
	elsif (/-iteration/) {
		$iterations=$ARGV[1];
                shift(@ARGV);
        }
        elsif (/-frequency/) {
                $result_dir .= "_F$ARGV[1]";
                $freq=$ARGV[1];
                shift(@ARGV);
        }
        elsif (/-mark/) {
                $mark="m$ARGV[1]";
                shift(@ARGV);
        }
	shift(@ARGV);
}

mkdir $result_dir unless -d $result_dir;
chdir "$result_dir";
mkdir $mark unless -d $mark;

my $TAO_ROOT=$ENV{"TAO_ROOT"};
chdir "$TAO_ROOT/orbsvcs/FTRT_Event_Service";

my $rd=$result_dir;
$result_dir .= "/$mark";



my $ns_ior="$TAO_ROOT/orbsvcs/FTRT_Event_Service/$result_dir/ns.ior";
unlink ($ns_ior) if -e $ns_ior;
chdir "$TAO_ROOT/orbsvcs/Naming_Service";
$ns_args .= " -o $ns_ior";
my $NS = new PerlACE::Process ("Naming_Service", "$ns_args");

$NS->Spawn ();

if (PerlACE::waitforfile_timed ("$ns_ior", 100) == -1) {
    print STDERR "ERROR: cannot find file <$ns_ior>\n";
    $NS->Kill ();
    exit 1;
}

$client_args .= " -ORBInitRef NameService=file://$ns_ior";

#starting the primary
my @ECS;
my $FILE_PATH="$TAO_ROOT/orbsvcs/FTRT_Event_Service/$result_dir";
my $pid;

system("ps -u hh1 | grep ftrt");

chdir "$TAO_ROOT/orbsvcs/FTRT_Event_Service/Event_Service";
my $crash_msg = $iterations/2;
$ECS[0]= new PerlACE::Process ("ftrt_eventservice", "$client_args -p -f $crash_msg -ORBLogFile $FILE_PATH/ec0.txt");
$ECS[0]->Spawn ();
print "primary started\n";

sleep 2;


#starting backups
my $i;
for ($i=1; $i< $num_ecs; ++$i) {
	my $FILENAME="$FILE_PATH/ec$i.txt";
        $ECS[$i]=new PerlACE::Process ("ftrt_eventservice", "$client_args -j -ORBLogFile $FILE_PATH/ec$i.txt");
	$ECS[$i]->Spawn ();
	print "backup $i started\n";
	sleep 2;
}

chdir "$TAO_ROOT/orbsvcs/tests/FtRtEvent";

#starting consumers

my @CONSUMERS;

for ($i=0 ; $i < $num_consumers ; ++$i){
	my $FILENAME="$FILE_PATH/c$i.txt";
        my $IOR="$FILE_PATH/c$i.ior";
	$CONSUMERS[$i] = new PerlACE::Process ("ftrtec_consumer", "$client_args -k $iterations -p $IOR -ORBLogFile  $FILENAME");
	$CONSUMERS[$i]->Spawn();
	print "consumer $i started\n";
}

#starting suppliers

my $supplier_status;

my @SUPPLIERS;
for ($i=0 ; $i < $num_suppliers ; ++$i){
	my $FILENAME="$FILE_PATH/s$i.txt";
        my $IOR="$FILE_PATH/c$i.ior";
	print "starting supplier $i\n";
	$SUPPLIERS[$i] = new PerlACE::Process ("ftrtec_supplier", "$client_args -k $iterations -f $freq -p $IOR -ORBLogFile $FILENAME");
	my $status = $SUPPLIERS[$i]->SpawnWaitKill(3+$iterations*2/$freq);
	$supplier_status = -1 if ($status == -1);
}

for ($i=0 ; $i < $num_consumers ; ++$i){
        $CONSUMERS[$i]->TerminateWaitKill(2);
}

for ($i=0; $i< @ECS; ++$i) {
        $ECS[$i]->TerminateWaitKill(0);
}

$NS->TerminateWaitKill(0);

#my @supplier_subscription;

#chdir "$FILE_PATH";
#foreach (<s*.txt>) {
#  open (LINE, $_);
#  while (<LINE>) {
#        /latency = (\d+)/ && push(@supplier_subscription, $1);
#  }
#}

#my @consumer_subscription;
#my @event_time;
#my @losts;

#foreach(<c*.txt>) {
#  open (LINE, $_);
#  while (<LINE>) {
#       /latency = (\d+)/     && push(@consumer_subscription, $1);
#       /elapsed time = (\d+)/ && push(@event_time, $1);
#       /(\d+) events lost/ && push(@losts, $1);
#  }
#}


#my $sl = mean(@supplier_subscription);
#my $cl = mean(@consumer_subscription);
#my $em = mean(@event_time);
#my $dev_em = dev(@event_time);
#my $lost = mean(@losts);


#chdir "$TAO_ROOT/orbsvcs/FTRT_Event_Service";

#open(RESULT, ">>$TAO_ROOT/orbsvcs/FTRT_Event_Service/$rd/result.txt") or die "cannot open result.txt";

#write (RESULT);


#format RESULT =
#  @>>    @>>    @>>    @>>    @>>>   @>>   @>>    @>>>>>>>>>>    @>>>>>>>>>     @>>>>>>>>>       @######.###    @>>
#$num_ecs, $num_suppliers, $num_consumers, $transaction_depth, $is_AMI, $heartbeat, $freq ,$sl, $cl, $em, $dev_em, $lost
#.

sub mean {
  my $count = @_;
  my $sum=0;
  my $value;
  foreach $value (@_) {
    $sum += $value;
  }
  return $sum/$count;
}

sub dev  {
  my $count = @_;
  my $result=0;
  my $average=mean(@_);
  my $value;
  foreach $value (@_) {
    my $tmp = $value-$average;
    $result += $tmp*$tmp;
  }

  return sqrt($result)/$count;
}
