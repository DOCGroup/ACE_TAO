# $Id$

package PerlSam::Generator;

###############################################################################
# Forward Declarations

sub ConvertPathToRelative ($);
sub ExpandIDLFiles (\%);
sub ExpandBaseNames (\@);
sub ExpandLibraries (\@);
sub ExpandIncludeDirs (\@);
sub ProjectOrder (\%);

# some private declarations
sub GetLibInfo ();
sub SetLibInfo (\%);
sub GenerateDependencies (\%);

###############################################################################
# Instantiations

use PerlSam::Generator::Borland;
use PerlSam::Generator::GNUMake;
use PerlSam::Generator::Automake;
use PerlSam::Generator::MSVC6;
#use PerlSam::Generator::MSVC7;
use PerlSam::Generator::View;
use PerlSam::Generator::VisualAge;

###############################################################################
# Global

use Cwd;
use Data::Dumper;
use strict;

###############################################################################
# Global

# Return the default generators
sub GetDefaults ()
{
    return ('msvc6', 'gnumake', 'borland');
}

###############################################################################
# Constructor

sub new (@)
{
    my $proto = shift;
    my $class = ref ($proto) || $proto;
    my $self = {};

    @{$self->{GENERATORS}} = ();

    foreach my $name (@_) {
        if ($name eq 'view') {
            push @{$self->{GENERATORS}}, new PerlSam::Generator::View;
        }
        elsif ($name eq 'msvc6') {
            push @{$self->{GENERATORS}}, new PerlSam::Generator::MSVC6;
        }
        elsif ($name eq 'gnumake') {
            push @{$self->{GENERATORS}}, new PerlSam::Generator::GNUMake;
        }
        elsif ($name eq 'borland') {
            push @{$self->{GENERATORS}}, new PerlSam::Generator::Borland;
        }
        elsif ($name eq 'automake') {
            push @{$self->{GENERATORS}}, new PerlSam::Generator::Automake;
        }
        elsif ($name eq 'visualage') {
            push @{$self->{GENERATORS}}, new PerlSam::Generator::VisualAge;
        }
        else {
            print STDERR "Error: Unrecognized Generator <$name>\n";
            exit 1;
        }
    }

    bless ($self, $class);
    return $self;
}

###############################################################################
# Public Methods

sub GenerateWorkspace (\%)
{
    my $self = shift;

    foreach my $generator (@{$self->{GENERATORS}}) {
        $generator->GenerateWorkspace (@_);
    }
}

sub GenerateProjects (\%)
{
    my $self = shift;

    foreach my $generator (@{$self->{GENERATORS}}) {
        $generator->GenerateProjects (@_);
    }
}

sub SetLibraryInfo (\%)
{
    my $self = shift;
    my $data = shift;

    SetLibInfo (%{$data});
}

###############################################################################
# Private Methods

# global data
my $generator_lib_info;

sub GetLibInfo ()
{
    return $generator_lib_info;
}

sub SetLibInfo (\%)
{
    $generator_lib_info = shift;
}

# Takes in a string of paths based on ACE_ROOT and converts them to relative
# paths based on the current directory (example: /bin -> ../../bin)
sub ConvertPathToRelative ($)
{
    my $args = shift;
    $args =~ s/\s\s*/ /g;
    $args =~ s/\s$//g;
    my @list = split / /, $args;
    my $result = "";
    my $root;

    my $current = getcwd ();
    my $traverse = $current;
    my $external = 0;
    my @current_list;

    # This little bit of code walks up the path looking ACE_ROOT.  If
    # not found, just default to using "$ACE_ROOT/"

    while (1) {
        # Are we in the "root" yet?
        if (-r "$traverse/samwise/PerlSam/Generator.pm") {
            last;
        }

        # Move up in the directory tree by lopping off the last part of
        # the path
        if ($traverse =~ m/(.*)\/[^\/]*/) {
            $traverse = $1;
            $root .= "../";
        }
        else {
            # Ran out of path, default to environment variable
            $root = "\$ACE_ROOT/";
            $external = 1;
            last;
        }
    }

    # Remove the trailing slash
    $root =~ s/\/$//;

    if (!$external) {
        # Figure out what our relative current directory is
        $current =~ s/^\Q$traverse\E\///;
        @current_list = split /\//, $current;
    }

    # Rebuild the stringified list
    foreach my $entry (@list) {
        my $this_root = $root;
        my @this_current = @current_list;

        # Loop off any common parts.  So if current directory is
        # "\TAO\tests" and the entry is "\TAO\" then reduce the root
        if (!$external) {
            if ($entry =~ m/^\//) {
                while ($#this_current >= 0) {
                    my $top_dir = shift @this_current;
                    if ($entry && $entry =~ s/^\/$top_dir//) {
                        if ($this_root eq '..') {
                            $this_root = '.';
                        } else {
                            $this_root =~ s/^\.\.\///;
                        }
                    } else {
                        last;
                    }
                }
		$result .= $this_root . $entry . " ";
            } else {
	        $result .= $entry . " ";
	    }
        }

    }

    # Remove the trailing space from the stringified list.
    $result =~ s/ $//;

    return $result;
}

