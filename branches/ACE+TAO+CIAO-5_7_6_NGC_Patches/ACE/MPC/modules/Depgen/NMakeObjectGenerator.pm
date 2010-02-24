# ************************************************************
# Description   : Generates object files for NMake Makefiles.
# Author        : Chad Elliott
# Create Date   : 5/23/2003
# ************************************************************

package WinProjectBaseEx;

use WinProjectBase;
use DirectoryManager;

use vars qw(@ISA);
@ISA = qw(WinProjectBase DirectoryManager);

sub new {
  return bless {}, $_[0];
}

1;


package NMakeObjectGenerator;

# ************************************************************
# Pragmas
# ************************************************************

use strict;
use ObjectGenerator;

use vars qw(@ISA);
@ISA = qw(ObjectGenerator);

# ************************************************************
# Data Section
# ************************************************************

my $wpb = new WinProjectBaseEx();

# ************************************************************
# Subroutine Section
# ************************************************************

sub process {
  my $noext = $wpb->translate_directory($_[1]);
  $noext =~ s/\.[^\.]+$//o;
  return [ "\"\$(INTDIR)\\$noext.obj\"" ];
}


1;
