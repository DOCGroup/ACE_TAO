#!/usr/bin/perl
#
# $Id$
#

use Cwd;

# Customize the following variables for your installation

# The URL prefix that should be generated for each log file
$BASE_URL='http://doc.ece.uci.edu/~bugzilla/auto_compile_logs';

# The real location of the logs
$BASE_DIR='/home/bugzilla/.www-docs/auto_compile_logs';

# The subdirectories that contain logs, all other directories are ignored
@KNOWN_DIRS= ('balar_Minimum',
	      'doc_KCC',
	      'kelvar_Static',
	      'sirion_Full',
	      'tango_Core',
	      'tango_Exceptions'
	      );

# After customizing the variables above, you must remove or comment
# out the following code
print "Content-Type: text/html\n\n";
print "Error: You must customize this script before installation\n";
exit (0);

# END OF CUSTOMIZABLE SECTION

$title = $ENV{QUERY_STRING};

### Set up the directory for the configs we support

@matches = grep {/^$title$/} @KNOWN_DIRS;

if ($#matches == -1) {
    print "Content-Type: text/html\n\n";
    print "Error: Unknown config: $title\n";
    exit (0);
}

$dir = $title;

### Get the directory list

if (!opendir (DIR, "$BASE_DIR/$dir"))
{
    print "Content-Type: text/html\n\n";
    print "Error Could not open $dir<br>$!";
    exit (1);
}

@log_files = grep !/^\./, readdir (DIR);
closedir (DIR);

############################################################################
####
sub one_liner ($$)
{
    my $basename = shift;
    my $datetime = shift;

    my @months = qw (blank January February March April May June July August
                     September October November December);
    ### Decode the time/date

    if ($datetime =~ /(.{4}?)_(.{2}?)_(.{2}?)_(.{2}?)_(.{2}?)/) {
        $datetime = "$months[$2] $3 at $4\:$5";
    }

    return "$title build for $datetime : "
           . "<a href = \"$BASE_URL/$dir/$basename.txt\">[TXT]</a> "
           . "<a href = \"$BASE_URL/$dir/$basename.html\">[HTML]</a> "
           . "<a href = \"$BASE_URL/$dir/$basename\_brief.html\">[Brief]</a>";
}

sub log_detail ($$)
{
    my $basename = shift;
    my $datetime = shift;
    my $string = "";

    if (!open (LOG, "<$BASE_DIR/$dir/$basename\_brief.html")) {
        return "Error: Could not open $dir/$basename\_brief.html";
    }

    while (<LOG>) {
        if (/^Compiler Totals:  Errors: (\d*)  Warnings: (\d*)/) {
            if ($1 > 0) {
                $string = $string."<font
color=\"FF0000\">".$_."</font><br>";
            }
            elsif ($2 > 0) {
                $string = $string."<font
color=\"FF7700\">".$_."</font><br>";
            }
            else {
                $string = $string.$_."<br>";
            }
        }

        if (/^Test Failures: (\d*)/) {
            if ($1 > 0) {
                $string = $string."<font
color=\"FF0000\">".$_."</font><br>";
            }
            else {
                $string = $string.$_."<br>";
            }
        }
    }

    close (LOG);

    return $string;
}

############################################################################
####

print "Content-Type: text/html\n\n";

print <<HEADER;
<html>
<head>
<title>Nightly Build Results for $title</title>
</head>
HEADER

print "<body bgcolor=\"#FFFFFF\">\n";

print "<h1>Nightly Build Results for $title</h1>\n";

### Remove all non-logs from the list

@new_logs = ();
foreach $log (@log_files) {
    if ($log =~ /_brief.html$/) {
        push @new_logs, ($log);
    }
}
@log_files = @new_logs;

### Sort in reverse order, so the latest date comes first

@log_files = sort {$b cmp $a} @log_files;

### Print out latest log

$latest = shift @log_files;

print "<h2>Latest Log</h2>";
$latest =~ /(.*)_brief\.html/;

print log_detail ("$1", $1)."<br>";
print one_liner ("$1", $1);

### Print out a list of the files

print "<h2>Old Logs</h2>";
print "<ul>\n";
foreach $log (@log_files) {
    if ($log =~ /(.*)_brief\.html$/) {
        my $basename = $1;

        $log =~ /(.*)_brief\.html/;

        print "<li> ".one_liner ($basename, $1)."\n";
    }
}
print "</ul>\n";

print "<a href = \"../\">Back</a>";
print "</body>\n";
print "</html>\n";
