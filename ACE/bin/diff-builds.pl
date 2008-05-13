eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use File::Spec qw/ tmpdir /;
use File::Temp qw/ tempfile tempdir /;
use POSIX qw/ strftime /;

my $debugging = 0; # Print additional info
my $verbose = '-q'; # WGET verbosity
my $new_errors_only = 0; # Show new errors only
my $clean_builds_only = 1; # Only diff todays clean builds

# The root of the test statistics
my $teststaturl = "http://download.theaceorb.nl/teststat/builds/";

my $allbuildsurl = "http://download.theaceorb.nl/teststat/buildscore.txt";
my $cleanbuildsurl = "http://download.theaceorb.nl/teststat/cleanbuildtests.txt";

# Determine the available timestamps for a build on a date,
# by scanning the index page (build.html)
sub find_timestamps ($$)  {

    my ($file,$date) = @_;

#    print "\nSearching for $file, $date\n";
    open (INDEX, "wget " . $verbose . " \'" . $teststaturl . $file . ".html\' -O - |")
        || die "***Could not read the index page for $file\n";

    # Split at all HTML tags, except <a ..>
    my @suffixes = split ( /[<][b-zB-Z\/]+[>]/, <INDEX>);
    close (INDEX);

    # Select only those of the "href=..." that match our file and date
    my $rx = quotemeta ( $file . '_' . $date);
    my @temp = map { (/${rx}_([0-9][0-9]_[0-9][0-9])/) ? $1 : "" } @suffixes; 
    return grep /^[0-9]/, @temp; 
}

# Determine the timestamp by scanning the index
sub find_closest_earlier  {

    my ($file,$date) = @_;

    open (INDEX, "wget " . $verbose . " \'" . $teststaturl . $file . ".html\' -O - |")
        || die "***Could not read the index page for $file\n";

    # Split at all HTML tags, except <a ..>
    my @suffixes = split ( /[<][b-zB-Z\/]+[>]/, <INDEX>);
    close (INDEX);

    # Select only those of the "href=..." that match our file
    my $rx = quotemeta ( $file);
    my @temp = map { (/${rx}_([0-9][0-9][0-9][0-9]_[0-9][0-9]_[0-9][0-9])/ && $1 le $date) ? $1 : undef } @suffixes;
    my @temp2 = grep /^[0-9]/, @temp;

    if ($#temp2 == -1) {
        return undef;
    }

    return $temp2[0];
}


sub select_builds ($$$)
{
    my ($rdates, $rbuilds, $rfiles) = @_;
    my @dates = @{$rdates};
    my @builds = @{$rbuilds};

    if ($#dates eq 1) {
        $rfiles->[0] = $rbuilds->[0];
        $rfiles->[1] = $rbuilds->[0];
    }
    elsif ($#builds eq 1) {
        $rfiles->[0] = $rbuilds->[0];
        $rfiles->[1] = $rbuilds->[1];
        
        $rdates->[1] = $rdates->[0];
        
    }
    else {
        die "Dates: $#dates, Builds: $#builds\n";
    }

    return 0;
}


sub load_failed_tests_list ($$)
{
    my ($file, $original_date) = @_;
    
    my $date = $original_date;
    my $last_tried_date = $original_date;
    my @timestamps = ();

    while ($#timestamps < 0) {
     
        @timestamps = find_timestamps ($file, $date);

        if ($#timestamps == -1) {
            $date = find_closest_earlier ($file, $date);
            if (!$date) {
                print "***Found no builds for $file on, or before $original_date\n";
                return File::Spec->devnull();
            }

            print "***No builds for $file on $last_tried_date. The closest earlier is " 
                . $date . "\n";

            $last_tried_date = $date;
            next;
        }

        print "Build times for $file on $date are " 
            . join (', ', @timestamps) . "\n" unless !$debugging;
    }
    
    my $tmpdir = File::Spec->tmpdir();
    my $fullfile = $file .'_' . $date . '_' . $timestamps[0];
    my ($fh, $tmpfile) = tempfile ($fullfile . ".XXXXXX", UNLINK => 1, DIR => $tmpdir);

    print "wget " . $verbose . " \'" .$teststaturl 
            . $fullfile . ".txt\' -O - | sort >\'" . $tmpfile . '\'' . "\n" unless !$debugging;

    system ("wget " . $verbose . " \'" .$teststaturl 
            . $fullfile . ".txt\' -O - | sort >\'" . $tmpfile . '\'');
    close ($fh);
    
    return $tmpfile;
}

