#!/usr/bin/perl
#
# $Id$
# 
# This script eases the transition from the ACE_TRY_ENV declarations
# and method parameters to the TAO_ENV_ARG macros.
# It was used for converting TAO between versions 1.2.1 and 1.2.2.
# The converted output is written to stdout. 
# Typical usage would be as follows:
#   perl subst_env.pl file_with_acetryenv.cpp >file_with_taoenvarg.cpp
#
$last="";
$envdecl = "CORBA(::|_)Environment *& *ACE_TRY_ENV";
$default = " *= *TAO_default_environment";
while (($l = <>)) {
  if ($l =~ /ACE_TRY_ENV/) {
    if ($l =~ /$envdecl/) {
      if ($l =~ /^\s*$envdecl/) {
        my $single = "SINGLE_";
        if ($last =~ /,\s*$/) {
            $last =~ s/,\s*$/\n/;
            $single = "";
        }
        if ($l =~ /$default/) {
          $l =~ s/$envdecl${default} *\(\)/TAO_ENV_${single}ARG_DECL_WITH_DEFAULTS/;
        } else {
          $l =~ s/$envdecl/TAO_ENV_${single}ARG_DECL/;
        }
      } elsif ($l =~ /,\s*$envdecl/) {
        if ($l =~ /$default/) {
          $l =~ s/,\s*$envdecl${default} *\(\)/ TAO_ENV_ARG_DECL_WITH_DEFAULTS/;
        } else {
          $l =~ s/,\s*$envdecl/ TAO_ENV_ARG_DECL/;
        }
      } else {
        if ($l =~ /$default/) {
          $l =~ s/$envdecl${default} *\(\)/TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS/;
        } else {
          $l =~ s/$envdecl/TAO_ENV_SINGLE_ARG_DECL/;
        }
      }
    } elsif ($l =~ /^\s*ACE_TRY_ENV/) {
      $last =~ s/,\s*$/\n/;
      $l =~ s/ACE_TRY_ENV/TAO_ENV_ARG_PARAMETER/;
    } elsif ($l =~ /, *ACE_TRY_ENV/) {
      $l =~ s/, *ACE_TRY_ENV/ TAO_ENV_ARG_PARAMETER/;
    } else {
      $l =~ s/ACE_TRY_ENV/TAO_ENV_SINGLE_ARG_PARAMETER/;
    }
    print $last;
    print $l;
    $last = "";
  } elsif ($l =~ /CORBA(::|_)Environment *&/) {
    if ($l =~ /^\s*CORBA(::|_)Environment *&/) {
      $last =~ s/,\s*$/\n/;
      $l =~ s/CORBA(::|_)Environment *&/TAO_ENV_ARG_DECL_NOT_USED/;
    } elsif ($l =~ /,\s*CORBA(::|_)Environment *&/) {
      $l =~ s/,\s*CORBA(::|_)Environment *&/ TAO_ENV_ARG_DECL_NOT_USED/;
    } else {
      $l =~ s/CORBA(::|_)Environment *&/TAO_ENV_SINGLE_ARG_DECL_NOT_USED/;
    }
    print $last;
    print $l;
    $last = "";
  } else {
    print $last;
    $last = $l;
  }
}
print $last;

