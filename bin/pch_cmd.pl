eval '(exit $?0)' && eval 'exec perl -w -S $0 ${1+"$@"}'
    & eval 'exec perl -w -S $0 $argv:q'
    if 0;

# ******************************************************************
#      Author: Chad Elliott
#        Date: 6/17/2003
#         $Id$
# Description: This script can be used with any command that generates
#              c++ files that are going to be used in conjunction
#              with precompiled headers.  If the command in question
#              does not support the generation of a precompiled
#              header #include, then this script can be used to do so.
#              The given command is executed and then the output file
#              generated is edited to add the #include of the
#              precompiled header at the top.
# ******************************************************************

# ******************************************************************
# Pragma Section
# ******************************************************************

use strict;
use FileHandle;
use File::Basename;

# ************************************************************
# Subroutine Section
# ************************************************************

sub usageAndExit {
  my($msg)  = shift;
  my($base) = basename($0);

  if (defined $msg) {
    print "$msg\n";
  }
  print "Usage: $base [-t <output option>] [-p <pch header>] " .
        "[-c <command>]\n" .
        "        " . " " x length($base) . "[command options]\n\n" .
        "-t   Pass the output option of the command.  This is used to\n" .
        "     determine the output name for the command.  If the command\n" .
        "     does not have an output option, then this script can not be " .
        "used.\n" .
        "-p   Pass the name of the precompiled header file.\n" .
        "-c   Pass the name of the command to execute.\n" .
        "     NOTE: This option must be used after -t and -p.\n";
  exit(1);
}

# ************************************************************
# Main Section
# ************************************************************

my(@cmdargv) = ();
my($cmd)     = undef;
my($outopt)  = undef;
my($output)  = undef;
my($pch)     = undef;
my($next)    = 0;

for(my $i = 0; $i <= $#ARGV; ++$i) {
  ## If $cmd hasn't been set, then we assume that these
  ## parameters are for this script.
  if (!defined $cmd) {
    if ($ARGV[$i] =~ /\-c(.*)?/) {
      ## Set the command
      if (defined $1) {
        $cmd = $1;
      }
      elsif (defined $ARGV[$i + 1]) {
        $cmd = $ARGV[++$i];
      }
      else {
        usageAndExit('-c requires a command option');
      }
    }
    elsif ($ARGV[$i] =~ /\-t(.*)?/) {
      ## Save the commands output switch
      if (defined $1 && $1 ne '') {
        $outopt = $1;
      }
      elsif (defined $ARGV[$i + 1]) {
        $outopt = $ARGV[++$i];
      }
      else {
        usageAndExit('-t requires an output option');
      }
    }
    elsif ($ARGV[$i] =~ /\-p(.*)?/) {
      ## Save the name of the precompiled header file
      if (defined $1) {
        $pch = $1;
      }
      elsif (defined $ARGV[$i + 1]) {
        $pch = $ARGV[++$i];
      }
      else {
        usageAndExit('-p requires a pch file option');
      }
    }
    else {
      usageAndExit("Unknown option: $ARGV[$i]");
    }
  }
  else {
    ## Save the argument for the command
    push(@cmdargv, $ARGV[$i]);

    if ($next) {
      ## Save the output name and clear the next flag
      $output = $ARGV[$i];
      $next   = 0;
    }
    elsif (defined $outopt && $ARGV[$i] =~ /$outopt(.*)?/) {
      if (defined $1 && $1 ne '') {
        ## If the output name is given as part of the output option
        ## we will save it here.
        $output = $1;
      }
      else {
        ## Otherwise, the next argument is the output name.
        $next = 1;
      }
    }
  }
}

if (!defined $cmd) {
  usageAndExit();
}

## Run the command and save the status
my($status) = system("$cmd @cmdargv");
if ($status > 0 && $status < 256) {
  ## A signal killed the process
}
else {
  ## Get the status down to the return value
  $status /= 256;

  ## Only process the file if the status is zero
  if ($status == 0) {
    $status = 1;

    ## Set up the signal handlers
    my($ih)   = new FileHandle();
    my($oh)   = new FileHandle();
    my($tmp)  = "$output.$>.$$";
    foreach my $signum ('INT', 'TERM') {
      $SIG{$signum} = sub { if (-r $tmp) {
                              close($oh);
                              close($ih);
                              unlink($output);
                              rename($tmp, $output);
                            }
                            exit(2);
                          };
    }

    ## Begin editing the output file.  We save the original,
    ## print out the #include for the precompiled header and
    ## then copy the original into the output file.
    if (rename($output, $tmp)) {
      if (open($ih, $tmp)) {
        if (open($oh, ">$output")) {
          print $oh "#include \"$pch\"\n";
          while(<$ih>) {
            print $oh $_;
          }
          close($oh);
          $status = 0;
        }
        close($ih);
        unlink($tmp);
      }
    }
  }
}

exit($status);
