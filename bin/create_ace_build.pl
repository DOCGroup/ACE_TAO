eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
#
# Creates an ACE build tree in directory "build/<build name>" below the $ACE_ROOT
# directory.  The build tree directory structure mirrors that of the ACE
# top level directory structure, except that instead of containing any plain
# files, it contains only links to the files in the ACE top level structure.
# Symbolic links will be used instead of hard links if available.
#
# Makefiles, projects, and other build files are not linked. This allows
# use of MPC to generate the correct project types as needed. Use the
# -nompc option to disable this.
#
# This program has a similar purpose to "clone", but in addition to
# only creating symlinks (clone creates hard links, by default), this
# script:
# 1) uses relative rather than absolute symlinks,
# 2) tries not to put junk files into the build tree,
# 3) only creates a new tree in a build/ directory below the current,
#    top level ACE directory (it's a feature :-), but it does enforce
#    consistency).
#
# This program can be re-run on a build tree at any time in order to
# update it.  It will add links for newly added files, and remove
# any that are no longer valid.
# Specifying no command line options will cause all current builds
# to be updated.
#
# If the <build name> starts with "build/", that part will be removed
# from it.

use File::Find ();
use File::Basename;
use FileHandle;
use File::stat;

$usage = "usage: $0 -? | [-d <directory mode>] [-v] [-nompc] <build name>\n";
$directory_mode = 0777;   #### Will be modified by umask, also.
$verbose = 0;
$mpc = 1;   #### When using mpc, we don't want links created for mpc-generated files.
$update_all = 1;
$source='.';

####
#### Check that we're in an ACE "top level" directory.
####
unless (-d 'ace' && -d 'include')  {
  die "Must be in an ACE top level (ACE_ROOT) directory!\n";
}
if (-e 'create_ace_build.links') {
  die "Must be in an ACE top level (ACE_ROOT) directory!\n";
}

$perl_version = $] + 0;
if ($perl_version >= 5) {
  #### Use an eval so that this script will compile with perl4.
  eval <<'PERL5_CWD'
  require Cwd;
  sub cwd {
    Cwd::getcwd ();
  }
PERL5_CWD
} else {
  sub cwd {
    local ($pwd);

    chop ($pwd = `pwd`);
    $pwd;
  }
}

my($starting_dir) = cwd ();
my(@nlinks)       = ();
my(@build_re)     = ();

print "Creating or updating builds in $starting_dir\n";

sub is_changed {
  my($real, $fake) = @_;
  unless (-e $real) {
    die "ERROR: is_changed() real $real not exist.\n";
  }
  my($status1) = stat($real);
  my($status2) = stat($fake);
  my($tm1) = $status1->mtime;

  if ($status1->mtime != $status2->mtime) {
    return 1;
  }
  if ($status1->size != $status2->size) {
    return 1;
  }
  return 0;
}
  
sub cab_link {
  my($real,$fake,$build_regex) = @_;
  my($uif)  = ($^O eq 'MSWin32' ? 'link' : 'symlink');

  my($status) = 0;
  if ($^O eq 'MSWin32') {
    my($fixed) = $fake;
    $fixed =~ s/$build_regex//;
    push(@nlinks, $fixed);

    if (-d "$starting_dir/$fake") {
       die "ERROR: cab_link() $fake is a directory.\n";
    }

    #### chdir does NOT work with relative path names. (ActivePerl 5.6 WinXP)
    my($curdir) = "$starting_dir/" . dirname($fake);
    $status = chdir($curdir);
    if (! $status) {
       die "ERROR: cab_link() chdir " . $curdir . " failed.\n";
    }
    
    my($base_fake) = basename($fake);

    if (-e $base_fake) {
      if (is_changed($real, $base_fake)) {
        unlink($base_fake);
      } else {
        return;
      }
    }

    print "$uif $real $fake\n" if $verbose;
    $status = link ($real, $base_fake);
    chdir($starting_dir);
  } else {
    print "$uif $real $fake\n" if $verbose;
    $status = symlink ($real, $fake);
  }
  if (!$status) {
    die "$0: $real -> $fake failed\n";
  }
}

