# This is a Perl script that runs the client and all the other servers that
# are needed

use Win32::Process;

$nsiorfile = $ENV{TEMP}."\\qns_ior";

# amount of delay between running the 
$sleeptime = 2;

# variables for parameters

$nsport = 20002;
$clport = 20003;
$svport = 20004;
$ffport = 20005;
$gfport = 20006;

# other variables

$n = 1;
$leave = 0;
$ior = 0;
$done = "";
$debug = "";
$mt = "";
$other = "";

sub read_nsior
{
	open (FH, "<".$nsiorfile);

	read (FH, $ior, 255);

	close (FH);
}

sub name_server
{
	my $exe = sprintf ("..\\..\\orbsvcs\\Naming_Service\\Naming_Service.exe %s". 
	                   " -ORBport %d -ORBobjrefstyle url -o %s",
	                   $other,
	                   $nsport,
	                   $nsiorfile);

	Win32::Process::Create ($NS, "..\\..\\orbsvcs\\Naming_Service\\Naming_Service.exe",
	                        $exe, 1, CREATE_NEW_CONSOLE, ".");
}

sub server
{
	my $exe = sprintf ("server.exe %s %s -ORBport %d -ORBobjrefstyle url -ORBnameserviceior %s -ORBsvcconf server.conf",
	                   $other,
			           $debug,
			           $svport,
			           $ior);

	Win32::Process::Create ($SV, "server.exe",
	                        $exe, 1, CREATE_NEW_CONSOLE, ".");
}

sub factory_finder
{
	my $exe = sprintf ("Factory_Finder.exe %s -ORBport %d -ORBobjrefstyle url -ORBnameserviceior %s -ORBsvcconf svc.conf",
	                   $other,
			           $ffport,
			           $ior);
	
	Win32::Process::Create ($FF, "Factory_Finder.exe",
	                        $exe, 1, CREATE_NEW_CONSOLE, ".");

}

sub generic_factory
{
	my $exe = sprintf ("Generic_Factory.exe %s -ORBport %d -ORBobjrefstyle url -ORBnameserviceior %s -ORBsvcconf svc.conf",
	                   $other,
			           $gfport,
			           $ior);

	Win32::Process::Create ($GF, "Generic_Factory.exe",
	                        $exe, 1, CREATE_NEW_CONSOLE, ".");

}

sub client
{
	my $exe = sprintf ("client.exe %s %s %s -ORBobjrefstyle url -ORBport %d -ORBnameserviceior %s -ORBsvcconf client.conf",
	                   $other,
			           $debug,
			           $mt,
			           $clport,
			           $ior);

#	Win32::Process::Create ($CL, "client.exe",
#	                        $exe, 1, 0, ".");
	system ($exe);

}

# Parse the arguments

for ($i = 0; $i <= $#ARGV; $i++)
{
	SWITCH: {
		if ($ARGV[i] eq "-h" || $ARGV[i] eq "-?")
		{
			print "run_test [-n num] [-leave] [-d] [-h] [-m] [-ns|sv|ff|cl|gf]\n";
			print "\n";
			print "-n num              -- runs the client num times\n";
			print "-leave              -- leaves the servers running and their windows open\n";
			print "-d	               -- runs each in debug mode\n";
			print "-h                  -- prints this information\n";
			print "-m                  -- use more than one thread in the client\n";
			print "-ns -sv -ff -cl -gf -- runs only one of the executables\n";
			exit;
		}
		if ($ARGV[i] eq "-n")
		{
			$n = $ARGV[i + 1];
			$i++;
			last SWITCH;
		}
		if ($ARGV[i] eq "-d")
		{
			$debug = "-d";
			last SWITCH;
		}
		if ($ARGV[i] eq "-m")
		{
			$mt = "-m";
			last SWITCH;
		}
		if ($ARGV[i] eq "-leave")
		{
			$leave = 1;
			last SWITCH;
		}
		if ($ARGV[i] eq "-ns")
		{
			name_server ();
			exit;
		}
		if ($ARGV[i] eq "-sv")
		{
			server ();
			exit;
		}
		if ($ARGV[i] eq "-ff")
		{
			factory_finder ();
			exit;
		}
		if ($ARGV[i] eq "-gf")
		{
			generic_factory ();
			exit;
		}
		if ($ARGV[i] eq "-cl")
		{
			client;
			exit;
		}
		# all other args are ignored.
	}
}

name_server ();

read_nsior ();

sleep $sleeptime;

server ();

sleep $sleeptime;

factory_finder ();

sleep $sleeptime;

generic_factory ();

sleep $sleeptime;

client ();

if (leave == 0)
{
	$GF->Kill (0);
	$FF->Kill (0);
	$SV->Kill (0);
	$NS->Kill (0);
}