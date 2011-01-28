eval '(exit $?0)' && eval 'exec perl -w -S $0 ${1+"$@"}'
    & eval 'exec perl -w -S $0 $argv:q'
    if 0;

# ******************************************************************
#      Author: Chad Elliott
#        Date: 1/24/2007
#         $Id$
# Description: This script attempts to remove the ACE related exception
#              macros from source code provided on the comand line.  It
#              is not perfect, but does handle a large number of cases.
#              You may need to hand edit files after running this
#              script.
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

my(%macros) = ('ACE_ENV_TYPE'                  => 'CORBA::Environment',
               'ACE_TRY_ENV'                   => '_ACE_CORBA_Environment_variable',
               'ACE_EXCEPTION_TYPE'            => 'CORBA::Exception',
               'ACE_DEFAULT_GET_ENV_METHOD'    => 'TAO_default_environment',
               'ACE_DECLARE_NEW_ENV'           => '',
               'ACE_DECLARE_NEW_CORBA_ENV'     => '',
               'ACE_CHECK *\(.*\)'             => '',
               'ACE_CHECK'                     => '',
               'ACE_CHECK_RETURN *\(.*\)'      => '',
               'ACE_THROW_INT *\((.*)\)'       => 'throw $1',
               'ACE_THROW *\((.*)\)'           => 'throw $1',
               'ACE_THROW_RETURN *\((.*),.+\)' => 'throw $1',
               'ACE_THROW_SPEC *\((.*)\)'      => '',
               'ACE_TRY'                       => 'try',
               'ACE_TRY_NEW_ENV'               => 'try',
               'ACE_TRY_EX *\([^\)]+\)'        => 'try',
               'ACE_TRY_CHECK'                 => '',
               'ACE_TRY_CHECK_EX *\([^\)]+\)'  => '',
               'ACE_TRY_THROW *\((.*)\)'       => 'throw $1',
               'ACE_TRY_THROW_EX *\((.*),.+\)' => 'throw $1',
               'ACE_CATCH *\((.*),(.+)\)'      => 'catch (const $1& $2)',
               'ACE_CATCHANY'                  => 'catch (const CORBA::Exception& ex)',
               'ACE_CATCHALL'                  => 'catch (...)',
               'ACE_RETHROW'                   => 'throw',
               'ACE_RE_THROW'                  => 'throw',
               'ACE_RE_THROW_EX *\(.+\)'       => 'throw',
               'ACE_ENDTRY'                    => '',
               'ACE_ENV_RAISE *\((.+)\)'       => '$1->_raise ()',
               'ACE_PRINT_TAO_EXCEPTION *\(([^,]+),(.+)\)' => '$1._tao_print_exception ($2)',
               'ACE_PRINT_EXCEPTION *\(([^,]+),(.+)\)'     => '$1._tao_print_exception ($2)',
               'TAO_INTERCEPTOR_CHECK_RETURN *\(.*\)'      => '',
               'TAO_INTERCEPTOR_CHECK'                     => '',
              );
my(@keys)   = sort { length($b) <=> length($a) } keys %macros;

# ******************************************************************
# Subroutine Section
# ******************************************************************

