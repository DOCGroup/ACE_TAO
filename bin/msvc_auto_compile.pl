# $Id$
#   Win32 auto_compile script.

use File::Find;
use Cwd;

if (!$ENV{ACE_ROOT}) {
  $ACE_ROOT = ".";
}
else {
  $ACE_ROOT = $ENV{ACE_ROOT};
}

@directories = ($ACE_ROOT);

$Verbose = 0;
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
sub Find_dsp ($)
{
  my ($dir) = @_;
  my @array = ();
  my @config_array = ();

  # wanted is only used for the File::Find
  sub wanted 
  {
    $array[++$#array] = $File::Find::name if ($File::Find::name =~ /\.dsp/i);
  }

  sub get_config ($)
  {
    my ($file) = @_;
    my @configs = ();
  
    print "Looking at $file\n" if ($Verbose);
  
    open (DSP, "<$file");

    while (<DSP>)
    {
      push @configs, $1 if (/# Name \"([^\"]+)\"/);
    }
 
    close (DSP);
    return @configs;
  }

  my $old_cwd = getcwd ()."\\";

  if (!chdir ($dir))
  {
    print "Error changing dir to $dir: $!\n";
    return @config_array;
  }

  find(\&wanted, ".");

  for ($i = 0; $i <= $#array; ++$i) {
    my $filename = "$dir/$array[$i]";
    $filename =~ s@/./@/@g;
    $filename =~ s@/@\\@g;
    my @dsp_configs = get_config ($array[$i]);

    for ($j = 0; $j <= $#dsp_configs; ++$j) {
      push @config_array, "$filename--$dsp_configs[$j]";
    }
  }

  if (!chdir ($old_cwd."\\"))
  {
    print "Error changing dir to $old_cwd\\: $!\n";
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
  return system ("msdev.com $project /MAKE \"$config\" $Build_Cmd");
}

# Only builds the TAOACE core.
sub Build_Core ()
{
  print "Building Core of ACE/TAO\n" if ($Verbose == 1);

  print "Build \n" if ($Verbose);
  print "Debug " if ($Verbose) && ($Build_Debug);
  print "Release " if ($Verbose) && ($Build_Release);
  print "DLL " if ($Verbose) && ($Build_DLL);
  print "LIB " if ($Verbose) && ($Build_LIB);
  print "\n" if ($Verbose);

  die "Cannot chdir to ACE_ROOT\n" if (!chdir ($ACE_ROOT));

  if ($Build_DLL)
  {
    if ($Build_Debug)
    {
      $Status = Build_Config ($ACE_ROOT."\\TAO\\TAOACE.dsw--Naming_Service - Win32 Debug");
      return if $Status != 0 && !$Ignore_errors;
    }
    if ($Build_Release)
    {
      $Status = Build_Config ($ACE_ROOT."\\TAO\\TAOACE.dsw--Naming_Service - Win32 Release");
      return if $Status != 0 && !$Ignore_errors;
    }
  }

  if ($Build_LIB) 
  {
    if ($Build_Debug)
    {
      $Status = Build_Config ("$ACE_ROOT\\TAO\\TAOACE_static.dsw--Naming_Service Static - Win32 Static Debug");
      return if $Status != 0 && !$Ignore_errors;
    }
    if ($Build_Release)
    {
      $Status = Build_Config ("$ACE_ROOT\\TAO\\TAOACE_static.dsw--Naming_Service Static - Win32 Static Release");
      return if $Status != 0 && !$Ignore_errors;
    }
  }
}

sub Build_All ()
{
  my @configurations = ();
  
  foreach $d (@directories) 
  {
    push @configurations, Find_dsp ($d);
  }

  print "\nmsvc_auto_compile: First Pass (libraries)\n";

  foreach $c (@configurations)
  {
    if ($Build_All
        || ($Build_DLL && $Build_Debug && $c =~ /Win32 Debug/)
        || ($Build_DLL && $Build_Release && $c =~ /Win32 Release/)
        || ($Build_LIB && $Build_Debug && $c =~ /Win32 Static Debug/)
        || ($Build_LIB && $Build_Release && $c =~ /Win32 Static Release/))
    {
      Build_Config ($c) if (($c =~ /Library/) || ($c =~ /DLL/) || ($c =~ /LIB/));
    }
  }


  print "\nmsvc_auto_compile: Second Pass\n";

  foreach $c (@configurations)
  {
    Build_Config ($c) 
      if ($Build_All
         || ($Build_DLL && $Build_Debug && $c =~ /Win32 Debug/)
         || ($Build_DLL && $Build_Release && $c =~ /Win32 Release/)
         || ($Build_LIB && $Build_Debug && $c =~ /Win32 Static Debug/)
         || ($Build_LIB && $Build_Release && $c =~ /Win32 Static Release/));
  }
}


## Parse command line argument
while ( $#ARGV >= 0  &&  $ARGV[0] =~ /^(-|\/)/ )
{
    if ( $ARGV[0] =~ '(-|\/)k' )     # Ignore error.  Compile the whole thing
    {
        print "Ignore errors\n" if ( $Verbose );
        $Ignore_errors = 1;      # in the same configuration.
    }
    elsif ( $ARGV[0] =~ '(-|\/)v' )   # Verbose mode
    {
        $Verbose = 1;
    }
    elsif ( $ARGV[0] =~ '(-|\/)core' )  # Build only the core of ace/tao
    {
        print "Building Core only\n" if ( $Verbose );
	$build_core_only = 1;
    }
    elsif ( $ARGV[0] =~ '(-|\/)dir' )
    {
        shift;
        print "Adding directory $ARGV[0]\n" if ( $Verbose );
        if (!$use_custom_dir)
        {
            $use_custom_dir = 1;
            @directories = ();
        }
        push @directories, $ARGV[0];
    }
    elsif ( $ARGV[0] =~ '(-|\/)rebuild' )  # Rebuild all
    {
        print "Rebuild all\n" if ( $Verbose );
        $Build_Cmd = "/REBUILD";
    }
    elsif ( $ARGV[0] =~ '(-|\/)clean' )  # Clean
    {
        print "Cleaning all\n" if ( $Verbose );
        $Build_Cmd = "/CLEAN";
    }
    elsif ( $ARGV[0] =~ '(-|\/)Debug' )  # Debug versions
    {
        print "Building Debug Version\n" if ( $Verbose );
        $Build_Debug = 1;
        $Build_All = 0;
    }
    elsif ( $ARGV[0] =~ '(-|\/)Release' )  # Release versions
    {
        print "Building Release Version\n" if ( $Verbose );
        $Build_Release = 1;
        $Build_All = 0;
    }
    elsif ( $ARGV[0] =~ '(-|\/)DLL' )  # Build DLL only
    {
        print "Build DLL only\n" if ( $Verbose );
        $Build_DLL = 1;
        $Build_All = 0;
    }
    elsif ( $ARGV[0] =~ '(-|\/)LIB' )  # Build LIB only
    {
        print "Build LIB only\n" if ( $Verbose );
        $Build_LIB = 1;
        $Build_All = 0;
    }
    elsif ( $ARGV[0] =~ '(-|\/)(\?|h)')
    {
        print "Options\n";
        print "-k         = Ignore Errors\n";
        print "-v         = Script Verbose Mode\n";
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
    else
    {
        warn "$0:  unknown option $ARGV[0]\n";
        die -1
    }
    shift;
}

if (!$Build_DLL && !$Build_LIB)
{
  $Build_DLL = 1;
  $Build_LIB = 1;
}

if (!$Build_Debug && !$Build_Release)
{
  $Build_Debug = 1;
  $Build_Release = 1;
}

print "msvc_auto_compile: Begin\n";

Build_Core if (!$use_custom_dir || $build_core_only);
Build_All if (!$build_core_only);

print "msvc_auto_compile: End\n";
