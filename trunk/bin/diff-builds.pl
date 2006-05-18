eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use File::Temp qw/ tempfile tempdir /;


my $debugging = 0; # Print additional info
my $verbose = '-q'; # WGET verbosity
my $new_errors_only = 0; # Show new errors only

# The root of the test statistics
my $teststaturl = "wget $verbose http://www.dre.vanderbilt.edu/~remedynl/teststat/builds/";

# Determine the timestamp by scanning the index
sub find_timestamps  {

    my ($file,$date) = @_;

#    print "\nSearching for $file, $date\n";
    open (INDEX, $teststaturl . $file . ".html -O - |")
        || die "Could not read the index page\n";

    # Split at all HTML tags, except <a ..>
    my @suffixes = split ( /[<][b-zB-Z\/]+[>]/, <INDEX>);
    close (INDEX);

    # Select only those of the "href=..." that match our file and date
    my $rx = quotemeta ( $file . '_' . $date);
    my @temp = map { (/${rx}_([0-9][0-9]_[0-9][0-9])/) ? $1 : undef } @suffixes; 
    return grep /^[0-9]/, @temp; 
}

# Determine the timestamp by scanning the index
sub find_closest_earlier  {

    my ($file,$date) = @_;

    open (INDEX, $teststaturl . $file . ".html -O - |")
        || die "Could not read the index page\n";

    # Split at all HTML tags, except <a ..>
    my @suffixes = split ( /[<][b-zB-Z\/]+[>]/, <INDEX>);
    close (INDEX);

    # Select only those of the "href=..." that match our file
    my $rx = quotemeta ( $file);
    my @temp = map { (/${rx}_([_0-9]+)([0-9][0-9]_[0-9][0-9])/ && $1 le $date) ? $1 : undef } @suffixes;
    my @temp2 = grep /^[0-9]/, @temp; 
    return $temp2[0];
}


my @dates, @builds, @files, @tmpfiles, @fh;

while ($arg = shift(@ARGV)) {

    if ($arg eq "-h" || $arg eq "-?") {
      print "Prints a diff for the list of test failures, for two builds on a certain date\n\n";
      print "diff-builds [-n] [-d] [-D date] build [build ...]\n";
      print "\n";
      print "-n                  -- show only new test failing (default=no)\n";
      print "-d                  -- show debug info\n";
      print "-h                  -- prints this information\n";
      print "-D date             -- specify a date, format is YYYY_MM_DD\n";
      print "                       use two to specify an interval\n";
      print "build               -- specify the build name, as it appears on the scoreboard\n";
      print "                       also can use two builds, with one date\n";
      exit 0;
    }
    if ($arg eq '-D') {
        push (@dates, shift(@ARGV));
    }
    elsif ($arg eq '-v') {
        $verbose = undef;
    }
    elsif ($arg eq '-d') {
        $debugging = 1;
    }
    elsif ($arg eq '-n') {
        $new_errors_only = 1;
    }
    else {
        push (@builds, $arg);
    }
}

die "More than one date or buld name are required" 
    unless ($#dates + $#builds ge 1);

print "dates=@dates ($#dates)\n"
    unless !$debugging;

print "builds=@builds ($#builds)\n"
    unless !$debugging;

if ($#dates eq 1) {
    $files[0] = $builds[0];
    $files[1] = $builds[0];
}
elsif ($#builds eq 1) {
    $files[0] = $builds[0];
    $files[1] = $builds[1];

    $dates[1] = $dates[0];

}
else {
        die "Dates: $#dates, Builds: $#builds\n";
}

print "files[0]=$files[0]\n"
    unless !$debugging;

print "files[1]=$files[1]\n"
    unless !$debugging;

my @timestamps0 = find_timestamps ($files[0], $dates[0]);
print "Build times for $files[0] on $dates[0] are " . join (', ', @timestamps0) . "\n"
    unless !$debugging;

die "No builds for $files[0] on $dates[0]. The closest ealier date is " 
    . find_closest_earlier ($files[0], $dates[0]) . "\n" 
    unless ($#timestamps0 > -1);

my @timestamps1 = find_timestamps ($files[1], $dates[1]);
print "Build times for $files[1] on $dates[1] are " . join (', ', @timestamps1) . "\n"
    unless !$debugging;

die "No builds for " . $files[1] . " on " . $dates[1] . ". The closest earlier date is " 
    . find_closest_earlier ($files[1], $dates[1]) . "\n" 
    unless ($#timestamps1 > -1);


# Retrieve the list of failed tests
($fh[0], $tmpfiles[0]) = tempfile();
system ($teststaturl 
        . $files[0] 
        .'_' . $dates[0] 
        . '_' . $timestamps0[0] 
        . ".log -O - | sort >" 
        . $tmpfiles[0]);
close ($fh[0]);

($fh[1], $tmpfiles[1]) = tempfile();
system ($teststaturl 
        . $files[1] 
        .'_' . $dates[1] 
        . '_' . $timestamps1[0] 
        . ".log -O - | sort >" 
        . $tmpfiles[1]);
close ($fh[1]);

print ("diff -u " . $tmpfiles[0] . ' ' . $tmpfiles[1] . " 2>&1 \n")
    unless !$debugging;

open (DIFF, "diff -u " . $tmpfiles[0] . ' ' . $tmpfiles[1] . " 2>&1 |")
    || die "Could not diff the files\n";

while (<DIFF>) {
    if (/^[^\+]/) {
        print unless ($new_errors_only == 1);
    } else { 
        print;
    }
}

close (DIFF);


__END__

=head1 diff-builds.pl Diff the lists of failing tests 

=item DESCRIPTION
Prints a diff for the list of test failures, for two builds on a certain date. 
Or, for two dates and a certain build.


=item EXAMPLE

diff-builds.pl WinXP_VC71_NET_Static_Debug -D 2006_04_17 -D 2006_05_12

=item AUTHOR
Iliyan Jeliazkov <iliyan@ociweb.com>
