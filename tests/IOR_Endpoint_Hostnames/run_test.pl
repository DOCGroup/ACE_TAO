eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

#
# This run_test.pl does not use the standard harness as used by other
# run_test.pl.  Since it is testing a behavior of TAO which is
# affected by environmental influences, it uses perl features as well
# as separate executables to obtain information about the environment
# so that it can inspect the information inside IORs and decide if
# that information is "correct" given the environment.
#
# At the end, it prints out a matrix of each permutation of the test
# and indicates whether or not that permutation FAILed.
#

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use Socket;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

$server->AddLibPath ($server->ExeSubDir ());

my $iorbase = "server.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $database = "intf_run.data";
my $server_datafile = $server->LocalFile ($database);
$server->DeleteFile($iorbase);
$server->DeleteFile($database);

$LI = $server->CreateProcess ("list_interfaces");
$GI = $server->CreateProcess ("generate_ior");
$CI = $server->CreateProcess ("../../utils/catior/tao_catior",
                              "-f $server_iorfile");

open (OLDOUT, ">&STDOUT");
open (STDOUT, ">$server_datafile") or die "can't redirect stdout: $!";
open (OLDERR, ">&STDERR");
open (STDERR, ">&STDOUT") or die "can't redirect stderror: $!";

$server_status = $LI->SpawnWaitKill ($server->ProcessStartWaitInterval());

open (STDOUT, ">&OLDOUT");
open (STDERR, ">&OLDERR");

open (INTERFACES, "<$server_datafile") || die "Unable to open $server_datafile: $!\n";
chomp(@IPADDRS = <INTERFACES>);
close (INTERFACES);

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

# Fill up the array of hostnames; can't use the hostname() from
# Sys :: Hostname because it's too good at figuring out that IP
# addresses that aren't in a host table actually DO match to a host
# name.  So, we use gethostbyaddr().
@HOSTNAMES = map { (gethostbyaddr(inet_aton($_),AF_INET))[0] || $_ } @IPADDRS;

$HN = hostname();  # Shorthand so we do not have to use 'hostname' all over.

@PERL_IPADDRS = map { inet_ntoa($_) || $_ } (gethostbyname($HN))[4];

#
# $TEST_DATA is a reference to an anonymous hash that has
#   key:  string representation/description of a test
#   value: ref to anonymous array consisting of
#          [0] = -ORBDottedDecimalAddresses value (must be 0 or 1)
#          [1] = -ORBendpoint hosname spec (undef if not provided)
#          [2] = -ORBendpoint "hostname_in_ior" value (undef if not provided)
#          [3] = ref to array containing profiles expected in IOR
#

$TEST_DATA = {

#
# This is kind of like a "truth table" for what should happen when
# -ORBDottedDecimalAddresses (DDA) and the "hostname_in_ior" (HIOR)
# option for -ORBListenEndpoints (nee -ORBEndpoint) interact.  Note
# that DDA's default value is "0", so there is no way to have an
# unspecified value for DDA.
#
#
#
#        | -ORBendpoint  |
#  DDA   |    hostspec   |   HIOR    | Profile(s) in IOR
# --------------------------------------------------------------
#   0    |    unspec     |  unspec   | One profile for each interface
#                                      discovered in
#                                      TAO_IIOP_Acceptor::probe_interfaces(),
#                                      where the host's name is that
#                                      returned from a reverse lookup of
#                                      the interface's address from
#                                      whatever facility is doing
#                                      name<->address translations for
#                                      that host.

    "0:unspec:unspec" => [ 0, undef, undef, \@HOSTNAMES ],

#   1    |    unspec     |  unspec   | One profile for each interface
#                                      discovered in
#                                      TAO_IIOP_Acceptor::probe_interfaces(),
#                                      where the host's name is the IP
#                                      address associated with the
#                                      interface.

    "1:unspec:unspec" => [ 1, undef, undef, \@IPADDRS ],

#   0    |    "foo"      |  unspec   | Exactly one profile where the
#                                      host's name is "foo".

    "0:$HN:unspec" => [ 0, $HN, undef, [$HN] ],

#   1    |    "foo"      |  unspec   | Exactly one profile where the
#                                      host's name is the IP address
#                                      from the name<->address
#                                      translation for the host.

    "1:$HN:unspec" => [ 1, $HN, undef, \@PERL_IPADDRS ],

#   X    |    unspec     |  "bar"    | Exactly one profile where the
#                                      host's name is "bar".
#"d/c:unspec:".$HN."_blech" => [ undef, undef, $HN."_blech", [$HN."_blech"] ],
#"d/c:unspec:blech" => [ undef, undef, "blech", ["blech"] ],
    "0:unspec:blech" => [ 0, undef, "blech", ["blech"] ],
    "1:unspec:blech" => [ 1, undef, "blech", ["blech"] ],
#
#   X    |    "foo"      |  "bar"    | Exactly one profile where the
#                                      host's name is "bar".
#"d/c:$HN:".$HN."_blech" => [ undef, $HN, $HN."_blech", [$HN."_blech"] ],
#"d/c:$HN:blech" => [ undef, $HN, "blech", ["blech"] ],
    "0:$HN:blech" => [ 0, $HN, "blech", ["blech"] ],
    "1:$HN:blech" => [ 1, $HN, "blech", ["blech"] ],
};


