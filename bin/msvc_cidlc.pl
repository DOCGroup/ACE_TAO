# $Id$
#   Win32 auto_compile script.
if (!$ENV{CIAO_ROOT}) {
    $CIAO_ROOT = getcwd ()."TAO\CIAO\\";
    warn "CIAO_ROOT not defined, defaulting to CIAO_ROOT=$CIAO_ROOT";
}
else {
    $ACE_ROOT = $ENV{ACE_ROOT};
    $CIAO_ROOT = $ENV{CIAO_ROOT};
}

use lib "$ENV{ACE_ROOT}/bin";

use File::Find;
use PerlACE::Run_Test;
use Cwd;

@directories = ();

$debug = 0;
$verbose = 0;
$print_status = 0;
$Ignore_errors = 0;              # By default, bail out if an error occurs.
$Build_All = 1;
$Build_Cmd = "/BUILD";
$use_custom_dir = 0;
$useenv = '';
$vc7 = 0;
$Build_Debug = 1;

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

# Build
sub Build_VC7 ($$)
{
  my ($project, $config) = @_;

  if ($debug == 1) {
    print "$project\n";
    return 0;
  }
  else {
    print "Auto_compiling $project : $config\n";

    print "Building $project $config\n" if $verbose;

    return system ("devenv.com $project $Build_Cmd $config $useenv");
  }
}



sub Find_Sln (@)
{
    my (@dir) = @_;
    @array = ();

    sub wanted_sln {
        $array[++$#array] =
            $File::Find::name if ($File::Find::name =~ /\.sln$/i);
    }

    find (\&wanted_sln, @dir);

    print "List of sln's \n" if ($verbose == 1);
    return @array;
}

sub Build_All ()
{
    my @configurations = Find_Sln (@directories);

    print STDERR "Building selected projects\n" if ($print_status == 1);
    print "\nmsvc_cidlc: Building selected projects\n";

    $count = 0;
    foreach $c (@configurations) {
        print STDERR "Configuration ".$count++." of ".$#configurations."\n" if ($print_status == 1);
        if ($Build_Debug) {
            $Status = Build_VC7 ($c, "debug");
            return if $Status != 0 && !$Ignore_errors;
        }
    }
    print STDERR "ERROR: No configurations have been build\n" if ($count == 0);
}


## Parse command line argument
while ( $#ARGV >= 0  &&  $ARGV[0] =~ /^(-|\/)/ )
{
    if ($ARGV[0] =~ '-vc7') {    # Use VC7 project and solution files.
        print "Using VC7 files\n" if ( $verbose );
        $vc7 = 1;
        $proj_ext = '.vcproj';
    }
    elsif ($ARGV[0] =~ '-vc8') {    # Use VC8 project and solution files.
        print "Using VC8 files\n" if ( $verbose );
        $vc7 = 1; # VC8 is like VC7
        $proj_ext = '.vcproj';
    }
    elsif ($ARGV[0] =~ '-v') {          # verbose mode
        $verbose = 1;
    }
    elsif ($ARGV[0] =~ '-s') {          # status messages
        $print_status = 1;
    }
    elsif ($ARGV[0] =~ '-clean') {      # Clean
        print "Cleaning all\n" if ( $verbose );
        $Build_Cmd = "/CLEAN";
    }
    elsif ($ARGV[0] =~ '-(\?|h)') {     # Help information
        print "Options\n";
        print "-vc7       = Use MSVC 7 toolset\n";
        print "-vc8       = Use MSVC 8 toolset\n";
        print "-clean     = Clean\n";
        exit;
    }
    else {
        warn "$0:  unknown option $ARGV[0]\n";
        die -1;
    }
    shift;
}

if ($#directories < 0) {
    print "Using VC7 files\n" if ( $verbose );
    push @directories, ("$CIAO_ROOT\\CIDLC");
}

print "msvc_cidlc: Begin\n";
print STDERR "Beginning CIDLC Build\n" if ($print_status == 1);
Build_All ();

print "msvc_cidlc: End\n";
print STDERR "End\n" if ($print_status == 1);
