eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$

use warnings;
use strict;

use lib '../../bin';
use PerlACE::Run_Test;
use Getopt::Long;
use Pod::Usage;

#### Program Documentation ####

=head1 NAME

run_spectrum.pl - Run a spectrum of performance tests.

=head1 SYNOPSIS

 run_spectrum.pl [ --help ]
                 [ --manual ] 
                 [ --service (STREAM|SEQPACK) ]
                 [ --protocol (TCP|SCTP) ]
                 [ --config_file filename ]

=head1 OPTIONS

=over 8

=item B<--help, -h>

Print this help message and exit.

=item B<--manual, -m>

Print full documentation and exit.

=item B<--service, -s> (STREAM|SEQPACK)

Select SOCK_STREAM or SOCK_SEQPACK service respectively.  The default
is SOCK_STREAM.

=item B<--protocol, -p> (TCP|SCTP)

Select TCP or SCTP transport protocol.  The default is TCP.  Note that
TCP cannot be used together with the SEQPACK service.

=item B<--config_file, -c> filename

Read configuration parameters from the nominated file.  By default,
parameters are read from C<run_spectrum.config> in the current
directory.

=back

=head1 DESCRIPTION

This directory contains performance tests for measuring round-trip
latency statistics of ACE synchronous messaging using unmarshalled
ACE_CDR::Octet.

The SOCK_STREAM performance test comprises SOCK_STREAM_clt on the
client side and SOCK_STREAM_srv on the server side.  The SOCK_SEQPACK
performance test comprises SOCK_SEQPACK_clt on the client side and
SOCK_SEQPACK_srv on the server side.

People who are interested in round-trip latency often want to see a
I<spectrum> of statistics for a range of payload sizes.  The job of
this script, B<run_spectrum.pl>, is to run the client-side
performance-test program repeatedly, so as to generate a spectrum of
statistics.

The script supports two types of services (SOCK_STREAM and
SOCK_SEQPACK) and two protocols (TCP and UDP).  All combinations are
valid except TCP over SOCK_SEQPACK.  Please see L<"OPTIONS"> for
information about the command-line options that specify the service
and the protocol.

By default, B<run_spectrum.pl> will execute SOCK_STREAM_clt (or
SOCK_SEQPACK_clt) with a minimal set of command-line options.  You can
specify additional options in an external configuration file.  By
default, the configuration file is called C<run_spectrum.config>.  You
may name an alternate configuration file using the B<--config_file>
option.

The configuration file should contain the options that you would
ordinarly give to SOCK_STREAM_clt or SOCK_SEQPACK_clt on the command
line.  To see the command-line options available for these programs,
please run C<SOCK_STREAM_clt --help> or C<SOCK_SEQPACK_clt --help>.
Note that the options B<-s> and B<-t> are used by this script when it
launches the client.  An occurrence of either of these options in the
configuration file will be ignored as redundant.

The client is executed 15 times: once for each payload size from 2^2
bytes to 2^16 bytes.  (SCTP tests only go up to 2^15 bytes because of
limitations in our SCTP implementation.)  Histograms summarizing the
round-trip latency performance are dumped to a file called
C<TIMESTAMP.spectrum> where C<TIMESTAMP> is the current time in
hyphen-delimited form.

=head1 EXAMPLES

B<Example 1>: Run a spectrum of TCP performance tests using
SOCK_STREAM with both endpoints on the local machine.  Each test in
the spectrum should have 10,000 data points.

=over 4

=item 1.

Start the SOCK_STREAM server on the local machine with the S<C<-t
tcp>> option:

    % ./SOCK_STREAM_srv -t tcp

=item 2.

Configure the client to run 10,000 individual trials:

    % echo '"-c 10000"' > run_spectrum.config

=item 3.

Execute B<run_spectrum.pl>:

    % ./run_spectrum.pl

(The script uses TCP and SOCK_STREAM by default.)

=back

B<Example 2>: Run a spectrum of SCTP performance tests using
SOCK_SEQPACK with one endpoint on the local machine and one endpoint
on a remote machine that happens to be named oneida.  Each test in the
spectrum should have 50,000 data points.

=over 4

=item 1.

On the remote host oneida, start the SOCK_SEQPACK server:

    % ./SOCK_SEQPACK_srv

=item 2.

On the local host, configure the client to run 50,000 individual
trials and to look for the server on oneida:

    % echo '"-c 50000 -H oneida"' > run_spectrum.config

=item 3.

On the local host, execute B<run_spectrum.pl>:

    % ./run_spectrum.pl --service SEQPACK --protocol SCTP

=back

=cut

# Global variable declarations
my $help        = 0;
my $manual      = 0;
my $service     = "stream";
my $protocol    = "tcp";
my $config_file = "run_spectrum.config";
my $options     = "";
my $output_file;

# Map from services to client programs
my %client_programs = (stream  => "SOCK_STREAM_clt",
                       seqpack => "SOCK_SEQPACK_clt");

#### MAIN PROGRAM ####

# Canonicalize arguments to lower-case
tr/A-Z/a-z/ for @ARGV;

# Use Getopt::Long to parse options
GetOptions ("help" => \$help,
            "manual" => \$manual,
            "service=s" => \$service,
            "protocol=s" => \$protocol,
            "config_file=s" => \$config_file
            );

# Show help if requested
pod2usage(1) if $help;

# Show manual if requested
pod2usage(-verbose => 2) if $manual;

# Validate service option
$service =~ s/seqpacket/seqpack/;    # Accept "seqpacket" as synonym for "seqpack"
$service =~ /^(stream|seqpack)$/
    or pod2usage("Service must be STREAM or SEQPACK\n");

$protocol =~ /^(tcp|sctp)$/ 
    or pod2usage("Protocol must be TCP or SCTP\n");

$service eq "seqpack" and $protocol eq "tcp"
    and pod2usage("Cannot use SEQPACK service with TCP\n");
                                  
# Open config file
open CONFIG, "< $config_file"
    or die "cannot open file: $config_file\n";

# Read options from the config file.  The options are defined as the
# first double-quoted string that is not to the right of a pound sign.
# (Pound signs and quotes cannot be escaped.)

while (<CONFIG>) {
    chomp;
    if (/^[^\#]*?\"(.*?)\"/) {
        $options = $1;
        last;
    }
}

# Close config file    
close CONFIG;

# Print options
print "Read options from $config_file: \"$options\"\n\n";

# Name a unique file to store the results in.
($output_file = localtime() . ".spectrum") =~ tr/ /-/;

# Run the test for message sizes ranging from 2^2 bytes to 2^16 bytes
# (SCTP tests only go up to 2^15 bytes because of limitations in our
#  SCTP implementation)
my $max_size = 16;
$max_size = 15 if $protocol eq "sctp";
for (my $i = 2; $i <= $max_size; ++$i) {
    
    # Assemble client parameters and print out the command line
    my $client_params = "-t $protocol -s $i $options >> $output_file";
    print "$client_programs{$service} $client_params\n";
    
    # Configure client
    my $client = 
        new PerlACE::Process($client_programs{$service}, $client_params);

    # Spawn client
    $client->Spawn();

    # Wait for client to finish
    $client->Wait();

    # Sleep for a while
    sleep 5;
}

