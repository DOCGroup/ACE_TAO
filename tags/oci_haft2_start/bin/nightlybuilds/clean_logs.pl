# $Id$

# The purpose of this script is to clean out a log directory that
# is used usually with the scoreboard and show_log_dir stuff.

use DirHandle;

$keptlogs = 20;
$debug = 0;

while ($#ARGV >= 0)
{
    if ($ARGV[0] =~ m/^-debug/i) {
        $debug = 1;
    }
    elsif ($ARGV[0] =~ m/^-keptlogs/i) {
        $keptlogs = $ARGV[1];
        shift;
    }    
    elsif ($ARGV[0] =~ m/^-(\?|h)/i) {     # Help information
        print "clean_logs.pl [-debug] [-keptlogs <num>] (dirs...)\n";
        print "\n";
        print "Cleans out directories of YYYY_MM_DD_HH_MM logs\n";
        print "\n";
        print "Options\n";
        print "-debug           - Do not delete, just print\n";
        print "-keptlogs <num>  - Delete all but <num> sets of logs (default:20)\n";
        exit;
    }
    elsif ($ARGV[0] =~ m/^-/) {
        warn "$0:  unknown option $ARGV[0]\n";
        exit 1;
    }
    else {
        my $dir = $ARGV[0];
        $dir =~ s/\\/\//g;
        push @dirs, $dir;
    }
    shift;
}

if (!defined @dirs) {
    push @dirs , ".";
}

foreach $dir (@dirs) {
    my @existing;
    $d = new DirHandle ($dir);

    if (defined $d) {
        while (defined($_ = $d->read)) { 
            if ($_ =~ m/^(....)_(..)_(..)_(..)_(..)/) {
                push @existing, $dir . '/' . $_;
            }
        }
        undef $d;
    }

    @existing = reverse sort @existing;

    for ($i = 0; $i < $keptlogs; ++$i) {
        shift @existing;  
        shift @existing;  
        shift @existing;  
    }

    foreach my $file (@existing) {
        print "        Removing $file\n";
        if ($debug == 0) {
            unlink $file;
        }
    }
}
