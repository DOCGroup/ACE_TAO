eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$

use FindBin;
use lib $FindBin::Bin;
use PerlSam::Parser;
use PerlSam::Generator;

use strict;

package main;

$main::verbose = 0;

################################################################################

my $parser_name;
my @target_names;
my @file_names;

################################################################################
# Parse Args

sub print_usage ()
{
    print "Compiles SAM files into Makefiles/Projects\n";
    print "\n";
    print "sam.pl [-parser <name>] [-target <name>] [-target <name>]... [files...]\n";
    print "\n";
    print " -parser <name>      Use <name> parser instead of the default\n";
    print " -target <name>      Add <name> as a target\n";
    print " files               Files that should be compiled (defaults to sam.xml)\n";
}

# Read in ARGV

while ($#ARGV >= 0)
{
    if ($ARGV[0] =~ m/^-parser/i) {
        if (defined $parser_name) {
            print STDERR "Error:  Only one parser allowed\n";
            exit 1;
        }
        $parser_name = $ARGV[1];
        shift;
    }
    elsif ($ARGV[0] =~ m/^-target/i) {
        push @target_names, $ARGV[1];
        shift;
    }
    elsif ($ARGV[0] =~ m/^-v/i) {
        shift;
        if (!defined $ARGV[0] || $ARGV[0] =~ m/^-/) {
            print STDERR "Error:  Expecting a positive verbosity level\n";
            exit 1;
        }
        $main::verbose = $ARGV[0];
    }
    elsif ($ARGV[0] =~ m/^-(\?|h)/i) {
        print_usage ();
        exit;
    }
    elsif ($ARGV[0] =~ m/^-/) {
        print "Error: Unknown option $ARGV[0]\n";
        exit 1;
    }
    else {
        push @file_names, $ARGV[0];
    }
    shift;
}

# Check for valid parameters

if (!defined $parser_name) {
    $parser_name = PerlSam::Parser::GetDefault ();
}

if ($#target_names < 0) {
    @target_names = PerlSam::Generator::GetDefaults ();
}

if ($#file_names < 0) {
    @file_names = ('sam.xml');
}

# Print out verbose info

if ($main::verbose >= 1) {
    print "Parser Name: ", $parser_name, "\n";
    print "Target Names: ", join (' ', @target_names), "\n";
    print "File Names: ", join (' ', @file_names), "\n";
}

################################################################################
# Parse and Generate

if ($main::verbose >= 1) {
    print "=== Parse and Generate\n";
}

foreach my $file (@file_names) {
    my $parser = new PerlSam::Parser ($parser_name);
    my $generator = new PerlSam::Generator (@target_names);

    print "Compiling $file\n";
    my %data;
    my %libdata;

    if ($parser->Parse ($file, \%data)) {
        if ($parser->ParseLibraryFile ("$FindBin::Bin/libs.xml", \%libdata)) {
            $generator->SetLibraryInfo (\%libdata);
            $generator->GenerateDependencies (\%data);
            $generator->GenerateWorkspace (\%data);
            $generator->GenerateProjects (\%data);
        }
        else {
            print STDERR "Error: Unable to parse library file\n";
        }
    }
    else {
        print STDERR "Error: Unable to parse file <$file>\n";
    }
}
