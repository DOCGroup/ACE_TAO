eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$

use Getopt::Std;
use strict;
use English;
use FileHandle;
use File::Path;
use LWP::UserAgent;
use Time::Local;

###############################################################################
# Big bad variables

# %builds->{$name}->{GROUP}       <- Group this build is in
#                 ->{LATEST_FILE} <- The filename of the latest build
#                 ->{LATEST_TIME} <- The timestamp of the latest build
#                 ->{LIST_LINK}   <- Link to use to download builds from
#                 ->{WEB_LINK}    <- Link to use for directing to all logs
#                 ->{STATUS_LINK} <- Link to grab the build status from
#                 ->{MANUAL_LINK} <- Link to use to manually start a build
#                 ->{ORANGE_TIME} <- Number of hours before build turns orange
#                 ->{RED_TIME}    <- Number of hours before build turns red
#                 ->{STATUS}      <- Results of query of status link

my %builds;

# %groups->{$name} <- list of builds for $name

my %groups;

my @nogroup;

my $orange_default = 24;
my $red_default = 48;

###############################################################################
#
# load_build_list
#
# Reads the list of builds from a file
#
# Arguments:  $ - file to read
#             $ - report flag
#
# Returns:    Nothing
#
###############################################################################
sub load_build_list ($$)
{
    my $file = shift;
    my $report = shift;
    my @latest = ();

    print "Loading Build List\n";

    my $filehandle = new FileHandle;

    unless ($filehandle->open ("<$file")) {
        warn 'Cannot open: ', $file, ' ', $_;
        return;
    };

    my @data = <$filehandle>;

    $filehandle->close ();

    my $buildname = undef ;

    foreach (@data) {
        chomp;

        # Skip blank lines and comments
        if (m/^\s*$/) { next; };
        if (m/^\#/) { next; }

        # Remove trailing and leading spaces
        s/^\s*//;
        s/\s*$//;

        if (m/\[(.*)\]/) {
            $buildname = $1;

            # $todo: Could probably check for other bad character (\:) here

            if (defined %builds->{$buildname}) {
                print "ERROR: Build [$buildname] redefined, skipping\n";
                $buildname = undef;
            }
            elsif ($buildname =~ m/\s/) {
                print "ERROR: Name [$buildname] contains spaces, skipping\n";
                $buildname = undef;
            }

            next;
        }

        if (defined $buildname) {
            if (m/^\s*TYPE\s*=\s*(.*)/) {
                print "WARNING: TYPE is deprecated\n";
            }
            if (m/^\s*GROUP\s*=\s*(.*)/) {
                %builds->{$buildname}->{GROUP} = $1;
            }
            if (m/^\s*LIST\s*=\s*(.*)/) {
                %builds->{$buildname}->{LIST_LINK} = $1;
            }
            if (m/^\s*ORANGE\s*=\s*(.*)/) {
                %builds->{$buildname}->{ORANGE_TIME} = $1;
            }
            if (m/^\s*RED\s*=\s*(.*)/) {
                %builds->{$buildname}->{RED_TIME} = $1;
            }

            if (!$report) {
                if (m/^\s*WEB\s*=\s*(.*)/) {
                    %builds->{$buildname}->{WEB_LINK} = $1;
                }
                if (m/^\s*STATUS\s*=\s*(.*)/) {
                    %builds->{$buildname}->{STATUS_LINK} = $1;
                }
                if (m/^\s*MANUAL\s*=\s*(.*)/) {
                    %builds->{$buildname}->{MANUAL_LINK} = $1;
                }
            }
        }
    }
}

###############################################################################
#
# build_group_hash
#
# Looks at all the groups specified and collects the builds together
#
# Arguments:  Nothing
#
# Returns:    Nothing
#
###############################################################################
sub build_group_hash ()
{
    print "Grouping builds\n";

    foreach my $buildname (keys %builds) {
        if (defined %builds->{$buildname}->{GROUP}) {
            push @{%groups->{%builds->{$buildname}->{GROUP}}}, $buildname;
        }
        else {
            push @nogroup, $buildname;
        }
    }
}



###############################################################################
#
# query_latest
#
# Queries the web servers to figure out the latest build available and saves
# the list in LATEST_FILE and LATEST_TIME
#
# Arguments:  Nothing
#
# Returns:    Nothing
#
###############################################################################
sub query_latest ()
{
    print "Getting file lists\n";

    foreach my $buildname (keys %builds) {
        my @files = load_web_dir (%builds->{$buildname}->{LIST_LINK});

        my $latest_time= 0;
        my $latest_file = "";

        foreach my $file (@files) {
            if ($file =~ m/(....)_(..)_(..)_(..)_(..)\.txt/) {
                my $time = scalar ($1.$2.$3.$4.$5);

                if ($time > $latest_time) {
                    $latest_time = $time;
                    $latest_file = $file;
                }
            }
            elsif ($file =~ m/\Q$buildname\E\_(...)(..)_(....)\.txt/) {
                my %mon = ('Jan' =>  1, 'Feb' =>  2, 'Mar' =>  3,
                           'Apr' =>  4, 'May' =>  5, 'Jun' =>  6,
                           'Jul' =>  7, 'Aug' =>  8, 'Sep' =>  9,
                           'Oct' => 10, 'Nov' => 11, 'Dec' => 12);

                my $time = scalar (sprintf ('%0.4s%0.2s%0.2s0000', $3, $mon{$1}, $2));

                if ($time > $latest_time) {
                    $latest_time = $time;
                   $latest_file = $file;
                }
            }
            elsif ($file =~ m/\Q$buildname\E\_(....)_(....)\.txt/) {

                my $time = scalar ("2001".$1.$2);

                if ($time > $latest_time) {
                    $latest_time = $time;
                    $latest_file = $file;
                }
            }
            elsif ($file =~ m/\Q$buildname\E\_(....)\.txt/) {
                my $time = scalar ("2001".$1."0000");

                if ($time > $latest_time) {
                    $latest_time = $time;
                    $latest_file = $file;
                }
            }
        }

        # Save the latest results
        if ($latest_time > 0) {
            %builds->{$buildname}->{LATEST_FILE} = $latest_file;
            %builds->{$buildname}->{LATEST_TIME} = $latest_time;
        }
    }
}


###############################################################################
#
# query_status
#
# Queries the status links to figure out the latest status and stores in
# STATUS
#
# Arguments:  Nothing
#
# Returns:    Nothing
#
###############################################################################
sub query_status ()
{
    print "Getting status messages\n";

    foreach my $buildname (keys %builds) {
        my $link = %builds->{$buildname}->{STATUS_LINK};
        if (defined $link) {
            print "    Status [$buildname] from $link\n";

            my $ua = LWP::UserAgent->new;

            ### We are impatient, so don't wait more than 20 seconds for a
            ### response (the default was 180 seconds)
            $ua->timeout(20);

            my $request = HTTP::Request->new('GET', $link);
            my $response = $ua->request($request);

            if (!$response->is_success ()) {
                print "        ERROR: Could not load status\n";
                next;
            }

            my @contents = split /\n/, $response->content ();

            ### Now look for files

            foreach my $line (@contents) {
                if ($line =~ m/SCOREBOARD_STATUS\:(.*)$/) {
                    %builds->{$buildname}->{STATUS} = $1;
                }
            }
        }
    }
}



###############################################################################
#
# load_web_dir
#
# Loads the listing from a web resource, works with both Apache and IIS.
#
# Arguments:  $ - The URI of the directory on the web
#
# Returns:    @ - Listing of the files in that directory
#
###############################################################################
sub load_web_dir ($)
{
    my $address = shift;
    my $web_dir = '';
    my @files = ();

    print "    Loading list from $address\n";

    ### Split web_dir up a bit

    if ($address =~ m/^http:\/\/[\w.]*(.*)/) {
        $web_dir = $1;
    }
    else {
        warn "load_web_dir (): Badly formed http address";
        return ();
    }

    ### Request the web dir page

    my $ua = LWP::UserAgent->new;

    ### We are impatient, so don't wait more than 20 seconds for a
    ### response (the default was 180 seconds)
    $ua->timeout(20);

    my $request = HTTP::Request->new('GET', $address);
    my $response = $ua->request($request);

    if (!$response->is_success ()) {
        print "        ERROR: Could not load web dir\n";
        return ();
    }

    ### Pull out the contents based on the server type

    my $content = $response->content ();
    my $server = $response->server ();

    if ($server =~ m/Microsoft-IIS/) {

        ### split it up based on \n and <BR> and <P>

        $content =~ s/<BR>/\n/gi;
        $content =~ s/<P>/\n/gi;
        my @contents = split /\n/, $content;

        ### Now look for files

        foreach my $line (@contents) {
            if ($line =~ m/\"$web_dir([^\"\/]*)\"/) {
                push (@files, $1);
            }
        }
    }
    elsif ($server =~ m/Apache/ || $server =~ m/Boa/) {
        my @contents = split /\n/, $content;

        ### Now look for files
        foreach my $line (@contents) {
            if ($line =~ m/A HREF=\"([^\"\/\?]*)\"/i) {
                push (@files, $1);
            }
        }
    }
    else {
        warn 'load_web_dir (): Unrecognized server: '.$server;
    }

    return @files;
}


###############################################################################
#
# decode_timestamp
#
# Decodes a YYYYMMDDHHMM timestamp
#
# Arguments:  $ - encoded timestamp
#
# Returns:    $ - timestamp description
#
###############################################################################
sub decode_timestamp ($)
{
    my $timestamp = shift;
    my $description = '';

    if ($timestamp =~ m/(....)(..)(..)(..)(..)/) {
        my %mon = ( 1 => 'Jan',  2 => 'Feb',  3 => 'Mar',
                    4 => 'Apr',  5 => 'May',  6 => 'Jun',
                    7 => 'Jul',  8 => 'Aug',  9 => 'Sep',
                   10 => 'Oct', 11 => 'Nov', 12 => 'Dec');
        $description =
            sprintf ('%s %s, %s - %s:%s', $mon{int ($2)}, $3, $1, $4, $5);

    }
    else {
        warn 'Unable to decode time';
        $description = 'Unknown Time';
    }

    return $description;
}

###############################################################################
#
# update_cache
#
# Updates the local cache
#
# Arguments:  $ - directory to place files in
#
# Returns:    Nothing
#
###############################################################################
sub update_cache ($)
{
    my $directory = shift;

    print "Updating Local Cache\n";

    if (!-w $directory) {
        warn "Cannot write to $directory";
        return;
    }

    foreach my $buildname (keys %builds) {
        ### Check to see if we had problems.  If there is no latest time,
        ### we had problems downloading.
        if (!defined %builds->{$buildname}->{LATEST_TIME}) {
            next;
        }

        my $time = %builds->{$buildname}->{LATEST_TIME};
        my $oldtime = '';
        my $address = %builds->{$buildname}->{LIST_LINK} . "/" . %builds->{$buildname}->{LATEST_FILE};

        my $filename = $buildname.'_'.$time.'.txt';

        print "    Looking at $buildname\n";

        mkpath "$directory/$buildname";

        if (! -r "$directory/$buildname/$filename") {
            print "        Downloading\n";
            my $ua = LWP::UserAgent->new;
            my $request = HTTP::Request->new('GET', $address);
            my $response = $ua->request($request, "$directory/$buildname/$filename");

            if (!$response->is_success ()) {
                print "WARNING: Unable to download $address\n";
                return;
            }
        }

        my @existing = glob ($directory . '/' . $buildname
                             . '/' . $buildname . '_*');

        @existing = reverse sort @existing;

        # shift off the ones we want to keep
        shift @existing;  # for now, just the current one

        foreach my $file (@existing) {
            print "        Removing $file\n";
            unlink $file;

        }
    }
}


###############################################################################
#
# timestamp_color
#
# Decodes a YYYYMMDDHHMM timestamp and figures out the color
#
# Arguments:  $ - encoded timestamp
#             $ - orange hours
#             $ - red hours
#
# Returns:    $ - color
#
###############################################################################
sub timestamp_color ($$$)
{
    my $timestamp = shift;
    my $orange = shift;
    my $red = shift;

    if ($timestamp =~ m/(....)(..)(..)(..)(..)/) {
        my $buildtime = timegm (0, $5, $4, $3, $2 - 1, $1);

        my $nowtime = timegm (gmtime ());

        if ($nowtime - $buildtime > (60 * 60 * $red)) {
            return 'red';
        }

        if ($nowtime - $buildtime > (60 * 60 * $orange)) {
            return 'orange';
        }

        return 'white'
    }

    warn 'Unable to decode time';

    return 'gray';
}


###############################################################################
#
# determine_type
#
# Figures out whether a log is msvc or makefile
#
# Arguments:  $ - log file
#
# Returns:    Nothing
#
###############################################################################
sub determine_type ($)
{
    my $file = shift;
    my $msvc = 0;

    my $log = new FileHandle;

    unless ($log->open ("<$file")) {
        warn 'Error: Could not open '.$file.' '.$_;
        return 'makefile';
    }

    while (<$log>) {
        if (m/^--------------------Configuration/) {
            $msvc = 1;
            last;
        }
    }
    $log->close ();

    if ($msvc == 1) {
        return 'msvc';
    }
    else {
        return 'makefile';
    }
}


###############################################################################
#
# get_color
#
# Returns a color based on errors/warnings
#
# Arguments:  $ - input file
#             $ - Total type (cvs/compiler/tests)
#
# Returns:    Nothing
#
###############################################################################
sub get_color ($$)
{
    my $file = shift;
    my $type = shift;

    my $errors = 0;
    my $warnings = 0;
    my $found = 0;

    my $results = new FileHandle;

    unless ($results->open ("<$file")) {
        print STDERR 'Error: Could not open '.$file.": $!\n";
        return 'gray';
    }

    while (<$results>) {
        if ($type eq 'cvs') {
            if (m/^CVS Totals.*Modified:(.*) Conflicts:(.*) Unknown/) {
                $found = 1;
                $errors += $2;
                $warnings += $1;
            }
        }
        elsif ($type eq 'compiler') {
            if (m/Compiler Totals:  Errors:(.*)  Warnings:(.*)/) {
                $found = 1;
                $errors += $1;
                $warnings += $2;
            }
        }
        elsif ($type eq 'tests') {
            if (m/Test Failures:(.*)/) {
                $found = 1;
                $errors += $1;
            }
        }
    }
    $results->close ();

    if ($errors > 0) {
        return 'red';
    }
    elsif ($warnings > 0) {
        return 'orange';
    }
    elsif ($found == 1) {
        return 'lime';
    }
    else {
        return 'white';
    }
}


###############################################################################
#
# found_section
#
# Returns 1 if the file contains a section
#
# Arguments:  $ - input file
#             $ - type (config)
#
# Returns:    Nothing
#
###############################################################################
sub found_section ($$)
{
    my $file = shift;
    my $type = shift;

    my $found = 0;

    my $results = new FileHandle;

    unless ($results->open ("<$file")) {
        print STDERR 'Error: Could not open '.$file.": $!\n";
        return 0;
    }

    while (<$results>) {
        if ($type eq 'config') {
            if (m/#config/) {
                $found = 1;
                last;
            }
        }
    }
    $results->close ();

    return $found;
}



###############################################################################
#
# update_html
#
# Runs make_pretty on a bunch of files and creates an index.html
#
# Arguments:  $ - directory
#             $ - make_pretty script
#
# Returns:    Nothing
#
###############################################################################
sub update_html ($$)
{
    my $dir = shift;
    my $script = shift;
    my $filename = "$dir/index.html";

    my $indexhtml = new FileHandle;

    print "Generating Scoreboard\n";

    unless ($indexhtml->open (">$filename")) {
        warn 'Could not create file: '.$filename." ".$_;
        return;
    }

    ### Print Header

    print $indexhtml "<HTML>\n<HEAD>\n<TITLE>Build Scoreboard</TITLE>\n</HEAD>\n";

    ### Start body

    print $indexhtml "<BODY bgcolor=white>\n<H1>Build Scoreboard</H1>\n<HR>\n";

    ### Print link to new scoreboard

    print $indexhtml "<h2>Most nightly builds have been moved to the ";
    print $indexhtml "<A HREF=\"http://doc.ece.uci.edu/scoreboard\">New ";
    print $indexhtml "Scoreboard</A>.  Don't forget to check it out.</h2><HR>\n";

    ### Print tables (first the empty one)

    update_html_table ($dir, $script, $indexhtml, undef);
    foreach my $group (sort keys %groups) {
        update_html_table ($dir, $script, $indexhtml, $group);
    }

    ### Print timestamp

    print $indexhtml '<BR>Last updated at '.scalar (gmtime ())." UTC<BR>\n";

    ### Print the Footer

    print $indexhtml "</BODY>\n</HTML>\n";

    $indexhtml->close ();
}


###############################################################################
#
# update_html_table
#
# helper for update_html that prints a single table
#
# Arguments:  $ - directory
#             $ - make_pretty script
#             $ - output file handle
#             $ - group name
#
# Returns:    Nothing
#
###############################################################################
sub update_html_table ($$$@)
{
    my $dir = shift;
    my $script = shift;
    my $indexhtml = shift;
    my $name = shift;
    my $havestatus = 0;
    my $havemanual = 0;

    my @builds;

    ### Table

    # check to see if we are doing the "NONE" group
    if (!defined $name) {
        print "    Building table for ungrouped\n";
        @builds = sort @nogroup;
    }
    else {
        print "    Building table for group $name\n";
        @builds = sort @{%groups->{$name}};
        print $indexhtml "<A NAME=\"$name\"><H2></A>$name</H2>\n";
    }

    foreach my $buildname (@builds) {
        if (defined %builds->{$buildname}->{STATUS}) {
            $havestatus = 1;
        }
        if (defined %builds->{$buildname}->{MANUAL_LINK}) {
            $havemanual = 1;
        }
    }

    print $indexhtml "<TABLE border=1><TH>Build Name<TH>Last Finished";
    print $indexhtml "<TH>Config<TH>CVS<TH>Compile<TH>Tests";
    print $indexhtml "<TH>Manual" if ($havemanual);
    print $indexhtml "<TH>Status" if ($havestatus);
    print $indexhtml "\n";

    foreach my $buildname (@builds) {
        mkpath "$dir/$buildname/pretty";

        print "        Looking at $buildname\n";

        print $indexhtml '<TR><TD>';

        if (defined %builds->{$buildname}->{WEB_LINK}) {
            print $indexhtml "<A HREF=\"".%builds->{$buildname}->{WEB_LINK} ."\">" ;
            print $indexhtml $buildname;
            print $indexhtml "</A> ";
        }
        else {
            print $indexhtml $buildname;
        }

        my $webfile;
        my $newfile;

        if (defined %builds->{$buildname}->{LATEST_TIME}) {
            my $time = %builds->{$buildname}->{LATEST_TIME};

            my $log = $dir . '/' . $buildname . '/' . $buildname . '_' . $time . ".txt";

            $webfile = "$buildname/pretty/$buildname" . "_$time";
            $newfile = "$dir/$webfile";

            $newfile =~ s/\//\\/g if ($OSNAME eq "MSWin32");
            $log =~ s/\//\\/g if ($OSNAME eq "MSWin32");

            if (!-e $newfile.'.html') {
                print "            Creating HTML for $time\n";

                my $command = 'perl ' . $script . ' -c ' . determine_type ($log) .
                              ' -i ' . $log . ' -o ' . $newfile . '.html';

                system ($command);
            }

            if (!-e $newfile.'_Brief.html') {
                print "            Creating HTML Brief for $time\n";

                my $command = 'perl '.$script.' -b -c ' . determine_type ($log) .
                              ' -i ' . $log . ' -o ' . $newfile . '_Brief.html';

                system ($command);
            }

            my $orange = $orange_default;
            my $red = $red_default;

            if (defined %builds->{$buildname}->{ORANGE_TIME}) {
                $orange = %builds->{$buildname}->{ORANGE_TIME};
            }

            if (defined %builds->{$buildname}->{RED_TIME}) {
                $red = %builds->{$buildname}->{RED_TIME};
            }

            print $indexhtml '<TD bgcolor=';
            print $indexhtml timestamp_color ($time, $orange, $red);
            print $indexhtml '>',decode_timestamp ($time);

            my $color;

            print $indexhtml '<TD>';
            if (found_section ($newfile.'.html', 'config')) {
                print $indexhtml "[<A HREF=\"".$webfile.".html#config\">Config</A>] ";
            }
            else {
                print $indexhtml "&nbsp;";
            }

            $color = get_color ($newfile.'_Brief.html', 'cvs');
            print $indexhtml "<TD bgcolor=$color>";
            if ($color ne 'gray' && $color ne 'white') {
                print $indexhtml "[<A HREF=\"".$webfile.".html#cvs\">Full</A>] ";
                print $indexhtml "[<A HREF=\"".$webfile."_Brief.html#cvs\">Brief</A>]";
            }
            else {
                print $indexhtml "&nbsp;";
            }

            $color = get_color ($newfile.'_Brief.html', 'compiler');
            print $indexhtml "<TD bgcolor=$color>";
            if ($color ne 'gray' && $color ne 'white') {
                print $indexhtml "[<A HREF=\"".$webfile.".html#compiler\">Full</A>] ";
                print $indexhtml "[<A HREF=\"".$webfile."_Brief.html#compiler\">Brief</A>]";
            }
            else {
                print $indexhtml "&nbsp;";
            }

            $color = get_color ($newfile.'_Brief.html', 'tests');
            print $indexhtml "<TD bgcolor=$color>";
            if ($color ne 'gray' && $color ne 'white') {
                print $indexhtml "[<A HREF=\"".$webfile.".html#tests\">Full</A>] ";
                print $indexhtml "[<A HREF=\"".$webfile."_Brief.html#tests\">Brief</A>]";
            }
            else {
                print $indexhtml "&nbsp;";
            }
        }
        else {
            print $indexhtml '<TD bgcolor=gray>&nbsp;'; # Time
            print $indexhtml '<TD bgcolor=gray>&nbsp;'; # Config
            print $indexhtml '<TD bgcolor=gray>&nbsp;'; # CVS
            print $indexhtml '<TD bgcolor=gray>&nbsp;'; # Compiler
            print $indexhtml '<TD bgcolor=gray>&nbsp;'; # Tests
        }


        if ($havemanual) {
            print $indexhtml "<TD align=center>";
            if (defined %builds->{$buildname}->{MANUAL_LINK}) {
                print $indexhtml "<INPUT TYPE=\"BUTTON\" VALUE=\"Start\" ";
                print $indexhtml "ONCLICK=\"window.location.href='";
                print $indexhtml %builds->{$buildname}->{MANUAL_LINK};
                print $indexhtml "'\">";
            }
            else {
                print $indexhtml "&nbsp;";
            }
        }
        if ($havestatus) {
            print $indexhtml "<TD>";
            if (defined %builds->{$buildname}->{STATUS}) {
                print $indexhtml "<A HREF=\"", %builds->{$buildname}->{STATUS_LINK}, "\"\>";
                print $indexhtml %builds->{$buildname}->{STATUS};
                print $indexhtml "</A>";
            }
            else {
                print $indexhtml "&nbsp;";
            }
        }

        print $indexhtml "\n";

        my @existing = glob ($dir . '/' . $buildname . '/pretty/' . $buildname . '_*.html');

        @existing = reverse sort @existing;

        # shift off the ones we want to keep
        # shift only twice to keep the last *.html and *_Brief.html
        shift @existing;
        shift @existing;

        foreach my $file (@existing) {
            print "            Removing $file\n";
            unlink $file;

        }
    }

    print $indexhtml "</TABLE>\n";
}



###############################################################################
###############################################################################

# Getopts

use vars qw/$opt_c $opt_h $opt_m $opt_o $opt_r/;

if (!getopts ('c:hm:o:r') || defined $opt_h) {
    print "scoreboard_update.pl [-c file] [-h] [-o dir] [-m script] [-r]\n";
    print "\n";
    print "    -c file    use <file> as the configuration file [def: bin/nightlybuilds/builds.lst]\n";
    print "    -h         display this help\n";
    print "    -m script  make pretty with this script [def:bin/make_pretty.pl\n";
    print "    -o dir     directory to place files [def: html]\n";
    print "    -r         generate report\n";
    exit (1);
}

my $pretty = "bin/make_pretty.pl";
my $file = "bin/nightlybuilds/builds.lst";
my $dir = "html";
my $report = 0;

if (defined $opt_c) {
    $file = $opt_c;
}

if (defined $opt_o) {
    $dir = $opt_o;
}

if (defined $opt_m) {
    $pretty = $opt_m;
}

if (defined $opt_r) {
    $report = 1;
}

# Do the stuff

print 'Running Scoreboard Update at '.scalar (gmtime ())."\n";

load_build_list ($file, $report);
build_group_hash ();
query_latest ();
update_cache ($dir);
query_status ();
update_html ($dir, $pretty);

print 'Finished Scoreboard Update at '.scalar (gmtime ())."\n";

###############################################################################
###############################################################################
