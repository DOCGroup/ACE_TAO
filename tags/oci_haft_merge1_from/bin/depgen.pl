eval '(exit $?0)' && eval 'exec perl -w -S $0 ${1+"$@"}'
    & eval 'exec perl -w -S $0 $argv:q'
    if 0;

# ************************************************************
# Description   : Generate dependencies for GNU Make and NMake.
# Author        : Chad Elliott
# Create Date   : 5/06/2002
#            $Id$
# ************************************************************

# ************************************************************
# Pragma Section
# ************************************************************

use strict;
use Cwd;
use FileHandle;
use File::Basename;

my($execPath) = getExecutePath($0);
unshift(@INC, "$execPath/DependencyGenerator");

require DependencyEditor;

# ************************************************************
# Data Section
# ************************************************************

my($version)  = '0.3';
my($os)       = ($^O eq 'MSWin32' || $^O eq 'cygwin' ? 'Windows' : 'UNIX');
my(%types)    = ('gnu'   => 1,
                 'nmake' => 1,
                );
my(%defaults) = ('UNIX'    => ['/lib/cpp', 'gnu'],
                 'Windows' => ['CL', 'nmake'],
                );

# ************************************************************
# Subroutine Section
# ************************************************************

sub which {
  my($prog)   = shift;
  my($exec)   = $prog;
  my($part)   = '';
  my($envSep) = ($^O eq 'MSWin32' ? ';' : ':');

  if (defined $ENV{'PATH'}) {
    foreach $part (split(/$envSep/, $ENV{'PATH'})) {
      $part .= "/$prog";
      if ( -x $part ) {
        $exec = $part;
        last;
      }
    }
  }

  return $exec;
}


sub getExecutePath {
  my($prog) = shift;
  my($loc)  = '';

  if ($prog ne basename($prog)) {
    if ($prog =~ /^[\/\\]/ ||
        $prog =~ /^[A-Za-z]:[\/\\]?/) {
      $loc = dirname($prog);
    }
    else {
      $loc = getcwd() . '/' . dirname($prog);
    }
  }
  else {
    $loc = dirname(which($prog));
  }

  if ($loc eq '.') {
    $loc = getcwd();
  }

  if ($loc ne '') {
    $loc .= '/';
  }

  return $loc;
}


sub usageAndExit {
  my($base) = shift;
  my($opt)  = shift;

  if (defined $opt) {
    print "$opt.\n";
  }

  print "$base v$version\n" .
        "Usage: $base [-D<MACRO>[=VALUE]] [-I<include dir>] [-A] " .
        "[-R <VARNAME>]\n" .
        "       " . (" " x length($base)) .
        " [-P <preprocessor>] [-f <output file>] [-t <type>] <files...>\n" .
        "\n" .
        "-D   This option sets a macro to an optional value.\n" .
        "-I   The -I option adds an include directory.\n" .
        "-A   Replace \$ACE_ROOT and \$TAO_ROOT paths with \$(ACE_ROOT) " .
        "and \$(TAO_ROOT)\n     respectively.\n" .
        "-R   Replace \$VARNAME paths with \$(VARNAME).\n" .
        "-P   Specifies which preprocessor to use.\n" .
        "     The default is ";
  my(@keys) = sort keys %defaults;
  for(my $i = 0; $i <= $#keys; ++$i) {
    my($def) = $keys[$i];
    print $defaults{$def}->[0] . " on $def" .
          ($i != $#keys ? $i == $#keys - 1 ? ' and ' : ', ' : '');
  }
  print ".\n" .
        "-f   Specifies the output file.  This file will be edited if it " .
        "already\n     exists.\n" .
        "-t   Use specified type (";
  @keys = sort keys %types;
  for(my $i = 0; $i <= $#keys; ++$i) {
    print "$keys[$i]" .
          ($i != $#keys ? $i == $#keys - 1 ? ' or ' : ', ' : '');;
  }
  print ") instead of the default.\n" .
        "     The default is ";
  @keys = sort keys %defaults;
  for(my $i = 0; $i <= $#keys; ++$i) {
    my($def) = $keys[$i];
    print $defaults{$def}->[1] . " on $def" .
          ($i != $#keys ? $i == $#keys - 1 ? ' and ' : ', ' : '');
  }
  print ".\n";
  exit(0);
}


# ************************************************************
# Main Section
# ************************************************************

my($base)    = basename($0);
my($cpp)     = $defaults{$os}->[0];
my($type)    = $defaults{$os}->[1];
my(@files)   = ();
my(%macros)  = ();
my(@ipaths)  = ();
my(%replace) = ();
my($output)  = '-';


if (defined $ENV{ACE_ROOT} && !defined $ENV{TAO_ROOT}) {
  $ENV{TAO_ROOT} = "$ENV{ACE_ROOT}/TAO";
}

for(my $i = 0; $i <= $#ARGV; ++$i) {
  my($arg) = $ARGV[$i];
  if ($arg =~ /^\-D(\w+)(=(.*))?/) {
    $macros{$1} = $3;
  }
  elsif ($arg =~ /^\-I(.*)/) {
    push(@ipaths, $1);
  }
  elsif ($arg eq '-A') {
    $replace{$ENV{ACE_ROOT}} = '$(ACE_ROOT)';
    $replace{$ENV{TAO_ROOT}} = '$(TAO_ROOT)';
  }
  elsif ($arg eq '-R') {
    ++$i;
    $arg = $ARGV[$i];
    if (defined $arg) {
      my($val) = $ENV{$arg};
      if (defined $val) {
        $replace{$val} = "\$($arg)";
      }
    }
    else {
      usageAndExit($base, 'Invalid use of -R');
    }
  }
  elsif ($arg eq '-f') {
    ++$i;
    $arg = $ARGV[$i];
    if (defined $arg) {
      $output = $arg;
    }
    else {
      usageAndExit($base, 'Invalid use of -f');
    }
  }
  elsif ($arg eq '-h') {
    usageAndExit($base);
  }
  elsif ($arg eq '-P') {
    ++$i;
    $arg = $ARGV[$i];
    if (defined $arg) {
      $cpp = $arg;
    }
    else {
      usageAndExit($base, 'Invalid use of -P');
    }
  }
  elsif ($arg eq '-t') {
    ++$i;
    $arg = $ARGV[$i];
    if (defined $arg && defined $types{$arg}) {
      $type = $arg;
    }
    else {
      usageAndExit($base, 'Invalid use of -t');
    }
  }
  elsif ($arg =~ /^\-/) {
    ## We will ignore unknown options
  }
  else {
    push(@files, $arg);
  }
}

if (!defined $files[0]) {
  usageAndExit($base, 'No files specified');
}

my($editor) = new DependencyEditor();
my($status) = $editor->process($output, $type, $cpp,
                               \%macros, \@ipaths, \%replace, \@files);
exit($status);
