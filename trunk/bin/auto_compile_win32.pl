
# $Id$
#   Win32 autobuild helper.

# use lib "$ENV{ACE_ROOT}/bin";

## TO-DO
##   * Added command line options to selective compile library, et.al.
##   * Allow controlling the "ignore error" from command line. (-k?)
##   * Added cvs update.  Also controllable from command line.
##   * Added building an running version_tests/.  It uses a different
##     naming scheme.
##   * Selectively build some specific configs from command line.
##   * Control of build/rebuild all from command line.

@Win32_Lists= ("Win32 Debug",
               "Win32 Release",
               "Win32 Unicode Debug",
               "Win32 Unicode Release");

@Alpha_Lists= ("Win32 Alpha Debug",
               "Win32 Alpha Release",
               "Win32 Alpha Unicode Debug",
               "Win32 Alpha Unicode Release");

%Win32_DLL_Collections = ( "$Win32_Lists[0]" => <<EOD , # Debug
ace/ace.dsw, ACE dynamic library:
apps/gperf/src/gperf.dsw, ALL:
TAO/tao/TAO.dsw, TAO:
TAO/TAO_IDL/tao_idl.dsw, TAO_IDL Compiler:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, orbsvcs:
tests/tests.dsw, ALL:
EOD
                     "$Win32_Lists[1]" => <<EOD , # Release
ace/ace.dsw, ACE dynamic library:
apps/gperf/src/gperf.dsw, ALL:
TAO/tao/TAO.dsw, TAO:
TAO/TAO_IDL/tao_idl.dsw, TAO_IDL Compiler:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, orbsvcs:
EOD
                     "$Win32_Lists[2]" => <<EOD , # Unicode Debug
ace/ace.dsw, ACE dynamic library:
EOD
                     "$Win32_Lists[3]" => <<EOD # Unicode Release
ace/ace.dsw, ACE dynamic library:
EOD
                  );

%Alpha_DLL_Collections = ( "$Alpha_Lists[0]" => <<EOD , # Debug
ace/ace.dsw, ACE dynamic library:
apps/gperf/src/gperf.dsw, ALL:
TAO/tao/TAO.dsw, TAO:
TAO/TAO_IDL/tao_idl.dsw, TAO_IDL Compiler:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, orbsvcs:
tests/tests.dsw, ALL:
EOD
                     "$Alpha_Lists[1]" => <<EOD , # Release
ace/ace.dsw, ACE dynamic library:
apps/gperf/src/gperf.dsw, ALL:
TAO/tao/TAO.dsw, TAO:
TAO/TAO_IDL/tao_idl.dsw, TAO_IDL Compiler:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, orbsvcs:
EOD
                     "$Alpha_Lists[2]" => <<EOD , # Unicode Debug
ace/ace.dsw, ACE dynamic library:
EOD
                     "$Alpha_Lists[3]" => <<EOD # Unicode Release
ace/ace.dsw, ACE dynamic library:
EOD
                  );

%Win32_Lib_Collections = ( "$Win32_Lists[0]" => <<EOD , # Debug
ace/ace.dsw, ACE static library:
TAO/tao/TAO.dsw, TAO Static:
TAO/TAO_IDL/tao_idl.dsw, TAO_IDL Compiler Static:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, ORB Services Static:
EOD
                     "$Win32_Lists[1]" => <<EOD , # Release
ace/ace.dsw, ACE static library:
TAO/tao/TAO.dsw, TAO Static:
TAO/TAO_IDL/tao_idl.dsw, TAO_IDL Compiler Static:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, ORB Services Static:
EOD
                     "$Win32_Lists[2]" => <<EOD , # Unicode Debug
ace/ace.dsw, ACE static library:
EOD
                     "$Win32_Lists[3]" => <<EOD # Unicode Release
ace/ace.dsw, ACE static library:
EOD
                  );

%Alpha_Lib_Collections = ( "$Alpha_Lists[0]" => <<EOD , # Debug
ace/ace.dsw, ACE static library:
EOD
                     "$Alpha_Lists[1]" => <<EOD , # Release
ace/ace.dsw, ACE static library:
EOD
                     "$Alpha_Lists[2]" => <<EOD , # Unicode Debug
ace/ace.dsw, ACE static library:
EOD
                     "$Alpha_Lists[3]" => <<EOD # Unicode Release
ace/ace.dsw, ACE static library:
EOD
                  );

