eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$

$EXE = "synch_driver";
$Win32 = 0;
if ($^O eq "MSWin32")
{
    $Win32 = 1;
}

$debug = 0;
$name = "release";
$result_dir = "results";
$svcconf_dir = "svcconf";
$conf_ext = ".conf";

@Null_List = ();

# This is called "baseline"
@Baseline_List = ("base_acquire",
                  "base_tryacquire",
                  "base_acquire_read",
                  "base_tryacquire_read",
                  "base_acquire_write",
                  "base_tryacquire_write");

# this is called "perf_thrno"
@Perf_Thr_Number_List = ("perf_t1",
                         "perf_t2",
                         "perf_t4",
                         "perf_t8",
                         "perf_t16",
                         "perf_t32",
                         "perf_t64");

@Target = @Null_List;

while ( $#ARGV >= 0  &&  $ARGV[0] =~ /^-/ )
{
    if ($ARGV[0] eq '-d')       # Run debug mode
    {
        $name = "debug";
    }
    elsif ($ARGV[0] eq '-p')    # Debug perl script
    {
        $debug = 1;
        print "debug perl scirpt\n";
    }
    elsif ($ARGV[0] eq '-D')    # Subdir name to put the result
    {
        shift;
        $result_dir = $ARGV[0];
    }
    elsif ($ARGV[0] eq '-S')    # Subdir to svc.conf files.
    {
        shift;
        $svcconf_dir = $ARGV[0];
    }
    elsif ($ARGV[0] eq '-N')    # Specify test name.
    {
        shift;
        if ($ARGV[0] eq "baseline")
        {
            @Target = @Baseline_List;
        }
        elsif ($ARGV[0] eq "perf_thrno")
        {
            @Target = @Perf_Thr_Number_List;
        }
        else
        {
            die "Unknown test \"$ARGV[0]\"\n";
        }
    }
    else
    {
        warn "$0:  unknown option $ARGV[0]\n";
        die $usage;
    }
    shift;
}

die "You must specify a test to run\n" if (scalar (@Target) == 0);

if ($Win32 != 0)
{
    $execname = "$name\\$EXE";
    $DIR_SEPARATOR = '\\';
}
else
{
    $execname = "./$EXE";       # Notice that on UNIX, you much build
                                # Debug/Release program explicitly
                                # before running the script.
    $DIR_SEPARATOR = '/';
}

for ($Cntr = 0; $Cntr < scalar (@Target); $Cntr++)
{
    $Redirect_Output = "$result_dir$DIR_SEPARATOR$Target[$Cntr].$name";
    if ($debug != 0)             # Only redirect output in actual run
    {
        print "Redirectling output to $Redirect_Output\n";
    }
    else
    {
        open STDOUT, "> $Redirect_Output";
        open STDERR, ">&STDOUT";
    }

    @args = ("$execname",
             "-f",
             "$svcconf_dir$DIR_SEPARATOR$Target[$Cntr]$conf_ext");

    if ($debug != 0)
    {
        print "Debug mode: Executing -- ";
        for ($args_c = 0; $args_c < scalar (@args); $args_c ++)
        {
            print "$args[$args_c] ";
        }
        print "\n";
    }
    else
    {
        system (@args);
    }
}
