# $Id$

package PerlSam::Generator::View;

use Data::Dumper;
use strict;

###############################################################################
# Constructor

sub new (@)
{
    my $proto = shift;
    my $class = ref ($proto) || $proto;
    my $self = {};

    bless ($self, $class);
    return $self;
}

###############################################################################
# Methods

sub GenerateWorkspace (\%)
{
    my $self = shift;
    my $data = shift;

    print "-----Workspace\n";
    print Dumper ($data->{WORKSPACE});
}

sub GenerateProjects (\%)
{
    my $self = shift;
    my $data = shift;

    print "-----Projects\n";
    print Dumper ($data->{PROJECTS});
}

1;