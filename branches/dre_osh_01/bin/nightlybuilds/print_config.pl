eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

#
# @file print_config.pl
#
# $Id$
#
# @author Darrell Brunsch <brunsch@doc.ece.uci.edu>
#
# Print out the build configuration, i.e. the contents of the config.h
# file, the latest ChangeLog entries and (if present) the
# platform_macros.GNU file.
#

use strict;
use FileHandle;

##############################################################################

sub print_file ($$)
{
    my $filename = shift;
    my $printall = shift;

    my $filehandle = new FileHandle ($filename, "r");

    while (<$filehandle>) {
        print $_;

        last if ($printall == 0);
    }
}

##############################################################################

if ($#ARGV < 0) {
    print STDERR "Not enough arguments, pass in ACEROOT\n";
    exit 1;
}

my $root = shift @ARGV;

# convert to forward slash
$root =~ s/\\/\//g;

# chop off trailing slash
if ($root =~ m/^(.*)\/$/) {
    $root = $1;
}

# last ACE Changelog Entry

if (-r "$root/ChangeLog") {
    print "================ ACE ChangeLog ================\n";
    print_file ("$root/ChangeLog", 0);
}

# last TAO Changelog Entry
if (-r "$root/TAO/ChangeLog") {
    print "================ TAO ChangeLog ================\n";
    print_file ("$root/TAO/ChangeLog", 0);
}

# config.h, if it exists
if (-r "$root/ace/config.h") {
    print "================ config.h ================\n";
    print_file ("$root/ace/config.h", 1);
}

# platform_macros.GNU, if it exists
if (-r "$root/include/makeinclude/platform_macros.GNU") {
    print "================ platform_macros.GNU ================\n";
    print_file ("$root/include/makeinclude/platform_macros.GNU", 1);
}