sub differentiate ($$)
{
    my ($rfiles, $rdates) = @_;

    print "Difference for dates " . join (', ', @$rdates) . "\n" unless !$debugging;

    my $first_file = load_failed_tests_list ($rfiles->[0], $rdates->[0]);
    my $second_file = load_failed_tests_list ($rfiles->[1], $rdates->[1]);

    open (DIFF, "diff -u \'" . $first_file . "\' \'" . $second_file . "\' 2>&1 |")
        || die "***Failed to diff \'" . $first_file . "\' \'" . $second_file . "\'\n";

    while (<DIFF>) {
        
        # Don't filter out the build details when printing the new errors only
        if (/^---/) { 
            print;
        }
        elsif (/^[^\+]/) {
            print unless ($new_errors_only == 1);
        } 
        else {
            print;
        }
    }

    close (DIFF);
    print "\n";
}


sub find_builds ($$$)
{
    my ($rbuilds, $buildscoreurl, $selectcolumn) = @_;

    print "Reading from $buildscoreurl\n" unless !$debugging;

    open (CLEANS, "wget " . $verbose . " \'" . $buildscoreurl . "\' -O - |")
        || die "Could not read builds score page $buildscoreurl\n";

    # Split at all spaces
    for(my $begin=0; <CLEANS>;)
    {
        chomp;
        my @columns = split (/ +/);

        if (/=+/) {
            $begin++;
            next;
        }

        push (@{$rbuilds}, $columns[$selectcolumn]) unless !$begin;

    }
    close (CLEANS);
    sort @{$rbuilds};

    print "Using builds @{$rbuilds}\n" unless !$debugging;
}

my @dates = ();
my @builds = (); 
my @files = ();


while ($arg = shift(@ARGV)) {

    if ($arg eq "-h" || $arg eq "-?") {
      print "Prints a diff for the list of test failures, for two builds on a certain date\n\n";
      print "diff-builds [-n] [-d] [-D date] [-A] [build ...]\n";
      print "\n";
      print "  -n          -- Show only new test failing (default=no)\n";
      print "  -d          -- Show debug info\n";
      print "  -h          -- Prints this information\n";
      print "  -D date     -- Specify a date. Either YYYY_MM_DD or YYYY-MM-DD works\n";
      print "                 Use two date parameters to specify an interval\n";
      print "  -A          -- Use all builds, not just the clean (successful) ones\n"; 
      print "  build       -- Specify the build name. As it appears on the scoreboard\n";
      print "                 Works with two builds and one date to show the differences\n";
      print "                 between them. One build and two dates works, too.\n";
      print "                 Just a single date (no builds) implies comparing all of \n";
      print "                 today's builds with the builds on the supplied date.\n";
      exit 0;
    }
    if ($arg eq '-D') {
        my $date = shift(@ARGV);
        $date =~ s/-/_/g;
        push (@dates, $date);
        print "Date=$date\n"
            unless !$debugging;
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
    elsif ($arg eq '-A') {
        $clean_builds_only = 0;
    }
    else {
        push (@builds, $arg);
        print "Build=$arg\n"
            unless !$debugging;
    }
}


# Diff the todays clean builds with the ones from a specific date
if ($#builds == -1 && $#dates >= 0)
{
    if ($clean_builds_only) {
        find_builds (\@builds, $cleanbuildsurl, 7);
    }
    else {
        find_builds (\@builds, $allbuildsurl, 3);
    }
        
    # only the start date given - implies we should 
    # use the today's date
    if ($#dates == 0) { 
        $dates[1] = strftime ("%Y_%m_%d", gmtime);
    }

    foreach $build (sort @builds) {
        $files[0] = $files[1] = $build;
        differentiate (\@files, \@dates);
    }
}
else
{

    die "More than one date or build name are required" 
        unless ($#dates + $#builds ge 1);
    
    print "dates=@dates ($#dates)\n"
        unless !$debugging;
    
    print "builds=@builds ($#builds)\n"
        unless !$debugging;
    
    select_builds (\@dates, \@builds, \@files);
    differentiate (\@files, \@dates);
}
__END__

=head1 diff-builds.pl Diff the lists of failing tests 

=item DESCRIPTION
Prints a diff for the list of test failures, for two builds on a certain date. 
Or, for two dates and a certain build.


=item EXAMPLE

diff-builds.pl WinXP_VC71_NET_Static_Debug -D 2006_04_17 -D 2006_05_12

=item AUTHOR
Iliyan Jeliazkov <iliyan@ociweb.com>

