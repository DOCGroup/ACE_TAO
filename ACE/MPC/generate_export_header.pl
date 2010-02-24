#! /usr/bin/perl
eval '(exit $?0)' && eval 'exec perl -w -S $0 ${1+"$@"}'
    & eval 'exec perl -w -S $0 $argv:q'
    if 0;

# ******************************************************************
#      Author: Chad Elliott
#        Date: 3/1/2006
#         $Id$
# Description: Generate an export header file for use with various compilers
# ******************************************************************

# ******************************************************************
# Pragma Section
# ******************************************************************

use strict;
use FileHandle;
use File::Basename;

# ******************************************************************
# Data Section
# ******************************************************************

my $version = '1.2';

# ******************************************************************
# Subroutine Section
# ******************************************************************

sub generate_export_header {
  my($name, $output) = @_;
  my $fh = new FileHandle();
  my $status = 0;

  if (open($fh, ">$output")) {
    $name = uc($name);
    print $fh <<EOM
#ifndef ${name}_EXPORT_H
#define ${name}_EXPORT_H

#if !defined(${name}_HAS_DLL)
# if defined(${name}_AS_STATIC_LIBS)
#   define ${name}_HAS_DLL 0
# else
#   define ${name}_HAS_DLL 1
# endif
#endif

#if (${name}_HAS_DLL == 1)
#  if defined(__SUNPRO_CC) && (__SUNPRO_CC >= 0x550)
#    if defined(${name}_BUILD_DLL)
#      define ${name}_Export __symbolic
#    else
#      define ${name}_Export __global
#    endif
#  elif defined(WIN32) || defined(UNDER_CE) || defined(__CYGWIN__)
#    if defined(${name}_BUILD_DLL)
#      define ${name}_Export __declspec(dllexport)
#    else
#      define ${name}_Export __declspec(dllimport)
#    endif
#  elif (defined(__GNUC__) && (__GNUC__ >= 4))
#    if defined(${name}_BUILD_DLL)
#      define ${name}_Export __attribute__((visibility("default")))
#    else
#      define ${name}_Export
#    endif
#  else
#    define ${name}_Export
#  endif
#else
#  define ${name}_Export
#endif

#endif
EOM
;
    close($fh);
    print "Output written to $output\n";
  }
  else {
    print STDERR "ERROR: Unable to write to $output\n";
    ++$status;
  }

  return $status;
}

sub usageAndExit {
  my $str = shift;

  print STDERR "$str\n" if (defined $str);
  print STDERR "Generate Export Header v$version\n",
               "Usage: ", basename($0), " <library name> [output file]\n";
  exit(0);
}

# ******************************************************************
# Main Section
# ******************************************************************

my $name   = shift;
my $output = shift;

if (!defined $name) {
  usageAndExit();
}
elsif (index($name, '-') == 0) {
  usageAndExit();
}

if (!defined $output) {
  $output = $name . '_' . ($name =~ /^[A-Z]/ ? 'E' : 'e') .
            'xport.h';
}

if ($name =~ s/^\d+//) {
  print "WARNING: Removing beginning numbers from export name.\n";
}
if ($name =~ s/-\s/_/g) {
  print "WARNING: Converting dashes and ",
        "whitespace to underscores in export name.\n";
}

exit(generate_export_header($name, $output));
