
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

$Ignore_error = 0;

@Lists= ("Win32 Debug",
         "Win32 Release",
         "Win32 Unicode Debug",
         "Win32 Unicode Release");

%DLL_Collections = ( "$Lists[0]" => <<EOD ,
ace/ace.dsw, ACE dynamic library:
apps/gperf/src/gperf.dsw, ALL:
TAO/tao/TAO.dsw, TAO:
TAO/TAO_IDL/tao_idl.dsw, TAO_IDL Compiler:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, orbsvcs:
tests/tests.dsw, ALL:
EOD
                     "$Lists[1]" => <<EOD ,
ace/ace.dsw, ACE dynamic library:
apps/gperf/src/gperf.dsw, ALL:
TAO/tao/TAO.dsw, TAO:
TAO/TAO_IDL/tao_idl.dsw, TAO_IDL Compiler:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, orbsvcs:
EOD
                     "$Lists[2]" => <<EOD ,
ace/ace.dsw, ACE dynamic library:
EOD
                     "$Lists[3]" => <<EOD
ace/ace.dsw, ACE dynamic library:
EOD
                  );

%Lib_Collections = ( "$Lists[0]" => <<EOD ,
ace/ace.dsw, ACE static library:
TAO/tao/TAO.dsw, TAO Static:
TAO/TAO_IDL/tao_idl.dsw, TAO_IDL Compiler Static:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, ORB Services Static:
EOD
                     "$Lists[1]" => <<EOD ,
ace/ace.dsw, ACE static library:
TAO/tao/TAO.dsw, TAO Static:
TAO/TAO_IDL/tao_idl.dsw, TAO_IDL Compiler Static:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, ORB Services Static:
EOD
                     "$Lists[2]" => <<EOD ,
ace/ace.dsw, ACE static library:
EOD
                     "$Lists[3]" => <<EOD
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

    @Collection = split /:\s*/, $Collection;
    for ($Cntr = 0; $Cntr < scalar (@Collection); $Cntr++)
    {
        $Project_Dir = $Collection[$Cntr];
        $Project_Dir =~ s/(.*)\/[^\/]*/$1/;
        $Bname = $Collection[$Cntr];
        $Bname =~ s/.*\/([^\/]*)/$1/;
        ($Project_File, $Project_Name) = split /,\s*/, $Bname;
        chdir ("$ENV{ACE_ROOT}/$Project_Dir");
        $Status =
            system "msdev.com $Project_File /MAKE \"$Project_Name - $Config\" /USEENV /BUILD /Y3";
#        print "$Status = msdev.com $Project_File /MAKE \"$Project_Name - $Config\"\n";
        if ($Ignore_error == 0)
        {
            return if $Status != 0;
        }
    }
}

sub Build_Collection
{
    my $Cntr = 0;
    for (; $Cntr < scalar(@Lists); $Cntr ++)
    {
        $Config = $Lists[$Cntr];
        Build_Config ($Config, $Target{$Config});
    }
}

$Ignore_error = 1;

%Target = %DLL_Collections;
Build_Collection;

%Target = %Lib_Collections;
Build_Collection;

print "End\n";
