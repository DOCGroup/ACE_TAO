package GHSProjectCreator;

# ************************************************************
# Description   : A GHS project creator for version 4.x.
#                 By default, this module assumes Multi will
#                 be used on Windows.  If it is not, you must
#                 set the MPC_GHS_UNIX environment variable.
# Author        : Chad Elliott
# Create Date   : 4/19/2002
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use ProjectCreator;

use vars qw(@ISA);
@ISA = qw(ProjectCreator);

# ************************************************************
# Data Section
# ************************************************************

my $startre;
my $ghsunix = 'MPC_GHS_UNIX';

# ************************************************************
# Subroutine Section
# ************************************************************

sub convert_slashes {
  return (defined $ENV{$ghsunix} ? 0 : 1);
}


sub case_insensitive {
  return (defined $ENV{$ghsunix} ? 0 : 1);
}


sub use_win_compatibility_commands {
  return (defined $ENV{$ghsunix} ? 0 : 1);
}


sub post_file_creation {
  my $self = shift;

  ## These special files are only used if it is a custom only project or
  ## there are no source files in the project.
  if ((defined $self->get_assignment('custom_only') ||
       !defined $self->get_assignment('source_files')) &&
      defined $self->get_assignment('custom_types')) {
    my $fh = new FileHandle();
    if (open($fh, '>.custom_build_rule')) {
      print $fh ".empty_html_file\n";
      close($fh);
    }
    if (open($fh, '>.empty_html_file')) {
      close($fh);
    }
  }
}

sub compare_output {
  #my $self = shift;
  return 1;
}


sub project_file_extension {
  #my $self = shift;
  return '.gpj';
}


sub fill_value {
  my($self, $name) = @_;
  my $value;

  if (!defined $startre) {
    $startre = $self->escape_regex_special($self->getstartdir());
  }

  ## The Green Hills project format is strange and needs all paths
  ## relative to the top directory, no matter where the source files
  ## reside.  The template uses reltop_ in front of the real project
  ## settings, so we get the value of the real keyword and then do some
  ## adjusting to get it relative to the top directory.
  if ($name =~ /^reltop_(\w+)/) {
    $value = $self->relative($self->get_assignment($1));
    if (defined $value) {
      my $part = $self->getcwd();
      $part =~ s/^$startre[\/]?//;
      if ($part ne '') {
        if ($value eq '.') {
          $value = $part;
        }
        else {
          $value = $part . '/' . $value;
        }
      }
    }
  }
  elsif ($name eq 'reltop') {
    $value = $self->getcwd();
    $value =~ s/^$startre[\/]?//;
    $value = '.' if ($value eq '');
  }
  elsif ($name eq 'slash') {
    ## We need to override the slash value so that we can give the right
    ## value for Windows or UNIX.
    $value = (defined $ENV{$ghsunix} ? '/' : '\\');
  }
  elsif ($name eq 'postmkdir') {
    ## If we're on Windows, we need an "or" command that will reset the
    ## errorlevel so that a mkdir on a directory that already exists
    ## doesn't cause the build to cease.
    $value = ' || type nul' if (!defined $ENV{$ghsunix});
  }

  return $value;
}

sub get_dll_exe_template_input_file {
  #my $self = shift;
  return 'ghsdllexe';
}


sub get_lib_exe_template_input_file {
  #my $self = shift;
  return 'ghslibexe';
}


sub get_lib_template_input_file {
  #my $self = shift;
  return 'ghslib';
}


sub get_dll_template_input_file {
  #my $self = shift;
  return 'ghsdll';
}


sub get_properties {
  my $self = shift;

  ## Get the base class properties and add the properties that we
  ## support.
  my $props = $self->ProjectCreator::get_properties();

  ## This project creator can work for UNIX and Windows.  Set the
  ## property based on the environment variable.
  $$props{'windows'} = 1 if (!defined $ENV{$ghsunix});

  return $props;
}

1;