####
#### Process command line args.
####
while ($#ARGV >= 0  &&  $ARGV[0] =~ /^-/) {
  if ($ARGV[0] eq '-v') {
    $verbose = 1;
  } elsif ($ARGV[0] eq '-d') {
    if ($ARGV[1] =~ /^\d+$/) {
      $directory_mode = eval ($ARGV[1]); shift;
    } else {
      warn "$0:  must provide argument for -d option\n";
      die $usage;
    }
  } elsif ($ARGV[0] =~ /-[?hH]$/) {
    die "$usage";
  } elsif ($ARGV[0] eq '-nompc') {
    $mpc = 0;
  } else {
    warn "$0:  unknown option $ARGV[0]\n";
    die $usage;
  }
  shift;
}

@builds = ();

if ($#ARGV == 0) {
  $update_all = 0;
  $builds[0] = $ARGV[0];
  $builds[0] =~ s%^build[/\\]%%;        #### remove leading "build/", if any
  $builds[0] = "build/$builds[0]";
} else {
  @builds = glob "build/*";
}

sub create_build_regex {
  if ($^O eq 'MSWin32') {
    for ($idx = 0; $idx <= $#builds; $idx++) {
      ## Get the original build name
      $build_re[$idx] = $builds[idx];

      ## Remove any trailing slashes
      $build_re[$idx] =~ s/[\\\/]+$//;

      ## Add a single trailing slash
      $build_re[$idx] .= '/';

      ## Escape any special characters
      $build_re[$idx] =~ s/([\\\$\[\]\(\)\.])/\\$1/g;
    }
  }
}

create_build_regex();

# all builds go in ACE_wrappers\build
unless (-d "$starting_dir/build") {
  print "Creating $starting_dir/build\n";
  mkdir ("$starting_dir/build", $directory_mode);
}
foreach $build (@builds) {
  unless (-d "$starting_dir/$build") {
    print "Creating $starting_dir/$build\n";
    mkdir ("$starting_dir/$build", $directory_mode);
  }
}

####
#### Get all ACE plain file and directory names.
####
@files = ();

sub wanted {
    my ($dev,$ino,$mode,$nlink,$uid,$gid);

    $matches = ! (/^CVS\z/s &&
    ($File::Find::prune = 1)
    ||
    /^build\z/s &&
    ($File::Find::prune = 1)
    ||
    /^\..*obj\z/s &&
    ($File::Find::prune = 1)
    ||
    /^Templates\.DB\z/s &&
    ($File::Find::prune = 1)
    ||
    /^Debug\z/s &&
    ($File::Find::prune = 1)
    ||
    /^Release\z/s &&
    ($File::Find::prune = 1)
    ||
    /^Static_Debug\z/s &&
    ($File::Find::prune = 1)
    ||
    /^Static_Release\z/s &&
    ($File::Find::prune = 1));

    $matches = $matches &&
    ( 
        ($nlink || (($dev,$ino,$mode,$nlink,$uid,$gid) = lstat($_))) &&
        ! -l $_ &&
        ! /^core\z/s &&
        ! /^.*\.state\z/s &&
        ! /^.*\.so\z/s &&   
        ! /^.*\.[oa]\z/s && 
        ! /^.*\.dll\z/s &&   
        ! /^.*\.lib\z/s && 
        ! /^.*\.obj\z/s &&
        ! /^.*~\z/s &&
        ! /^\.\z/s && 
        ! /^\.#.*\z/s &&
        ! /^.*\.log\z/s 
    );

    if ($mpc && $matches) {
      $matches = (
        ! /^.*\.dsp\z/s && 
        ! /^.*\.vcproj\z/s && 
        ! /^.*\.bor\z/s && 
        ! /^.*\.dsw\z/s && 
        ! /^.*\.sln\z/s && 
        ! /^.*\.vcp\z/s && 
        ! /^.*\.nmake\z/s && 
        ! /^.*\.am\z/s && 
        ! /^.*\.vcw\z/s && 
        ! /^.*\.mak\z/s && 
        ! /^.*\.bld\z/s && 
        ! /^.*\.icc\z/s && 
        ! /^.*\.icp\z/s && 
        ! /^.*\.ncb\z/s &&
        ! /^.*\.opt\z/s &&
        ! /^\.cvsignore\z/s &&
        ! /^Makefile.*\z/s
      );
    }

    if ($matches) {
      push(@files, $File::Find::name);
    }
}

