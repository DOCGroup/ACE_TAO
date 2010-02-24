#! /usr/bin/perl
eval '(exit $?0)' && eval 'exec perl -w -S $0 ${1+"$@"}'
    & eval 'exec perl -w -S $0 $argv:q'
    if 0;

# ******************************************************************
#      Author: Chad Elliott
#        Date: 4/8/2004
#         $Id$
# Description: Combined multiple dsw's into a single dsw
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

my $version = '1.3';

# ******************************************************************
# Subroutine Section
# ******************************************************************

sub usageAndExit {
  my $str = shift;
  print STDERR "$str\n" if (defined $str);
  print STDERR "Combine DSW v$version\n",
               "Usage: ", basename($0),
               " [-u] <output file> <input files...>\n\n",
               "-u  Each input file will be removed after successful ",
               "combination\n\n",
               "NOTE: This script will work for vcw's too.\n\n",
               "Combine multiple dsw's into a single dsw.  You can use ",
               "MPC to generate\n",
               "dynamic projects and then generate static projects using ",
               "the -static,\n",
               "-name_modifier and -apply_project options together.  You ",
               "can then run this\n",
               "script to combine the workspaces into one.\n";
  exit(0);
}

# ******************************************************************
# Main Section
# ******************************************************************

my $output;
my $unlink;
my @input;

for(my $i = 0; $i <= $#ARGV; $i++) {
  my $arg = $ARGV[$i];
  if ($arg =~ /^-/) {
    if ($arg eq '-u') {
      $unlink = 1;
    }
    else {
      usageAndExit("Unknown option: $arg");
    }
  }
  else {
    if (!defined $output) {
      $output = $arg;
    }
    else {
      push(@input, $arg);
    }
  }
}

## Print the usage if there is no output file provided or there isn't at
## least two input files.
usageAndExit() if (!defined $output ||
                   !defined $input[0] || !defined $input[1]);

my $tmp = "$output.$$.tmp";
my $oh  = new FileHandle();

if (open($oh, ">$tmp")) {
  my $msident;
  for(my $i = 0; $i <= $#input; ++$i) {
    ## We only want to take the global settings from the last input file.
    my $input  = $input[$i];
    my $global = ($i == $#input);

    ## Read in the input file and write out the parts that are common
    ## between multiple workspace files (but only on the first input
    ## file).  After that, write out the project information from each
    ## input file.
    my $fh = new FileHandle();
    if (open($fh, $input)) {
      my $in_global = 0;
      while(<$fh>) {
        if (/Microsoft\s+(Developer\s+Studio|eMbedded\s+Visual)/) {
          ## We only want to print out the identifier from the first
          ## input file.
          if (!$msident) {
            $msident = 1;
            print $oh $_;
          }
        }
        else {
          if (/^Global:/) {
            $in_global = 1;
          }
          elsif ($in_global && /^[#]{79,}/) {
            $in_global = 0;
            $_ = '';
          }

          ## Only print out the line if it's not part of the global
          ## settings (unless this is the last input file).
          print $oh $_ if (!$in_global || ($global && $in_global));
        }
      }
      close($fh);
    }
    else {
      print STDERR "ERROR: Unable to open '$input' for reading\n";
      exit(2);
    }
  }
  close($oh);

  ## If the user wants us to remove the input files after successfully
  ## combining them, then do so here.
  unlink(@input) if ($unlink);

  ## We have written the new workspace to a temporary file to allow an
  ## input file to be the same as an output file.  Once we're done, we
  ## need to move it to it's correct output name.
  unlink($output);
  rename($tmp, $output);
}
else {
  print STDERR "ERROR: Unable to open '$tmp' for writing\n";
  exit(1);
}
