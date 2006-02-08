package GNUACEProjectCreator;

# ************************************************************
# Description   : A GNU Project Creator for ACE
# Author        : Chad Elliott
# Create Date   : 3/13/2002
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;
use File::Basename;

use MakeProjectBase;
use ProjectCreator;

use vars qw(@ISA);
@ISA = qw(MakeProjectBase ProjectCreator);

# ************************************************************
# Subroutine Section
# ************************************************************

sub expand_variables_from_template_values {
  #my($self) = shift;
  return 0;
}


sub convert_slashes {
  #my($self) = shift;
  return 0;
}


sub list_mpc_files {
  my($self)  = shift;
  my($hash)  = shift;
  my(@files) = ();

  foreach my $key (keys %$hash) {
    push(@files, $self->reverse_relative($key), $self->list_mpc_files($$hash{$key}));
  }

  return @files;
}


sub fill_value {
  my($self)  = shift;
  my($name)  = shift;
  my($value) = undef;
  my($names) = $self->{'source_files'};

  if ($name eq 'mpc_files') {
    my(@mpc_files) = $self->list_mpc_files($self->get_inheritance_tree());
    $value = \@mpc_files;
  }
  elsif ($name eq 'vpath') {
    my(%vpath) = ();
    foreach my $name (keys %$names) {
      my($comps) = $$names{$name};
      foreach my $key (keys %$comps) {
        foreach my $item (@{$$comps{$key}}) {
          my($dname) = $self->relative($self->mpc_dirname($item));
          if ($dname ne '.') {
            $vpath{$dname} = 1;
          }
        }
      }
    }
    my($str) = join(':', keys %vpath);
    if ($str ne '') {
      $value = 'VPATH = .:' . $str . $self->crlf();
    }
  }
  elsif ($name eq 'tao') {
    my($incs) = $self->get_assignment('includes');
    my($libs) = $self->get_assignment('libpaths');
    $value = ((defined $incs && $incs =~ /tao/i) ||
              (defined $libs && $libs =~ /tao/i));
  }
  elsif ($name eq 'ciao') {
      my($incs) = $self->get_assignment('includes');
      my($libs) = $self->get_assignment('libpaths');
    $value = ((defined $incs && $incs =~ /ciao/i) ||
              (defined $libs && $libs =~ /ciao/i));
  }

  return $value;
}


sub project_file_prefix {
  #my($self) = shift;
  return 'GNUmakefile.';
}


sub get_dll_exe_template_input_file {
  #my($self) = shift;
  return 'gnuexe';
}


sub get_dll_template_input_file {
  #my($self) = shift;
  return 'gnudll';
}


sub get_template {
  #my($self) = shift;
  return 'gnu';
}

1;