sub Build_Config
{
    my $Config = shift;
    my $Collection = shift;
    my $Bname = "";
    my $Project_File = "";
    my $Project_Name = "";
    my $Status = -1;
    my $Cntr = 0;
    my $Command_Line = "";

    @Collection = split /:\s*/, $Collection;
    for ($Cntr = 0; $Cntr < scalar (@Collection); $Cntr++)
    {
        $Project_Dir = $Collection[$Cntr];
        $Project_Dir =~ s/(.*)\/[^\/]*/$1/;
        $Bname = $Collection[$Cntr];
        $Bname =~ s/.*\/([^\/]*)/$1/;
        ($Project_File, $Project_Name) = split /,\s*/, $Bname;
        chdir ("$ENV{ACE_ROOT}/$Project_Dir");
        $Command_Line =  "msdev.com $Project_File /MAKE \"$Project_Name - $Config\" /USEENV $Build_Cmd /Y3";
        if ( $Debug == 0 )
        {
            $Status =
                system "$Command_Line";
        }
        else
        {
            $Status = 0;
            print "chdir (\"$ENV{ACE_ROOT}/$Project_Dir\");\n";
            print "$Status = $Command_Line\n";
        }

        if ($Ignore_error == 0)
        {
            return if $Status != 0;
        }
    }
}

sub Build_Collection
{
    print "Build_Collection\n" if ( $Verbose );
    my $Cntr = 0;
    for (; $Cntr < scalar(@Lists); $Cntr ++)
    {
        $Config = $Lists[$Cntr];
        print "Building $Config of $Target{$Config}\n" if ( $Debug );
        Build_Config ($Config, $Target{$Config});
    }
}


$Verbose = 0;
$Debug = 0;
$Ignore_error = 0;              # By default, bail out if an error occurs.
$Build_DLL = 1;
$Build_LIB = 1;
$Build_Cmd = "/BUILD";
@Lists = @Win32_Lists;
%DLL_Collections = %Win32_DLL_Collections;
%Lib_Collections = %Win32_Lib_Collections;

## Parse command line argument
while ( $#ARGV >= 0  &&  $ARGV[0] =~ /^-/ )
{
    if ( $ARGV[0] eq '-k' )     # Ignore error.  Compile the whole thing
    {
        print "Ignore errors\n" if ( $Verbose );
        $Ignore_error = 1;      # in the same configuration.
    }
    elsif ( $ARGV[0] eq '-a' )  # Use Alpha
    {
        print "Build Alpha\n" if ( $Verbose );
        @Lists = @Alpha_Lists;
        %DLL_Collections = %Alpha_DLL_Collections;
        %Lib_Collections = %Alpha_Lib_Collections;
    }
    elsif ( $ARGV[0] eq '-d')   # Script debugging mode
    {
        print "Debug mode on\nVerbose mode on\n";
        $Debug = 1;
        $Verbose = 1;
    }
    elsif ( $ARGV[0] eq '-D' )  # Build DLL only
    {
        print "Build DLL only" if ( $Verbose );
        $Build_LIB = 0;
    }
    elsif ( $ARGV[0] eq '-L' )  # Build LIB only
    {
        print "Build LIB only" if ( $Verbose );
        $Build_DLL = 0;
    }
    elsif ( $ARGV[0] eq '-v' )   # Verbose mode
    {
        $Verbose = 1;
    }
    elsif ( $ARGV[0] eq '-r' )  # Rebuild all
    {
        print "Rebuild all\n" if ( $Verbose );
        $Build_Cmd = "/REBUILD";
    }
    else
    {
        warn "$0:  unknown option $ARGV[0]\n";
        die $usage;
    }
    shift;
}

if ( $Verbose )
{
    for ($II = 0; $II < scalar (@Lists); $II++)
    {
        printf "$Lists[$II]\n";
    }
}

if ( $Build_DLL )
{
    print "Building DLL\n" if ( $Verbose );
    %Target = %DLL_Collections;
    Build_Collection;
}

if ( $Build_LIB )
{
    print "Building LIB\n" if ( $Verbose );
    %Target = %Lib_Collections;
    Build_Collection;
}

print "End\n";
