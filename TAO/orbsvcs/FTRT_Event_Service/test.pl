#!/usr/bin/perla

#$Id$

use strict;
use lib '../../../bin';
use PerlACE::Run_Test;
use Proc::Background;

my $HOSTNAME=`hostname`;

my $client_args;
my $NameServicePort=$ENV{"ORBNameServicePort"};
my $result_dir="R";
my $iterations=100;
my $num_ecs;
my $num_consumers;
my $num_suppliers;
my $transaction_depth;
my $is_AMI="NO";
my $heartbeat="NA";
my $ns_args;

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
	shift(@ARGV)
}

mkdir $result_dir unless -d $result_dir;
my $TAO_ROOT=$ENV{"TAO_ROOT"};

my $ns_ior="$TAO_ROOT/orbsvcs/FTRT_Event_Service/$result_dir/ns.ior";
unlink ($ns_ior) if -e $ns_ior;
chdir "$TAO_ROOT/orbsvcs/Naming_Service";
$ns_args .= " -o $ns_ior";
my $NS = new PerlACE::Process ("Naming_Service", "$ns_args");

$NS->Spawn ();

if (PerlACE::waitforfile_timed ("$ns_ior", 5) == -1) {
    print STDERR "ERROR: cannot find file <$ns_ior>\n";
    $NS->Kill ();
    exit 1;
} 

$client_args .= " -ORBInitRef NameService=file://$ns_ior";

#starting the primary
my @ECS;
my $FILE_PATH="$TAO_ROOT/orbsvcs/FTRT_Event_Service/$result_dir";

chdir "$TAO_ROOT/orbsvcs/FTRT_Event_Service/Event_Service";
#$ECS[0]=Proc::Background->new("./ftrt_eventservice $client_args -p -d 3 2>&1 | tee  $FILE_PATH/ec0.txt")
$ECS[0]=Proc::Background->new("./ftrt_eventservice $client_args -p")
 || die "Cannot start Primary\n";
print "Primary Started\n";

sleep 5;

#starting backups
my $i;
for ($i=1; $i< $num_ecs; ++$i) {
#	$ECS[$i]=Proc::Background->new("./ftrt_eventservice $client_args -j -d 3 2>&1 | tee  $FILE_PATH/ec$i.txt")
        $ECS[$i]=Proc::Background->new("./ftrt_eventservice $client_args -j")
	 || die "Cannot start backup $i";
	print "backup $i started\n";
	sleep 10;
}

chdir "$TAO_ROOT/orbsvcs/tests/FtRtEvent";

#starting consumers

my @CONSUMERS;

for ($i=0 ; $i < $num_consumers ; ++$i){
	my $FILENAME="$FILE_PATH/c$i.txt";
	$CONSUMERS[$i] = Proc::Background->new("./consumer $client_args -k $iterations > $FILENAME 2>&1");
	print "consumer $i started\n";
}

#starting suppliers

my @SUPPLIERS;
for ($i=0 ; $i < $num_suppliers ; ++$i){
	my $FILENAME="$FILE_PATH/s$i.txt";
	$SUPPLIERS[$i] = Proc::Background->new("./supplier $client_args -k $iterations -d 1  2>&1 | tee $FILENAME");
	print "supplier $i started\n";
}

for ($i=0 ; $i < $num_consumers ; ++$i){
	$CONSUMERS[$i]->wait;
}

for ($i=1; $i< @ECS; ++$i) {
	$ECS[$i]->die;
}

$NS->TerminateWaitKill(0);

my @supplier_subscription;

chdir "$FILE_PATH";
foreach (<s*.txt>) {
  open (LINE, $_);
  while (<LINE>) {
        /latency = (\d+)/ && push(@supplier_subscription, $1);
  }
}

my @consumer_subscription;
my @event_time;

foreach(<c*.txt>) {
  open (LINE, $_);
  while (<LINE>) {
       /latency = (\d+)/     && push(@consumer_subscription, $1);
       /single trip time = (\d+)/ && push(@event_time, $1);
  }
}


my $sl = mean(@supplier_subscription);
my $cl = mean(@consumer_subscription);
my $em = mean(@event_time);
my $dev_em = dev(@event_time);


chdir "$TAO_ROOT/orbsvcs/FTRT_Event_Service";

open(RESULT, ">>$TAO_ROOT/orbsvcs/FTRT_Event_Service/result.txt") or die "cannot open result.txt";

write (RESULT);


format RESULT =
  @>>          @>>        @>>             @>>          @>>>       @>>      @>>>>>>>>>>    @>>>>>>>>>     @>>>>>>>>>       @######.###
$num_ecs, $num_suppliers, $num_consumers, $transaction_depth, $is_AMI, $heartbeat, $sl, $cl, $em, $dev_em
.

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
