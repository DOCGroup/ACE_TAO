# $Id$
#
# File: dsp2dsp.pl
# Description: This perl script is used to "create" new dsp files
#              by modifying an existing dsp file.
# Requirement: perl for Win32.
# Usage: perl dsp2dsp.pl Basic_Types_Test Atomic_Op_Test
#        This will create Atomic_Op_Test.dsp by using
#        Basic_Types_Test.dsp as a template.
#
# Author: Nanbor Wang <nanbor@cs.wustl.edu>

die "$0 requires two arguments.\n" if $#ARGV < 1;
$TOFILE="$ARGV[1].dsp" ;
die "$TOFILE already exists.  Remove it first.\n" if -e $TOFILE;

$FROM = $ARGV[0];
$TO = $ARGV[1];

open (FROM, "<$FROM.dsp");
open (TO, ">$TO.dsp");

while (<FROM>)
{
    s/$FROM/$TO/g;
    print TO ;
}

close (TO);
