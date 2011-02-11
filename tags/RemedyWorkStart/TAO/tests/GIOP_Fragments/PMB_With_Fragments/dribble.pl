eval '(exit $?0)' && eval 'exec perl -w -S $0 ${1+"$@"}'
    & eval 'exec perl -w -S $0 $argv:q'
    if 0;

# ******************************************************************
#      Author: Chris Cleeland
#        Date: 10/26/2002
#         $Id$
# ******************************************************************

use Getopt::Long;
use File::Basename;
use IO::Socket::INET;
use IO::File;
use Carp;

sub getnum {
  use POSIX qw(strtoul);
  my $str = shift;
  $str =~ s/^\s+//;
  $str =~ s/\s+$//;
  $! = 0;
  my($num, $unparsed) = strtoul($str);
  if (($str eq '') || ($unparsed != 0) || $!) {
    return undef;
  } else {
    return $num;
  }
}

my $corba_server_addr = "127.0.0.1";
my $corba_server_port = 12345;
my $progress_interval = 10;
my $verbosity = 0;
my $interactive = undef;
my $datastreamfile = 'datastream.dat';
my $dataxmitlayoutfile = undef;
my $delay = .25;

my %options = ('host|h=s'             => \$corba_server_addr,
               'port|p=s'             => \$corba_server_port,
               'interval|i=i'         => \$progress_interval,
               'verbose|verbosity|v+' => \$verbosity,
               'interactive'          => \$interactive,
               'stream|s=s'           => \$datastreamfile,
               'layout|l=s'           => \$dataxmitlayoutfile,
               'delay|d=f'            => \$delay,
              );
my $result = GetOptions (%options);

if (!$result) {
  my($str)     = 'Usage: ' . basename($0);
  my($initial) = length($str);
  my($length)  = $initial;
  my($maxLine) = 78;

  print $str;

  foreach my $key (sort keys %options) {
    my($opt, $type) = split(/[|=]/, $key);
    my($str) = " [--$opt" . (defined $type ? " <$opt>" : "") . "]";
    my($len) = length($str);
    if ($length + $len > $maxLine) {
      print "\n" . (" " x $initial);
      $length = $initial;
    }
    print $str;
    $length += $len;
  }
  print "\n";
  exit(0);
}

croak "Must supply a data file using --stream and a layout file using --layout"
  unless defined $datastreamfile && defined $dataxmitlayoutfile;

my $sock = IO::Socket::INET->new(PeerAddr => $corba_server_addr,
                                 PeerPort => $corba_server_port,
                                 Proto    => 'tcp')
  or croak "Unable to establish connection to $corba_server_addr:$corba_server_port: $!\n";

#
# Infer a name for the layout file
#
if (! defined $dataxmitlayoutfile) {
  $dataxmitlayoutfile = $datastreamfile;
  $dataxmitlayoutfile =~ s/\.dat$/\.layout/;
}

# Now we just start reading from <> and writing to the socket We
# currently assume all are oneways, so we don't worry about reading
# from the socket.

my $stream = new IO::File $datastreamfile, "r"
  or croak "Unable to open $datastreamfile for reading: $!";
my $layout = new IO::File $dataxmitlayoutfile, "r"
  or croak "Unable to open $dataxmitlayoutfile for reading: $!";

$| = 1;
print "Sending...(1 dot every $progress_interval hunks)\n";
print "Hit <RETURN> key to send a packet\n" if defined $interactive;
my $numread;
do {

  #
  # Determine the size of the hunk we have to read/send
  #
  $_ = $layout->getline;
  s/#.*$//;
  chomp $_;
  next if ($_ =~ /^\s*$/);

  my $requested_hunksize = getnum($_);
  my $l = $layout->input_line_number;

  my $hunk;

  #
  # Read in the hunk size we want
  #
  $numread = $stream->sysread($hunk, $requested_hunksize);
  if (! defined $numread) {
    carp "\nHunk $l: Error reading from stream: $!";
    next;
  }
  elsif ($numread != $requested_hunksize) {
    carp "\nHunk $l: short read (expected $requested_hunksize, got $numread); going on.";
  }

  print "Hunk $l: length ", length($hunk), "\n" if ($verbosity >= 1);
  if (defined $interactive) {
    <STDIN>;
  }
  else {
    select(undef, undef, undef, $delay);
  }

  my $ret = $sock->send($hunk, 0);
  if ($ret != length($hunk)) {
    carp "\nHunk $l: problem sending hunk $.: $!\n";
    exit(1);
  }

  print "." if ($. % $progress_interval == 0);
} until $numread == 0 || $layout->eof;
print "\nDone.\n";

## Sleep 1 second to allow the socket on Windows to get flushed
## before it's destroyed and this process exits.
sleep(1);
