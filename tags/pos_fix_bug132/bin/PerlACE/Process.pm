# $Id$

package PerlACE::Process;

use strict;
use English;

$PerlACE::Process::ExeSubDir = './';

### Check for -ExeSubDir commands, store the last one
my @new_argv = ();

for(my $i = 0; $i <= $#ARGV; ++$i) {
    if ($ARGV[$i] eq '-ExeSubDir') {  
        if (defined $ARGV[$i + 1]) {
            $PerlACE::Process::ExeSubDir = $ARGV[++$i].'/';
        }
        else {
            print STDERR "You must pass a directory with ExeSubDir\n";
            exit(1);
        }
    }
    else {
        push @new_argv, $ARGV[$i];
    }
}
@ARGV = @new_argv;

if ($OSNAME eq "MSWin32") {
	require PerlACE::Process_Win32;
}
else {
	require PerlACE::Process_Unix;
}

1;
