# $Id$
#   Win32 auto_compile script.

use File::Find;
use Cwd;

if (!$ENV{ACE_ROOT}) {
    $ACE_ROOT = getcwd ()."\\";
    warn "ACE_ROOT not defined, defaulting to ACE_ROOT=$ACE_ROOT";
}
else {
    $ACE_ROOT = $ENV{ACE_ROOT};
}

@directories = ();

@ace_dirs = ("$ACE_ROOT\\ace",
             "$ACE_ROOT\\ACEXML\\common",
             "$ACE_ROOT\\ACEXML\\parser",
             "$ACE_ROOT\\ACEXML\\apps",
             "$ACE_ROOT\\ACEXML\\tests",
             "$ACE_ROOT\\ACEXML\\examples",
             "$ACE_ROOT\\apps",
             "$ACE_ROOT\\ASNMP",
             "$ACE_ROOT\\examples",
             "$ACE_ROOT\\netsvcs",
             "$ACE_ROOT\\performance-tests",
             "$ACE_ROOT\\tests",
             "$ACE_ROOT\\websvcs");

$debug = 0;
$verbose = 0;
$print_status = 0;
$Ignore_errors = 0;              # By default, bail out if an error occurs.
$Use_MPC = 0;
$Build_DLL = 0;
$Build_LIB = 0;
$Build_Debug = 0;
$Build_Release = 0;
$Build_All = 1;
$build_core_only = 0;
$Build_Cmd = "/BUILD";
$use_custom_dir = 0;
$useenv = '';

