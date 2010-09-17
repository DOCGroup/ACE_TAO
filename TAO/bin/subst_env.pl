#!/usr/bin/perl
#
# $Id$
#
# This script eases the transition from the CORBA::Environment& ACE_TRY_ENV
# declarations and method parameters to the ACE_ENV_ARG macros.
# It was used for converting TAO between versions 1.2.1 and 1.2.2.
# The converted output is written to stdout.
# Typical usage would be as follows:
#   perl subst_env.pl file_with_acetryenv.cpp >file_with_taoenvarg.cpp
#
$linenum = 0;
$last="";
$corbaenv = 'CORBA(::|_)Environment';
$envdecl = 'CORBA(::|_)Environment *& *\w*[Ee][Nn][Vv]\w*';
$default = '(TAO_|CORBA\::Environment\::)default_environment *\(\)';
$envvar = '(ACE|TAO)_TRY_ENV';
while (($l = <>)) {
  $linenum++;
  if ($l =~ /$corbaenv/) {
    my $single = "SINGLE_";
    if ($last =~ /,\s*$/) {
      $last =~ s/,\s*$/\n/;
      $single = "";
    } elsif ($l =~ /,\s*$corbaenv/) {
      $l =~ s/,\s*$corbaenv/  CORBA_Environment/;  # Preparation.
      $single = "";
    }
    unless ($l =~ /${corbaenv}.*\)/) {  # Need the method's closing paren.
      chop $l;
      my $nxt = <>;
      $linenum++;
      $nxt =~ s/^\s+//;
      $l .= $nxt;
    }
    if ($l =~ /$envdecl/) {
      if ($l =~ /$envdecl *= *$default/) {
        $l =~ s/$envdecl *= *$default/ACE_ENV_${single}ARG_DECL_WITH_DEFAULTS/;
      } else {
        $l =~ s/$envdecl/ACE_ENV_${single}ARG_DECL/;
      }
    } else {
      $l =~ s/$corbaenv *& */ACE_ENV_${single}ARG_DECL_NOT_USED/;
    }
    print $last;
    print $l;
    $last = "";
  } elsif ($l =~ /$envvar/) {
    my $single = "SINGLE_";
    if ($last =~ /,\s*$/) {
      $last =~ s/,\s*$/\n/;
      $single = "";
    } elsif ($l =~ /,\s*$envvar/) {
      $l =~ s/,\s*$envvar/  ACE_TRY_ENV/;  # Preparation.
      $single = "";
    }
    $l =~ s/$envvar/ACE_ENV_${single}ARG_PARAMETER/;
    print $last;
    print $l;
    $last = "";
  } else {
    print $last;
    $last = $l;
  }
}
print $last;

