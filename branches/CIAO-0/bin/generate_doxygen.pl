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
$exclude_ace = 0;
$exclude_tao = 0;
$verbose = 0;
$perl_path = '/usr/bin/perl';
$dot_path = '/usr/local/bin';
@ACE_DOCS = ('ace',
             'ace_man',
             'ace_rmcast',
             'ace_ssl',
             'ace_qos',
             'acexml');
@TAO_DOCS = ('tao'
             ,'tao_portableserver'
             ,'tao_rtportableserver'
             ,'tao_rtcorba'
             ,'tao_dynamicany'
             ,'tao_dynamicinterface'
             ,'tao_iormanip'
             ,'tao_iortable'
             ,'tao_esf'
             ,'tao_rtevent'
             ,'tao_cosevent'
             ,'tao_cosnotification'
             ,'tao_implrepo'
             ,'tao_strategies'
             ,'tao_smartproxies'
             ,'tao_av'
	     ,'tao_security'
	     ,'tao_ssliop'
            );

# Modify defaults using the command line arguments
&parse_args ();

open(CONFIG_H, ">ace/config.h")
  || die "Cannot create config file\n";
print CONFIG_H "#include \"ace/config-doxygen.h\"\n";
close (CONFIG_H);

&generate_doxy_files ('ACE',     'VERSION', @ACE_DOCS) if (!$exclude_ace);
&generate_doxy_files ('TAO', 'TAO/VERSION', @TAO_DOCS) if (!$exclude_tao);

unlink "ace/config.h";

exit 0;

sub parse_args {
  my @ARGS = ();
  while ($#ARGV >= 0) {
    if (!($ARGV[0] =~ m/^-/)) {
      push @ARGS, $ARGV[0];
    } elsif ($ARGV[0] eq "-is_release") {
      $is_release = 1;
    } elsif ($ARGV[0] eq "-exclude_ace") {
      $exclude_ace = 1;
    } elsif ($ARGV[0] eq "-exclude_tao") {
      $exclude_tao = 1;
    } elsif ($ARGV[0] eq "-verbose") {
      $verbose = 1;
    } elsif ($ARGV[0] eq "-perl_path" && $#ARGV >= 1) {
      $perl_path = $ARGV[1];
      shift;
    } elsif ($ARGV[0] eq "-dot_path" && $#ARGV >= 1) {
      $dot_path = $ARGV[1];
      shift;
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
    POSIX::strftime("%Y/%m/%d-%H:%M", localtime)
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

    my $generate_man  = 0;
    my $generate_html = 0;
    my @output_dirs = ();
    while (<DOXYINPUT>) {
      chomp;
      if (/^PROJECT_NUMBER/) {
        print DOXYOUTPUT "PROJECT_NUMBER        = ", $VERSION, "\n";
        next;
      } elsif (/^PERL_PATH /) {
	print DOXYOUTPUT "PERL_PATH = $perl_path\n";
	next;
      } elsif (/^DOT_PATH /) {
	print DOXYOUTPUT "DOT_PATH = $dot_path\n";
	next;
      } elsif (/^QUIET / && $verbose) {
	print DOXYOUTPUT "QUIET = NO\n";
	next;
      } elsif (/^INLINE_SOURCES/ && $is_release) {
	print DOXYOUTPUT "INLINE_SOURCES = NO\n";
	next;
      } elsif (/^SOURCE_BROWSER/ && $is_release) {
	print DOXYOUTPUT "SOURCE_BROWSER = NO\n";
	next;
      } elsif (/^VERBATIM_HEADERS/ && $is_release) {
	print DOXYOUTPUT "VERBATIM_HEADERS = NO\n";
	next;
#      } elsif (/^INCLUDE_GRAPH/ && $is_release) {
#	print DOXYOUTPUT "INCLUDE_GRAPH = NO\n";
#	next;
#      } elsif (/^INCLUDED_BY_GRAPH/ && $is_release) {
#	print DOXYOUTPUT "INCLUDED_BY_GRAPH = NO\n";
#	next;
      } elsif (/^GENERATE_MAN/ && /= YES/) {
        $generate_man = 1;
      } elsif (/^GENERATE_HTML/ && /= YES/) {
        $generate_html = 1;
      } elsif ($generate_html && /^HTML_OUTPUT/) {
        my @field = split(' = ');
        if ($#field >= 1) {
          push @output_dirs, $field[1];
        }
      } elsif ($generate_html && /^MAN_OUTPUT/) {
        my @field = split(' = ');
        if ($#field >= 1) {
          push @output_dirs, $field[1];
        }
      }
      print DOXYOUTPUT $_, "\n";
    }
    close (DOXYOUTPUT);
    close (DOXYINPUT);

    foreach my $i (@output_dirs) {
      File::Path::mkpath($i, 0, 0755);
    }

    &run_doxy ($output);

    unlink $output;
  }
}

sub run_doxy {
  my $config = shift;
  open(DOX,"doxygen $config 2>&1 |")
    || die "cannot start ACE doxygen process\n";
  while (<DOX>) {
    print $_;
  }
  close (DOX)
    || die "error while running doxygen on $config\n";
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