# Find_dsp will search a directory for *.dsp files and return a list
# of strings that include the project name and the configuration
sub Find_dsp (@)
{
    my (@dir) = @_;
    @array = ();
    my @config_array = ();

    # wanted is only used for the File::Find
    sub wanted
    {
        $array[++$#array] = $File::Find::name if ($File::Find::name =~ /\.dsp$/i);
    }

    # get_config grabs the configurations out of a dsp file.
    sub get_config ($)
    {
        my ($file) = @_;
        my @configs = ();

        print "Looking at $file\n" if ($verbose);

        open (DSP, "< $file") || die $!;

        while (<DSP>)
        {
            push @configs, $1 if (/# Name \"([^\"]+)\"/);
        }

        close (DSP);
        return @configs;
    }

    unshift @dir, (\&wanted);

    find @dir;

    for ($i = 0; $i <= $#array; ++$i) {
        my $filename = "$array[$i]";

        $filename =~ s@/./@/@g;
        $filename =~ s@/@\\@g;
        my @dsp_configs = get_config ($array[$i]);

        for ($j = 0; $j <= $#dsp_configs; ++$j) {
          push @config_array, "$filename--$dsp_configs[$j]";
        }
    }

    return @config_array;
}


# Build_Config takes in a string of the type "project--configuration" and
# runs msdev to build it.
sub Build_Config ($)
{
    my ($arg) = @_;
    my ($project, $config) = split /--/, $arg;

    return Build ($project, $config);
}

# Build
sub Build ($$)
{
  my ($project, $config) = @_;

  if ($debug == 1) {
    print "$project\n";
    return 0;
  }
  else {
    print "Auto_compiling $project : $config\n";

    print "Building $project $config\n" if $verbose;

    return system ("msdev.com $project /MAKE \"$config\" $Build_Cmd $useenv");
  }
}

# Only builds the core libraries.
sub Build_Core ()
{
    print STDERR "Building Core of ACE/TAO\n" if ($print_status == 1);
    print "Building Core of ACE/TAO\n" if ($verbose == 1);

    print "Build \n" if ($verbose);
    print "Debug " if ($verbose) && ($Build_Debug);
    print "Release " if ($verbose) && ($Build_Release);
    print "DLL " if ($verbose) && ($Build_DLL);
    print "LIB " if ($verbose) && ($Build_LIB);
    print "\n" if ($verbose);

    my @core_list = ();

    if ($Build_DLL) {
        @core_list = ($ACE_ROOT."\\TAO\\TAOACE.dsw");

        foreach $c (@core_list) {
            if ($Build_Debug) {
                $Status = Build ($c, "ALL - Win32 Debug");
                return if $Status != 0 && !$Ignore_errors;
            }
            if ($Build_Release) {
                $Status = Build ($c, "ALL - Win32 Release");
                return if $Status != 0 && !$Ignore_errors;
            }
        }
    }
    if ($Build_LIB) {
      if ($Use_MPC == 1) {
        @core_list = ($ACE_ROOT."\\ace\\ace.dsp",
                      $ACE_ROOT."\\Kokyu\\Kokyu.dsp",
                      $ACE_ROOT."\\ACEXML\\parser\\parser\\ACEXML_Parser.dsp",
                      $ACE_ROOT."\\ACEXML\\common\\ACEXML.dsp",
                      $ACE_ROOT."\\apps\\gperf\\src\\gperf.dsp",
                      $ACE_ROOT."\\TAO\\TAO_IDL\\TAO_IDL_BE.dsp",
                      $ACE_ROOT."\\TAO\\TAO_IDL\\TAO_IDL_FE.dsp",
                      $ACE_ROOT."\\TAO\\TAO_IDL\\TAO_IDL_EXE.dsp",
                      $ACE_ROOT."\\TAO\\tao\\TAO.dsp",
                      $ACE_ROOT."\\TAO\\tao\\Valuetype\\Valuetype.dsp",
                      $ACE_ROOT."\\TAO\\tao\\IORInterceptor\\IORInterceptor.dsp",
                      $ACE_ROOT."\\TAO\\tao\\ObjRefTemplate\\ObjRefTemplate.dsp",
                      $ACE_ROOT."\\TAO\\tao\\PortableServer\\PortableServer.dsp",
                      $ACE_ROOT."\\TAO\\tao\\DynamicAny\\DynamicAny.dsp",
                      $ACE_ROOT."\\TAO\\tao\\IORManipulation\\IORManipulation.dsp",
                      $ACE_ROOT."\\TAO\\tao\\IORTable\\IORTable.dsp",
                      $ACE_ROOT."\\TAO\\tao\\Messaging\\Messaging.dsp",
                      $ACE_ROOT."\\TAO\\tao\\DynamicInterface\\DynamicInterface.dsp",
                      $ACE_ROOT."\\TAO\\tao\\Strategies\\Strategies.dsp",
                      $ACE_ROOT."\\TAO\\tao\\RTCORBA\\RTCORBA.dsp",
                      $ACE_ROOT."\\TAO\\tao\\RTPortableServer\\RTPortableServer.dsp",
                      $ACE_ROOT."\\TAO\\tao\\Utils\\Utils.dsp",
                      $ACE_ROOT."\\TAO\\tao\\RTScheduling\\RTScheduler.dsp",
                      $ACE_ROOT."\\TAO\\tao\\IFR_Client\\IFR_Client.dsp",
                      $ACE_ROOT."\\TAO\\tao\\TypeCodeFactory\\TypeCodeFactory.dsp",
                      $ACE_ROOT."\\TAO\\orbsvcs\\orbsvcs\\Svc_Utils.dsp",
                      $ACE_ROOT."\\TAO\\orbsvcs\\orbsvcs\\CosNaming.dsp",
                      $ACE_ROOT."\\TAO\\orbsvcs\\orbsvcs\\CosProperty.dsp",
                      $ACE_ROOT."\\TAO\\orbsvcs\\orbsvcs\\CosTrading.dsp",
                      $ACE_ROOT."\\TAO\\orbsvcs\\orbsvcs\\AV.dsp",
                      $ACE_ROOT."\\TAO\\orbsvcs\\orbsvcs\\RTEvent.dsp",
                      $ACE_ROOT."\\TAO\\orbsvcs\\orbsvcs\\RTSched.dsp",
                      $ACE_ROOT."\\TAO\\orbsvcs\\orbsvcs\\RTSchedEvent.dsp",
                      $ACE_ROOT."\\TAO\\orbsvcs\\orbsvcs\\RTOLDEvent.dsp",
                      $ACE_ROOT."\\TAO\\orbsvcs\\orbsvcs\\CosEvent.dsp",
                      $ACE_ROOT."\\TAO\\orbsvcs\\orbsvcs\\RTKokyuEvent.dsp",
                      $ACE_ROOT."\\TAO\\orbsvcs\\orbsvcs\\CosConcurrency.dsp",
                      $ACE_ROOT."\\TAO\\orbsvcs\\orbsvcs\\CosLifeCycle.dsp",
                      $ACE_ROOT."\\TAO\\orbsvcs\\orbsvcs\\CosTime.dsp",
                      $ACE_ROOT."\\TAO\\orbsvcs\\orbsvcs\\CosNotification.dsp",
                      $ACE_ROOT."\\TAO\\orbsvcs\\orbsvcs\\DsLogAdmin.dsp",
                      $ACE_ROOT."\\TAO\\orbsvcs\\orbsvcs\\DsEventLogAdmin.dsp",
                      $ACE_ROOT."\\TAO\\orbsvcs\\orbsvcs\\DsNotifyLogAdmin.dsp",
                      $ACE_ROOT."\\TAO\\orbsvcs\\orbsvcs\\FTORB_Utils.dsp",
                      $ACE_ROOT."\\TAO\\orbsvcs\\orbsvcs\\FT_ClientORB.dsp",
                      $ACE_ROOT."\\TAO\\orbsvcs\\orbsvcs\\FT_ServerORB.dsp",
                      $ACE_ROOT."\\TAO\\orbsvcs\\orbsvcs\\PortableGroup.dsp",
                      $ACE_ROOT."\\TAO\\orbsvcs\\orbsvcs\\CosLoadBalancing.dsp",
                      $ACE_ROOT."\\TAO\\orbsvcs\\orbsvcs\\IFRService.dsp",
                      $ACE_ROOT."\\TAO\\orbsvcs\\orbsvcs\\RT_Notification.dsp",
                      $ACE_ROOT."\\TAO\\orbsvcs\\Naming_Service\\Naming_Service.dsp",
                      $ACE_ROOT."\\tests\\Test_Output.dsp");
         } 


      if ($Use_MPC == 1) {
        foreach $c (@core_list) {
            if ($Build_Debug) {
                $Status = Build ($c, "ALL - Win32 Debug");
                return if $Status != 0 && !$Ignore_errors;
            }
            if ($Build_Release) {
                $Status = Build ($c, "ALL - Win32 Release");
                return if $Status != 0 && !$Ignore_errors;
            }
        }
      }
    }
}

sub Build_All ()
{
    my @configurations = Find_dsp (@directories);

    print STDERR "First pass (libraries)\n" if ($print_status == 1);
    print "\nmsvc_auto_compile: First Pass (libraries)\n";

    foreach $c (@configurations) {
        if ($Build_All
            || ($Build_DLL && $Build_Debug && $c =~ /Win32 Debug/)
            || ($Build_DLL && $Build_Release && $c =~ /Win32 Release/)
            || ($Build_LIB && $Use_MPC == 0 && $Build_Debug && $c =~ /Win32 Static Debug/)
            || ($Build_LIB && $Use_MPC == 0 && $Build_Release && $c =~ /Win32 Static Release/)
            || ($Build_LIB && $Use_MPC == 1 && $Build_Debug && $c =~ /Win32 Debug/)
            || ($Build_LIB && $Use_MPC == 1 && $Build_Release && $c =~ /Win32 Release/))
        {
            my $Status = 0;
            $Status = Build_Config ($c)
                if (($c =~ /Library/) || ($c =~ / DLL /) || ($c =~ / LIB /));
            return if ($Status != 0 && !$Ignore_errors);
        }
    }


    print STDERR "Second pass \n" if ($print_status == 1);
    print "\nmsvc_auto_compile: Second Pass\n";

    $count = 0;
    foreach $c (@configurations) {
        print STDERR "Configuration ".$count++." of ".$#configurations."\n" if ($print_status == 1);
        Build_Config ($c)
        if (($Build_DLL && $Build_Debug && $c =~ /Win32 Debug/)
           || ($Build_DLL && $Build_Release && $c =~ /Win32 Release/)
           || ($Build_LIB && $Use_MPC == 0 && $Build_Debug && $c =~ /Win32 Static Debug/)
           || ($Build_LIB && $Use_MPC == 0 && $Build_Release && $c =~ /Win32 Static Release/)
           || ($Build_LIB && $Use_MPC == 1 && $Build_Debug && $c =~ /Win32 Debug/)
           || ($Build_LIB && $Use_MPC == 1 && $Build_Release && $c =~ /Win32 Release/));
    }
}


## Parse command line argument
while ( $#ARGV >= 0  &&  $ARGV[0] =~ /^(-|\/)/ )
{
    if ($ARGV[0] =~ '-k') {             # Ignore errors
        print "Ignore errors\n" if ( $verbose );
        $Ignore_errors = 1;
    }
    elsif ($ARGV[0] =~ /^-d$/i) {       # debug
        $debug = 1;
    }
    elsif ($ARGV[0] =~ '-v') {          # verbose mode
        $verbose = 1;
    }
    elsif ($ARGV[0] =~ '-s') {          # status messages
        $print_status = 1;
    }
    elsif ($ARGV[0] =~ '-u') {          # USEENV
        print "Using Environment\n" if ($verbose);
        $useenv = '/USEENV';
    }
    elsif ($ARGV[0] =~ '-CORE') {       # Build the core of ace/tao
        print "Building only Core\n" if ( $verbose );
        $build_core_only = 1;
    }
    elsif ($ARGV[0] =~ '-ACE') {       # Build ACE and its programs
        print "Building ACE\n" if ( $verbose );
        $use_custom_dir = 1;
	push @directories, @ace_dirs;
    }
    elsif ($ARGV[0] =~ '-TAO') {       # Build TAO and its programs
        print "Building TAO\n" if ( $verbose );
        $use_custom_dir = 1;
	# Other tests depend on the lib in this dir so we need to force it 
	# to the front of the build list. This is pretty ugly.
	push @directories, ("$ACE_ROOT\\TAO\\orbsvcs\\tests\\Notify\\lib");
	push @directories, ("$ACE_ROOT\\TAO");
    }
    elsif ($ARGV[0] =~ '-tests_TAO') {       # Build TAO tests
        print "Building TAO tests\n" if ( $verbose );
        $use_custom_dir = 1;
	push @directories, ("$ACE_ROOT\\TAO\\tests");
    }
    elsif ($ARGV[0] =~ '-examples_TAO') {       # Build TAO examples
        print "Building TAO examples\n" if ( $verbose );
        $use_custom_dir = 1;
	push @directories, ("$ACE_ROOT\\TAO\\examples");
    }
    elsif ($ARGV[0] =~ '-dir') {        # Compile only a specific directory
        shift;
        print "Adding directory $ARGV[0]\n" if ( $verbose );
        $use_custom_dir = 1;
        push @directories, $ARGV[0];
    }
    elsif ($ARGV[0] =~ '-rebuild') {    # Rebuild all
        print "Rebuild all\n" if ( $verbose );
        $Build_Cmd = "/REBUILD";
    }
    elsif ($ARGV[0] =~ '-clean') {      # Clean
        print "Cleaning all\n" if ( $verbose );
        $Build_Cmd = "/CLEAN";
    }
    elsif ($ARGV[0] =~ '-Debug') {      # Debug versions
        print "Building Debug Version\n" if ( $verbose );
        $Build_Debug = 1;
        $Build_All = 0;
    }
    elsif ($ARGV[0] =~ '-Release') {    # Release versions
        print "Building Release Version\n" if ( $verbose );
        $Build_Release = 1;
        $Build_All = 0;
    }
    elsif ($ARGV[0] =~ '-DLL') {        # Build DLL only
        print "Build DLL only\n" if ( $verbose );
        $Build_DLL = 1;
        $Build_All = 0;
    }
    elsif ($ARGV[0] =~ '-LIB') {        # Build LIB only
        print "Build LIB only\n" if ( $verbose );
        $Build_LIB = 1;
        $Build_All = 0;
    }
    elsif ($ARGV[0] =~ '-MPC') {        # Use MPC
        print "Use MPC generated files\n" if ( $verbose );
        $Use_MPC = 1;
    }
    elsif ($ARGV[0] =~ '-(\?|h)') {     # Help information
        print "Options\n";
        print "-d         = Debug (only print out projects)\n";
        print "-k         = Ignore Errors\n";
        print "-v         = Script verbose Mode\n";
        print "-s         = Print status messages to STDERR\n";
        print "-u         = Tell MSVC to use the environment\n";
        print "\n";
        print "-CORE      = Build the Core libraries\n";
        print "-ACE       = Build ACE and its programs\n";
        print "-TAO       = Build TAO and its programs\n";
        print "-dir <dir> = Compile custom directories\n";
        print "\n";
        print "-rebuild   = Rebuild All\n";
        print "-clean     = Clean\n";
        print "-Debug     = Compile Debug versions\n";
        print "-Release   = Compile Release versions\n";
        print "-DLL       = Comple DLL Configurations\n";
        print "-LIB       = Comple LIB Configurations\n";
        print "-MPC       = Use MPC generate project files\n";
        exit;
    }
    else {
        warn "$0:  unknown option $ARGV[0]\n";
        die -1;
    }
    shift;
}

if (!$Build_DLL && !$Build_LIB) {
    $Build_DLL = 1;
    $Build_LIB = 1;
}

if (!$Build_Debug && !$Build_Release) {
    $Build_Debug = 1;
    $Build_Release = 1;
}

if ($#directories < 0) {
    @directories = ($ACE_ROOT);
}

print "msvc_auto_compile: Begin\n";
print STDERR "Beginning Core Build\n" if ($print_status == 1);
Build_Core if (!$use_custom_dir || $build_core_only);
print STDERR "Beginning Full Build\n" if ($print_status == 1);
Build_All if !$build_core_only;

print "msvc_auto_compile: End\n";
print STDERR "End\n" if ($print_status == 1);
