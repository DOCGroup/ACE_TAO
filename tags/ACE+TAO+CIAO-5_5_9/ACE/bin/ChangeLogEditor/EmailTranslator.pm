package EmailTranslator;

# ************************************************************
# Description   : Translate the given user name and domain into
#                 an email address.
# Author        : Chad Elliott
# Create Date   : 6/18/2002
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;
use FileHandle;

# ************************************************************
# Subroutine Section
# ************************************************************

sub new {
  my($class)  = shift;
  my($domain) = shift;
  my($self)   = bless {'domain' => "\@$domain",
                      }, $class;
  return $self;
}


sub translate {
  my($self)    = shift;
  my($name)    = shift;
  my($domain)  = $self->{'domain'};
  my($email)   = "$name$domain";
  my(%special) = ();

  if (defined $ENV{REPLYTO}) {
    $email = $ENV{REPLYTO};
  }
  elsif (defined $special{$name}) {
    $email = $special{$name} . $domain;
  }
  elsif ($name =~ /([^\s]+)\s+([^\s]+)/) {
    ## Last name underscore first initial
    $email = lc($2 . '_' . substr($1, 0, 1)) . $domain;
  }

  return $email;
}


1;
