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

sub translate_value {
  my($self) = shift;
  my($key)  = shift;
  my($val)  = shift;

  if ($key eq 'depends' && $val ne "") {
    my($arr) = $self->create_array($val);
    $val = "";
    foreach my $entry (@$arr) {
      $val .= "\"" . $self->project_file_name($entry) . "\" ";
    }
    $val =~ s/\s+$//;
  }
  return $val;
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


sub fill_value {
  my($self)  = shift;
  my($name)  = shift;
  my($value) = undef;

  if ($name eq "guid") {
    my($guid) = new GUID();
    $value = $guid->generate($self->project_file_name());
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


sub get_dll_exe_template_input_file {
  my($self) = shift;
  return "vc7exe";
}


sub get_dll_template_input_file {
  my($self) = shift;
  return "vc7dll";
}


sub get_template {
  my($self) = shift;
  return "vc7";
}


1;
