package VC8ProjectCreator;

# ************************************************************
# Description   : A VC8 Project Creator
# Author        : Johnny Willemsen
# Create Date   : 4/21/2004
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use VC7ProjectCreator;

use vars qw(@ISA);
@ISA = qw(VC7ProjectCreator);

# ************************************************************
# Data Section
# ************************************************************

my %info = (Creator::cplusplus => {'ext'      => '.vcproj',
                                   'dllexe'   => 'vc8exe',
                                   'libexe'   => 'vc8libexe',
                                   'dll'      => 'vc8dll',
                                   'lib'      => 'vc8lib',
                                   'template' => 'vc8',
                                  },
            Creator::csharp => {'ext'      => '.csproj',
                                'dllexe'   => 'vc8csharp',
                                'libexe'   => 'vc8csharp',
                                'dll'      => 'vc8csharp',
                                'lib'      => 'vc8csharp',
                                'template' => 'vc8csharp',
                               },
            Creator::java   => {'ext'      => '.vjsproj',
                                'dllexe'   => 'vc8java',
                                'libexe'   => 'vc8java',
                                'dll'      => 'vc8java',
                                'lib'      => 'vc8java',
                                'template' => 'vc8java',
                               },
            Creator::vb     => {'ext'      => '.vbproj',
                                'dllexe'   => 'vc8vb',
                                'libexe'   => 'vc8vb',
                                'dll'      => 'vc8vb',
                                'lib'      => 'vc8vb',
                                'template' => 'vc8vb',
                               },
           );

my %config = ('vcversion' => '8.00');

# ************************************************************
# Subroutine Section
# ************************************************************

sub languageSupported {
  return defined $info{$_[0]->get_language()};
}


sub webapp_supported {
  #my $self = shift;
  return 1;
}


sub require_dependencies {
  ## With vc8, they fixed it such that static libraries that depend on
  ## other static libraries will not be included into the target library
  ## by default.  Way to go Microsoft!
  return 1;
}

sub post_file_creation {
  my($self, $file) = @_;

  ## VC8 stores information in a .user file that may conflict
  ## with information stored in the project file.  If we have
  ## created a new project file, we will remove the corresponding
  ## .user file to avoid strange conflicts.
  unlink("$file.user");
}

sub get_configurable {
  #my($self, $name) = @_;
  return $config{$_[1]};
}

sub get_info_hash {
  my($self, $key) = @_;

  ## If we have the setting in our information map, the use it.
  return $info{$key} if (defined $info{$key});

  ## Otherwise, see if our parent type can take care of it.
  return $self->SUPER::get_info_hash($key);
}

sub translate_value {
  my($self, $key, $val) = @_;

  ## Microsoft uses AnyCPU in the project file, but uses Any CPU in the
  ## solution file.
  $val = 'Any CPU' if ($key eq 'platform' && $val eq 'AnyCPU');

  return $self->SUPER::translate_value($key, $val);
}

1;
