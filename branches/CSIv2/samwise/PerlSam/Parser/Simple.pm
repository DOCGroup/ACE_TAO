# $Id$

package PerlSam::Parser::Simple;

use strict;
use FileHandle;

my $good_versions = ":0.1:";

###############################################################################
# Constructor

sub new  
{
    my $proto = shift;
    my $class = ref ($proto) || $proto;
    my $self = {};
    
    bless ($self, $class);
    return $self;
}

###############################################################################
# Methods

sub Parse ($\%)
{
    my $self = shift;
    my $file = shift;
    my $data = shift;
    
    my $file_handle = new FileHandle ($file, 'r');
    
    if (!defined $file_handle) {
        print STDERR "Error: Could not open file <$file>: $!\n";
        return 0;
    }
    
    my $state = 'none';
    my $current_project;
    my $current_type;
    my $current_file;
        
    while (<$file_handle>) {
        chomp;
        
        # Ignore comments and blank lines
        s/<!--(.*?)-->//g;
        next if (m/^\s*$/);
        
        if ($state eq 'none') {
            if (m/^\s*<sam version="([^\"]*)">\s*$/i) {
                if ($good_versions !~ m/:\Q$1\E:/) {
                    print STDERR "Error: Unsupported Samwise Format $1 (Supported: $good_versions)\n";
                    return 0;
                }
                $state = 'sam';
            }
            elsif (m/^\s*<\?.*\?>\s*/i) {
                # ignore
            }
            else {
                print STDERR "Error: Unexpected in state <$state>: $_\n";
                return 0;
            }
        }
        elsif ($state eq 'sam') {
            if (m/^\s*<\/sam>\s*$/i) {
                $state = 'none';
            }
            elsif (m/^\s*<workspace>\s*$/i) {
                $state = 'workspace';
            }
            elsif (m/^\s*<project>\s*$/i) {
                $state = 'project';
            }
            else {
                print STDERR "Error: Unexpected in state <$state>: $_\n";
                return 0;
            }
        }
        elsif ($state eq 'workspace') {
            if (m/^\s*<\/workspace>\s*$/i) {
                $state = 'sam';
            }
            elsif (m/^\s*<name>(.*)<\/name>\s*$/i) {
                $data->{WORKSPACE}->{NAME} = $1;
            }
            elsif (m/^\s*<projectlink dir="([^\"]*)">(.*)<\/projectlink>\s*$/i) {
                $data->{WORKSPACE}->{PROJECTLINKS}->{DETAILS}->{$2}->{DIR} = $1;
                push @{$data->{WORKSPACE}->{PROJECTLINKS}->{LIST}}, $2;
            }
            elsif (m/^\s*<projectlink>(.*)<\/projectlink>\s*$/i) {
                push @{$data->{WORKSPACE}->{PROJECTLINKS}->{LIST}}, $1;
            }
            elsif (m/^\s*<subdir>(.*)<\/subdir>\s*$/i) {
                push @{$data->{WORKSPACE}->{SUBDIRS}}, $1;
            }
            else {
                print STDERR "Error: Unexpected in state <$state>: $_\n";
                return 0;
            }
        }
        elsif ($state eq 'project') {
            if (m/^\s*<\/project>\s*$/i) {
                $state = 'sam';
                $current_project = undef;
                next;
            }
            elsif (m/^\s*<name>(.*)<\/name>\s*$/i) {
                $current_project = $1;
                $data->{PROJECTS}->{$1}->{NAME} = $1;
                next;
            }
            
            if (!defined $current_project) {
                print STDERR "Error: Expecting name element <$state>: $_\n";
                return 0;
            }
            elsif (m/^\s*<description>(.*)<\/description>\s*$/i) {
                $data->{PROJECTS}->{$current_project}->{DESCRIPTION} = $1;
            }
            elsif (m/^\s*<target type="([^>"\s]*)">(.*)<\/target>\s*$/i) {
                if ($1 ne 'library' && $1 ne 'executable') {
                    print STDERR "Error: Unknown target type <$1>: $_\n";
                    return 0;
                }

                $data->{PROJECTS}->{$current_project}->{TYPE} = $1;
                $data->{PROJECTS}->{$current_project}->{INSTALL} = 'no';
                $data->{PROJECTS}->{$current_project}->{TARGET} = $2;
            }
            elsif (m/^\s*<target type="([^>"\s]*)" install="([^>"\s]*)">(.*)<\/target>\s*$/i) {
                if ($1 ne 'library' && $1 ne 'executable') {
                    print STDERR "Error: Unknown target type <$1>: $_\n";
                    return 0;
                }
                if ($2 ne 'yes' && $2 ne 'no') {
                    print STDERR "Error: Invalid install option <$2> (use 'yes' or 'no'): $_\n";
                    return 0;
                }
                $data->{PROJECTS}->{$current_project}->{TYPE} = $1;
                $data->{PROJECTS}->{$current_project}->{INSTALL} = $2;
                $data->{PROJECTS}->{$current_project}->{TARGET} = $3;
            }
            elsif (m/^\s*<libinfo>\s*$/i) {
                $state = 'libinfo'
            }
            elsif (m/^\s*<libinfo export="([^"]*)" \/>\s*$/) {
                $data->{PROJECTS}->{$current_project}->{LIBINFO}->{EXPORT} = $1;
            }
            elsif (m/^\s*<sources>\s*$/i) {
                $state = 'sources';
            }            
            elsif (m/^\s*<libs>\s*$/i) {
                $state = 'libs';
            }            
            else {
                print STDERR "Error: Unexpected in state <$state>: $_\n";
                return 0;
            }
        }
        elsif ($state eq 'libinfo') {
            if (m/^\s*<\/libinfo>\s*$/i) {
                $state = 'project';
            }
            elsif (m/^\s*<namespace>(.*)<\/namespace>\s*$/i) {
                $data->{PROJECTS}->{$current_project}->{LIBINFO}->{NAMESPACE} = $1;
            }
            elsif (m/^\s*<name>(.*)<\/name>\s*$/i) {
                $data->{PROJECTS}->{$current_project}->{LIBINFO}->{NAME} = $1;
            }
            elsif (m/^\s*<include>(.*)<\/include>\s*$/i) {
                $data->{PROJECTS}->{$current_project}->{LIBINFO}->{INCLUDE} = $1;
            }
            elsif (m/^\s*<base>(.*)<\/base>\s*$/i) {
                $data->{PROJECTS}->{$current_project}->{LIBINFO}->{BASE} = $1;
            }
            elsif (m/^\s*<export>(.*)<\/export>\s*$/i) {
                $data->{PROJECTS}->{$current_project}->{LIBINFO}->{EXPORT} = $1;
            }
            else {
                print STDERR "Error: Unexpected in state <$state>: $_\n";
                return 0;
            }
        }
        elsif ($state eq 'sources') {
            if (m/^\s*<\/sources>\s*$/i) {
                $state = 'project';
            }
            elsif (m/^\s*<source>\s*$/i) {
                $state = 'source';
            }
            elsif (m/^\s*<source type="([^">]*)">\s*$/i) {
                $state = 'source';
                $current_type = $1;
                
                if ($1 ne 'template'
                    && $1 ne 'clientidl'
                    && $1 ne 'idl') 
                {
                    print STDERR "Error: Unknown source type <$1>: $_\n";
                    return 0;
                }
            }
            else {
                print STDERR "Error: Unexpected in state <$state>: $_\n";
                return 0;
            }
        }
        elsif ($state eq 'source') {
            if (m/^\s*<\/source>\s*$/i) {
                $current_file = undef;
                $current_type = undef;
                $state = 'sources';
                next;
            }
            elsif (m/^\s*<file>(.*)<\/file>\s*$/i) {
                $data->{PROJECTS}->{$current_project}->{SOURCES}->{$1} = ();
                if (defined $current_type) {
                    $data->{PROJECTS}->{$current_project}->{SOURCES}->{$1}->{TYPE} = $current_type;
                }
                $current_file = $1;
                next;
            }
            
            if (!defined $current_file) {
                print STDERR "Error: Expecting file element <$state>: $_\n";
                return 0;
            }
            elsif (m/^\s*<type>(.*)<\/type>\s*$/i) {
                $data->{PROJECTS}->{$current_project}->{SOURCES}->{$current_file}->{TYPE} = $1;
            }
            elsif (m/^\s*<options>(.*)<\/options>\s*$/i) {
                $data->{PROJECTS}->{$current_project}->{SOURCES}->{$current_file}->{OPTS} = $1;
            }
            else {
                print STDERR "Error: Unexpected in state <$state>: $_\n";
                return 0;
            }
        }
        elsif ($state eq 'libs') {
            if (m/^\s*<\/libs>\s*$/i) {
                $state = 'project';
            }
            elsif (m/^\s*<lib namespace="([^"]*)">(.*)<\/lib>\s*$/i) {
                push @{$data->{PROJECTS}->{$current_project}->{LIBS}}, $1 . '::' . $2;
            }
            elsif (m/^\s*<lib>(.*)<\/lib>\s*$/i) {
                push @{$data->{PROJECTS}->{$current_project}->{LIBS}}, '::' . $1;
            }
            else {
                print STDERR "Error: Unexpected in state <$state>: $_\n";
                return 0;
            }
        }
        else {
            print STDERR "Error: Parser reached unknown state <$state>\n";
            return 0;
        }
    }
    
    return 1;
}

