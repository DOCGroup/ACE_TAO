#! /usr/bin/perl
eval '(exit $?0)' && eval 'exec perl -w -S $0 ${1+"$@"}'
    & eval 'exec perl -w -S $0 $argv:q'
    if 0;

# ******************************************************************
#      Author: Chad Elliott
#        Date: 4/8/2004
#         $Id$
# Description: Clone a build tree into an alternate location.
#              This script is a rewrite of create_ace_build.pl and
#              does not restrict the user to place the build
#              in any particular location or that it be used with
#              ACE_wrappers.  Some of the functions were barrowed
#              from create_ace_build.pl, but were modified quite a
#              bit.
# ******************************************************************

# ******************************************************************
# Pragma Section
# ******************************************************************

use strict;
use Cwd;
use FileHandle;
use File::Copy;
use File::Find;
use File::Path;
use File::stat;
use File::Basename;

# ******************************************************************
# Data Section
# ******************************************************************

my $exclude;
my @foundFiles;
my $verbose = 0;
my $lbuildf = 0;
my $version = '1.16';

eval 'symlink("", "");';
my $hasSymlink = ($@ eq '');

# ******************************************************************
# Subroutine Section
# ******************************************************************

sub findCallback {
  my $matches = !(/^CVS\z/s && ($File::Find::prune = 1)            ||
                  /^\.svn\z/s && ($File::Find::prune = 1)          ||
                  defined $exclude &&
                  /^$exclude\z/s && ($File::Find::prune = 1)       ||
                  /^\.cvsignore\z/s && ($File::Find::prune = 1)    ||
                  /^build\z/s && ($File::Find::prune = 1)          ||
                  /^\..*obj\z/s && ($File::Find::prune = 1)        ||
                  /^Templates\.DB\z/s && ($File::Find::prune = 1)  ||
                  /^Debug\z/s && ($File::Find::prune = 1)          ||
                  /^Release\z/s && ($File::Find::prune = 1)        ||
                  /^Static_Debug\z/s && ($File::Find::prune = 1)   ||
                  /^Static_Release\z/s && ($File::Find::prune = 1)
                 );

  if ($matches) {
    $matches &&= (! -l $_           &&
                  ! /^core\z/s      &&
                  ! /^.*\.rej\z/s   &&
                  ! /^.*\.state\z/s &&
                  ! /^.*\.so\z/s    &&
                  ! /^.*\.[oa]\z/s  &&
                  ! /^.*\.dll\z/s   &&
                  ! /^.*\.lib\z/s   &&
                  ! /^.*\.obj\z/s   &&
                  ! /^.*~\z/s       &&
                  ! /^\.\z/s        &&
                  ! /^\.#.*\z/s     &&
                  ! /^.*\.ncb\z/s   &&
                  ! /^.*\.opt\z/s   &&
                  ! /^.*\.bak\z/s   &&
                  ! /^.*\.suo\z/s   &&
                  ! /^.*\.ilk\z/s   &&
                  ! /^.*\.pdb\z/s   &&
                  ! /^.*\.pch\z/s   &&
                  ! /^.*\.log\z/s   &&
                  ! ( -f $_ && /^.*\.d\z/s )
                 );

    if ($matches) {
      if (!$lbuildf) {
        $matches = (! /^.*\.dsp\z/s        &&
                    ! /^.*\.dsw\z/s        &&
                    ! /^.*\.vcproj\z/s     &&
                    ! /^.*\.sln\z/s        &&
                    ! /^Makefile.*\z/s     &&
                    ! /^GNUmakefile.*\z/s  &&
                    ! /^.*\.am\z/s         &&
                    ! /^\.depend\..*\z/s   &&
                    ! /^.*\.vcn\z/s        &&
                    ! /^.*\.vcp\z/s        &&
                    ! /^.*\.vcw\z/s        &&
                    ! /^.*\.vpj\z/s        &&
                    ! /^.*\.vpw\z/s        &&
                    ! /^.*\.cbx\z/s        &&
                    ! /^.*\.bpgr\z/s       &&
                    ! /^.*\.bmak\z/s       &&
                    ! /^.*\.bmake\z/s      &&
                    ! /^.*\.mak\z/s        &&
                    ! /^.*\.nmake\z/s      &&
                    ! /^.*\.bld\z/s        &&
                    ! /^.*\.icc\z/s        &&
                    ! /^.*\.icp\z/s        &&
                    ! /^.*\.project\z/s    &&
                    ! /^.*\.wrproject\z/s  &&
                    ! /^.*\.wrmakefile\z/s &&
                    ! /^.*\.vxtest\z/s
                  );
      }

      if ($matches) {
        ## Remove the beginning dot slash as we save the file
        push(@foundFiles, $File::Find::name);
        $foundFiles[$#foundFiles] =~ s/^\.[\\\/]+//;
      }
    }
  }
}


sub getFileList {
  File::Find::find({wanted => \&findCallback}, '.');
  return \@foundFiles;
}


sub backupAndMoveModified {
  my($realpath, $linkpath) = @_;
  my $mltime   = -M $linkpath;
  my $mrtime   = -M $realpath;
  my $status   = 1;

  ## -M returns the number of days since modification.  Therefore,
  ## a smaller time means that it has been modified more recently.
  ## This is different than what stat() returns.

  ## If the hard linked file is newer than the original file, that means
  ## the link has been broken by something and needs to be "fixed".  We
  ## will back up the original file and move the modified file into it's
  ## place.
  if ($mltime < $mrtime) {
    $status = 0;

    ## Move the real file to a backup
    unlink("$realpath.bak");
    if (rename($realpath, "$realpath.bak")) {
      ## Move the linked file to the real file name
      if (move($linkpath, $realpath)) {
        $status = 1;
      }
      else {
        ## The move failed, so we will attempt to put
        ## the original file back.
        unlink($realpath);
        rename("$realpath.bak", $realpath);
      }
    }
  }
  elsif ($mltime != $mrtime || -s $linkpath != -s $realpath) {
    ## The two files are different in some way, we need to make a backup
    ## so that we don't cause a loss of data/work.
    $status = 0;
  }

  if (!$status) {
    ## We were not able to properly deal with this file.  We will
    ## attempt to preserve the modified file.
    unlink("$linkpath.bak");
    rename($linkpath, "$linkpath.bak");
  }
}


sub hardlink {
  my($realpath, $linkpath) = @_;

  if ($^O eq 'MSWin32' && ! -e $realpath) {
    ## If the real file "doesn't exist", then we need to
    ## look up the short file name.
    my $short = Win32::GetShortPathName($realpath);

    ## If we were able to find the short file name, then we need to
    ## try again.
    if (defined $short) {
      $realpath = $short;
    }
    else {
      ## This should never happen, but there appears to be a bug
      ## with the underlying Win32 APIs on Windows Server 2003.
      ## Long paths will cause an error which perl will ignore.
      ## Unicode versions of the APIs seem to work fine.
      ## To experiment try Win32 _fullpath() and CreateHardLink with
      ## long paths.
      print "WARNING: Skipping $realpath.\n";
      return 1;
    }
  }

  return link($realpath, $linkpath);
}


sub symlinkFiles {
  my($files, $fullbuild, $dmode, $startdir, $absolute) = @_;
  my $sdlength = length($startdir) + 1;
  my $partial  = ($absolute ? undef :
                              substr($fullbuild, $sdlength,
                                     length($fullbuild) - $sdlength));

  foreach my $file (@$files) {
    my $fullpath = "$fullbuild/$file";
    if (-e $fullpath) {
      ## We need to make sure that we're not attempting to mix hardlinks
      ## and softlinks.
      if (! -d $fullpath && ! -l $fullpath) {
        my $stat = stat($fullpath);
        if ($stat->nlink() > 1) {
          print STDERR "ERROR: Attempting to mix softlinks ",
                       "with a hardlink build.\n",
                       "$fullpath has ", $stat->nlink(), " links.\n";
          return 1;
        }
      }
    }
    else {
      if (-d $file) {
        if ($verbose) {
          print "Creating $fullpath\n";
        }
        if (!mkpath($fullpath, 0, $dmode)) {
          print STDERR "ERROR: Unable to create $fullpath\n";
          return 1;
        }
      }
      else {
        if ($absolute) {
          if ($verbose) {
            print "symlink $startdir/$file $fullpath\n";
          }
          if (!symlink("$startdir/$file", $fullpath)) {
            print STDERR "ERROR: Unable to symlink $fullpath\n";
            return 1;
          }
        }
        else {
          my $buildfile = "$partial/$file";
          my $slashcount = ($buildfile =~ tr/\///);
          my $real = ($slashcount == 0 ? './' : ('../' x $slashcount)) .
                     $file;

          print "symlink $real $fullpath\n" if ($verbose);
          if (!symlink($real, $fullpath)) {
            print STDERR "ERROR: Unable to symlink $fullpath\n";
            return 1;
          }
        }
      }
    }
  }

  ## Remove links that point to non-existant files.  The subroutine is
  ## now anonymous to avoid the "will not stay shared" warning for %dirs.
  my %dirs;
  File::Find::find({wanted => sub {
                                if (-l $_ && ! -e $_) {
                                  unlink($_);
                                  $dirs{$File::Find::dir} = 1;
                                  if ($verbose) {
                                    print "Removing $File::Find::dir/$_\n";
                                  }
                                }
                              }
                   }, $fullbuild);
  foreach my $key (keys %dirs) {
    rmdir($key);
  }
  return 0;
}


sub hardlinkFiles {
  my($files, $fullbuild, $dmode, $startdir) = @_;
  my @hardlinks;

  foreach my $file (@$files) {
    my $fullpath = "$fullbuild/$file";
    if (-d $file) {
      if (! -e $fullpath) {
        if ($verbose) {
          print "Creating $fullpath\n";
        }
        if (!mkpath($fullpath, 0, $dmode)) {
          print STDERR "ERROR: Unable to create $fullpath\n";
          return 1;
        }
      }
    }
    else {
      if (-e $fullpath) {
        ## We need to make sure that we're not attempting to mix hardlinks
        ## and softlinks.
        if (-l $fullpath) {
          print STDERR "ERROR: Attempting to mix hardlinks ",
                       "with a softlink build.\n",
                       "$fullpath is a softlink.\n";
          return 1;
        }
        backupAndMoveModified($file, $fullpath);
      }
      if (! -e $fullpath) {
        if ($verbose) {
          print "hardlink $file $fullpath\n";
        }
        if (!hardlink($file, $fullpath)) {
          print STDERR "ERROR: Unable to link $fullpath\n";
          return 1;
        }
      }

      ## If we successfully linked the file or it already exists,
      ## we need to keep track of it.
      push(@hardlinks, $file);
    }
  }

  ## Remove links that point to non-existant files
  my $lfh = new FileHandle();
  my $txt = "$fullbuild/clone_build_tree.links";
  if (open($lfh, $txt)) {
    my %dirs;
    while(<$lfh>) {
      my $line = $_;
      $line =~ s/\s+$//;
      if (! -e $line) {
        my $full = "$fullbuild/$line";
        unlink($full);
        $dirs{dirname($full)} = 1;
        print "Removing $full\n" if ($verbose);
      }
    }
    close($lfh);
    foreach my $key (keys %dirs) {
      rmdir($key);
    }
  }

  ## Rewrite the link file.
  unlink($txt);
  if (open($lfh, ">$txt")) {
    foreach my $file (@hardlinks) {
      print $lfh "$file\n";
    }
    close($lfh);
  }

  return 0;
}


sub linkFiles {
  my($absolute, $dmode, $hardlink, $builddir, $builds) = @_;
  my $status    = 0;
  my $starttime = time();
  my $startdir  = getcwd();

  ## Ensure that the build directory exists and is writable
  mkpath($builddir, 0, $dmode);
  if (! -d $builddir || ! -w $builddir) {
    print STDERR "ERROR: Unable to create or write to $builddir\n";
    return 1;
  }

  ## Search for the clonable files
  print "Searching $startdir for files...\n";
  my $files = getFileList();
  my $findtime = time() - $starttime;
  print 'Found ', scalar(@$files), ' files and directories in ',
        $findtime, ' second', ($findtime == 1 ? '' : 's'), ".\n";

  foreach my $build (@$builds) {
    my $fullbuild = "$builddir/$build";

    ## Create all of the links for this build
    if (-d $fullbuild) {
      print "Updating $fullbuild\n";
    }
    else {
      print "Creating $fullbuild\n";
      mkpath($fullbuild, 0, $dmode);
    }

    if ($hardlink) {
      $status += hardlinkFiles($files, $fullbuild, $dmode, $startdir);
    }
    else {
      $status += symlinkFiles($files, $fullbuild,
                              $dmode, $startdir, $absolute);
    }
    print "Finished in $fullbuild\n";
  }

  print 'Total time: ', time() - $starttime, " seconds.\n" if ($status == 0);

  return $status;
}


sub usageAndExit {
  my $msg = shift;

  print STDERR "$msg\n" if (defined $msg);

  my $base = basename($0);
  my $spc  = ' ' x (length($base) + 8);

  print STDERR "$base v$version\n\n",
               "Create a tree identical in layout to the current directory\n",
               "with the use of ", ($hasSymlink ? "symbolic links or " : ''),
               "hard links.\n\n",
               "Usage: $base [-b <builddir>] [-d <dmode>] [-f] ",
               ($hasSymlink ? "[-a] [-l] " : ''),
               "[-v]\n",
               $spc, "[build names...]\n\n",
               ($hasSymlink ?
               "-a  Use absolute paths when creating soft links.\n" .
               "-l  Use hard links instead of soft links.\n" : ''),
               "-b  Set the build directory. It defaults to the ",
               "<current directory>/build.\n",
               "-d  Set the directory permissions mode.\n",
               "-f  Link build files (Makefile, .dsw, .sln, .etc).\n",
               "-s  Set the start directory. It defaults to the ",
               "<current directory>.\n",
               "-v  Enable verbose mode.\n";

  exit(0);
}


# ******************************************************************
# Main Section
# ******************************************************************

my $dmode    = 0777;
my $absolute = 0;
my $hardlink = !$hasSymlink;
my $builddir;
my @builds;
my $startdir;

for(my $i = 0; $i <= $#ARGV; ++$i) {
  if ($ARGV[$i] eq '-a') {
    $absolute = 1;
  }
  elsif ($ARGV[$i] eq '-b') {
    ++$i;
    if (defined $ARGV[$i]) {
      $builddir = $ARGV[$i];

      ## Convert backslashes to slashes
      $builddir =~ s/\\/\//g;

      ## Remove trailing slashes
      $builddir =~ s/\/+$//;

      ## Remove duplicate slashes
      while($builddir =~ s/\/\//\//g) {
      }
    }
    else {
      usageAndExit('-b requires an argument');
    }
  }
  elsif ($ARGV[$i] eq '-d') {
    ++$i;
    if (defined $ARGV[$i]) {
      $dmode = $ARGV[$i];
    }
    else {
      usageAndExit('-d requires an argument');
    }
  }
  elsif ($ARGV[$i] eq '-f') {
    $lbuildf = 1;
  }
  elsif ($ARGV[$i] eq '-l') {
    $hardlink = 1;
  }
  elsif ($ARGV[$i] eq '-v') {
    $verbose = 1;
  }
  elsif ($ARGV[$i] eq '-s') {
    ++$i;
    if (defined $ARGV[$i]) {
      $startdir = $ARGV[$i];
    }
    else {
      usageAndExit('-s requires an argument');
    }
  }
  elsif ($ARGV[$i] =~ /^-/) {
    usageAndExit('Unknown option: ' . $ARGV[$i]);
  }
  else {
    push(@builds, $ARGV[$i]);
  }
}

if (defined $startdir && !chdir($startdir)) {
  print "ERROR: Unable to change directory to $startdir\n";
  exit(1);
}

$builddir = getcwd() . '/build' if (!defined $builddir);

if (index($builddir, getcwd()) == 0) {
  $exclude = substr($builddir, length(getcwd()) + 1);
  $exclude =~ s/([\+\-\\\$\[\]\(\)\.])/\\$1/g;
}
else {
  $absolute = 1;
}

if (!defined $builds[0]) {
  my $cwd = getcwd();
  if (chdir($builddir)) {
    @builds = glob('*');
    chdir($cwd);
  }
  else {
    usageAndExit('There are no builds to update.');
  }
}

exit(linkFiles($absolute, $dmode, $hardlink, $builddir, \@builds));
