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

@Baseline_List = ("base_acquire",
                  "base_tryacquire",
                  "base_acquire_read",
                  "base_tryacquire_read",
                  "base_acquire_write",
                  "base_tryacquire_write");

while ( $#ARGV >= 0  &&  $ARGV[0] =~ /^-/ )
{
    if ($ARGV[0] eq '-d') # Run debug
    {
        $debug = 1;
        $name = "debug";
    }
    elsif ($ARGV[0] eq '-D') # Subdir name to put the result
    {
        shift;
        $result_dir = $ARGV[0];
    }
    elsif ($ARGV[0] eq '-S') # Subdir to svc.conf files.
    {
        shift;
        $svcconf_dir = $ARGV[0];
    }
    else
    {
        warn "$0:  unknown option $ARGV[0]\n";
        die $usage;
    }
    shift;
}

@Target = @Baseline_List;
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
    open STDOUT, "> $result_dir$DIR_SEPARATOR$Target[$Cntr].$name";
    open STDERR, ">&STDOUT";

    @args = ("$execname",
             "-f",
             "$svcconf_dir$DIR_SEPARATOR$Target[$Cntr]$conf_ext");

    system (@args);
}
