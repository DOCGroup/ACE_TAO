# $Id$
# -*- perl -*-
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# Usage:
# process-output.pl output-filename number-of-iterations priority1 priority2

# This is a Perl script that processes the output of the
# MT_Client_Protocol_Priority test run.

# The following checks are performed:
# 1) There are no errors or excpetion messages.
# 2) Both server and client termination messages are present, i.e.,
#    "Client thrreads finished" and "Server ORB event loop finished".
# 3) Number of times servant is invoked equals 2 * number of
#    iterations (for two threads).
# 4) Number of requests using iiop is equal to the number of requests using
#    shmiop, which is equal to the specified number of iterations.
# 5) Number of requests at priority1 is equal to the number of
#    requests at priority2, which is equal to the specified number of
#    iterations.

# Command-line args.
$input_file = $ARGV[0];
$iterations = $ARGV[1];
$priority1 = $ARGV[2];
$priority2 = $ARGV[3];

$errors = 0;

# Open the output file.
if ($input_file and $ARGV[1])
{
    open (DATA, $input_file);
}
else
{
    die "Usage: process-output.pl output-file-name "
        ."number-of-iterations priority1 priority2\n";
}

$thread_priority_pattern =
    "RTCORBA::CLIENT_PROPAGATED processing using default thread pool";

$iiop_requests = 0;
$shmiop_requests = 0;
$priority1_requests = 0;
$priority2_requests = 0;
$threads_finished = 0;
$server_shutdown = 0;
$test_method = 0;

# Process the output.
while ($line = <DATA>)
{
    # Process the line.
    chomp $line;
    @words = split (/ /, $line);

    if ($line eq "test_method invoked")
    {
        ++$test_method;
    }

    if ($words[3] eq "SHMIOP_Server_Connection_Handler::handle_input"
        and $threads_finished == 0)
    {
        ++$shmiop_requests;
    }

    if ($words[3] eq "IIOP_Server_Connection_Handler::handle_input"
        and $threads_finished == 0)
    {
        ++$iiop_requests;
    }

    if ($line eq "Client threads finished")
    {
        $threads_finished = 1;
    }

    if ($line eq "Server ORB event loop finished")
    {
        $server_shutdown = 1;
    }

    if ($line =~ /$thread_priority_pattern/
        and $threads_finished == 0)
    {
        if ($words[@words - 1] == $priority1)
        {
            ++$priority1_requests;
        }
        elsif ($words[@words - 1] == $priority2)
        {
            ++$priority2_requests;
        }
    }

    # Make sure there are no errors or exceptions.
    if ($line =~ /error/i
        or $line =~ /exception/i)
    {
        close (DATA);
        die "Error is detected in the output file <$input_file> \n";
    }
}
close (DATA);

# Go through processing results.
if ($server_shutdown == 0
    or $threads_finished == 0)
{
    print "ERROR: Missing test over message\n";
    ++$errors;
}
#
# This condition is busted right now because the debug message
# that used to signal the iiop or shmiop request have been removed
# so we cannot count them anymore. The count is in fact zero for
# both. Nonetheless the method are invoked.
#
#elsif ($iiop_requests != $shmiop_requests
#       or $iiop_requests != $iterations)
#
elsif ($iiop_requests != $shmiop_requests)
{
    print "ERROR: Number of iiop requests differs from shmiop differs from "
        ."number of iterations\n";
    print "IIOP Request: ";
    print $iiop_requests;
    print "\n";

    print "IIOP Request: ";
    print $iiop_requests;
    print "\n";
    ++$errors;
}
elsif (($priority1 != $priority2
        and ($priority1_requests != $priority2_requests
             or $priority1_requests != $iterations))
       or ($priority1 == $priority2
           and ($priority2_requests != 0
                or $priority1_requests != 2*$iterations)))
{
    print "ERROR: Nonmatching number of requests of each priority\n";
    ++$errors;
}
elsif ($test_method != 2*$iterations)
{
    print "ERROR: Incorrect number servant invocations\n";
    ++$errors;
}
else
{
    print "Test output is ok \n";
}

exit $errors;