sub ParseLibraryFile ($\%)
{
    my $self = shift;
    my $file = shift;
    my $data = shift;
    
    my $file_handle = new FileHandle ($file, 'r');
    
    if (!defined $file_handle) {
        print STDERR "Error: Could not open file <$file>: $!\n";
        return 0;
    }
    
    my $state = 'none';
    my $namespace;
        
    while (<$file_handle>) {
        chomp;
        if ($state eq 'none') {
            if (m/^\s*<libs>\s*$/i) {
                $state = 'libs';
            }
        }
        elsif ($state eq 'libs') {
            if (m/^\s*<\/libs>\s*$/i) {
                $state = 'none';
            }
            elsif (m/^\s*<order>(.*)<\/order>\s*$/i) {
                @{$data->{ORDER}} = split / /, $1;
            }
            elsif (m/^\s*<namespace name="([^"]*)">\s*$/i) {
                $state = 'namespace';
                $namespace = "$1";
            }
            else {
                print STDERR "Error: Unexpected in state <$state>: $_\n";
                return 0;
            }
        }
        elsif ($state eq 'namespace') {
            if (m/^\s*<\/namespace>\s*$/i) {
                $state = 'libs';
                $namespace = undef;
            }
            elsif (m/^\s*<lib name="([^"]*)" include="([^"]*)" link="([^"]*)" base="([^"]*)" \/>\s*$/i) {
                push @{$data->{$namespace}->{LIST}}, $1;
                $data->{$namespace}->{DETAILS}->{$1}->{INCLUDE} = $2;
                $data->{$namespace}->{DETAILS}->{$1}->{LINK} = $3;
                $data->{$namespace}->{DETAILS}->{$1}->{BASE} = $4;
            }
            else {
                print STDERR "Error: Unexpected in state <$state>: $_\n";
                return 0;
            }
        }
        else {
            print STDERR "Error: Parser reached unknown state <$state>\n";
            return 0;
        }
    }
    
    return 1;
}


1;