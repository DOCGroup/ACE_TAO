eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# ******************************************************************
#      Author: Chad Elliott (elliott_c@ociweb.com)
#        Date: 8/14/2000
#         $Id$
# Description: Modify c++ source for combination with other source
#              files into a VxWorks module.
# ******************************************************************

# ******************************************************************
# Pragma Section
# ******************************************************************

use strict;
use File::Basename;

# ******************************************************************
# Data Section
# ******************************************************************

my($name)  = "";
my(@types) = ("short", "int", "long", "unsigned", "size_t",
              "char", "float", "double", "void",
              "CORBA::Boolean",    "CORBA::Short", "CORBA::UShort",
              "CORBA::Long",       "CORBA::ULong", "CORBA::Octet",
              "CORBA::Char",       "CORBA::WChar", "CORBA::LongLong",
              "CORBA::ULongLong",  "CORBA::Float", "CORBA::Double",
              "CORBA::LongDouble", "CORBA::Environment",
             );

# **************************************************************
# Subroutine Section
# **************************************************************

sub needsToBeStatic {
  my($line) = shift;
  $line =~ s/^\s+//;
  $line =~ s/\s+$//;

  if ($line !~ /\s*static\s+/) {
    foreach my $type (@types) {
      if ($line =~ /^(const\s+)?$type\s*[\*]*[\&]*\s*[^:]+/ ||
          $line =~ /^(const\s+)?$type\s*[\*]*[\&]*$/) {
        return 1;
      }
    }
  }
}


sub countChar {
  my($line)     = shift;
  my($char)     = shift;
  my($len)      = length($line);
  my($count)    = 0;
  my($indouble) = 0;
  my($insingle) = 0;

  for(my $i = 0; $i < $len; $i++) {
    my($ch) = substr($line, $i, 1);
    if ($char ne '"' && $ch eq '"') {
      $indouble ^= 1;
    }
    elsif ($char ne '\'' && $ch eq '\'') {
      $indouble ^= 1;
    }
    elsif ($ch eq $char && !$indouble && !$insingle) {
      $count++;
    }
  }
  return $count;
}


my($orbManager) = undef;
sub lookForOrbRun {
  my($line)   = shift;
  my($status) = 0;

  if ($line =~ /([\w:\-\>\.\(\)]+)->run\s*\(.*\)/ ||
      $line =~ /.*orb.*\.run/ ||
      (defined $orbManager && ($line =~ /$orbManager.run/ ||
                               $line =~ /$orbManager->run/))) {
    $status = 1;
  }
  return $status;
}


sub modifyOrbRun {
  my($line) = shift;
  if (defined $orbManager && $line =~ /$orbManager/ &&
      $line =~ /(.*->run\s*\()([^\)]*)(\).*)/) {
    my($p1) = $1;
    my($p2) = $2;
    my($p3) = $3;

    $p2 =~ s/^\s+//;
    $p2 =~ s/\s+$//;

    # I am relying on convention here
    if ($p2 !~ /\s/) {
      $line = "$p1" . "TAO_TestCombinedThreads::getTimeout() " .
              "TAO_ENV_ARG_PARAMETER$p3\n";
    }
  }
  elsif ($line =~ /(.*->run\s*\()([^\)]*)(\).*)/) {
    my($p1) = $1;
    my($p2) = $2;
    my($p3) = $3;

    $p2 =~ s/^\s+//;
    $p2 =~ s/\s+$//;

    # I am relying on convention here
    if ($p2 !~ /\s/ && $p2 =~ /env/i) {
      $p3 = " TAO_ENV_ARG_PARAMETER$p3";
      $p2 = "";
    }

    if ($p2 eq "") {
      $line = $p1 . "TAO_TestCombinedThreads::getTimeout()" . "$p3\n";
    }
  }
  elsif ($line =~ /(.*\.run\s*\()([^\)]*)(\).*)/) {
    my($p1) = $1;
    my($p2) = $2;
    my($p3) = $3;

    $p2 =~ s/^\s+//;
    $p2 =~ s/\s+$//;

    # I am relying on convention here
    if ($p2 !~ /\s/ && $p2 =~ /env/i) {
      $line = "$p1" . "TAO_TestCombinedThreads::getTimeout() ".
              "TAO_ENV_ARG_PARAMETER$p3\n";
    }
  }
  return $line;
}


sub lookForActivate {
  my($line)     = shift;
  my($taskBase) = shift;
  if ($line =~ /(\w+)\.activate/) {
    $$taskBase = $1;
  }
}

