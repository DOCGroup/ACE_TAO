eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-
# $Id$
#

require POSIX;
require File::Path;

# Configuration and default values

$is_release = 0;
@ACE_DOCS = ('ace',
             'ace_rmcast',
             'ace_ssl');
@TAO_DOCS = ('tao',
             'tao_portableserver',
             'tao_dynamicany',
             'tao_dynamicinterface',
             'tao_iormanip',
             'tao_iortable',
             'tao_esf',
             'tao_rtevent');

# Modify defaults using the command line arguments
&parse_args ();

open(CONFIG_H, ">ace/config.h")
  || die "Cannot create config file\n";
print CONFIG_H "#include \"ace/config-doxygen.h\"\n";
close (CONFIG_H);

&generate_doxy_files ('ACE',     'VERSION', @ACE_DOCS);
&generate_doxy_files ('TAO', 'TAO/VERSION', @TAO_DOCS);

unlink "ace/config.h";

exit 0;

sub parse_args {
  my @ARGS = ();
  while ($#ARGV >= 0) {
    if (!($ARGV[0] =~ m/^-/)) {
      push @ARGS, $ARGV[0];
    } elsif ($ARGV[0] eq "-is_release") {
      $is_release = 1;
    } else {
      print "Ignoring option $ARGV[0]\n";
    }
    shift @ARGV;
  }
  @ARGV = @ARGS;
}

sub generate_doxy_files {

  my $KIT = shift;
  my $VERSION_FILE = shift;
  my @DOCS = @_;

  my $VERSION = 'Snapshot ('.
    POSIX::strftime("%Y/%m/%d @ %H:%M", localtime)
      .')';

  foreach my $i (@DOCS) {
    if ($is_release) {
      my ($major, $minor, $beta) = &get_versions ($KIT, $VERSION_FILE);
      $VERSION = $major.'.'.$minor.'.'.$beta;
    }

    my $input = "etc/".$i.".doxygen";
    my $output = "/tmp/".$i.".".$$.".doxygen";

    open(DOXYINPUT, $input)
      || die "Cannot open doxygen input file $input\n";
    open(DOXYOUTPUT, ">$output")
      || die "Cannot open doxygen output file $output\n";

    my $output_dir = 'html';
    while (<DOXYINPUT>) {
      chomp;
      if (/^PROJECT_NUMBER/) {
        print DOXYOUTPUT "PROJECT_NUMBER        = ", $VERSION, "\n";
      } elsif (/^HTML_OUTPUT/) {
        my @field = split(' = ');
        $output_dir = $field[1];
        print DOXYOUTPUT $_, "\n";
      } else {
        print DOXYOUTPUT $_, "\n";
      }
    }
    close (DOXYOUTPUT);
    close (DOXYINPUT);

    File::Path::mkpath($output_dir, 0, 0755);

    &run_doxy ($output, $output_dir);

    unlink $output;
  }
}

sub run_doxy {
  my $config = shift;
  my $output = shift;
  open(DOX,"doxygen $config 2>&1 |")
    || die "cannot start ACE doxygen process\n";
  while (<DOX>) {
    print $_;
  }
  close (DOX)
    || die "error while running doxygen on $config\n";

  if (!-d $output) {
    die "cannot find output directory <$output> \n";
  }
}

########
######## Retrieve version information from VERSION file(s).
########
sub get_versions () {
  my $KIT = shift;
  my $VERSION_FILE = shift;
  my ($major_version, $minor_version, $beta_version);

  open (VERSION, '<'.$VERSION_FILE)  ||
    die "$0: unable to open VERSION\n";
  while (<VERSION>) {
    chomp;
    if (/$KIT version (\d+)\.(\d+)\.(\d+)/) {
      $major_version = $1;
      $minor_version = $2;
      $beta_version = $3;
      last;
    } elsif (/$KIT version (\d+)\.(\d+)[^\.]/) {
      #### Previous release was a minor.
      $major_version = $1;
      $minor_version = $2;
      $beta_version  = '0';
      last;
    } elsif (/$KIT version (\d+)[^\.]/) {
      #### Previous release was a major.
      $major_version = $1;
      $minor_version = '0';
      $beta_version  = '0';
      last;
    }
  }
  close VERSION;

  return ($major_version, $minor_version, $beta_version);
}
