# $Id$
# -*- perl -*-
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

unshift @INC, '../../../../bin';
require Process;
require Uniqueid;

# This is a Perl script that processes the output of the multithreaded
# client test.  Usage: process-m-output.pl output-file-name number-of-threads

# The following checks are performed:
# 1) Number of sucessful binds equals to the number of sucessful
# unbinds.
# 2) Each thread id has 1 output line for each of the following: bind,
# resolve and unbind.
# 3) There are no unexpected output lines (e.g., more lines than
# expected or with unexpected content like Exceptions, seg faults).

# Open the output file.
$input_file = $ARGV[0];
if ($input_file and $ARGV[1])
{
    open (DATA, $input_file);
}
else
{
    die "Usage: process-m-output.pl output-file-name number-of-threads \n";
}

$binds = 0;
$unbinds = 0;

while ($line = <DATA>)
{
    # Process the line.
    chomp $line;
    @words = split (/ /, $line);

    # Make sure the line contains expected output.
    if (not ($words[0] eq "Unbound" or
             $words[0] eq "Bound" or
             $words[0] eq "Resolved" or
             $words[0] eq "Unable"))
    {
        close (DATA);
        die "Error is detected in the output file <$input_file> \n";
    }

    # Keep track of sucessful binds/unbinds.
    if ($words[0] eq "Bound")
    {
        ++$binds;
    }
    if ($words[0] eq "Unbound")
    {
        ++$unbinds;
    }

    # Keep track of output lines for each thread.
    $count = $threads{$words[5]};

    if ($count eq "" and
        ($words[0] eq "Bound" or $words[2] eq "bind"))
    {
        ++$threads{$words[5]};
    }
    elsif ($count eq 1 and
        ($words[0] eq "Resolved" or $words[2] eq "resolve"))
    {
        ++$threads{$words[5]};
    }
    elsif ($count eq 2 and
        ($words[0] eq "Unbound" or $words[2] eq "unbind"))
    {
        ++$threads{$words[5]};
    }
    else
    {
        close (DATA);
        die "Wrong output for thread $word[5]\n";
    }
}
close (DATA);
# Check that each thread performed 3 operations.
if (grep {not $_ eq 3} values %threads)
{
    die "Some thread has performed a wrong number of operations\n";
}
$number = values %threads;
if ($number ne $ARGV[1])
{
    die "Not all threads performed operations\n";
}

# Check that number of binds equals to those of unbinds.
if ($binds == $unbinds)
{
    print "Multithreaded output is ok \n";
}
else
{
    print "Number of sucessfule binds is different from number of
sucessful unbinds\n";
}
