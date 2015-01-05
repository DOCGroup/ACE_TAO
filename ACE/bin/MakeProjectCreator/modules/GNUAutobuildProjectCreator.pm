package GNUAutobuildProjectCreator;

# ************************************************************
# Description   : A Project creator for the GNUAutobuild project type
# Author        : Chad Elliott
# Create Date   : 3/13/2002
# $Id$
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;
use File::Basename;

use GNUACEProjectCreator;
use MakeProjectBase;
use ProjectCreator;

use vars qw(@ISA);
@ISA = qw(GNUACEProjectCreator MakeProjectBase ProjectCreator);

sub extractType {
    return 'gnuace';
}

1;