sub do_test {
    # pass in undef for 'unspec' in the table
    my ($dda, $endpointhost, $hior) = @_;

    $dda = 0 if (!defined($dda));
    $endpointhost = '' if (!defined($endpointhost));
    $hior_opt = ($hior ne '') ? "/hostname_in_ior=$hior" : '';

    $server->DeleteFile($database);

    $GI->Arguments ("-ORBListenEndpoints iiop://$endpointhost".$hior_opt." ".
                    "-ORBDottedDecimalAddresses $dda ".
                    "-o $server_iorfile");

    $server_status = $GI->SpawnWaitKill ($server->ProcessStartWaitInterval());

    if ($server_status != 0) {
        print STDERR "ERROR: server returned $server_status\n";
        exit 1;
    }

    open (OLDOUT, ">&STDOUT");
    open (STDOUT, ">$server_datafile") or die "can't redirect stdout: $!";
    open (OLDERR, ">&STDERR");
    open (STDERR, ">&STDOUT") or die "can't redirect stderror: $!";

    $server_status = $CI->SpawnWaitKill ($server->ProcessStartWaitInterval());

    open (STDOUT, ">&OLDOUT");
    open (STDERR, ">&OLDERR");

    if ($server_status != 0) {
        print STDERR "ERROR: server returned $server_status\n";
        exit 1;
    }

    my @profiles;
    my $line;
#    print $GI->Executable()." ".$GI->Arguments()."\n";
#    print $CI->Executable()." ".$CI->Arguments()."\n";
    open (PIOR, "<$server_datafile") || die "Unable to exec generate_ior: $!\n";
#    print "XXX: $_" while (<PIOR>);
    while ($line = <PIOR>) {
        # Need to look for the following lines:
        #   Host Name: <ipaddr_or_host>
        # and
        #   endpoint: <ipaddr_or_host>:<portnum>
        chomp $line;
        my $x;
#        print "Looking at $line\n";
        if ($line =~ /.*Host Name:\s+(.+)$/) {
            chomp($x = $1);
#            print "HN pushing $x\n";
            push @profiles, $x;
        } elsif ($line =~ /.*endpoint: ([^:]+):.*/) {
            chomp($x = $1);
#            print "EP pushing $x\n";
            push @profiles, $x;
        }
    }
    close (PIOR);

    $server->DeleteFile($database);

    return @profiles;
}

# Usage:
#   $are_equal = compare_arrays(\@frogs, \@toads);
sub compare_arrays {
    my ($first, $second) = @_;
    no warnings;                  # silence spurious -w undef complaints
    return 0 unless @$first == @$second;

    @sorted_first = sort @$first;
    @sorted_second = sort @$second;

    $first = \@sorted_first;
    $second = \@sorted_second;
    for (my $i = 0; $i < @$first; $i++) {
        return 0 if $first->[$i] ne $second->[$i];
    }

    return 1;
}

sub print_profiles {
    my ($test_info, $profiles_a) = @_;

    print "$test_info: ", join(' ', @$profiles_a), "\n";
}

sub check_profiles {
    my ($test_info, $found_profiles, $expected_profiles) = @_;
#    &print_profiles($test_info, $profiles);

    my $failinfo = [];
    # Do number of found profiles match expected?
    if ($#$found_profiles != $#$expected_profiles) {
        push @$failinfo, "(num IOR profiles[$#$found_profiles] != expected[$#$expected_profiles]";
    }

    # Really need to compare these as hashes to avoid ordering issues.
    if (compare_arrays ($found_profiles, $expected_profiles) == 0) {
        push @$failinfo, "(profiles in IOR != profiles expected)";
        push @$failinfo, "Found profiles (".join(',', @$found_profiles).")";
    }

    return $failinfo;
}

# Brute force implementation of each of the lines in the table above

format STDOUT_TOP =
      |     |    -ORBendpoint     |                      |       Expected
FAIL? | DDA |      hostspec       |    hostname_in_ior   |   Profile(s) in IOR
*=============================================================================*
.
format STDOUT =
@<<<  | @|| | @<<<<<<<<<<<<<<<<<< | @<<<<<<<<<<<<<<<<<<< | ^<<<<<<<<<<<<<<<<<<<
$pf,    $dda, $endpointhost,        $hior,                 $expected_prof_in_ior
~~    |     |                     |                      | ^<<<<<<<<<<<<<<<<<<<
                                                           $expected_prof_in_ior
~     | Details: ^<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
                    $detail
~~    |     |    ^<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
                    $detail
------+-----+---------------------+----------------------+---------------------
.

$: = ', ';
for $test (sort keys %$TEST_DATA) {
    my $testargs = $TEST_DATA->{$test};
    my @p = &do_test ($testargs->[0], $testargs->[1], $testargs->[2]);
    my $failinfo = &check_profiles ($test, \@p, $TEST_DATA->{$test}[3]);

    # Set up all the global vars so we can write our output
    ($dda, $endpointhost, $hior) = split(':', $test);
    $expected_prof_in_ior = join(',', @{$TEST_DATA->{$test}[3]});
    if ($#$failinfo != -1) {
        $pf = 'FAIL';
        $detail = join("\r", @$failinfo);
    } else {
        $pf = 'OK';
        $detail = '';
    }

    write;
}

$server->DeleteFile($iorbase);
$server->DeleteFile($database);

exit $status;

# @p = &do_test(0, undef, undef);
# &check_profiles("0 unspec unspec", \@p, \@HOSTNAMES);

# @p = &do_test(1, undef, undef);
# &check_profiles("1 unspec unspec", \@p, \@IPADDRS);

# @p = &do_test(0, hostname, undef);
# &check_profiles("0 ".hostname." undef", \@p, [hostname]);

# @p = &do_test(1, hostname, undef);
# &check_profiles("1 ".hostname." undef", \@p, [ inet_ntoa((gethostbyname(hostname))[4]) ] );

# @p = &do_test(undef, undef, hostname . "_blech");
# &check_profiles("undef undef ".hostname."_blech", \@p, [hostname."_blech"]);

# @p = &do_test(undef, hostname, hostname."_blech");
# &check_profiles("undef ".hostname." ".hostname."_blech", \@p, [hostname."_blech"]);
