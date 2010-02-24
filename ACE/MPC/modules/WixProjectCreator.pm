package WixProjectCreator;

# ************************************************************
# Description   : A Wix Project Creator
# Author        : James H. Hill
# Create Date   : 6/23/2009
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;
use ProjectCreator;
use WinProjectBase;
use XMLProjectBase;
use GUID;

use vars qw(@ISA);
@ISA = qw(XMLProjectBase WinProjectBase ProjectCreator);

# ************************************************************
# Subroutine Section
# ************************************************************

sub expand_variables_from_template_values {
  return 1;
}

sub warn_useless_project {
  return 0;
}

sub convert_slashes {
  return 0;
}

sub fill_value {
  my($self, $name) = @_;

  if ($name eq 'guid') {
    ## Return a repeatable GUID for use within the template.  The values
    ## provided will be hashed and returned in a format expected by Wix.
    return GUID::generate($self->project_file_name(),
                          $self->{'current_input'}, $self->getcwd());
  }
  elsif ($name eq 'source_directory') {
    my $source;

    if ($self->get_assignment('sharedname')) {
      $source = $self->get_assignment('dllout');

      if ($source eq '') {
        $source = $self->get_assignment('libout');
      }
    }
    elsif ($self->get_assignment('staticname')) {
      $source = $self->get_assignment('libout');
    }
    else {
      $source = $self->get_assignment('exeout');
    }

    ## Check for a variable in the source directory. We have to make
    ## sure we transform this correctly for WIX by adding the correct
    ## prefix. Otherwise, WIX will complain.
    if (defined $source && $source =~ /.*?\$\((.+?)\).*/) {
      my $prefix;
      my $varname = $1;

      if ($ENV{$varname}) {
        $prefix = "env";
      }
      else {
        $prefix = "var";
      }

      ## Add the correct prefix to the variable.
      $source =~ s/$varname/$prefix.$varname/g;
    }

    return $source;
  }

  return undef;
}

sub project_file_extension {
  return '.wxi';
}


sub get_dll_exe_template_input_file {
  #my $self = shift;
  return 'wix';
}


sub get_lib_exe_template_input_file {
  #my $self = shift;
  return 'wix';
}


sub get_lib_template_input_file {
  #my $self = shift;
  return 'wix';
}


sub get_dll_template_input_file {
  #my $self = shift;
  return 'wix';
}

sub get_template {
  return 'wix';
}

1;
