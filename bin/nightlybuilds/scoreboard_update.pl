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

my @builds = ();

# Hashes based on builds
my %build_latest_file;
my %build_latest_time;
my %build_type;
my %build_list;
my %build_web;

###############################################################################
#
# load_build_list
#
# Reads the list of builds from a file
#
# Arguments:  $ - file to read
#
# Returns:    Nothing
#
###############################################################################
sub load_build_list ($)
{
    my $file = shift;
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
    my %build;

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
            push (@builds, $buildname);
            next;
        }

        if (defined $buildname) {
            if (m/^\s*TYPE\s*=\s*(.*)/) {
                if (defined $build_type{$buildname}) {
                    print "Warning: Redefining TYPE for $buildname\n";
                }
                $build_type{$buildname} = $1;
            }
            if (m/^\s*LIST\s*=\s*(.*)/) {
                if (defined $build_list{$buildname}) {
                    print "Warning: Redefining LIST for $buildname\n";
                }
                $build_list{$buildname} = $1;
            }
            if (m/^\s*WEB\s*=\s*(.*)/) {
                if (defined $build_web{$buildname}) {
                    print "Warning: Redefining WEB for $buildname\n";
                }
                $build_web{$buildname} = $1;
            }
        }
    }

    @builds = sort (@builds);
}


