package BorlandProjectCreator;

# ************************************************************
# Description   : Not a complete implementation for Borland
# Author        : Chad Elliott
# Create Date   : 3/14/2002
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

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


sub project_file_name {
  my($self) = shift;
  my($name) = shift;

  if (!defined $name) {
    $name = $self->project_name();
  }

  return "Makefile" . ($name eq "" ? "" : ".$name") . ".bor";
}


sub get_dll_exe_template_input_file {
  my($self) = shift;
  return "borexe";
}


sub get_dll_template_input_file {
  my($self) = shift;
  return "bordll";
}


sub get_template {
  my($self) = shift;
  return "bor";
}


1;
