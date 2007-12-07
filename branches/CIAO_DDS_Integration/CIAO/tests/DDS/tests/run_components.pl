eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{'ACE_ROOT'}/bin";
use PerlACE::Run_Test;
 
$CIAO_ROOT = "$ENV{'CIAO_ROOT'}";
$TAO_ROOT = "$ENV{'TAO_ROOT'}";
$DAnCE = "$ENV{'CIAO_ROOT'}/DAnCE";
#$debug_param = "-ORBDebug -ORBDebugLevel 100";
$daemons_running = 0;
$em_running = 0;
$ns_running = 0;
$status = 0;
$nodes;
$shutdown = 0;
@ior_files;
$E = 0;
$EM = 0;

$nsior = PerlACE::LocalFile ("ns.ior");
unlink $nsior;

# first argument will be interpreted as .cdp file
die "please specifiy a .cdp file!" unless (@ARGV[0]);
$cdp_file = @ARGV[0];

sub gen_dat_name {
    my $datname = shift;
    $datname =~ s{.cdp}{.dat};
    return $datname;
}

sub get_node_list {
    my $cdp_file = shift;
    my $cdp;
    my $node_list; # array of hashes, containing node info
    my $nodeno = 0;
    my $portcount = 60001;
    my %nodenames;

    # read file into a string
    open (my $FILE, $cdp_file) or die "could not open $cdp_file!";
    while(<$FILE>) {
	$cdp .= $_;
    }
    close ($FILE) or die "could not close $cdp_file!";

    #do regex evaluation on string
    while ($cdp =~ m{\G.*?<instance.*?>.*?
		     <node>(\w+)<\/node>.*?
		     <\/instance>}xcgs ) {
	unless (exists($nodenames{$1})) {
	    $nodenames{$1} = true;
	    $node_list->[$nodeno]->{"node"} = $1; 
	    $node_list->[$nodeno]->{"ip"} = "localhost";
	    $node_list->[$nodeno++]->{"port"} = $portcount++;
	}
    }

    return $node_list;
}

sub write_dat_file {
    my $nodes = shift;
    my $datfile = shift;
    my $maxlen = 0;

    open (my $FILE, ">$datfile") or die "could not open $datfile!";

    for (my $i = 0; $i < scalar(@$nodes); ++$i) {
	my $thislen = length($nodes->[$i]->{"node"});
	if ($thislen > $maxlen) {
	    $maxlen = $thislen;
	}
    }

    for (my $i = 0; $i < scalar(@$nodes); ++$i) {
	print $FILE "\n" if $i;
	print $FILE $nodes->[$i]->{"node"} . 
	       " " x ($maxlen - length($nodes->[$i]->{"node"}) + 4) .
	       "corbaloc:iiop:" . 
	       $nodes->[$i]->{"ip"} . 
	       ":" .
	       $nodes->[$i]->{"port"} .
	       "/NodeManager"
    }

    close ($FILE) or die "could not close $datfile!";
}

sub get_ior_files {
    my $cdp_file = shift;
    my $cdp;
    my @iorfiles;

    # read file into a string
    open (my $FILE, $cdp_file) or die "could not open $cdp_file!";
    while(<$FILE>) {
	$cdp .= $_;
    }
    close ($FILE) or die "could not close $cdp_file!";

    #do regex evaluation on string
    while ($cdp =~ m{\G.*?<configProperty>.*?
		     <name>(\w+)<\/name>.*?
		     <string>(.*?)<\/string>.*?
		     <\/configProperty>}xcgs ) {
	# I'm interested in all ComponentIOR configProperties
	if ($1 == "ComponentIOR") {
	    push (@iorfiles, $2);
	}
    }

    return @iorfiles;
}

# Delete if there are any .ior files.
sub delete_output_files {
    for ($i = 0; $i < scalar(@$nodes); ++$i) {
        unlink $nodes->[$i]->{"node"} . ".ior";
    }
    unlink PerlACE::LocalFile ("EM.ior");
    unlink PerlACE::LocalFile ("DAM.ior");
    unlink PerlACE::LocalFile ("ns.ior");

    foreach my $ior_file (@ior_files) {
	unlink PerlACE::LocalFile ($ior_file);
    }

    unlink PerlACE::LocalFile (gen_dat_name($cdp_file));
}

sub kill_node_daemons {
  for ($i = 0; $i < scalar(@$nodes); ++$i) {
    $Daemons[$i]->Kill (); $Daemons[$i]->TimedWait (1);
  }
}

