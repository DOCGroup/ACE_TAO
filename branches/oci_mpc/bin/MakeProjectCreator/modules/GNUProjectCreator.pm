package GNUProjectCreator;

# ************************************************************
# Description   : A GNU Project Creator
# Author        : Chad Elliott
# Create Date   : 3/13/2002
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;
use File::Basename;

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


sub convert_slashes {
  my($self) = shift; 
  return 0;
}
 
 
sub fill_value {
  my($self)  = shift;
  my($name)  = shift;
  my($value) = undef;
  my($crlf)  = $self->crlf();

  if ($name eq "gnu_source_files") {
    my(%vpath) = ();
    my($tag)   = 'source_files';
    my($names) = $self->{$tag};

    $value = "";
    foreach my $name (keys %$names) {
      my($comps) = $$names{$name};

      foreach my $key (sort keys %$comps) {
        my($a)   = $$comps{$key};
        my(@arr) = @$a;
        my($cpy) = $key;

        $cpy =~ s/^\d+_//;
        $value .= "$crlf$crlf$cpy = \\";
        for(my $i = 0; $i <= $#arr; $i++) {
          my($item) = $arr[$i];
          my($dname) = dirname($item);

          $item =~ s/\.[^\.]+$//;
          if ($dname ne ".") {
            $vpath{$dname} = 1;
          }
          $value .= "$crlf  $item" . ($i != $#arr ? " \\" : "");
        }
      }
    }
    foreach my $name (keys %$names) {
      my($fname) = "";
      my($comps) = $$names{$name};
      foreach my $key (sort keys %$comps) {
        $fname = $key;
        $fname =~ s/^\d+_//;
        last;
      }

      if ($name ne "default") {
        $fname = "FILES";
        $value .= "$crlf$crlf" . "ifndef $name$crlf" .
                  "  $name = \\$crlf";
        my(@keys) = sort keys %$comps;
        for(my $i = 0; $i <= $#keys; $i++) {
          $keys[$i] =~ s/^\d+_//;
          my($key) = $keys[$i];  
          $value .= "    $key" . ($i != $#keys ? " \\" : "") . $crlf;
        }
        $value .= "endif # $name";

        foreach my $key (@keys) {
          $value .= "$crlf$crlf" . "ifneq (,\$(findstring $key, \$($name)))$crlf" .
                    "  $fname += \$($key)$crlf" .
                    "endif # $key";
        }
      }
    }
    my(@vkeys) = sort keys %vpath;
    if ($#vkeys >= 0) {
      $value .= "$crlf$crlf" . "VPATH = .";
      foreach my $key (@vkeys) {
        $value .= ":$key";
      }
    }
  }

  return $value;
}


sub project_file_name {
  my($self) = shift;
  my($name) = shift;

  if (!defined $name) {
    $name = $self->project_name();
  }

  return $self->transform_file_name(
            "Makefile" . ($name ne "" ? "." : "") . $name);
}


sub get_dll_exe_template_input_file {
  my($self) = shift;
  return "gnuexe";
}


sub get_dll_template_input_file {
  my($self) = shift;
  return "gnudll";
}


sub get_template {
  my($self) = shift;
  return "gnu";
}

1;