my($orbInitArg) = 0;
sub lookForOrbInit {
  my($line)   = shift;
  my($status) = 0;
  if ($line =~ /CORBA::ORB_init\s*\(/) {
    $orbInitArg = 0;
    $status = 1;
  }
  return $status;
}


sub replaceOrbName {
  my($line) = shift;
  if ($orbInitArg < 3) {
    if ($line =~ /TAO_ENV_ARG_PARAMETER/) {
      $line =~ s/TAO_ENV_ARG_PARAMETER/,TAO_ENV_ARG_PARAMETER/;
    }
    my($length)   = length($line);
    my($previous) = 0;
    for(my $i = 0; $i < $length; $i++) {
      my($ch) = substr($line, $i, 1);
      ## Add the substr check because the TAO_ENV_ARG_PARAMETER doesn't
      ## have a comma before it and the above search and replace doesn't
      ## work for multi-lined ORB_init's
      if ($ch eq "," || $ch eq ")" ||
          ($orbInitArg == 2 && $i == $length - 1 &&
           substr($line, $previous) !~ /^\s+$/)) {
        $orbInitArg++;
        if ($ch eq ")" && $orbInitArg == 2) {
          $orbInitArg = 3;
          $previous = $i;
        }
        if ($orbInitArg == 3) {
          my($size) = $i - $previous;
          my($part) = substr($line, $previous, $size);
          $part =~ s/^\s+//;
          $part =~ s/\s+$//;
          if ($part eq '""' || $part eq '0') {
            substr($line, $previous, $size) = " \"$name\"";
          }
          elsif ($part eq '') {
            substr($line, $previous, $size) = ", \"$name\"";
          }
          last;
        }
        $previous = $i + 1;
      }
    }
    if ($line =~ /,TAO_ENV_ARG_PARAMETER/) {
      $line =~ s/,TAO_ENV_ARG_PARAMETER/ TAO_ENV_ARG_PARAMETER/;
    }
  }
  return $line;
}


my($initChildPOAArg) = 0;
sub lookForInitChildPOA {
  my($line)   = shift;
  my($status) = 0;
  if ($line =~ /init_child_poa\s*\(/) {
    $initChildPOAArg = 0;
    $status = 1;
  }
  return $status;
}


sub replaceChildOrbName {
  my($line) = shift;
  if ($initChildPOAArg < 4) {
    if ($line =~ /TAO_ENV_ARG_PARAMETER/) {
      $line =~ s/TAO_ENV_ARG_PARAMETER/,TAO_ENV_ARG_PARAMETER/;
    }
    my($length)   = length($line);
    my($previous) = 0;
    my($replace) = " TAO_TestCombinedThreads::getRandomString(" .
                   "\"$name\").c_str()";

    for(my $i = 0; $i < $length; $i++) {
      my($ch) = substr($line, $i, 1);
      ## Add the substr check because the TAO_ENV_ARG_PARAMETER doesn't
      ## have a comma before it and the above search and replace doesn't
      ## work for multi-lined ORB_init's
      if ($ch eq "," || $ch eq ")" ||
          ($orbInitArg == 2 && $i == $length - 1 &&
           substr($line, $previous) !~ /^\s+$/)) {
        $initChildPOAArg++;
        if ($initChildPOAArg == 4) {
          my($size) = $i - $previous;
          my($part) = substr($line, $previous, $size);
          # I am relying on convention here
          if ($part !~ /env/i) {
            substr($line, $previous, $size) = $replace;
          }
          else {
            substr($line, $previous, 0) = "$replace,\n";
          }
          last;
        }
        $previous = $i + 1;
      }
    }
    if ($line =~ /,TAO_ENV_ARG_PARAMETER/) {
      $line =~ s/,TAO_ENV_ARG_PARAMETER/ TAO_ENV_ARG_PARAMETER/;
    }
  }
  return $line;
}


sub usageAndExit {
  my($str) = shift;
  if (defined $str) {
    print STDERR "$str\n";
  }
  print STDERR "Usage: " . basename($0) .
               " [-orbcore] [-unix] [-notimeout] [-main <main prefix>]\n" .
               "       <input file> <output file>\n";
  exit(1);
}


sub modifyFileAttributes {
  my($orig) = shift;
  my($new)  = shift;
  my(@buf)  = stat($orig);

  if (defined $buf[0]) {
    utime($buf[8], $buf[9] + 1, $new);
  }
}


# **************************************************************
# Main Section
# **************************************************************

my($useORBCore)  = 0;
my($useThreadM)  = 0;
my($unixDefines) = 0;
my($useTimeouts) = 1;

while(defined $ARGV[0] && $ARGV[0] =~ /^-/) {
  if ($ARGV[0] eq '-threadmanager') {
    $useThreadM = 1;
    shift;
  }
  elsif ($ARGV[0] eq '-orbcore') {
    $useORBCore = 1;
    shift;
  }
  elsif ($ARGV[0] eq '-unix') {
    $unixDefines = 1;
    shift;
  }
  elsif ($ARGV[0] eq '-main') {
    shift;
    if (defined $ARGV[0]) {
      $name = $ARGV[0];
      shift;
    }
    else {
      usageAndExit("-main requires a parameter");
    }
  }
  elsif ($ARGV[0] eq '-notimeout') {
    $useTimeouts = 0;
    shift;
  }
  else {
    usageAndExit("Unknown option: $ARGV[0]");
  }
}

## We should have an input and output file name at this point.
if ($#ARGV != 1) {
  usageAndExit();
}


# **************************************************************
# Get the basename and remove the .cpp
# We will use this as the prefix to our main function
# **************************************************************

if ($name eq "") {
  $name = basename($ARGV[0]);
  $name =~ s/\.cpp//;
  $name =~ s/\-/_/g;
}

# **************************************************************
# Read in the file and push it into an array.  Then, print it
# out when we are done modifying it.
# **************************************************************

my($status) = 0;
if (open(IN, $ARGV[0])) {
  if (open(OUT, ">$ARGV[1]")) {
    my(@lines) = ();
    my($line)  = "";
    if ($useThreadM) {
      while(<IN>) {
        $line = $_;
        $line =~ s/ACE_Thread\s*::\s*join/TAO_TestCombinedThreads::thr_join/g;
#        $line =~ s/^\s*template\s*class\s*.*;\s*$//g;
        push(@lines, $line);
      }
      unshift(@lines, "#include <tests/TestUtils/TestCombinedThreads.h>\n");
    }
    elsif ($useORBCore) {
      my($insideInstance) = 0;
      while(<IN>) {
        $line = $_;
#        $line =~ s/^\s*template\s*class\s*.*;\s*$//g;
        if ($line =~ /^TAO_ORB_Core_instance\s*\(.*\)/) {
          $insideInstance = 1;
        }
        if ($insideInstance && $line =~ /return\s+/) {
          $line = "  // Find the orb for the thread that " .
                  "registered the ORB id\n" .
                  "  TAO_ORB_Core* hack = orb_table->find(" .
                  "TAO_TestCombinedThreads::getORBId());\n" .
                  "  if (hack != 0) {\n" .
                  "    return hack;\n" .
                  "  }\n\n" . $line;
          $insideInstance = 0;
        }
        push(@lines, $line);
      }
      unshift(@lines, "#include <tests/TestUtils/TestCombinedThreads.h>\n");
    }
    else {
      my($insideComment) = 0;
      my($insideParens) = 0;
      my($scope) = 0;
      my($orbrunFound) = 0;
      my($insideORB_init) = 0;
      my($insideInitChildPOA) = 0;
      my($taskBase) = "";
      while(<IN>) {
        $line = $_;

        my($lookForClosingParens) = 1;
        ## Remove comments
        if ($line =~ /(.*)(\/\/[\/]+.*)/ || $line =~ /(.*)(\/\/.*)/) {
          my($qcount) = countChar($1, '"');
          $line = "$1";
          if (($qcount & 1) == 1) {
            $line .= $2;
          }
          $line .= "\n";
        }
        if ($line =~ /(.*)(\/\*.*\*\/)(.*)/) {
          $line = "$1$3\n";
        }
        elsif ($line =~ /(.*)(\/\*.*)/) {
          $insideComment = 1;
          $line = "$1\n";
        }
        elsif ($insideComment && $line =~ /(.*\*\/)(.*)/) {
          $insideComment = 0;
          $line = $2;
        }
        elsif ($insideComment) {
          $line = "\n";
        }

        if ($line =~ /TAO_ORB_Manager[\*\s]*\s+(\w+)/) {
          $orbManager = $1;
        }

        lookForActivate($line, \$taskBase);
        if ($taskBase ne "" && $line =~ /thr_mgr\s*\(\)\->wait\s*\(\)/) {
          $line =~ s/\->wait\s*\(.*\)/\->wait_grp \($taskBase.grp_id\(\)\)/;
        }

        $insideInitChildPOA = ($insideInitChildPOA ? 1 :
                                      lookForInitChildPOA($line));
        if ($insideInitChildPOA) {
          $line = replaceChildOrbName($line);
          if ($line =~ /\)\s*;/) {
            $insideInitChildPOA = 0;
          }
        }

        $insideORB_init = ($insideORB_init ? 1 : lookForOrbInit($line));
        if ($insideORB_init) {
          $line = replaceOrbName($line);
          if ($line =~ /\)\s*;/) {
            $insideORB_init = 0;
          }
        }


        if (!$orbrunFound && $useTimeouts) {
          if ($orbrunFound = lookForOrbRun($line)) {
            $line = modifyOrbRun($line);
          }
        }

        ## Check the scope
        if ($line =~ /{/) {
          $scope += countChar($line, '{');
        }
        ## Check for parens
        if ($line =~ /\(/) {
          $insideParens += countChar($line, '(');
          ## This takes care of things like
          ## int parse_args(...), but allows things like
          ## int parse_args(...,
          ##                ...) to pass through which is what we want
          if ($line =~ /\)/) {
            $insideParens -= countChar($line, ')');
            $lookForClosingParens = 0;
          }
        }

        if ($scope == 0) {
          ## This section is for forward declarations
          if ($line =~ /;$/) {
            my($forward_done) = 0;
            my($counter) = $#lines;
            while(!$forward_done) {
              if ($lines[$counter] =~ /,$/) {
                if ($lines[$counter] =~ /\s+\w+\s*\(.*,$/ &&
                    needsToBeStatic($lines[$counter])) {
                  $lines[$counter] = "static $lines[$counter]";
                  $forward_done = 1;
                }
              }
              else {
                $forward_done = 1;
              }
              $counter--;
            }
          }
          if (!$insideParens && needsToBeStatic($line)) {
            my($test) = $lines[$#lines];
            $test =~ s/^\s+//;
            $test =~ s/\s+$//;
            if ($test ne "static") {
              $line = "static $line";
            }
          }
        }

        if ($scope == 1 && $orbrunFound &&
            ($line =~ /\s*return/ || $line =~ /\s*ACE_RETURN/)) {
          $orbrunFound = 0;
        }

        # Going down in scope
        if ($line =~ /}/) {
          $scope -= countChar($line, '}');
          if ($orbrunFound && $scope == 1) {
            $orbrunFound = 0;
          }
        }
        if ($lookForClosingParens && $line =~ /\)/) {
          $insideParens -= countChar($line, ')');
        }

        ## Work backwards to remove the static from
        ## the main or a method body
        if ($scope == 0 &&
            ($line =~ /(main\s*(\()?)/ || $line =~ /\w+\s*::\s*\w+\s*\(/)) {
          if ($line =~ /^static\s+/) {
            $line =~ s/static\s+//;
          }
          else {
            if ($lines[$#lines] =~ /static\s+/) {
              $lines[$#lines] =~ s/static\s+//;
            }
          }
        }

        push(@lines, $line);
        if ($line =~ /(\smain\s*(\()?)/ || $line =~ /(^main\s*(\()?)/) {
          my($saved) = pop(@lines);
          if ($1 !~ /^\s*int/) {
            my($below) = pop(@lines);
            $saved = "$below$saved";
          }
          if ($unixDefines) {
            push(@lines, "#define main $name" . "_main\n\n");
          }
          else {
            push(@lines, "#define ace_main   $name" . "_main\n" .
                         "#define ace_main_i $name" . "_main_i\n\n");
          }
          push(@lines, $saved);
        }
      }
      ## Look for last include and append include statement
      for(my $i = $#lines; $i >= 0; $i--) {
        if ($lines[$i] =~ /#include\s+/) {
          $lines[$i] .= "#include <tests/" .
                        "TestUtils/TestCombinedThreads.h>\n";
          last;
        }
      }
      unshift(@lines, "// \$Id\$\n\n");
    }
    print OUT @lines;
    close(OUT);

    if ($useThreadM || $useORBCore) {
      modifyFileAttributes($ARGV[0], $ARGV[1]);
    }
  }
  else {
    print STDERR "Unable to open $ARGV[1] for output\n";
    $status = 1;
  }
  close(IN);
}
else {
  print STDERR "Unable to open $ARGV[0] for input\n";
  $status = 1;
}

exit($status);