sub process_file {
  my($file)   = shift;
  my($fh)     = new FileHandle();
  my($status) = 0;

  if (open($fh, $file)) {
    my(@lines) = ();
    my($mod)   = undef;
    my($line)  = '';
    my($cont_until_this) = undef;
    while(<$fh>) {
      my($part) = $_;
      $part =~ s/\s+$//;

      if ($cont_until_this) {
        if ($part =~ s/^\s+// && $line =~ /[,\)]$/) {
          $part = ' ' . $part;
        }
        $line .= $part;
        if (index($part, $cont_until_this) >= 0) {
          $cont_until_this = undef;
        }
        else {
          next;
        }
      }
      else {
        $line = $part;
      }

      my($skip_blank) = undef;
      foreach my $key (@keys) {
        my($ats)    = (index($key, 'ACE_THROW_SPEC') == 0);
        my($search) = ($ats ? '))' : ';');
        my($base)   = undef;
        if ($key =~ /^([^\s]+\s\*\\\()/) {
          $base = $1;
        }

        if ($line =~ /^(\s*)?($key\s*[;]?)/) {
          my($space)  = $1;
          my($rest)   = $2;
          my($first)  = $3 || '';
          my($second) = $4 || '';
          my($val)    = $macros{$key};
          while($val =~ /\$(\d+)/) {
            my($num) = $1;
            if ($num == 1) {
              $first =~ s/^\s+//;
              $first =~ s/\s+$//;
              $val =~ s/\$1/$first/;
            }
            elsif ($num == 2) {
              $second =~ s/^\s+//;
              $second =~ s/\s+$//;
              $val =~ s/\$2/$second/;
            }
            else {
              $val =~ s/\$\d+//;
            }
          }

          $line =~ s/^(\s*)?($key\s*[;]?)//;

          ## A special concession for ACE_THROW_SPEC.  In the header
          ## we want to ensure that the semi-colon is preserved and if
          ## possible placed on the previous line.  In the source file
          ## we want the whole thing to go away.
          $val .= ';' if (($ats || $val ne '') && $rest =~ /;$/);
          if ($ats && index($val, ';') == 0 &&
              !($lines[$#lines] =~ /\/\/.*$/ ||
                $lines[$#lines] =~ /\/\*.*\*\/$/)) {
            $lines[$#lines] .= $val;
            $line = '';
          }
          else {
            $line = $space . $val . $line;
            $line =~ s/^\s+$//;
          }

          ## Fix up problems where ACE_TRY_THROW is used
          ## on a line by itself with the parenthesis following
          ## on the second line.  The parser gets confused and
          ## thinks it needs to replace the ACE_TRY
          if ($key eq 'ACE_TRY') {
            $line =~ s/try_THROW/throw/g;
          }

          my($len) = length($line);
          if ($len > 78 || $line =~ /""/) {
            my($level) = 0;
            my($indouble) = 0;
            my($pch) = '';
            my($fix_ace_text) = undef;
            for(my $i = 0; $i < $len; $i++) {
              my($ch) = substr($line, $i, 1);
              if ($ch eq '"' && $pch ne '\\') {
                $indouble ^= 1;
              }
              if (!$indouble) {
                my($nch) = substr($line, $i + 1, 1);
                if ($ch eq ',' || ($ch eq '"' && $nch eq '"') || $ch eq '(') {
                  $level++ if ($ch eq '(');
                  $fix_ace_text = (substr($line, 0, $i) =~ /ACE_TEXT\s*$/);
                  if ($nch ne ')' && !$fix_ace_text) {
                    my($add) = "\n" . $space . ('  ' x $level);
                    my($start) = 0;
                    while(substr($line, $i + 1 + $start, 1) =~ /\s/) {
                      $start++;
                    }
                    substr($line, $i + 1, $start) = $add;
                    $i += length($add) - $start;
                    $len += length($add) - $start;
                  }
                }
                elsif ($ch eq ')') {
                  $level--;
                  if ($fix_ace_text) {
                    if (substr($line, $i) =~ /ACE_TEXT/) {
                      my($add) = "\n" . $space . ('  ' x $level);
                      my($start) = 0;
                      while(substr($line, $i + 1 + $start, 1) =~ /\s/) {
                        $start++;
                      }
                      substr($line, $i + 1, $start) = $add;
                      $i += length($add) - $start;
                      $len += length($add) - $start;
                    }
                    $fix_ace_text = undef;
                  }
                }
              }
              $pch = $ch;
            }
          }

          $mod = 1;
          $skip_blank = 1;
          last;
        }
        elsif (defined $base &&
               index($line, $search) == -1 && $line =~ /^(\s*)?$base/) {
          $cont_until_this = $search;
          last;
        }
      }

      if ($line =~ s/ACE_ANY_EXCEPTION/ex/g) {
        $mod = 1;
      }
      if (!$cont_until_this) {
        if ($line =~ s/(\s*)ACE_ENV(_SINGLE)?_ARG_DECL_WITH_DEFAULTS// ||
            $line =~ s/(\s*)ACE_ENV(_SINGLE)?_ARG_DECL_NOT_USED// ||
            $line =~ s/(\s*)ACE_ENV(_SINGLE)?_ARG_DECL// ||
            $line =~ s/(\s*)ACE_ENV(_SINGLE)?_ARG_(PARAMETER|NOT_USED)//) {
          my($sp) = $1;
          if ($line =~ /^\)/) {
            if ($lines[$#lines] =~ /\/\/.*$/ ||
                $lines[$#lines] =~ /\/\*.*\*\/$/) {
              $line = $sp . $line;
            }
            else {
              $lines[$#lines] .= $line;
              $line = '';
            }
          }
          $mod = 1;
          $skip_blank = 1;
        }
        push(@lines, $line) if ($line ne '' || !$skip_blank);
      }
    }
    close($fh);

    if ($mod) {
      if (open($fh, ">$file")) {
        foreach my $line (@lines) {
          print $fh $line, "\n";
        }
        close($fh);
      }
      else {
        print "ERROR: Unable to write to $file\n";
        $status++;
      }
    }
  }
  else {
    print "ERROR: Unable to open $file\n";
    $status++;
  }

  return $status;
}

sub process {
  my($arg)    = shift;
  my($status) = 0;

  if (-d $arg) {
    my($fh)  = new FileHandle();
    if (opendir($fh, $arg)) {
      foreach my $file (grep(!/^\.\.?$/, readdir($fh))) {
        if ($file ne '.svn') {
          $status += process($arg . '/' . $file);
        }
      }
      closedir($fh);
    }
  }
  elsif ($arg =~ /\.(h|hh|hpp|hxx|hh|cpp|cxx|cc|c|C|i|ipp|inl)$/) {
    $status += process_file($arg);
  }

  return $status;
}

sub usageAndExit {
  print "Usage: ", basename($0), " [directories or files]\n\n",
        "Removes all exception related ACE macros.\n";
  exit(0);
}

# ******************************************************************
# Main Section
# ******************************************************************

my($status) = 0;

if ($#ARGV == -1) {
  usageAndExit();
}

foreach my $arg (@ARGV) {
  if (index($arg, '-') == 0) {
    usageAndExit();
  }
  else {
    $status += process($arg);
  }
}

exit($status);