sub kill_open_processes {
  if ($daemons_running == 1) {
    kill_node_daemons ();
  }

  if ($em_running == 1) {
    $EM->Kill ();
    $EM->TimedWait (1);
  }

  if ($ns_running == 1) {
    $NS->Kill ();
    $NS->TimedWait (2);
  }

}

sub run_node_daemons {
  for ($i = 0; $i < scalar(@$nodes); ++$i)
  {
      $iorfile = $nodes->[$i]->{"node"} . ".ior";
      $iiop = "iiop://" . $nodes->[$i]->{"ip"} . ":" . $nodes->[$i]->{"port"};
      $node_app = "$CIAO_ROOT/bin/NodeApplication";

      $d_cmd = "$CIAO_ROOT/bin/NodeManager";
      $d_param = "-ORBEndpoint $iiop -s $node_app -o $iorfile -d 30 $debug_param";
      
      print $d_cmd . " " . $d_param . "\n";
      $Daemons[$i] = new PerlACE::Process ($d_cmd, $d_param);
      $result = $Daemons[$i]->Spawn ();
      push(@processes, $Daemons[$i]);

      if (PerlACE::waitforfile_timed ($iorfile,
                          $PerlACE::wait_interval_for_process_creation) == -1) {
          print STDERR
            "ERROR: The ior file of node daemon $i could not be found\n";
          for (; $i >= 0; --$i) {
            $Daemons[$i]->Kill (); $Daemons[$i]->TimedWait (1);
          }
          return -1;
      }
  }
  return 0;
}

$nodes = get_node_list ($cdp_file);
@ior_files = get_ior_files ($cdp_file);

delete_output_files ();

# Invoke naming service
$ENV{"CIAO_DEBUG_LEVEL"} = "20";
      
$NS = new PerlACE::Process ("$TAO_ROOT/orbsvcs/Naming_Service/Naming_Service", "-m 0 -ORBEndpoint iiop://localhost:60000 -o ns.ior");

$NS->Spawn ();

print STDERR "Starting Naming Service\n";

if (PerlACE::waitforfile_timed ($nsior, $PerlACE::wait_interval_for_process_creation) == -1)
{
    print STDERR "ERROR: cannot find naming service IOR file\n";
    $NS->Kill ();
    exit 1;
}

$Ns_running = 1;
      
# Invoke node daemons.
print "Invoking node daemons\n";
$status = run_node_daemons ();

if ($status != 0) {
    print STDERR "ERROR: Unable to execute the node daemons\n";
    kill_open_processes ();
    exit 1;
}

$daemons_running = 1;

my $dat_file = gen_dat_name($cdp_file);
write_dat_file ($nodes, $dat_file);

# Invoke execution manager.
print "Invoking execution manager\n";
$EM = new PerlACE::Process ("$CIAO_ROOT/bin/Execution_Manager",
			    "-o EM.ior -i $dat_file $debug_param");
$EM->Spawn ();

if (PerlACE::waitforfile_timed ("EM.ior",
				$PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR
	"ERROR: The ior file of execution manager could not be found\n";
    kill_open_processes ();
    exit 1;
}

$em_running = 1;

# Invoke executor - start the application -.
print "Invoking executor - start the application -\n";
$E =
    new PerlACE::Process ("$CIAO_ROOT/bin/plan_launcher",
			  "-p $cdp_file -k file://EM.ior -o DAM.ior $debug_param");

$E->SpawnWaitKill (5000);

foreach my $ior_file (@ior_files) {
    if (PerlACE::waitforfile_timed (
				    $ior_file,
				    $PerlACE::wait_interval_for_process_creation) == -1) {
	print STDERR "ERROR: The ior file $ior_file could not be found\n";
	kill_open_processes ();
	exit 1;
    }
}

print "\n*************************************************************************\n";
print "* Press any key to shutdown the test ...\n";
print "*************************************************************************\n\n";
$confirmation = <STDIN>;

# Invoke executor - stop the application -.
print "Invoking executor - stop the application -\n";
$E = new PerlACE::Process ("$CIAO_ROOT/bin/plan_launcher",
			   "-k file://EM.ior -i file://DAM.ior");
$E->SpawnWaitKill (3000);

print "Executor returned.\n";
print "Shutting down rest of the processes.\n";

delete_output_files ();
kill_open_processes ();

exit $status;