###############################################################################
#
# query_latest
#
# Queries the web servers to figure out the latest build available and saves
# the list in build_latest_file and
# build_latest_time.
#
# @todo Actually since query_latest doesn't depend on the type anymore, we
#       could feasibly remove it.
#
# Arguments:  Nothing
#
# Returns:    Nothing
#
###############################################################################
sub query_latest ()
{
    print "Getting file lists\n";

    foreach my $build (@builds) {
        my @files = load_web_dir ($build_list{$build});

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
            elsif ($file =~ m/\Q$build\E\_(...)(..)_(....)\.txt/) {
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
            elsif ($file =~ m/\Q$build\E\_(....)_(....)\.txt/) {

                my $time = scalar ("2001".$1.$2);

                if ($time > $latest_time) {
                    $latest_time = $time;
                    $latest_file = $file;
                }
            }
            elsif ($file =~ m/\Q$build\E\_(....)\.txt/) {
                my $time = scalar ("2001".$1."0000");

                if ($time > $latest_time) {
                    $latest_time = $time;
                    $latest_file = $file;
                }
            }
        }

        # Save the latest
        if ($latest_time > 0) {
            $build_latest_file{$build} = $latest_file;
            $build_latest_time{$build} = $latest_time;
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

    foreach my $build (@builds) {
        ### Check to see if we had problems.  If there is no latest time,
        ### we had problems downloading.
        if (!defined $build_latest_time{$build}) {
            next;
        }

        my $time = $build_latest_time{$build};
        my $oldtime = '';
        my $address = $build_list{$build} . "/" 
                      . $build_latest_file{$build};

        my $filename = $build.'_'.$time.'.txt';

        print "    Looking at $build\n";

	mkpath "$directory/$build";

        if (! -r "$directory/$build/$filename") {
            print "        Downloading\n";
            my $ua = LWP::UserAgent->new;
            my $request = HTTP::Request->new('GET', $address);
            my $response = $ua->request($request, "$directory/$build/$filename");

            if (!$response->is_success ()) {
                warn 'Unable to download '.$address;
                return;
            }
        }

        my @existing = glob ($directory . '/' . $build . '/' . $build . '_*');

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
#
# Returns:    $ - color
#
###############################################################################
sub timestamp_color ($)
{
    my $timestamp = shift; 

    if ($timestamp =~ m/(....)(..)(..)(..)(..)/) {
        my $buildtime = timelocal (0, $5, $4, $3, $2 - 1, $1);

	my $nowtime = timelocal (localtime);

        if ($nowtime - $buildtime > (60 * 60 * 24 * 2)) {
            return 'red';
        }

        if ($nowtime - $buildtime > (60 * 60 * 24 * 1)) {
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
        warn 'Could not open '.$file.' '.$_;
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
        warn 'Could not open '.$file.' '.$_;
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
        return 'gray';
    }
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
    my $localshift = '';

    my $indexhtml = new FileHandle;

    print "Updating HTML\n";

    unless ($indexhtml->open (">$filename")) {
        warn 'Could not create file: '.$filename." ".$_;
        return;
    }

    ### Print Header

    print $indexhtml "<HTML>\n<HEAD>\n<TITLE>Build Scoreboard</TITLE>\n</HEAD>\n";

    ### Start body

    print $indexhtml "<BODY bgcolor=white>\n<H1>Build Scoreboard</H1>\n<HR>\n";

    ### Table

    print $indexhtml '<TABLE border=1><TH>Config<TH>Last Run<TH>CVS<TH>Compilation'.
                     "<TH>Tests\n";

    foreach my $build (@builds) {
        mkpath "$dir/$build/pretty";

        print "    Looking at $build\n";

        if (defined $build_latest_time{$build}) {
            my $time = $build_latest_time{$build};

            my $log = $dir . '/' . $build . '/' . $build . '_' . $time . ".txt";

            my $webfile = "$build/pretty/$build" . "_$time";
            my $newfile = "$dir/$webfile";

            $newfile =~ s/\//\\/g if ($OSNAME eq "MSWin32");
            $log =~ s/\//\\/g if ($OSNAME eq "MSWin32");

            if (!-e $newfile.'.html') {
                print "        Creating HTML for $time\n";

                my $command = 'perl '.$script.' -c makefile < '.$log.' > '.
                              $newfile.'.html';

                system ($command);
            }

            if (!-e $newfile.'_Brief.html') {
                print "        Creating HTML Brief for $time\n";
    
                my $command = 'perl '.$script.' -b -c '
                              .determine_type ($log).
                              ' < '.$log.' > '.
                              $newfile.'_Brief.html';

                system ($command);
            }

            print $indexhtml '<TR><TD>';

            print $indexhtml "<A HREF=\"".$build_web{$build} ."\">" if defined $build_web{$build};
            print $indexhtml $build;
            print $indexhtml "</A> " if defined $build_web{$build};
            
            print $indexhtml '<TD bgcolor=';
            print $indexhtml timestamp_color ($time);
            print $indexhtml '>',decode_timestamp ($time);
            print $indexhtml '<TD bgcolor=';
            print $indexhtml get_color ($newfile.'_Brief.html', 'cvs');
            print $indexhtml '>';
            print $indexhtml "[<A HREF=\"".$webfile.".html#cvs\">Full</A>] ";
            print $indexhtml "[<A HREF=\"".$webfile."_Brief.html#cvs\">Brief</A>]";
            print $indexhtml '<TD bgcolor=';
            print $indexhtml get_color ($newfile.'_Brief.html', 'compiler');
            print $indexhtml '>';
            print $indexhtml "[<A HREF=\"".$webfile.".html#compiler\">Full</A>] ";
            print $indexhtml "[<A HREF=\"".$webfile."_Brief.html#compiler\">Brief</A>]";
            print $indexhtml '<TD bgcolor=';
            print $indexhtml get_color ($newfile.'_Brief.html', 'tests');
            print $indexhtml '>';
            print $indexhtml "[<A HREF=\"".$webfile.".html#tests\">Full</A>] ";
            print $indexhtml "[<A HREF=\"".$webfile."_Brief.html#tests\">Brief</A>]";
            print $indexhtml "\n";

        }
        else {
            print $indexhtml '<TR><TD>';
            print $indexhtml "<A HREF=\"".$build_web{$build} ."\">$build</A> ";
            print $indexhtml '<TD bgcolor=gray>';
            print $indexhtml '<TD bgcolor=gray>';
            print $indexhtml "[Full] [Brief]";
            print $indexhtml '<TD bgcolor=gray>';
            print $indexhtml "[Full] [Brief]";
            print $indexhtml '<TD bgcolor=gray>';
            print $indexhtml "[Full] [Brief]";
            print $indexhtml "\n";
        }

        my @existing = glob ($dir . '/' . $build . '/pretty/' . $build . '_*.html');

        @existing = reverse sort @existing;

        # shift off the ones we want to keep
        # shift only twice to keep the last *.html and *_Brief.html
        shift @existing;  
        shift @existing;  

        foreach my $file (@existing) {
            print "        Removing $file\n";
            unlink $file;
      
        }
    }

    print $indexhtml "</TABLE>\n";

    ### Print timestamp

    print $indexhtml '<BR>Last updated at '.scalar (localtime ())."<BR>\n";

    ### Print the Footer

    print $indexhtml "</BODY>\n</HTML>\n";

    $indexhtml->close ();
}

###############################################################################
###############################################################################

# Getopts

use vars qw/$opt_c $opt_h $opt_m $opt_o/;

if (!getopts ('c:hm:o:') || defined $opt_h) {
    print "scoreboard_update.pl [-c file] [-h] [-o dir] [-m script]\n";
    print "\n";
    print "  input        input directory\n";
    print "  output       output directory\n";
    print "\n";
    print "    -c file    use <file> as the configuration file [def: bin/nightlybuilds/builds.lst]\n";
    print "    -h         display this help\n";
    print "    -m script  make pretty with this script [def:bin/make_pretty.pl\n";
    print "    -o dir     directory to place files [def: html]\n";
    print "    -r         remove files\n";
    exit (1);
}

my $pretty = "bin/make_pretty.pl";
my $file = "bin/nightlybuilds/builds.lst";
my $dir = "html";

if (defined $opt_c) {
    $file = $opt_c;
}

if (defined $opt_o) {
    $dir = $opt_o;
}

if (defined $opt_m) {
    $pretty = $opt_m;
}

# Do the stuff

print 'Running Scoreboard Update at '.scalar (localtime ())."\n";

load_build_list ($file);
query_latest ();
update_cache ($dir);
update_html ($dir, $pretty);

print 'Finished Scoreboard Update at '.scalar (localtime ())."\n";

###############################################################################
###############################################################################
