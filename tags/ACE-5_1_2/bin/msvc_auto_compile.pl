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

@directories = ($ACE_ROOT);

$verbose = 0;
$print_status = 0;
$Ignore_errors = 0;              # By default, bail out if an error occurs.
$Build_DLL = 0;
$Build_LIB = 0;
$Build_Debug = 0;
$Build_Release = 0;
$Build_All = 1;
$build_core_only = 0;
$Build_Cmd = "/BUILD";
$use_custom_dir = 0;

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

    print "Auto_compiling $project : $config\n";
    return Build ($project, $config);
}

# Build
sub Build ($$)
{
  my ($project, $config) = @_;

  print "Building $project $config\n" if $verbose;

  return system ("msdev.com $project /MAKE \"$config\" $Build_Cmd");
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
        @core_list = ($ACE_ROOT."\\ace\\ace_dll.dsp",
                      $ACE_ROOT."\\apps\\gperf\\src\\gperf_lib.dsp",
                      $ACE_ROOT."\\apps\\gperf\\src\\gperf.dsp",
                      $ACE_ROOT."\\TAO\\TAO_IDL\\tao_idl.dsp",
                      $ACE_ROOT."\\TAO\\tao\\TAO.dsp",
                      $ACE_ROOT."\\TAO\\orbsvcs\\orbsvcs\\orbsvcs.dsw");

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
        @core_list = ($ACE_ROOT."\\ace\\ace_lib.dsp",
                      $ACE_ROOT."\\apps\\gperf\\src\\gperf_lib.dsp",
                      $ACE_ROOT."\\apps\\gperf\\src\\gperf.dsp",
                      $ACE_ROOT."\\TAO\\TAO_IDL\\tao_idl_static.dsp",
                      $ACE_ROOT."\\TAO\\tao\\TAO_Static.dsp",
                      $ACE_ROOT."\\TAO\\orbsvcs\\orbsvcs\\Svc_Utils_Static.dsp",
                      $ACE_ROOT."\\TAO\\orbsvcs\\orbsvcs\\CosNaming_Static.dsp",
                      $ACE_ROOT."\\TAO\\orbsvcs\\orbsvcs\\CosProperty_Static.dsp",
                      $ACE_ROOT."\\TAO\\orbsvcs\\orbsvcs\\CosTrading_Static.dsp",
                      $ACE_ROOT."\\TAO\\orbsvcs\\orbsvcs\\AV_Static.dsp",
                      $ACE_ROOT."\\TAO\\orbsvcs\\orbsvcs\\RTSched_Static.dsp",
                      $ACE_ROOT."\\TAO\\orbsvcs\\orbsvcs\\RTEvent_Static.dsp",
                      $ACE_ROOT."\\TAO\\orbsvcs\\orbsvcs\\CosEvent_Static.dsp",
                      $ACE_ROOT."\\TAO\\orbsvcs\\orbsvcs\\CosConcurrency_Static.dsp",
                      $ACE_ROOT."\\TAO\\orbsvcs\\orbsvcs\\CosLifeCycle_Static.dsp",
                      $ACE_ROOT."\\TAO\\orbsvcs\\orbsvcs\\CosTime_Static.dsp",
                      $ACE_ROOT."\\TAO\\orbsvcs\\orbsvcs\\DsLogAdmin_Static.dsp");

        foreach $c (@core_list) {
            if ($Build_Debug) {
                $Status = Build ($c, "ALL - Win32 Static Debug");
                return if $Status != 0 && !$Ignore_errors;
            }
            if ($Build_Release) {
                $Status = Build ($c, "ALL - Win32 Static Release");
                return if $Status != 0 && !$Ignore_errors;
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
            || ($Build_LIB && $Build_Debug && $c =~ /Win32 Static Debug/)
            || ($Build_LIB && $Build_Release && $c =~ /Win32 Static Release/))
        {
            my $Status = 0;
            $Status = Build_Config ($c)
                if (($c =~ /Library/) || ($c =~ /DLL/) || ($c =~ /LIB/));
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
             || ($Build_LIB && $Build_Debug && $c =~ /Win32 Static Debug/)
             || ($Build_LIB && $Build_Release && $c =~ /Win32 Static Release/));
    }
}


## Parse command line argument
while ( $#ARGV >= 0  &&  $ARGV[0] =~ /^(-|\/)/ )
{
    if ($ARGV[0] =~ '-k') {             # Ignore errors
        print "Ignore errors\n" if ( $verbose );
        $Ignore_errors = 1;
    }
    elsif ($ARGV[0] =~ '-v') {          # verbose mode
        $verbose = 1;
    }
    elsif ($ARGV[0] =~ '-s') {          # status messages
        $print_status = 1;
    }
    elsif ($ARGV[0] =~ '-core') {       # Build only the core of ace/tao
        print "Building Core only\n" if ( $verbose );
        $build_core_only = 1;
    }
    elsif ($ARGV[0] =~ '-dir') {        # Compile only a specific directory
        shift;
        print "Adding directory $ARGV[0]\n" if ( $verbose );
        if (!$use_custom_dir) {
            $use_custom_dir = 1;
            @directories = ();
        }
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
    elsif ($ARGV[0] =~ '-(\?|h)') {     # Help information
        print "Options\n";
        print "-k         = Ignore Errors\n";
        print "-v         = Script verbose Mode\n";
        print "-s         = Print status messages to STDERR\n";
        print "-core      = Build the Core\n";
        print "-dir <dir> = Compile custom directories\n";
        print "-rebuild   = Rebuild All\n";
        print "-clean     = Clean\n";
        print "-Debug     = Compile Debug versions\n";
        print "-Release   = Compile Release versions\n";
        print "-DLL       = Comple DLL Configurations\n";
        print "-LIB       = Comple LIB Configurations\n";
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

print "msvc_auto_compile: Begin\n";
print STDERR "Beginning Core Build\n" if ($print_status == 1);
Build_Core if (!$use_custom_dir || $build_core_only);
print STDERR "Beginning Full Build\n" if ($print_status == 1);
Build_All if (!$build_core_only);

print "msvc_auto_compile: End\n";
print STDERR "End\n" if ($print_status == 1);
