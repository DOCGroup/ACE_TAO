package DependencyEditor;

# ************************************************************
# Description   : Edits existing dependencies.
# Author        : Chad Elliott
# Create Date   : 2/10/2002
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;
use FileHandle;
use File::Basename;

use DependencyGenerator;
use ObjectGeneratorFactory;

# ************************************************************
# Subroutine Section
# ************************************************************

sub new {
  my($class) = shift;
  my($self)  = bless {
                     }, $class;
  return $self;
}


sub process {
  my($self)    = shift;
  my($output)  = shift;
  my($type)    = shift;
  my($cpp)     = shift;
  my($macros)  = shift;
  my($ipaths)  = shift;
  my($replace) = shift;
  my($files)   = shift;
  my($status)  = 0;
  my(@exclude) = ();
  my(@options) = ();

  ## Determine which include files to exclude based on the directory
  if (lc(basename($cpp)) eq 'cl' || lc(basename($cpp)) eq 'cl.exe') {
    push(@options, '/MD');
    if (defined $ENV{INCLUDE}) {
      foreach my $exc (split(';', $ENV{INCLUDE})) {
        $exc =~ s/\\/\\\\\\\\/g;
        push(@exclude, $exc);
      }
    }
  }
  else {
    @exclude = ('/usr/','/opt/', '/var/');
  }

  ## Back up the original file and receive the contents
  my(@contents) = ();
  if (-r $output) {
    if (!$self->backup($output, \@contents)) {
      print STDERR "ERROR: Unable to backup $output\n";
      return 1;
    }
  }

  ## Write out the new file
  my($fh) = new FileHandle();
  if (open($fh, ">$output")) {
    foreach my $line (@contents) {
      if ($line =~ /DO NOT DELETE THIS LINE/) {
        last;
      }
      print $fh $line;
    }

    print $fh "# DO NOT DELETE THIS LINE -- " . basename($0) . " uses it.\n" .
              "# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.\n\n";

    my($dep) = new DependencyGenerator($cpp, $macros, \@options, $ipaths);
    my($objgen) = ObjectGeneratorFactory::create($type);
    foreach my $file (sort @$files) {
      my(@objects) = $objgen->process($file);
      print $fh $dep->process($file, \@objects,
                              \@exclude, $replace, $type) . "\n";
    }

    print $fh "# IF YOU PUT ANYTHING HERE IT WILL GO AWAY\n";
    close($fh);
  }
  else {
    print STDERR "ERROR: Unable to open $output for output\n";
    $status++;
  }
  return $status;
}


sub backup {
  my($self)     = shift;
  my($source)   = shift;
  my($contents) = shift;
  my($status)   = 0;
  my($fh)       = new FileHandle();
  my($backup)   = "$source.bak";

  unlink($backup);
  if (open($fh, $source)) {
    my($oh) = new FileHandle();
    if (open($oh, ">$backup")) {
      $status = 1;
      while(<$fh>) {
        print $oh $_;
        push(@$contents, $_);
      }
      close($oh);

      ## Set file permission
      my(@buf) = stat($source);
      if (defined $buf[8] && defined $buf[9]) {
        utime($buf[8], $buf[9], $backup);
      }
    }
    close($fh);
  }
  return $status;
}


1;
