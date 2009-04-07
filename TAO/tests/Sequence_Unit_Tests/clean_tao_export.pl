eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use strict;

my $file_to_clean = $ARGV[0];

my @new_file_contents = ();
open (IFILE, $file_to_clean) or die "Can't open $file_to_clean: $!\n";
while (<IFILE>) {
    my $line = $_;
    if ($line =~ /^(.*)\s+TAO_Export\s+(.*)$/g) {
        push @new_file_contents, "$1 $2\n";
    } else {
        push @new_file_contents, $line;
    }
}
close IFILE;

open (OFILE, ">$file_to_clean") or die "Can't open $file_to_clean: $!\n";
print OFILE @new_file_contents;
close OFILE;

exit 0;