File::Find::find({wanted => \&wanted}, '.');

print "Found $#files files and directories.\n";

####
#### Create directories and symlinks to files.
####
foreach $file (@files) {
  $file =~ s%^./%%g;  #### excise leading ./ directory component
  #### -d does NOT work with relative path names. (ActivePerl 5.6 WinXP)
  my($fullname) = "$starting_dir/$file";
  for ($idx = 0; $idx <= $#builds; $idx++) {
    my($build) = $builds[$idx];
    if (-d $fullname) {
      unless (-d "$starting_dir/$build/$file") {
        print "Creating $build/$file\n" if $verbose;
        mkdir ("$starting_dir/$build/$file", $directory_mode);
      }
    } else {
      unless ((-e "$build/$file") && ($^O ne 'MSWin32')) {
        $up = '../..';
        while ($file =~ m%/%g) {
          $up .= '/..';
        }
        cab_link("$up/$file", "$build/$file", $build_re[$idx]);
      }
    }
  }
}

foreach $build (@builds) {
  ####
  #### Find all the symlinks in the build directory, and remove ones
  #### that are no longer actually linked to a file.
  ####

  if ($^O eq 'MSWin32') {
    my($lfh) = new FileHandle();
    my($links_file) = "$starting_dir/$build/create_ace_build.links";
    if (-e $links_file) {
      if (open($lfh, $links_file)) {
        while(<$lfh>) {
          my($line) = $_;
          $line =~ s/\s+$//;
          if (-e "$starting_dir/$line") {
            ## The links were already added in cab_link when they 
            ## were checked for changes.
          } else {
            print "Removing $build/$line \n" if $verbose;
            unlink("$starting_dir/$build/$line") || warn "$0: unlink of $build/$line failed\n";
          }
        }
        close($lfh);
      }
      unless (unlink($links_file)) {
        die "Couldn't delete links file.\n";
      }
    }
    print "Writing $#nlinks links to link file.\n";
    if (open($lfh, ">$links_file")) {
      foreach my $lnk (@nlinks) {
        print $lfh "$lnk\n";
      }
      close($lfh);
    } else {
      die "Couldn't open links file.\n";
    }
  }
  else {
    @lfiles = ();

    sub lcheck {
      ## There's no way to know if we have hard linked back to a now
      ## non-existent file.  So, just do the normal -l on the file
      ## which will cause no files to be pushed on Windows.
      if (-l $_) {
        push(@lfiles, $File::Find::name);
      }
    }

    File::Find::find({wanted => \&lcheck}, $build);

    foreach (@lfiles) {
      local @s = stat $_;
      if ($#s == -1) {
        print "Removing $_ \n" if $verbose;
        unlink $_  ||  warn "$0: unlink of $_ failed\n";
      }
    }
  }

  ####
  #### Done: print message.
  ####
  print "\nCompleted creation of $build/.\n";

  if (! -e "$build/ace/config.h") {
    print "Be sure to setup $build/ace/config.h";
  }

  if ($^O ne 'MSWin32' &&
      ! -e "$build/include/makeinclude/platform_macros.GNU") {
    print " and\n$build/include/makeinclude/platform_macros.GNU";
  }
  print ".\n";
}

#### EOF
