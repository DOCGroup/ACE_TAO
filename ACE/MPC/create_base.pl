#! /usr/bin/perl
eval '(exit $?0)' && eval 'exec perl -w -S $0 ${1+"$@"}'
    & eval 'exec perl -w -S $0 $argv:q'
    if 0;

# ******************************************************************
#      Author: Chad Elliott
#        Date: 9/13/2007
#         $Id$
# Description: Generate a base project based on a library project
# ******************************************************************

# ******************************************************************
# Pragma Section
# ******************************************************************

use strict;
use FindBin;
use FileHandle;
use File::Spec;
use File::Basename;

my $basePath = $FindBin::Bin;
if ($^O eq 'VMS') {
  $basePath = File::Spec->rel2abs(dirname($0)) if ($basePath eq '');
    $basePath = VMS::Filespec::unixify($basePath);
}
unshift(@INC, $basePath . '/modules');

require Creator;

# ******************************************************************
# Data Section
# ******************************************************************

my $version = '0.1';

# ******************************************************************
# Subroutine Section
# ******************************************************************

sub gather_info {
  my $name = shift;
  my $fh = new FileHandle();

  if (open($fh, $name)) {
    my @lines = ();
    my $pname = undef;
    while(<$fh>) {
      ## Get the line a remove leading and trailing white space
      my $line = $_;
      $line =~ s/^\s+//;
      $line =~ s/\s+$//;

      ## Look for the project declaration and pull out the name and any
      ## parents.
      if ($line =~ /^project\s*(\(([^\)]+)\))?\s*(:.+)?\s*{$/) {
        $pname = $2;
        my $parents = $3 || '';

        ## Create the default project name by removing the extension and
        ## converting back-slashes, spaces and dashes to underscores.
        ## This needs to be done regardless of whether the project name
        ## was provided or not since it's used below in the
        ## fill_type_name call.
        my $def = basename($name);
        $def =~ s/\.[^\.]+$//;
        $def =~ s/\\/_/g;
        $def =~ s/[\s\-]/_/g;

        if (!defined $pname || $pname eq '') {
          ## Take the default project name since one wasn't provided.
          $pname = $def;
        }
        else {
          ## Convert back-slashes, spaces and dashes to underscores.
          $pname =~ s/\\/_/g;
          $pname =~ s/[\s\-]/_/g;
        }

        ## If the project has a '*' we need to have MPC fix that up for
        ## us.
        $pname = Creator::fill_type_name(undef, $pname, $def);
        push(@lines, "project$parents {");
      }
      elsif ($line =~ /^(shared|static)name\s*=\s*(.+)$/) {
        ## Add in the libs and after settings.
        my $lib = $2;
        if (defined $pname && $lib ne '') {
          push(@lines, "  libs  += $2",
                       "  after += $pname",
                       "}");
        }
        last;
      }
    }
    close($fh);

    ## Only return the lines if there is more than one line.  It is
    ## possible (and likely) that we've read in the project declaration,
    ## but the project did not contain a sharedname or staticname
    ## setting.
    return @lines if ($#lines > 0);
  }

  return ();
}

sub write_base {
  my($in, $out) = @_;
  my @lines = gather_info($in);

  if ($#lines >= 0) {
    if (-r $out) {
      print STDERR "ERROR: $out already exists\n";
    }
    else {
      my $fh = new FileHandle();
      if (open($fh, ">$out")) {
        foreach my $line (@lines) {
          print $fh "$line\n";
        }
        close($fh);

        ## Everything was ok, return zero.
        return 0;
      }
      else {
        print STDERR "ERROR: Unable to write to $out\n";
      }
    }
  }
  else {
    if (-r $in) {
      print STDERR "ERROR: $in is not a valid MPC file\n";
    }
    else {
      print STDERR "ERROR: Unable to read from $in\n";
    }
  }

  ## Non-zero indicates an error (as in the shell $? value).
  return 1;
}

sub usageAndExit {
  my $str = shift;
  print STDERR "$str\n" if (defined $str);
  print STDERR "Create Base Project v$version\n",
               "Usage: ", basename($0), " <mpc files> <output file or ",
               "directory>\n\nThis script will create a base project ",
               "based on the contents of the\nsupplied MPC file.  ",
               "This is only useful if the project ",
               "explictly sets\nsharedname or staticname.\n";
  exit(0);
}

# ******************************************************************
# Main Section
# ******************************************************************

if ($#ARGV > 1) {
  ## Get the last argument and make sure it's a directory.
  my $dir = pop(@ARGV);
  if (!-d $dir) {
    usageAndExit("Creating multiple base projects, but the " .
                 "last argument, $dir, is not a directory");
  }

  ## Process each input file and create the base project with an implicit
  ## base project file name.
  my $status = 0;
  foreach my $input (@ARGV) {
    my $output = $dir . '/' . lc(basename($input));
    $output =~ s/mpc$/mpb/;
    $status += write_base($input, $output);
  }
  exit($status);
}
else {
  my $input  = shift;
  my $output = shift;

  ## Print the usage and exit if there is no input, output or the input
  ## file looks like an option.
  usageAndExit() if (!defined $output ||
                     !defined $input  || index($input, '-') == 0);

  ## If the output file is a directory, we will create the output file
  ## name based on the input file.
  if (-d $output) {
    $output .= '/' . lc(basename($input));
    $output =~ s/mpc$/mpb/;
  }

  ## Create the base project and return the status to the caller of the
  ## script.
  exit(write_base($input, $output));
}