sub ExpandIDLFiles (\%)
{
    my $project = shift;

    foreach my $file (keys %{$project->{SOURCES}}) {
        my $base = $file;
        $base =~ s/\.idl$//i;

        next if (!defined $project->{SOURCES}->{$file}->{TYPE});

        if ($project->{SOURCES}->{$file}->{TYPE} eq 'clientidl') {
            $project->{SOURCES}->{$base ."C.h"} = ();
            $project->{SOURCES}->{$base ."C.i"} = ();
            $project->{SOURCES}->{$base ."C.cpp"} = ();
        }
        elsif ($project->{SOURCES}->{$file}->{TYPE} eq 'idl') {
            $project->{SOURCES}->{$base ."C.h"} = ();
            $project->{SOURCES}->{$base ."C.i"} = ();
            $project->{SOURCES}->{$base ."C.cpp"} = ();
            $project->{SOURCES}->{$base ."S.h"} = ();
            $project->{SOURCES}->{$base ."S.i"} = ();
            $project->{SOURCES}->{$base ."S.cpp"} = ();
            $project->{SOURCES}->{$base ."S_T.h"} = ();
            $project->{SOURCES}->{$base ."S_T.i"} = ();
            $project->{SOURCES}->{$base ."S_T.cpp"} = ();
            $project->{SOURCES}->{$base ."S_T.cpp"}->{TYPE} = 'template';
        }
    }
}

sub ExpandBaseNames (\@)
{
    my $libs = shift;
    my $libinfo = GetLibInfo ();

    my @results;

    foreach my $namespace (@{$libinfo->{ORDER}}) {
        foreach my $maplib (@{$libinfo->{$namespace}->{LIST}}) {
            foreach my $lib (@{$libs}) {
                my $lib_namespace;
                my $lib_name;
                if ($lib =~ m/(.*)::(.*)/) {
                    $lib_namespace = $1;
                    $lib_name = $2;
                }

                next if ($lib_namespace ne $namespace);

                if ($lib_name eq $maplib) {
                    push @results, $libinfo->{$namespace}->{DETAILS}->{$maplib}->{BASE};
                }
            }
        }
    }

    return @results;
}

sub ExpandLibraries (\@)
{
    my $libs = shift;
    my $libinfo = GetLibInfo ();

    my $results = "";

    foreach my $namespace (@{$libinfo->{ORDER}}) {
        foreach my $maplib (@{$libinfo->{$namespace}->{LIST}}) {
            foreach my $lib (@{$libs}) {
                my $lib_namespace;
                my $lib_name;
                if ($lib =~ m/(.*)::(.*)/) {
                    $lib_namespace = $1;
                    $lib_name = $2;
                }

                next if ($lib_namespace ne $namespace);

                if ($lib_name eq $maplib) {
                    my $dir = $libinfo->{$namespace}->{DETAILS}->{$maplib}->{LINK};
                    my $data = $libinfo->{$namespace}->{DETAILS}->{$maplib}->{BASE};

                    $dir =~ s/\/$//;

                    if (defined $data) {
                        $results .= ConvertPathToRelative ($dir) . "/$data ";
                    }
                    else {
                        print STDERR "Error: No base name defined for lib <$maplib>\n";
                        exit 1;
                    }
                }
            }
        }
    }

    # Add in any local libs
    foreach my $lib (@{$libs}) {
        if ($lib =~ m/^::(.*)$/) {
            $results .= "$1 ";
        }
    }

    # Get rid of the last space
    $results =~ s/ $//;

    return $results;
}

