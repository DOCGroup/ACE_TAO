package VC7ProjectCreator;

# ************************************************************
# Description   : A VC7 Project Creator
# Author        : Chad Elliott
# Create Date   : 4/23/2002
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use GUID;
use ProjectCreator;

use vars qw(@ISA);
@ISA = qw(ProjectCreator);

# ************************************************************
# Subroutine Section
# ************************************************************

sub file_sorter {
  my($self)  = shift;
  my($left)  = shift;
  my($right) = shift;
  return lc($left) cmp lc($right);
}


sub specific_lookup {
  my($self) = shift;
  my($tag)  = shift;
  my($val)  = undef;

  if (defined $self->{'guid_names'} &&
      defined $self->{'guid_names'}->{$tag}) {
    $val = $self->{'guid_names'}->{$tag};
  }

  return $val;
}


sub save_project_value {
  my($self)  = shift;
  my($name)  = shift;
  my($value) = shift;

  if ($name eq 'guid') {
    if (!defined $self->{'guid_names'}) {
      $self->{'guid_names'} = {};
    }
    $self->{'guid_names'}->{$self->project_file_name()} = $value;
  }
}


sub crlf {
  my($self) = shift;
  return $self->windows_crlf();
}


sub get_vcversion {
  #my($self) = shift;
  return '7.00';
}


sub fill_value {
  my($self)  = shift;
  my($name)  = shift;
  my($value) = undef;

  if ($name eq 'guid') {
    my($guid) = new GUID();
    $value = $guid->generate($self->project_file_name(),
                             $self->get_current_input(),
                             $self->getcwd());
  }
  elsif ($name eq 'vcversion') {
    $value = $self->get_vcversion();
  }
  return $value;
}


sub project_file_name {
  my($self) = shift;
  my($name) = shift;

  if (!defined $name) {
    $name = $self->project_name();
  }

  return "$name.vcproj";
}


sub get_env_accessor {
  #my($self) = shift;
  return ('%', '%');
}


sub get_dll_exe_template_input_file {
  #my($self) = shift;
  return 'vc7exe';
}


sub get_dll_template_input_file {
  #my($self) = shift;
  return 'vc7dll';
}


sub get_template {
  #my($self) = shift;
  return 'vc7';
}


1;
