# $Id$

package PerlACE::ConfigList;
use strict;
use FileHandle;

@PerlACE::ConfigList::Configs = ();

my @new_argv = ();

for(my $i = 0; $i <= $#ARGV; ++$i) {
    if ($ARGV[$i] eq '-Config') {  
        if (defined $ARGV[$i + 1]) {
            push @PerlACE::ConfigList::Configs, $ARGV[++$i];
        }
        else {
            print STDERR "You must pass a configuration with Config\n";
            exit(1);
        }
    }
    else {
        push @new_argv, $ARGV[$i];
    }
}
@ARGV = @new_argv;


sub new () 
{
    my $self = {};
    @{$self->{MY_CONFIGS}} = @PerlACE::ConfigList::Configs;
    bless $self;
    return $self;
}

sub my_config_list
{
    my $self = shift;
    if (@_) { @{$self->{MY_CONFIGS}} = @_; }
    return @{$self->{MY_CONFIGS}};
}

sub add_one_config ($)
{
    my $self = shift;
    my $newconfig = shift;
    push @{$self->{MY_CONFIGS}}, $newconfig;
}

sub check_config (@)
{
    my $self = shift;
    my @testconfigs = @_;
    my $included = 0;
    my $excluded = 0;
    my $noincludes = 1;

    foreach my $config (@testconfigs) {
        if ($config =~ /^\w/) { $noincludes = 0; }
        foreach my $myconfig (@{$self->{MY_CONFIGS}}) {
            if ($config eq "!$myconfig") { $excluded = 1; }
            if ($config eq $myconfig) { $included = 1; }
        }
    }
    return ($included || $noincludes) && !$excluded;
}

sub load ($)
{
    my $self = shift;
    my $filename = shift;

    my $fh = new FileHandle;
    if (!$fh->open ("< $filename")) {
        print STDERR "Could not open $filename: $!\n";
        exit (1);
    }

    while (<$fh>) {
        chomp;
	if (/^\s*$/ || /^#/) { 
            next; 
        }
        # compress white space
	s/\s+/ /g;

        my $entry = '';
        my $configs = '';
            
        ($entry, $configs) = split /:/;

        push @{$self->{ENTRIES}}, $entry;
	if (defined $configs) {
            @{$self->{CONFIGS}->{$entry}} =  split (" ", $configs);
        }
    }

    $fh->close ();
}

sub valid_entries () 
{
    my $self = shift;
    my @entries = ();

    foreach my $entry (@{$self->{ENTRIES}}) {
        if ($self->check_config (@{$self->{CONFIGS}->{$entry}})) {
            push @entries, $entry;
        }
    }
    return @entries;
}

sub list_configs ()
{
    my $self = shift;
    my %allconfigs = {};
    my $list = '';

    foreach my $entry (@{$self->{ENTRIES}}) {
	
        foreach my $config (@{$self->{CONFIGS}->{$entry}}) {
            $config =~ s/!//g;
            if ($allconfigs{$config} != 1) {
                $list .= $config.' ';
                $allconfigs{$config} = 1;
            }
        }
    }

    return $list;
}

sub dump ()
{
    my $self = shift;	

    print "============================================================\n";
    print "Config\n";
    foreach my $config (@{$self->{MY_CONFIGS}}) {
        print $config, "\n";
    }
    print "\n";
    print "Entries\n";
    foreach my $entry (@{$self->{ENTRIES}}) {
        print "- ", $entry, ": ";
        foreach my $config (@{$self->{CONFIGS}->{$entry}}) {
            print $config, " ";
        }
        print "\n";
    }
    print "============================================================\n";
}

1;