sub ExpandIncludeDirs (\@)
{
    my $libs = shift;
    my $libinfo = GetLibInfo ();

    my $results = "";

    foreach my $namespace (@{$libinfo->{ORDER}}) {
        foreach my $maplib (@{$libinfo->{$namespace}->{LIST}}) {
            foreach my $lib (@{$libs}) {
                my $lib_namespace;
                my $lib_name;
                if ($lib =~ m/(.*)::(.*)/) {
                    $lib_namespace = $1;
                    $lib_name = $2;
                }

                next if ($lib_namespace ne $namespace);

                if ($lib_name eq $maplib) {
                    my $dir = $libinfo->{$namespace}->{DETAILS}->{$maplib}->{INCLUDE};
                    if ($results !~ m/^$dir / &&
                        $results !~ m/ $dir /)
                    {
                        $results .= $dir . " ";
                    }
                }
            }
        }
    }

    # Add in any local libs
    foreach my $lib (@{$libs}) {
        if ($lib =~ m/^::(.*)\/[^\/]+$/) {
            $results .= "$1 ";
        }
    }

    # Get rid of the last space
    $results =~ s/ $//;

    return $results;
}

sub GenerateDependencies (\%)
{
    my $self = shift;
    my $data = shift;

    print "====================\n" if ($main::verbose >= 2);
    print "Entering Generator::GenerateDependencies\n" if ($main::verbose >= 1);

    #
    # Create a mapping between output names to project names
    #

    my %output; # Mapping between output and project

    print "Calculating output/project mapping\n" if ($main::verbose >= 2);

    foreach my $project (values %{$data->{PROJECTS}}) {
        %output->{$project->{TARGET}} = $project->{NAME}
    }

    print Dumper (\%output) if ($main::verbose >= 2);

    #
    # Go through all the libraries needed by a project and create a DEPENDS
    # list for all dependencies on other projects in this workspace
    #

    foreach my $project (values %{$data->{PROJECTS}}) {
        print "Looking at project $project->{NAME}\n" if ($main::verbose >= 2);

        @{$data->{PROJECTS}->{$project->{NAME}}->{DEPENDS}} = ();

        foreach my $library (ExpandBaseNames (@{$project->{LIBS}})) {
            print "    Looking for library '$library' ..." if ($main::verbose >= 2);

            if (defined %output->{$library}) {
                print "found" if ($main::verbose >= 2);
                push @{$data->{PROJECTS}->{$project->{NAME}}->{DEPENDS}}, %output->{$library};
            }

            print "\n" if ($main::verbose >= 2);
        }
    }

    print "====================\n" if ($main::verbose>= 2);
}

sub ProjectOrder (\%)
{
    my $data = shift;

    my %done;   # Hash for indicating whether a project has been ordered for compilation
    my @order;  # Resulting order of the projects
    my $count;  # Counter to keep track of how many projects made "done" in a pass

    my $cycle = 0;  # Keep track of the cycle

    print "====================\n" if ($main::verbose >= 2);
    print "Entering Generator::ProjectOrder\n" if ($main::verbose >= 1);

    do {
        $count = 0;
        ++$cycle;

        print "Starting Cycle $cycle\n" if ($main::verbose >= 2);

        #
        # Search for projects that have all dependencies ready and add them to
        # the ordered list if so
        #

        foreach my $project (values %{$data->{PROJECTS}}) {
            print "  Looking at project $project->{NAME}..." if ($main::verbose >= 2);

            if (defined %done->{$project->{NAME}}) {
                print "done\n" if ($main::verbose >= 2);
            }
            else {
                my $notready = 0;
                foreach my $depend (@{$project->{DEPENDS}}) {
                    if (!defined %done->{$depend}) {
                        print "waiting on $depend\n" if ($main::verbose >= 2);
                        $notready = 1;
                        last;
                    }
                }

                if ($notready == 0) {
                    ++$count;
                    push @order, $project->{NAME};
                    %done->{$project->{NAME}} = 1;
                    print "ready\n" if ($main::verbose >= 2);
                }
            }
        }
    } while ($count > 0);

    #
    # Perform a verification that there are no leftovers.  If there are, then
    # it means there was a circular dependency
    #

    foreach my $project (values %{$data->{PROJECTS}}) {
        if (!defined %done->{$project->{NAME}}) {
            print STDERR "Error: Circular dependency detected for project: $project->{NAME}\n";
        }
    }

    print "Final Order: ", join (" ", @order), "\n" if ($main::verbose >= 2);
    print "====================\n" if ($main::verbose >= 2);

    return @order;
}

1;
