package NMakeProjectCreator;

# ************************************************************
# Description   : An NMake Project Creator
# Author        : Chad Elliott
# Create Date   : 5/31/2002
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
    my($app) = "";
    $val = "";
    foreach my $entry (@$arr) {
      $val .= "\"" . $self->project_file_name($entry) . "\" ";
    }
    $val =~ s/\s+$//;
  }
  return $val;
}


sub sort_files {
  my($self) = shift;
  return 1;
}


sub file_sorter {
  my($self)  = shift;
  my($left)  = shift;
  my($right) = shift;
  return lc($left) cmp lc($right);
}


sub crlf {
  my($self) = shift;
  if ($^O eq 'MSWin32') {
    return "\n";
  }
  else {
    return "\r\n";
  }
}


sub project_file_name {
  my($self) = shift;
  my($name) = shift;

  if (!defined $name) {
    $name = $self->project_name();
  }

  return $self->transform_file_name("$name.mak");
}


sub get_dll_exe_template_input_file {
  my($self) = shift;
  return "nmakeexe";
}


sub get_dll_template_input_file {
  my($self) = shift;
  return "nmakedll";
}


sub get_template {
  my($self) = shift;
  return "nmake";
}


1;
