# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use warnings;
use strict;

use FindBin;
use lib $FindBin::Bin;

# Global variable declarations

my @hostname;
my @iterations;
my $file;
my $cmdline;

#### MAIN PROGRAM ####

# If argv[1] is present then set the "remote host" command line option
if ($#ARGV >= 0) {
    print "hostname: $ARGV[0]\n";
    @hostname = ("-H", $ARGV[0]);
}

# If argv[2] is present then set the "number of iterations" command line option

if ($#ARGV >= 1) {
    print "number of iterations: $ARGV[0]\n";
    @iterations = ("-c", $ARGV[1]);
} else {
    @iterations = ("-c", "1000");
}

# Name a unique file to store the results in.

($file = localtime() . ".spectrum") =~ s/ /-/g;

# Run the test for message sizes ranging from 2^2 bytes to 2^16 bytes

for (my $i = 2; $i <= 16; ++$i) {

    # Assemble command line
    my @cmdline = ("./SOCK_STREAM_clt", @iterations, @hostname, "-s", "$i", "-t", "tcp");

    # Print command line
    print ((join ' ', "Command line: ", (map "'$_'", @cmdline)), "\n");

    # Fork client
    my $pid;
    die "cannot fork: $!" unless defined ($pid = open(CLIENT, "-|"));

    if ($pid) { # parent

        # Open spectrum file for writing
        open (SPECTRUM, ">> $file")
            or die "Couldn't open $file for writing: $!\n";

        # Copy output from the client to the spectrum file
        while (<CLIENT>) {
            print SPECTRUM $_;
        }

        # Close filehandles
        close SPECTRUM;
        close CLIENT;

    } else { # child

        exec { $cmdline[0] } @cmdline
            or die "can't exec '@cmdline': $!\n";

    }
}
