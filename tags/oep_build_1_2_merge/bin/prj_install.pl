eval '(exit $?0)' && eval 'exec perl -w -S $0 ${1+"$@"}'
    & eval 'exec perl -w -S $0 $argv:q'
    if 0;

# ******************************************************************
#      Author: Chad Elliott
# Create Date: 3/09/2004
#         $Id$
# ******************************************************************

# ******************************************************************
# Pragma Section
# ******************************************************************

use strict;
use FileHandle;
use File::Copy;
use File::Basename;

# ************************************************************
# Data Section
# ************************************************************

my($insext) = 'ins';
my($version) = '$Id$';
$version =~ s/.*\s+(\d+[\.\d]+)\s+.*/$1/;

# ************************************************************
# Subroutine Section
# ************************************************************

sub copyFiles {
  my($files)   = shift;
  my($insdir)  = shift;
  my($verbose) = shift;

  foreach my $file (@$files) {
    my($fulldir) = $insdir . '/' . dirname($file);
    if (! -d $fulldir) {
      my($tmp) = '';
      foreach my $part (split(/[\/\\]/, $fulldir)) {
        $tmp .= $part . '/';
        mkdir($tmp, 0755);
      }
    }

    if (! -e "$insdir/$file" || (-M $file) < (-M "$insdir/$file")) {
      if ($verbose) {
        print "Copying to $insdir/$file\n";
      }
      if (!copy($file, "$insdir/$file")) {
        print STDERR "ERROR: Unable to copy $file to $insdir\n";
        return 0;
      }
    }
    else {
      if ($verbose) {
        print "Skipping $file\n";
      }
    }
  }
  return 1;
}


sub loadInsFiles {
  my($files)   = shift;
  my($tags)    = shift;
  my($verbose) = shift;
  my(@copy)    = ();

  foreach my $file (@$files) {
    my($fh) = new FileHandle();
    if (open($fh, $file)) {
      if ($verbose) {
        print "Loading $file\n";
      }
      my($base) = dirname($file);
      if ($base eq '.') {
        $base = '';
      }
      else {
        $base =~ s/^\.[\/\\]+//;
      }

      my($current) = undef;
      while(<$fh>) {
        my($line) = $_;
        $line =~ s/^\s+//;
        $line =~ s/\s+$//;

        if ($line ne '') {
          if ($line =~ /^(\w+):$/) {
            if (defined $$tags{$1}) {
              $current = $1;
            }
            else {
              $current = undef;
            }
          }
          elsif (defined $current) {
            push(@copy, "$base/$line");
          }
        }
      }
      close($fh);
    }
    else {
      print STDERR "Unable to open $file\n";
      return ();
    }
  }

  return @copy;
}


sub getInsFiles {
  my($file)  = shift;
  my(@files) = ();

  if (-d $file) {
    my($fh) = new FileHandle();
    if (opendir($fh, $file)) {
      foreach my $f (grep(!/^\.\.?$/, readdir($fh))) {
        push(@files, getInsFiles("$file/$f"));
      }
      closedir($fh);
    }
  }
  elsif ($file =~ /\.$insext$/) {
    push(@files, $file);
  }
  return @files;
}


sub usageAndExit {
  my($msg) = shift;
  if (defined $msg) {
    print STDERR "$msg\n";
  }
  my($base) = basename($0);
  my($spc)  = ' ' x (length($base) + 8);
  print STDERR "$base v$version\n",
               "Usage: $base [-a tag1[,tagN]] [-s tag1[,tagN]] [-v]\n",
               $spc, "[install directory] [$insext files or directories]\n\n",
               "Install files matching the tag specifications found ",
               "in $insext files.\n\n",
               "-a  Adds to the default set of tags that get copied.\n",
               "-s  Sets the tags that get copied.\n",
               "-v  Enables verbose mode.\n";
  exit(0);
}

# ************************************************************
# Main Section
# ************************************************************

my($verbose)  = undef;
my($first)    = 1;
my($insdir)   = undef;
my(@insfiles) = ();
my(%tags)     = ('header_files'   => 1,
                 'idl_files'      => 1,
                 'inline_files'   => 1,
                 'template_files' => 1,
                );

for(my $i = 0; $i <= $#ARGV; ++$i) {
  my($arg) = $ARGV[$i];
  if ($arg =~ /^-/) {
    if ($arg eq '-a') {
      ++$i;
      if (defined $ARGV[$i]) {
        foreach my $tag (split(',', $ARGV[$i])) {
          $tags{$tag} = 1;
        }
      }
      else {
        usageAndExit('-a requires a parameter.');
      }
    }
    elsif ($arg eq '-s') {
      ++$i;
      if (defined $ARGV[$i]) {
        %tags = ();
        foreach my $tag (split(',', $ARGV[$i])) {
          $tags{$tag} = 1;
        }
      }

      else {
        usageAndExit('-s requires a parameter.');
      }
    }
    elsif ($arg eq '-v') {
      $verbose = 1;
    }
    else {
      usageAndExit('Unkown option: ' . $arg);
    }
  }
  elsif (!defined $insdir) {
    $insdir = $arg;
  }
  else {
    if ($first) {
      $first = 0;
      if ($verbose) {
        print "Collecting $insext files...\n";
      }
    }
    push(@insfiles, getInsFiles($arg));
  }
}

if (!defined $insdir) {
  usageAndExit();
}
elsif (!defined $insfiles[0]) {
  print "No $insext files were found.\n";
  exit(1);
}

my($status) = 1;
my(@files)  = loadInsFiles(\@insfiles, \%tags, $verbose);
if (defined $files[0]) {
  $status = (copyFiles(\@files, $insdir, $verbose) ? 0 : 1);
}

exit($status);
