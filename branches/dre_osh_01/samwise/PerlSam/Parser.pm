# $Id$

package PerlSam::Parser;

use PerlSam::Parser::Simple;
use strict;

###############################################################################
# Global methods

# Return the default parser
sub GetDefault ()
{
    return 'simple';
}

###############################################################################
# Constructor

sub new ($)
{
    my $proto = shift;
    my $class = ref ($proto) || $proto;
    my $self = {};

    my $name = shift;

    if ($name eq 'simple') {
        $self->{PARSER} = new PerlSam::Parser::Simple;
    }
    else {
        print STDERR "Error: Unrecognized Parser <$name>\n";
        exit 1;
    }

    bless ($self, $class);
    return $self;
}

###############################################################################
# Methods

sub Parse ($\%)
{
    my $self = shift;

    return $self->{PARSER}->Parse (@_);
}

sub ParseLibraryFile ($\%)
{
    my $self = shift;

    return $self->{PARSER}->ParseLibraryFile (@_);
}

1;