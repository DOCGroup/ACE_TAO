eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# ******************************************************************
#      Author: Chad Elliott (elliott_c@ociweb.com)
#        Date: 4/4/2001
#         $Id$
# Description: Generate run_test.cpp from run_test.pl for VxWorks
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

my($firstExecutable) = 1;
my($needMain)        = 0;
my($indent)          = 0;
my($pindent)         = 0;
my($taodebug)        = undef;
my(@cppheader)       = ();
my(@cppsubs)         = ();
my(@cppbody)         = ();
my(@subs)            = ();
my($timefactor)      = 2;
my($insub)           = 0;
my($indsc)           = 0;
my(%argsList)        = ();
my($shortString)     = 256;
my($unknownArgCount) = 20;
my($defaultTimeout)  = 60;
my($status)          = 0;
my($lineCount)       = 0;
my($needReadLine)    = 1;
my($needReadFile)    = 1;
my(%timeoutVars)     = ();

my($UNDEFINED_VARIABLE)    = 1;
my($UNKNOWN_VARIABLE_TYPE) = 2;
my($UNSUPPORTED_CONSTRUCT) = 3;

# ******************************************************************
# Subroutine Section
# ******************************************************************

sub generateError {
  my($error) = shift;
  my($extra) = shift;
  if ($error == $UNDEFINED_VARIABLE) {
    print STDERR "Error: Undefined variable ($extra) at line $lineCount\n";
    $status++;
  }
  elsif ($error == $UNKNOWN_VARIABLE_TYPE) {
    print STDERR "Error: Unknown variable type " .
                 "for ($extra) at line $lineCount\n";
    $status++;
  }
  elsif ($error == $UNSUPPORTED_CONSTRUCT) {
    print STDERR "Error: Unsupported construct ($extra) at line $lineCount\n";
    $status++;
  }
}


sub getIndent {
  if ($pindent != $indent) {
    my($str) = " " x ($pindent * 2);
    $pindent = $indent;
    return $str;
  }
  return " " x ($indent * 2);
}


sub concatenate {
  my($value)    = shift;
  my($length)   = length($value);
  my($indouble) = 0;
  my($insingle) = 0;

  for(my $i = 0; $i < $length; $i++) {
    my($ch) = substr($value, $i, 1);
    if (!$insingle && $ch eq '"') {
      $indouble ^= 1;
    }
    elsif (!$indouble && $ch eq '\'') {
      $insingle ^= 1;
    }
    elsif ($ch eq '.' && !$indouble && !$insingle) {
      my($rc) = undef;
      my($lc) = $i;
      for($i++; $i < $length; $i++) {
        my($a) = substr($value, $i, 1);
        if ($a !~ /\s/) {
          if ($a eq '\'' || $a eq '"') {
            $rc = $i + 1;
          }
          elsif ($a eq '$') {
            $rc = $i;
          }
          last;
        }
      }
      for($lc--; $lc >= 0; $lc--) {
        my($a) = substr($value, $lc, 1);
        if ($a !~ /\s/) {
          if ($a ne '\'' && $a ne '"') {
            $lc++;
          }
          last;
        }
      }
      if (defined $rc) {
        my($rold) = substr($value, $rc - 1, 1);
        my($lold) = substr($value, $lc, 1);

        substr($value, $lc, $rc - $lc) = "";
        $length = length($value);
        my($front) = substr($value, 0, $lc);
        my($back)  = substr($value, $lc);
        if ($lold eq '"') {
          if ($rold eq '"') {
            $indouble ^= 1;
          }
          else {
            if ($back !~ /\"/) {
              $indouble = 1;
            }
          }
        }
        elsif ($lold =~ /\s/ && $rold eq '"' && $front =~ /\"/) {
          $indouble ^= 1;
        }
        elsif ($lold eq '\'') {
          if ($rold eq '\'') {
            $insingle ^= 1;
          }
          else {
            if ($back !~ /\'/) {
              $insingle = 1;
            }
          }
        }
        elsif ($lold =~ /\s/ && $rold eq '\'' && $front =~ /\'/) {
          $insingle ^= 1;
        }

        if ($back =~ /^([\$\@][^\s;]+)/) {
          my($target) = $1;
          if ($indouble) {
            $back =~ s/[\$\@][^\s;]+/$target\"/;
          }
          elsif ($insingle) {
            $back =~ s/[\$\@][^\s;]+/$target\'/;
          }
          $value = $front . $back;
          $length++;
        }

        $i = $lc - 1;
#########################################################
# Fix for $debug = $debug . " -d";
#########################################################
        if (!$indouble && !$insingle &&
            substr($value, 0, $i) !~ /$rold/) {
          for(my $j = $i; $j >= 0; $j--) {
            my($ich) = substr($value, $j, 1);
            if ($ich eq '$') {
              substr($value, $j, 1) = "$rold\$";
              $i++;
              if ($rold eq '"') {
                $indouble ^= 1;
              }
              else {
                $insingle ^= 1;
              }
              last;
            }
          }
        }
#########################################################
      }
    }
  }

  if ($indouble || $insingle) {
    print "DEBUG1: $value\n" .
          "        indouble = $indouble insingle = $insingle\n\n";
  }

  return $value;
}


sub typeToCPPType {
  my($str) = shift;
  if ($str eq "%d") {
    return "int";
  }
  if ($str eq "%f") {
    return "float";
  }
  if ($str eq "%s") {
    return "char";
  }
  return "void";
}

my(@mainList) = ();
sub getMainList {
  if (!defined $mainList[0]) {
    my($fh) = new FileHandle();
    if (open($fh, "Makefile")) {
      my($line) = "";
      while(<$fh>) {
        $line .= $_;
        if ($line =~ /\\\s*$/) {
          $line =~ s/\\\s*$//;
        }
        else {
          if ($line =~ /BIN(_UNCHECKED|2)?\s*[\+]?=/ &&
              $line !~ /\$\(/ && $line !~ /run_test/) {
            if ($line =~ /\s*=\s*(.*)/) {
              my($bins) = $1;
              if ($bins =~ /patsubst/) {
                ## TBD: Fix for pattern substitution
              }
              else {
                foreach my $main (split(/\s+/, $bins)) {
                  $main =~ s/\-/_/g;
                  push(@mainList, $main . "_main");
                }
              }
            }
          }
          $line = "";
        }
      }
      close($fh);
    }
  }
  return @mainList;
}


my(%declared) = ("!" => "%s");
my(%decscope) = ("!" => -1);
sub addParameterToCurrentSub {
  my($var)  = shift;
  my($type) = shift;
  if (!defined $type) {
    $type = "char*";
  }

  for(my $i = $#cppsubs; $i >= 0; $i--) {
    if ($cppsubs[$i] =~ /\/\*params\*\//) {
      if ($cppsubs[$i] =~ /\(\/\*params/) {
        $cppsubs[$i] =~ s/\/\*params/$type $var \/\*params/;
      }
      else {
        $cppsubs[$i] =~ s/\/\*params/, $type $var \/\*params/;
      }
      $declared{$var} = "%s";
      $decscope{$var} = $indent;
      last;
    }
  }
}


sub getTop {
  my($os) = "OSNAME";
  my($top) = "// \$Id\$\n\n" .
             "#include <ace/OS.h>\n" .
             "#include <ace/Get_Opt.h>\n" .
             "#include <tao/debug.h>\n" .
             "#include <tests/TestUtils/TestCombinedThreads.h>\n\n" .
             "static const char* $os = \"VxWorks\";\n\n";

  $declared{$os} = "%s";
  $decscope{$os} = -1;

  foreach my $main (getMainList()) {
    $top .= "TEST_MAIN_FUNC_DECLARE($main);\n";
  }
  $top .= "\n" .
          "static int\nunlocated_main(int, char**)\n" .
          "{\n" .
          "  ACE_OS::printf(\"Translation of this module did \"\n" .
          "                 \"not work.  Please check the code.\\n\");\n" .
          "  return 0;\n" .
          "}\n\n\n" .

          "TEST_MAIN_TYPE_FUNC\n" .
          "translateStringToMainFunction(const char* string)\n" .
          "{\n" .
          "  ACE_UNUSED_ARG(string);\n";
  foreach my $main (getMainList()) {
    my($stripped) = $main;
    $stripped =~ s/_main//;
    $top .= "  if (ACE_OS_String::strcmp(string, \"$stripped\") == 0) {\n" .
            "    return $main;\n" .
            "  }\n";
  }
  $top .= "  return unlocated_main;\n" .
          "}\n\n";

  return $top;
}


sub incrementIndent {
  $pindent = $indent;
  $indent++;
}


sub decrementIndent {
  $indent--;
  foreach my $key (keys %decscope) {
    if ($decscope{$key} > $indent) {
      delete $declared{$key};
      delete $decscope{$key};
    }
  }
  $pindent = $indent;
}

sub getMainBeginning {
  return "\n\nint main(int argc, char* argv[])\n" .
         "{\n" .
         "  // Silence the compiler\n" .
         "  ACE_UNUSED_ARG(argc);\n" .
         "  ACE_UNUSED_ARG(argv);\n" .
         "  ACE_UNUSED_ARG(OSNAME);\n" .
         "  translateStringToMainFunction(\"\");\n\n";
}


sub getMainEnding {
  my($ending) = "  return 0;\n}\n\n";
  decrementIndent();
  return $ending;
}


my($currentBuild) = "";
sub buildLine {
  my($line)     = shift;
  my($length)   = length($line);
  my($previous) = "";
  my($insideDouble) = 0;
  my($insideSingle) = 0;
  my($copy)         = "";

  $currentBuild .= $line;
  for(my $i = 0; $i < $length; $i++) {
    my($ch) = substr($line, $i, 1);
    if ($previous ne '\\') {
      if (!$insideSingle && $ch eq '"') {
        $insideDouble ^= 1;
      }
      elsif (!$insideDouble && $ch eq '\'') {
        $insideSingle ^= 1;
      }
    }

    if (($ch eq ';' || $ch eq '{' || $ch eq '}') &&
        (!$insideDouble && !$insideSingle)) {
      $copy = $currentBuild;
      $currentBuild = "";
      last;
    }
    $previous = $ch;
  }

  return $copy;
}


sub stringToArray {
  my($str)      = shift;
  my(@array)    = ();
  my($length)   = length($str);
  my($previous) = 0;
  for(my $i = 0; $i < $length; $i++) {
    my($ch) = substr($str, $i, 1);
    if ($ch =~ /\s/) {
      push(@array, substr($str, $previous, $i - $previous));
      $previous = $i;
      while(substr($str, $i, 1) =~ /\s/) {
        $i++;
      }
      push(@array, substr($str, $previous, $i - $previous));
      $previous = $i;
    }
  }
  push(@array, substr($str, $previous));
  return @array;
}


sub lineNeedsToSplit {
  my($line) = shift;
  if ($line =~ /ACE_OS/) {
    if ($line =~ /(ACE_OS.*\((\w+),\s*)(.*)(\);)/ &&
        $line !~ /argv\[[^\]]+\]/) {
      return $1, $2, $3, $4;
    }
  }
  elsif ($line =~ /(.*\s+(\w+)\[.*\]\s+=\s+)(.*)(;)$/) {
    return $1, $2, $3, $4;
  }
  return undef;
}


my($genstr) = 0;
sub splitProcessArguments {
  my($av)     = shift;
  my($line)   = shift;
  my($assign) = shift;
  my($argcount) = 0;
  my(@array) = stringToArray($av);
  my($stillString) = 0;
  my($needACE) = 1;
  my($needClosing) = 0;
  my($stored) = "";

  for(my $a = 0; $a <= $#array; $a++) {
    $array[$a] =~ s/^[\"\']//;
    $array[$a] =~ s/[\"\']$//;
    if ($array[$a] =~ /\"\s*$/ || $array[$a] eq "") {
    }
    else {
      my($needQuotes) = 1;
      $argcount++;
      if ($array[$a] =~ /\$/) {
        $needQuotes = 0;
        if ($array[$a] !~ /^\$/) {
          ## Part string, part variable
          my($foo) = $array[$a];
          $foo =~ /([^\$]+)(\$\w+)/;
          my($p1) = $1;
          my($source) = $2;
          $p1 =~ s/^\"//;
          $stillString = 1;
          splice(@array, $a + 1, 1, $source);
          $array[$a] = $p1;
        }
        else {
          $stillString = 0;
          $needACE = 1;
        }
      }
      else {
        $stillString = 1;
      }
      $array[$a] =~ s/[\$\"\']//g;
      if ($needACE) {
        if ($needClosing) {
          if ($stored ne "") {
            $$line .= $stored;
            $stored = "";
          }
          $$line .= "\");\n" . (" " x (2 * $indent));
        }
        setNeedMain();
        $stored = "ACE_OS_String::strcat($assign, ";
        if ($needQuotes) {
          $stored .= "\"";
        }
        $needACE = 0;
        $needClosing = 0;
      }
      if ($stored ne "") {
        if (defined $declared{$array[$a]} &&
            $declared{$array[$a]} ne "%s") {
          my($newstr) = "char genstr$genstr" . "[64] = \"\";\n" .
                        getIndent() .
                        "ACE_OS::sprintf(genstr$genstr, \"%d\", " .
                        "$array[$a]);\n";
          $stored = "$newstr$stored";
          $array[$a] = "genstr$genstr";
          $genstr++;
        }
        $$line .= $stored;
        $stored = "";
      }
      if ($stillString) {
        $$line .= "$array[$a]";
        $needClosing = 1;
      }
      else {
        $$line .= "$array[$a]);\n" . (" " x (2 * $indent));
        if ($a != $#array) {
          setNeedMain();
          $stored = "ACE_OS_String::strcat($assign, \"";
          $stillString = 1;
          $needACE = 0;
          $needClosing = 1;
        }
        else {
          $needClosing = 0;
        }
      }
    }
  }
  if ($needClosing) {
    if ($stored ne "") {
      $$line .= $stored;
      $stored = "";
    }
    $$line .= "\");\n";
  }
  return $argcount;
}


my($gencount) = 0;
sub splitArguments {
  my($av)       = shift;
  my($line)     = shift;
  my($sep)      = shift;
  my($argcount) = 0;
  my(@array)    = ();
  if (defined $sep) {
    @array = split($sep, $av);
    for(my $i = 0; $i <= $#array; $i++) {
      $array[$i] =~ s/^\s+//;
      $array[$i] =~ s/\s+$//;
      if ($array[$i] eq "") {
        splice(@array, $i, 1);
      }
    }
  }
  else {
    @array = split(/\s+/, $av);
  }
  for(my $a = 0; $a <= $#array; $a++) {
    if ($array[$a] =~ /-ORBDebugLevel/i) {
      $a++;
      $array[$a] =~ s/[\$\"\']//g;
      if (defined $declared{$array[$a]} &&
          $declared{$array[$a]} eq "%d") {
        $taodebug = $array[$a];
      }
      elsif ($array[$a] =~ /^[-+]?\d+$/) {
        $taodebug = $array[$a];
      }
      else {
        $taodebug = "ACE_OS::atoi(\"$array[$a]\")";
      }
    }
    elsif ($array[$a] eq '"') {
    }
    else {
      my($needQuotes) = 1;
      $argcount++;
      if ($array[$a] =~ /\$/) {
        $needQuotes = 0;
        if ($array[$a] !~ /^\$/) {
          ## Part string, part variable
          my($foo) = $array[$a];
          $foo =~ /([^\$]+)\$(\w+)/;
          my($p1) = $1;
          my($source) = $2;
          $p1 =~ s/^\"//;
          my($old) = $$line;
          my($str) = "gen_partial$gencount";
          $$line = "char $str" . "[BUFSIZ] = \"$p1\";\n" .
                   (" " x (2 * $indent));
          if (defined $declared{$source} && $declared{$source} eq "%s") {
            setNeedMain();
            $$line .= "ACE_OS_String::strcat($str";
          }
          elsif (defined $declared{$source} && $declared{$source} eq "%d") {
            setNeedMain();
            $$line .= "char gen_partial" . ($gencount + 1) .
                      "[$shortString] = \"\";\n" .
                      (" " x (2 * $indent)) .
                      "ACE_OS::sprintf(gen_partial" . ($gencount + 1) .
                      ", \"%d\", $source);\n" . (" " x (2 * $indent)) .
                      "ACE_OS_String::strcat($str";
            $gencount++;
            $source = "gen_partial$gencount";
          }
          else {
            $$line .= "// *** Unknown source type for: ";
            generateError($UNKNOWN_VARIABLE_TYPE, $source);
          }
          $$line .= ", $source);\n" . (" " x (2 * $indent)) . $old;
          $array[$a] = $str;
          $gencount++;
        }
      }
      $array[$a] =~ s/[\$\"\']//g;
      if ($needQuotes) {
        $array[$a] = "\"$array[$a]\"";
      }
      $$line .= "$array[$a], ";
    }
  }
  return $argcount;
}

sub handleAssignment {
  my($line)   = shift;
  my($concat) = (/\$(\w+)\s*\.=\s*(.*);/);

  if ($line =~ /\$(\w+)\s*[\.]?=\s*(.*);/) {
    ## Regular assignment
    my($var) = $1;
    my($val) = $2;

    if ($val =~ /time/) {
      if (!defined $declared{$var}) {
        $declared{$var} = "%d";
        $decscope{$var} = $indent;
        $line = "time_t ";
      }
      else {
        $line = "";
      }
      $line .= "$var = time(NULL);";
    }
    elsif ($val =~ /^new\s+([\w:]+)\s*\((.*)\)/) {
      my($params) = $2;
      if ($1 eq "PerlACE::Process") {
        if (!defined $declared{$var}) {
          $declared{$var} = 0;
        }
        else {
          $declared{$var}++;
        }
        if ($params =~ /.*\s*(,\s*.*)?/) {
          my($mv) = undef;
          my($av) = "";
          if ($params =~ /(.*)\s*,\s*([\"\$].*)/) {
            $mv = $1;
            $av = $2;
          }
          elsif ($params =~ /(.*)/) {
            $mv = $1;
          }
#if ($av eq "") {
#  print "DEBUG4: '$mv' '$av'\n";
#}
          my($argcount) = 1;
          my($argvar) = $var;
          if ($declared{$argvar} != 0) {
            $argvar .= $declared{$argvar};
          }
          $line = "char* $argvar" . "Args[] = { ";
          if ($mv =~ /\$(\w+)/) {
            $mv = $1;
          }
          $line .= "$mv, ";
          my($arguments) = "";
          splitArguments($av, \$arguments);
          ## Determine if the result of splitArguments
          ## should be added to $argcount
          $argsList{$var} = $argcount;

          ## Is there a splitable variable in the argument list?
          my($resplit) = 1;
          if ($arguments =~ /^\w+,\s*$/ || $arguments =~ /,\s*\w+\s*,/) {
            my(@arr) = split(/\n/, $arguments);
            foreach my $test (split(",", $arr[$#arr])) {
              $test =~ s/^\s+//;
              $test =~ s/\s+$//;
              if (defined $declared{$test} && $declared{$test} eq "%s" &&
                  $test !~ /ior/ && $test !~ /conf/ && $test !~ /gen_/) {
                $resplit = 0;
                last;
              }
            }
            if (!$resplit) {
              $line =~ s/\[\]/\[$unknownArgCount\]/;
              $line .= "0 };\n";
              for(my $l = 0; $l < $#arr; $l++) {
                if ($l == 0) {
                  $line .= (" " x (2 * $indent));
                }
                $line .= "$arr[$l]\n";
              }
              $arguments = $arr[$#arr];
              $arguments =~ s/^(\w)/\$$1/;
              $arguments =~ s/,\s*(\w)/, \$$1/g;
              $line .= (" " x (2 * $indent)) .
                       handleArguments($line, $var, $arguments, ",");
            }
          }
          if ($resplit) {
            splitArguments($av, \$line);
            $line .= "0 };\n";
          }

          $line .= (" " x (2 * $indent)) .
                   ($declared{$var} > 0 ? "$var = TAO_TestCombinedThreads" :
                                          "TAO_TestCombinedThreads $var") .
                   "(" .
                   ($mv =~ /\"(.*)\"/ ? $mv = "$1" ."_main" :
                                        "translateStringToMainFunction($mv)") .
                   ", $argvar" . "Args);";
        }
        if (!$insub && $line =~ /ACE_OS/) {
          setNeedMain();
        }
      }
    }
    elsif ($val =~ /->/) {
      if (!defined $declared{$var}) {
        $declared{$var} = "%d";
        $decscope{$var} = ($firstExecutable ? -1 : $indent);
        $line = ($firstExecutable && !$insub ? "static " : "") .
                typeToCPPType($declared{$var}) . " ";
      }
      else {
        $line = "";
      }
      $line .= "$var = ";
      if ($val =~ /Spawn/) {
        my($wait) = "";
        if ($val =~ /\((.*)\)/) {
          my($tout) = $1;
          if ($tout =~ /^\d+$/ && $tout > $defaultTimeout) {
            $defaultTimeout = $tout;
          }
          $wait = "($tout * $timefactor)";
        }
        my($spvar, $spval) = handleSpawn($val);
        $line = "$spvar$spval\n" . (" " x (2 * $indent)) .
                $line . "$spvar.wait($wait);";
      }
      elsif ($val =~ /[\$]?([\w\[\]\$]+)->WaitKill/) {
        my($var)  = $1;
        my($ovar) = $var;
        my($op)   = ".";
        my($tfix) = $var;
        $var =~ s/\$//g;
        $tfix =~ s/\[.*//;
        if ($var =~ /\[/) {
          $ovar =~ s/[\[\]\$]//g;
          $op = "->";
        }
#        elsif ($declared{$tfix} != 0) {
#          $var .= $declared{$tfix};
#        }
        handleWaitKill($val, $ovar);
        $line .= "$var$op" . "wait($ovar" . "Timeout);";
      }
      elsif ($val =~ /WaitKill/) {
        handleWaitKill($val);
        $line .= "0;";
      }
    }
    elsif ($val =~ /[\"\']([^\"\']+)?[\"\']/ || $val =~ /(\w+)\[([^\]]+)\]/) {
      my($value) = "";
      my($needQuotes) = 1;
      if ($val =~ /[\"\'](.*)[\"\']/) {
# Quoted double quotes don't work with this regexp 8/21/2001
#      $val =~ /[\"\']([^\"\']+)[\"\']/) {
        $value = $1;
      }
      elsif ($val =~ /(\w+)\[([^\]]+)\]/) {
        $value = $1 . "[$2]";
        $needQuotes = 0;
      }

      my($pre) = "";
      my($post) = "";
      my($edit) = $val;
      $edit =~ s/\\\"//g;
      $edit =~ s/\\\'//g;
      my($fch) = substr($edit, 0, 1);

      if (($fch eq '"' || $fch eq '\'') && $edit =~ /$fch$/) {
      }
      elsif ($edit =~ /(\w+)\s+[\"\'][^\"\']+[\"\']/) {
        $pre = "$1 ";
      }
      if (!defined $declared{$var}) {
        $declared{$var} = "%s";
        $decscope{$var} = ($firstExecutable ? -1 : $indent);
        $line = ($firstExecutable && !$insub ? "static " : "") .
                typeToCPPType($declared{$var}) . " ";
        if ($pre ne "") {
          $post = "[BUFSIZ]";
        }
        else {
          $post = "[$shortString]";
        }
        $line .= "$var$post = ";
        if ($needQuotes) {
          $line .= "$pre\"$value\";";
        }
        else {
          if (defined $declared{$value} && $declared{$value} ne "%s") {
            $line .= "\"\"; ACE_OS::sprintf($var, \"$declared{$value}\", $value);";
          }
          else {
            $line .= "\"\"; ACE_OS_String::strcpy($var, $value);";
          }
        }
      }
      else {
        if (defined $declared{$var} && $declared{$var} eq "%d") {
          $line = "$var = ACE_OS::atoi($pre";
        }
        elsif (defined $declared{$var} && $declared{$var} eq "%f") {
          $line = "$var = ACE_OS::atof($pre";
        }
        else {
          setNeedMain();
          $line = "ACE_OS_String::str" . ($concat ? "cat" : "cpy") .
                  "($var, $pre";
        }
        if ($needQuotes) {
          $line .= "\"$value\");";
        }
        else {
          $line .= "$value);";
        }
      }

      if ($line =~ /[^\\]\$/) {
        my($left, $var, $right, $end) = lineNeedsToSplit($line);
        if (defined $left) {
          my($newStr) = "";

          setNeedMain();
          splitProcessArguments($right, \$newStr, $var);
          $line = ($left =~ /strcat/ ? "" :
                            $left . "\"\"$end\n" . (" " x (2 * $indent))) .
                  $newStr;
        }
        else {
          $line =~ s/\$//g;
        }
      }
    }
    elsif ($val =~ /^[+\-\d]+$/) {
      if (!defined $declared{$var}) {
        $declared{$var} = "%d";
        $decscope{$var} = ($firstExecutable ? -1 : $indent);
        $line = ($firstExecutable && !$insub ? "static " : "") .
                typeToCPPType($declared{$var}) . " ";
      }
      else {
        $line = "";
      }
      $line .= "$var = $val;";
    }
    elsif ($val =~ /^[+\-\d\.]+$/) {
      if (!defined $declared{$var}) {
        $declared{$var} = "%f";
        $decscope{$var} = ($firstExecutable ? -1 : $indent);
        $line = ($firstExecutable && !$insub ? "static " : "") .
                typeToCPPType($declared{$var}) . " ";
      }
      else {
        $line = "";
      }
      $line .= "$var = $val;";
    }
    elsif ($val =~ /TAO_TestCombinedThreads.*[+\-\d]+/) {
      if (!defined $declared{$var}) {
        $declared{$var} = "%d";
        $decscope{$var} = ($firstExecutable ? -1 : $indent);
        $line = ($firstExecutable && !$insub ? "static " : "") .
                typeToCPPType($declared{$var}) . " ";
      }
      else {
        $line = "";
      }
      $line .= "$var = $val;";
    }
    elsif ($val =~ /shift/) {
      $line = "// Parameters converted from: $var = $val;";
      if ($insub) {
        addParameterToCurrentSub($var);
      }
    }
    elsif ($val =~ /[+\-\d\s\w]+/) {  ## Come up with a better regexp
                                      ## for var to var assignment
      ## Strip of \* from FILE handles
      $val =~ s/\\\*//g;

      $val =~ s/\$//g;
      my($type) = "%d";
      my($vtype) = $declared{$val};

      if (!defined $vtype) {
        $vtype = $type;
      }

      if (!defined $declared{$var}) {
        $declared{$var} = $vtype;
        $decscope{$var} = ($firstExecutable ? -1 : $indent);
        $line = ($firstExecutable && !$insub ? "static " : "") .
                typeToCPPType($declared{$var}) . " ";
      }
      else {
        $line = "";
      }

      if (defined $vtype && $type ne $vtype) {
        if ($type eq "%d") {
          $line .= "$var = ";
          if ($vtype eq "%s") {
            $line .= "ACE_OS::atoi(";
          }
          elsif ($vtype eq "%f") {
            $line .= "int(";
          }
          else {
            $line .= "(";
          }
        }
        elsif ($type eq "%s") {
          $line .= "ACE_OS::sprintf($var, \"$vtype\", ";
        }
        elsif ($type eq "%f") {
          $line .= "$var = ";
          if ($vtype eq "%s") {
            $line .= "ACE_OS::atof(";
          }
          elsif ($vtype eq "%d") {
            $line .= "float(";
          }
          else {
            $line .= "(";
          }
        }
        else {
          $line .= "$var = (";
        }
      }
      else {
        $line .= "$var = ";
      }
      $line .= $val;
      if (defined $vtype && $type ne $vtype) {
        $line .= ")";
      }
      $line .= ";";
    }
    else {
      if (!$declared{$var}) {
        $declared{$var} = "%x";
        $decscope{$var} = $indent;
        $line = "// Assignment:  ";
      }
      else {
        $line = "";
      }
      $line .= "$var = $val;";
    }
  }
  elsif ($line =~ /\@(\w+)\s*=\s*(.*);/) {
    my($var) = $1;
    my($val) = $2;
    my($arrtype) = "char*";
    my($count) = 0;

    $val =~ s/^\(//;
    $val =~ s/\)$//;

    ## This is horrible code - CAE
    ## By convention only, the processes are called SV and CL.
    if ($var =~ /^[A-Z]+$/) {
      $arrtype = "TAO_TestCombinedThreads*";
    }

    if (!$declared{$var}) {
      $declared{$var} = "%s";
      $decscope{$var} = $indent;

      $line = "$arrtype $var\[\] = { ";
      foreach my $v (split(/,/, $val)) {
        $v =~ s/^\s+//;
        $v =~ s/\s+$//;
        my($embedded) = handleEmbeddedVar($v);
        if ($v ne $embedded) {
          $v = "gen_partial$gencount";
          $line = ($count != 0 ? getIndent() : "") .
                  "static char $v" . "[BUFSIZ] = \"\";\n" .
                  (" " x (2 * $indent)) .
                  "ACE_OS::sprintf($v, $embedded);\n" . $line;
          if ($count != 0) {
            $v = ", $v";
          }
          $gencount++;
        }
        else {
          if ($count != 0) {
            $line .= getIndent();
          }
        }
        ##
        $line .= $v;
        $count++;
      }
      if ($count == 0) {
        $line = "$arrtype $var" . "[256] = { 0 };";
        $declared{$var} = "%x";
      }
      else {
        $line .= " };\n" .
                 (" " x (2 * $indent)) .
                 "int $var" . "_length = $count;\n" .
                 (" " x (2 * $indent)) .
                 "ACE_UNUSED_ARG($var" . "_length);";
        $declared{$var . "_length"} = "%d";
        $decscope{$var . "_length"} = $indent;
      }
    }
    else {
      $line = "";
      foreach my $v (split(/,/, $val)) {
        $v =~ s/^\s+//;
        $v =~ s/\s+$//;
        my($embedded) = handleEmbeddedVar($v);
        if ($v ne $embedded) {
          $v = "gen_partial$gencount";
          $line = ($count != 0 ? getIndent() : "") .
                  "static char $v" . "[BUFSIZ] = \"\";\n" .
                  (" " x (2 * $indent)) .
                  "ACE_OS::sprintf($v, $embedded);\n" . $line;
          $gencount++;
        }
        if ($count != 0) {
          $line .= getIndent();
        }
        ##
        $line .= $var . "[$count] = $v;\n";
        $count++;
      }
    }
  }
  elsif ($line =~ /\$(\w+)\s*(\[[^\]]+\])\s*([\.]?=)\s*(.*);/) {
    my($name)  = $1;
    my($index) = $2;
    my($op)    = $3;
    my($value) = $4;
    my($fake)  = handleAssignment("\$$name $op $value;");

    $index =~ s/\$//g;
    if ($fake =~ /TAO_TestCombinedThreads/) {
      $line = $fake;
      $line =~ s/\w+\s+=/$name$index = new/;
      ## Need to reset this because of the above call to handleAssignemnt
      $declared{$name} = "%x";
    }
    elsif ($fake =~ /$name[\s,=]+(.*)[\);]+/) {
      $value = $1;
      $value =~ s/\)$//;
      $line = "$name$index $op $value;";
    }
    else {
      $line = "// Unconverted Assignment: $line";
      generateError($UNSUPPORTED_CONSTRUCT, "assignment");
    }
  }
  else {
    $line = "// Unsupported Assignment: $line";
    generateError($UNSUPPORTED_CONSTRUCT, "assignment");
  }
  return $line;
}


sub setNeedMain {
  if ($firstExecutable && !$insub) {
    $needMain = 1;
    $firstExecutable = 0;
    incrementIndent();
    ## Fix up so the first line in main() is properly indented
    $pindent = $indent;
  }
}


my($foreachcount) = 0;
sub handleForeach {
  my($line) = shift;
  setNeedMain();

  incrementIndent();

  my($newline) = "{ // Unsupported foreach";
  if ($line =~ /foreach\s+\$(\w+)\s*\((.*)\)/) {

    my($var)       = $1;
    my($var_count) = $var . "_count";
    my($arg)       = $2;

    $newline = "for(int $var_count = ";
    if ($arg =~ /\@ARGV/) {
      $newline .= "1; $var_count < argc; $var_count++) {\n" .
                  (" " x (2 * $indent)) .
                  "char* $var = argv[$var_count];";
      $declared{$var} = "%s";
      $decscope{$var} = $indent;
    }
    else {
      $arg =~ s/\@//;
      if ($arg !~ /^\w+$/) {
        my($arrtype) = "char*";
        my($avar) = "gen_foreach$foreachcount";
        $foreachcount++;
        if (!$declared{$avar}) {
          $declared{$avar} = "%s";
          $decscope{$avar} = $indent;
        }
        my($dcount) = $arg;
        $dcount =~ s/[^,]//g;
        $newline = "$arrtype $avar\[\] = { $arg };\n" .
                   (" " x (2 * ($indent - 1))) . "int $avar" .
                   "_length = " . (length($dcount) + 1) . ";\n" .
                   (" " x (2 * ($indent - 1))) . $newline;
        $arg = $avar;
      }
      $newline .= "0; $var_count < $arg" . "_length; $var_count++) {\n" .
                  (" " x (2 * $indent)) .
                  "char* $var = $arg" . "[$var_count];";
      $declared{$var} = "%s";
      $decscope{$var} = $indent;
    }
  }
  else {
    generateError($UNSUPPORTED_CONSTRUCT, "foreach");
  }

  return $newline;
}


sub handleFor {
  my($line) = shift;
  setNeedMain();

  my($newline) = "{ // Unsupported for";
  if ($line =~ /for\s*\(\$(\w+)/) {
    $newline = $line;
    if (!defined $declared{$1}) {
      $declared{$1} = "%d";
      $decscope{$1} = $indent;
      $newline = typeToCPPType($declared{$1}) .
                 " $1;\n" . (" " x (2 * $indent)) . $newline;
    }
    $newline =~ s/\$//g;
  }
  else {
    generateError($UNSUPPORTED_CONSTRUCT, "for");
  }

  if ($newline =~ /argc/ && $newline =~ /=\s*0\s*;/) {
    $newline =~ s/=\s*0\s*;/= 1;/;
  }

  incrementIndent();

  return $newline;
}


sub convertFILEtoInt {
  my($name) = shift;
  $name =~ s/STDIN/fileno(stdin)/g;
  $name =~ s/STDOUT/fileno(stdout)/g;
  $name =~ s/STDERR/fileno(stderr)/g;
  return $name;
}

my($genopen) = 0;
sub handleOpen {
  my($line)   = shift;
  my($fromIf) = shift;
  my($above)  = undef;
  $fromIf = (defined $fromIf ? $fromIf : 0);

  if ($line =~ /(.*(!)?)open\s*\((.*),\s*(.*)\)(.*)/) {
    my($before) = $1;
    my($fh)     = $3;
    my($file)   = $4;
    my($after)  = $5;
    my($newline) = "";

    $fh = convertFILEtoInt($fh);
    ## Dup'ing
    if ($file =~ /\"\>\&(.*)\"/) {
      my($other) = convertFILEtoInt($1);
      if ($fh =~ /fileno/) {
        $newline = "ACE_OS::dup2($fh, $other)";
      }
      else {
        if (!defined $declared{$fh}) {
          $declared{$fh} = "%d";
          $decscope{$fh} = $indent - ($fromIf ? 1 : 0);
          $newline = "int ";
        }
        $newline .= "$fh = ACE_OS::dup($other)";
      }
    }
    ## Regular open (write)
    elsif ($file =~ /\">(.*)\"/) {
      my($other) = $1;
      $other =~ s/\$//g;
      my($name) = "genopen$genopen";
      if ($fh =~ /fileno/) {
        $genopen++;
        $newline = "ACE_OS::dup2($fh, $name)";
      }
      else {
        $name = $fh;
        $newline = $name;
      }
      $above = (defined $declared{$name} ? "" : "int ") .
               "$name = " .
               "ACE_OS::open($other, O_WRONLY|O_CREAT, " .
               "S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);\n";
      if (!defined $declared{$name}) {
        $declared{$name} = "%d";
        $decscope{$name} = $indent - ($fromIf ? 1 : 0);
      }
    }
    ## Regular open (read)
    elsif ($file =~ /(.*)/) {
      my($other) = $1;
      $other =~ s/\$//g;
      $other =~ s/[\"\']//g;
      $other =~ s/\<//g;
      my($name) = $fh;
      $newline = $name;
      $above = (defined $declared{$name} ? "" : "int ") .
               "$name = " .
               "ACE_OS::open($other, O_RDONLY);\n";
      if (!defined $declared{$name}) {
        $declared{$name} = "%d";
        $decscope{$name} = $indent - ($fromIf ? 1 : 0);
      }
    }
    $line = (!$fromIf && defined $above ? $above : "") .
            "$before$newline$after";
  }
  else {
    generateError($UNSUPPORTED_CONSTRUCT, "open");
  }
  return $line, $above;
}

sub handleIf {
  my($line)  = shift;
  setNeedMain();

  my($newline) = $line;
  $newline =~ s/if\s*\(.*/if \(/;

  my($conditions)  = undef;
  if ($line =~ /if\s*\((.+)\)\s*{/) {
    $conditions = $1;
  }

  if ($line =~ /elsif/) {
    $newline =~ s/elsif/else if/;
  }
  if ($line !~ /}.*{/) {
    incrementIndent();
  }

  foreach my $inside (split(/(\&\&|\|\|)/, $conditions)) {
    if ($inside eq '&&' || $inside eq '||') {
      $newline .= " $inside ";
    }
#    elsif ($inside =~ /\$\^O\s+[nltgeq]+\s+[\"\']MSWin32[\"\']/) {
#      $newline .= "0";
#    }
    elsif ($inside =~ /open\s*\(.*\)/) {
      my($above) = undef;
      ($inside, $above) = handleOpen($inside, 1);
      if (defined $above) {
        $newline = $above . getIndent() . $newline;
      }
      $newline .= " $inside ";
    }
    elsif ($inside =~ /(.*)\s+([nltgeq]+)\s+(.*)/) {
      my($left)     = $1;
      my($op)       = $2;
      my($right)    = $3;
      my($var)      = undef;
      my($constant) = undef;

      if ($left =~ /\$(\w+)/) {
        $var = $1;
      }
      elsif ($right =~ /\$(\w+)/) {
        $var = $1;
      }
      if ($left =~ /\$?(\w+\[[^\]]+\])/) {
        $var = $1;
        $var =~ s/\$//g;
      }
      elsif ($right =~ /\$?(\w+\[[^\]]+\])/) {
        $var = $1;
        $var =~ s/\$//g;
      }

      if ($left =~ /\"[^\"]*\"/) {
        $constant = $left;
      }
      elsif ($right =~ /\"[^\"]*\"/) {
        $constant = $right;
      }
      if ($left =~ /\'([^\']*)\'/) {
        $constant = "\"$1\"";
      }
      elsif ($right =~ /\'([^\']*)\'/) {
        $constant = "\"$1\"";
      }
      if (defined $var && defined $constant) {
        my($check) = undef;
        if ($op eq 'eq') {
          $check = "== 0";
        }
        elsif ($op eq 'ne') {
          $check = "!= 0";
        }
        elsif ($op eq 'lt') {
          $check = "< 0";
        }
        elsif ($op eq 'le') {
          $check = "<= 0";
        }
        elsif ($op eq 'gt') {
          $check = "> 0";
        }
        elsif ($op eq 'ge') {
          $check = ">= 0";
        }

        $inside =~ s/.*\s+$op\s+.*/\($var != 0 && ACE_OS_String::strcmp\($var, $constant\) $check\)/;
        $newline .= "$inside";
      }
    }
    elsif ($inside =~ /([^!<=]+)\s*([!<>=]+)\s*([^!<>=]+)/) {
      my($left)     = $1;
      my($op)       = $2;
      my($right)    = $3;
      my($var)      = undef;
      my($constant) = undef;

      $left =~ s/\s+$//;
      $right =~ s/^\s+//;

      if ($left =~ /\$(\w+)/) {
        $var = $left;
        $var =~ s/\$//g;
      }
      elsif ($left =~ /\'([^\']+)\'/) {
        $var = "\"$1\"";
      }
      else {
        $var = $left;
      }

      if ($right =~ /\$(\w+)/) {
        $constant = $1;
      }
      elsif ($right =~ /\'([^\']+)\'/) {
        $constant = "\"$1\"";
      }
      else {
        $constant = $right;
      }

      if ($var =~ /Spawn/) {
        my($wait) = "";
        if ($var =~ /\((.*)\)/) {
          my($tout) = $1;
          if ($tout =~ /^\d+$/ && $tout > $defaultTimeout) {
            $defaultTimeout = $tout;
          }
          $wait = "($tout * $timefactor)";
        }
        my($spvar, $spval) = handleSpawn($var);
        $newline = "$spvar$spval\n" . (" " x (2 * ($indent - 1))) .
                $newline;
        $var = "$spvar.wait($wait)";
      }
      elsif ($var =~ /[\$]?([\w\[\]\$]+)->WaitKill/) {
        my($wvar) = $1;
        my($ovar) = $wvar;
        my($iop)   = ".";
        my($tfix) = $wvar;
        $wvar =~ s/\$//g;
        $tfix =~ s/\[.*//;
        if ($wvar =~ /\[/) {
          $ovar =~ s/[\[\]\$]//g;
          $iop = "->";
        }
#        if (defined $declared{$wvar} && $declared{$wvar} != 0) {
#          $wvar .= $declared{$wvar};
#        }
        handleWaitKill($var, $ovar);
        $var = "$wvar$iop" . "wait($ovar" . "Timeout)";
      }

      $newline .= "$var $op $constant";
    }
    elsif ($inside =~ /testGetopts/) {
      $newline .= $inside;
    }
    elsif ($inside =~ /(.*)defined\s+\$([^\)]+)/) {
      my($op)  = $1;
      my($var) = $2;
      $newline .= ($op ne "" ? "$op(" : "") .
                  $var . "[0] != '\\0'" . ($op ne "" ? ")" : "");
    }
    elsif ($inside =~ /(!)?(.*)/) {
      my($op) = $1;
      my($right) = $2;
      $right =~ s/\$//g;
      $inside = (defined $op ? $op : "") . $right;
      $newline .= "$inside";
    }
    else {
      $newline .= "/*$inside*/0";
    }
  }
  $newline .= ") {";

  return $newline;
}


my($argindexcount) = 0;
my($azlengthcount) = 0;
my($genargcount)   = 0;
sub handleArguments {
  my($line) = shift;
  my($var)  = shift;
  my($args) = shift;
  my($sep)  = shift;
  my($newline) = "// Arguments: $line";
  my($ok) = 0;

  if (defined $var && defined $args) {
    $ok = 1;
  }
  elsif ($line =~ /\$(\w+)->Arguments\s*\((.*)\)/) {
    $ok = 1;
    $var = $1;
    $args = $2;
  }

  if ($ok) {
    my($argline) = "";
    my($argcount) = splitArguments($args, \$argline, $sep);
    $argline .= "0";
    my(@lines) = split(/\n/, $argline);
    $newline = "int argindex$argindexcount = " .
               (defined $argsList{$var} ? "$argsList{$var}" :
                                         "1 /*undefined*/") . ";\n";
    for(my $i = 0; $i <= $#lines; $i++) {
      if ($i == $#lines) {
        my($current) = 0;
        my($varargs) = $var . "Args";
        foreach my $newarg (split(/,/, $lines[$i])) {
          $newarg =~ s/^\s+//;
          $newarg =~ s/\s+$//;
          if (defined $declared{$newarg} && $declared{$newarg} eq "%s" &&
              $newarg !~ /ior/ && $newarg !~ /conf/ && $newarg !~ /gen_/) {
            $current = $unknownArgCount;
            $newline .= (" " x (2 * $indent)) . $varargs .
                        "[argindex$argindexcount" . "++] = $newarg;\n" .
                        (" " x (2 * $indent)) . "int azlength$azlengthcount = ACE_OS::strlen($newarg);\n" .
                        (" " x (2 * $indent)) . "for(int az = 0; az < azlength$azlengthcount; az++) {\n" .
                        (" " x (2 * $indent)) . "  if (isspace($newarg" . "[az])) {\n" .
                        (" " x (2 * $indent)) . "    $newarg" . "[az] = '\\0';\n" .
                        (" " x (2 * $indent)) . "    for(;ACE_OS_String::ace_isspace($newarg" . "[az]); az++);\n" .
                        (" " x (2 * $indent)) . "    $varargs" . "[argindex$argindexcount" . "++] = $newarg + az + 1;\n" .
                        (" " x (2 * $indent)) . "  }\n" .
                        (" " x (2 * $indent)) . "}\n";
            $azlengthcount++;
          }
          elsif (defined $declared{$newarg} && $declared{$newarg} eq "%d") {
            $newline .= (" " x (2 * $indent)) .
                        "char gen_args$genargcount" . "[$shortString] = \"\";\n" .
                        (" " x (2 * $indent)) .
                        "ACE_OS::sprintf(gen_args$genargcount, \"%d\", $newarg);\n" .
                        (" " x (2 * $indent)) .
                        $varargs . "[argindex$argindexcount" .
                        "++] = gen_args$genargcount;\n";
            $genargcount++;
            $current++;
          }
          else {
            $newline .= (" " x (2 * $indent)) .
                        $varargs . "[argindex$argindexcount" .
                        "++] = $newarg;\n";
            $current++;
          }
        }
        $argindexcount++;
        my($foundInBody) = 0;
        for(my $i = $#cppbody; $i >= 0; $i--) {
          if ($cppbody[$i] =~ /char\* $varargs\[(.*)\]/) {
            my($index) = $1;
            my($max) = $argsList{$var} + $current;
            if (!defined $index || defined $index &&
                ($index eq "" || ($index =~ /^\d+$/ && $index < $max))) {
              $cppbody[$i] =~ s/$varargs\[.*\]/$varargs\[$max\]/;
              $foundInBody = 1;
              last;
            }
          }
        }
        for(my $i = $#cppheader; $i >= 0; $i--) {
          if ($cppheader[$i] =~ /char\* $varargs\[(.*)\]/) {
            my($index) = $1;
            my($max) = $argsList{$var} + $current;
            if (!defined $index || defined $index &&
                ($index eq "" || ($index =~ /^\d+$/ && $index < $max))) {
              $cppheader[$i] =~ s/$varargs\[.*\]/$varargs\[$max\]/;
              last;
            }
          }
        }
      }
      else {
        $newline .= "$lines[$i]\n";
      }
    }
  }
  return $newline;
}


sub handleWhile {
  my($line) = shift;
  generateError($UNSUPPORTED_CONSTRUCT, "while");
  return "while(0) {";
}


sub handleKeyword {
  my($key)  = shift;
  my($line) = shift;
  setNeedMain();
  $line =~ s/\$(\w+)/$1/;
  if ($line =~ /$key\s*[^(]/) {
    if ($line =~ /[\(\)]+/) {
      $line =~ s/$key\s*/ACE_OS::$key/;
    }
    else {
      $line =~ s/$key\s*/ACE_OS::$key\(/;
      $line =~ s/;/\);/;
    }
  }
  else {
    $line = "ACE_OS::$line";
  }
  return $line;
}


sub handlePrint {
  my($line) = shift;
  setNeedMain();

  my($stream) = undef;
  my($value)  = undef;
  if ($line =~ /print\s+(\w+)\s+(.+);/) {
    $stream = lc($1);
    $value = $2;
  }
  elsif ($line =~ /print\s+(.+);/) {
    $value = $1;
  }

  $value = handleEmbeddedVar($value);

  if (defined $stream) {
    $line = "ACE_OS::fprintf($stream, $value);";
  }
  else {
    $line = "ACE_OS::printf($value);";
  }

  return $line;
}


sub handleSpawn {
  my($line) = shift;
  setNeedMain();
  my(@parts) = ();
  if ($line =~ /([\w\[\$\]]+)->Spawn[\w]*\s*\(([^\)]*)\)/) {
    if (defined $taodebug) {
      my($part) = (" " x (2 * $indent)) . "TAO_debug_level = $taodebug;\n";
      if ($insub) {
        push(@cppsubs, $part);
      }
      else {
        push(@cppbody, $part);
      }
      $taodebug = undef;
    }
    my($var)     = $1;
    my($timeout) = $2;
    $var =~ s/\$//g;

#    if (defined $declared{$var} && $declared{$var} != 0) {
#      $var .= $declared{$var};
#    }
    if ($timeout =~ /^\d+$/ && $timeout > $defaultTimeout) {
      $defaultTimeout = $timeout;
    }
    my($time) = ($timeout ne "" ? "($timeout * $timefactor)" : "");
    if ($time eq "") {
      my($varfix) = $var;
      $varfix =~ s/[\[\]]+//g;
      $time = "$varfix" . "Timeout";
    }
    my($typeCheck) = $var;
    $typeCheck =~ s/\[.*//;
    @parts = ($var, ($declared{$typeCheck} eq "%x" ?
                                 "->" : ".") . "run($time);");
  }
  return @parts;
}


sub handleWaitKill {
  my($line) = shift;
  my($var)  = shift;
  setNeedMain();

  if (!defined $var) {
    if ($line =~ /(\w+)->.*Kill\s*\(([\w\$]*)\)/) {
      $var = $1;
    }
  }
  if ($line =~ /->.*Kill\s*\(([\w\$]*)\)/) {
    my($time) = $var . "Timeout";
    my($tout) = $1;
    if ($tout =~ /^\d+$/ && $tout > $defaultTimeout) {
      $defaultTimeout = $tout;
    }
    if (!defined $declared{$time}) {
      $timeoutVars{$time} = $tout;
      $declared{$time} = "%d";
      $decscope{$time} = -1;
    }
  }
}


sub handleSubroutine {
  my($line) = shift;
  incrementIndent();

  if ($line =~ /sub\s+(\w+)\s*(\(.*\))?\{/) {
    $insub = 1;
    $indsc = $indent - 1;
    $line = "void $1(/*params*/) {";
    push(@subs, $1);
  }
  else {
    $line = "// Subroutine call??? $line";
  }
  return $line;
}


my($firstSplit) = 1;
sub handleSplit {
  my($line) = shift;
  if ($firstSplit) {
    $firstSplit = 0;
    push(@cppheader, "void split(char** arr, int& length, const char* delim, char* str)\n" .
                     "{\n" .
                     "  int slength     = strlen(str);\n" .
                     "  int delimLength = strlen(delim);\n" .
                     "  int previous    = 0;\n" .
                     "  int index       = 0;\n" .
                     "  for(int i = 0; i <= slength; i++) {\n" .
                     "    if (str[i] == '\\0' || strncmp(str + i, delim, delimLength) == 0) {\n" .
                     "      arr[index++] = str + previous;\n" .
                     "      str[i] = '\\0';\n" .
                     "      i += delimLength;\n" .
                     "      previous = i;\n" .
                     "    }\n" .
                     "  }\n" .
                     "  length = index;\n" .
                     "}\n" .
                     "\n" .
                     "void split(char** arr, int& length, char delim, char* str)\n" .
                     "{\n" .
                     "  char delimStr[2] = \"\\0\";\n" .
                     "  delimStr[0] = delim;\n" .
                     "  split(arr, length, delimStr, str);\n" .
                     "}\n\n");
  }

  if ($line =~ /\@(\w+)\s*=\s*split\s*\((.*)\)/) {
    my($var) = $1;
    my($params) = $2;
    $params =~ s/\$//g;
    $params =~ s/^\/([^\/]+)\//\'$1\'/;
    $line = "split($var, $var" . "_length, $params);";
  }
  else {
    $line = "// Unhandled split: $line";
  }
  return $line;
}


sub handleLocal {
  my($line) = shift;
  if ($line =~ /local\s*\((.*)\)/) {
    my($list) = $1;
    foreach my $var (split(/,/, $list)) {
      $var =~ s/^\s+//;
      $var =~ s/\s+//;
      $var = handleAssignment($var . " = \"\";");
      if ($var !~ /^static/) {
        $var = "static $var";
      }
      push(@cppheader, "$var\n");
    }
  }
}

my($firstGetopts) = 1;
sub handleGetopts {
  my($line) = shift;
  my(%vars) = ();

  $line =~ s/\'/\"/g;
  if ($line =~ /getopts\s*\(([^\)]+)\)/) {
    my($param) = $1;
    my($length) = length($param);
    my($current) = "";
    for(my $i = 0; $i < $length; $i++) {
      my($ch) = substr($param, $i, 1);
      if ($ch  ne '"') {
        if ($ch eq ':') {
          $vars{$current} = 1;
        }
        else {
          $current = $ch;
          $vars{$current} = 0;
        }
      }
    }
  }

  if ($firstGetopts) {
    push(@cppsubs, "int testGetopts(int argc, char* argv[], const char* str)\n" .
                   "{\n" .
                   "  ACE_Get_Opt get_opts(argc, argv, str);\n" .
                   "  int status = 1;\n" .
                   "  for(int c = 0; c != -1; c = get_opts()) {\n" .
                   "    switch(c) {\n" .
                   "      case 0:\n" .
                   "        break;\n");
    foreach my $key (keys %vars) {
      push(@cppsubs, "      case '$key':\n");
      if ($vars{$key} == 0) {
        push(@cppsubs, "        ACE_OS_String::strcpy(opt_$key, \"1\");");
      }
      else {
        push(@cppsubs, "        ACE_OS_String::strcpy(opt_$key, get_opts.optarg);\n");
      }
      push(@cppsubs, "        break;\n");
    }
    push(@cppsubs, "      default:\n" .
                   "        status = 0;\n" .
                   "        break;\n" .
                   "    }\n" .
                   "  }\n" .
                   "  return status;\n" .
                   "}\n\n");
  }

  $line =~ s/getopts\s*\(/testGetopts\(argc, argv, /g;
  $firstGetopts = 0;
  return $line;
}


sub handleEmbeddedVar {
  my($value)   = shift;

  while ($value =~ /\$(\w+)\[([\$]?\w+)\]/) {
    my($param) = $1;
    my($index) = $2;
    $index =~ s/\$//;
    $value =~ s/([^\\])\$\w+\[[\$]?\w+\]/$1$declared{$param}/;
    $value .= ", $param" . "[$index]";
  }
  while ($value =~ /\.\s*([\$\"\']+)/) {
    my($param) = $1;
    $param =~ s/\$//;
    $value =~ s/\.\s*[\$\"\']+/ \"%s\"/;
    $value .= ", $param";
    $value =~ s/\"\s*\.\s*\"//;
    $value =~ s/\"\s+\"//;
  }
  while ($value =~ /[^\\]\$(\w+)/) {
    my($param) = $1;

    ## Perl Script Error
    if (!defined $declared{$param}) {
      generateError($UNDEFINED_VARIABLE, $param);
      $value =~ s/([^\\])\$\w+/$1%x/;
    }
    else {
      $value =~ s/([^\\])\$\w+/$1$declared{$param}/;
    }
    $value .= ", $param";
  }
#  my($length) = length($value);
#  my($prev)   = "";
#  for(my $i = 0; $i < $length; $i++) {
#    my($ch) = substr($value, $i, 1);
#    if ($prev ne '\\' && $ch eq '$') {
#      ## TBD?
#    }
#    $prev = $ch;
#  }
  return $value;
}


my($subcallcount) = 0;
sub handleSubCall {
  my($line) = shift;
  my($newline) = "// Sub Call:";
  if ($line =~ /(\"[^\"]+\")/) {
    my($orig) = $1;
    my($str)  = handleEmbeddedVar($orig);
    if ($orig ne $str) {
      $newline = "char gen_subcall$subcallcount" . "[$shortString] = \"\";\n" .
                 (" " x (2 * $indent)) .
                 "ACE_OS::sprintf(gen_subcall$subcallcount, $str);\n";
      $line =~ s/\"[^\"]+\"/gen_subcall$subcallcount/;
      $line = $newline . (" " x (2 * $indent)) . $line;
      $subcallcount++;
    }
  }
  else {
    $line =~ s/\$//g;
  }
  return $line;
}

sub convertAngleToRead {
  my($line) = shift;
  my($length)   = length($line);
  my($indouble) = 0;
  my($insingle) = 0;
  my($inangle)  = 0;

  for(my $i = 0; $i < $length; $i++) {
    my($ch) = substr($line, $i, 1);
    if (!$insingle && $ch eq '"') {
      $indouble ^= 1;
    }
    elsif (!$indouble && $ch eq '\'') {
      $insingle ^= 1;
    }
    elsif ($ch eq '<' && !$indouble && !$insingle) {
      $inangle = $i;
    }
    elsif ($ch eq '>' && $inangle && !$indouble && !$insingle) {
      my($func) = "read_line";
      if ($line =~ /while/) {
        if ($needReadLine) {
          push(@cppsubs, "static char* read_line (int fd)\n" .
                         "{\n" .
                         "  static const size_t blen = 2048;\n" .
                         "  static char buffer[blen];\n" .
                         "\n" .
                         "  buffer[0] = '\\0';\n" .
                         "  off_t current = lseek(fd, 0, SEEK_CUR);\n" .
                         "  ssize_t amount = read (fd, buffer, blen);\n" .
                         "  if (amount > 0)\n" .
                         "    {\n" .
                         "      for(ssize_t i = 0; i < amount; i++)\n" .
                         "        {\n" .
                         "          if (buffer[i] == '\\n')\n" .
                         "            {\n" .
                         "              buffer[i + 1] = '\\0';\n" .
                         "              lseek (fd, current + i + 1, SEEK_SET);\n" .
                         "              break;\n" .
                         "            }\n" .
                         "        }\n" .
                         "    }\n" .
                         "\n" .
                         "  return buffer;\n" .
                         "}\n" .
                         "\n" .
                         "\n");
          $needReadLine = 0;
        }
      }
      else {
        $func = "read_file";
        if ($needReadFile) {
          push(@cppsubs, "static char* read_file (int fd)\n" .
                         "{\n" .
                         "  static const size_t blen = 5120;\n" .
                         "  static char buffer[blen];\n" .
                         "\n" .
                         "  buffer[0] = '\\0';\n" .
                         "  ssize_t amount = read (fd, buffer, blen);\n" .
                         "  if (amount > 0)\n" .
                         "    buffer[amount] = '\\0';\n" .
                         "\n" .
                         "  return buffer;\n" .
                         "}\n");
          $needReadFile = 0;
        }
      }
      substr($line, $i, 1) = ")";
      substr($line, $inangle, 1) = "$func(";
      $inangle = 0;
      $length = length($line);
      $i += length($func);
    }
  }

  return $line;
}


sub usageAndExit {
  my($str) = shift;
  if (defined $str) {
    print "$str\n";
  }
  print "Usage: " . basename($0) . " [-t <time factor>] <input> <output>\n\n" .
        "       -t <time factor>   The multiple to be applied to " .
        "each timeout value.\n" .
        "       <input>            Input perl script.\n" .
        "       <output>           Output c++ file.\n";
  exit(0);
}

# ******************************************************************
# Main Section
# ******************************************************************

my($ifile)   = "run_test.pl";
my($ofile)   = "-";

while(1) {
  if ($ARGV[0] eq "-h") {
    usageAndExit();
  }
  elsif ($ARGV[0] eq "-t") {
    shift(@ARGV);
    $timefactor = $ARGV[0];
    if (!defined $timefactor || $timefactor !~ /^\d+$/) {
      usageAndExit("Invalid timeout factor");
    }
  }
  elsif ($ARGV[0] =~ /^\-/) {
    usageAndExit("Unknown option: $ARGV[0]");
  }
  else {
    last;
  }
  shift(@ARGV);
}

if (defined $ARGV[0]) {
  $ifile = $ARGV[0];
}
if (defined $ARGV[1]) {
  $ofile = $ARGV[1];
}

my($fh) = new FileHandle();
if (open($fh, $ifile)) {
  my($oh) = new FileHandle();
  if (open($oh, ">$ofile")) {
    my($line)      = "";
    my($inspect)   = "";
    my($hostValue) = "localhost";

    push(@cppheader, getTop());
    while(<$fh>) {
      my($injected) = undef;
      my($output)   = undef;
      $line = $_;
      $lineCount++;

      ## For now, comment out anything using $ENV{} or
      ## PerlACE::check_n_cleanup_files
      if ($line =~ /\$ENV/ ||
          $line =~ /PerlACE::check_n_cleanup_files/) {
        $line = "## $line";
      }

      ## Convert die's
      if ($line =~ /(.*)\s+(or|and)\s+die\s*(.*);/) {
        my($left)  = $1;
        my($op)    = $2;
        my($right) = $3;
        $line = "if (" . ($op eq "or" ? "!" : "") . "$left) {\n";
        $right =~ s/\((.*)\)/$1/;
        my($space) = getIndent();
        $injected = "$space  ACE_OS::fprintf(stderr, $right);\n" .
                    "$space  exit(1);\n" .
                    "$space}\n";
      }

      ## Convert $^O to $OSNAME
      $line =~ s/\$\^O/\$OSNAME/g;

      $line =~ s/^\s+//g;
      $line =~ s/[^\$\\]#.*//g;
      $line =~ s/^#.*//;
      $line =~ s/\s+$//g;

      ## PerlACE transform
      $line =~ s/PerlACE::LocalFile\s*\((.*)\)/FULL_PATH $1/g;
      $line =~ s/PerlACE::waitforfile_timed/TAO_TestCombinedThreads::waitForFileTimed/g;
      $line =~ s/TAO_TestCombinedThreads::waitForFileTimed\s*\(([^,]+),([^\)]+)\)/TAO_TestCombinedThreads::waitForFileTimed \($1, \($2 * $timefactor\)\)/;
      $line =~ s/PerlACE::uniqueid\s*/TAO_TestCombinedThreads::getRandomPortBase/g;

      ## time transform
      $line =~ s/time/time_variable/g;

      ## main arguments transform
      $line =~ s/\$#ARGV/\(argc - 1\)/g;
      $line =~ s/\$ARGV\[([^\]]+)\]/argv\[$1\]/g;
      if ($line =~ /argv\[(\d+)\]/) {
        my($val) = $1 + 1;
        $line =~ s/argv\[\d+\]/argv\[$val\]/;
      }

      ## TARGETHOSTNAME transform
      if ($line =~ /\$TARGETHOSTNAME\s*=\s*\"(.*)\"/) {
        $hostValue = $1;
        $line = "";
      }
      $line =~ s/\$TARGETHOSTNAME/$hostValue/g;

      $inspect = concatenate(buildLine($line));

      if ($inspect =~ /getopts/) {
        $inspect = handleGetopts($inspect);
      }

      if ($inspect eq "") {
        $output = "";
      }
      elsif ($inspect =~ /^eval\s+/) {
      }
      elsif ($inspect =~ /^use\s+/) {
      }
      elsif ($inspect =~ /^require\s+/) {
      }
      elsif ($inspect =~ /split\s*\(/) {
        $output = handleSplit($inspect);
      }
      elsif ($inspect =~ /^(my\s+)?[\$\%\@]\w+(\s*\[[\$\w]+\])?\s*(\.)?=/) {
        $output = handleAssignment($inspect);
      }
      elsif ($inspect =~ /^foreach\s+/) {
        $output = handleForeach($inspect);
      }
      elsif ($inspect =~ /^for\s+/) {
        $output = handleFor($inspect);
      }
      elsif ($inspect =~ /^while\s+/) {
        $output = handleWhile($inspect);
      }
      elsif ($inspect =~ /^if\s*\(/ || $inspect =~ /[}]?\s*elsif\s*\(/) {
        $output = handleIf($inspect);
      }
      elsif ($inspect =~ /else/) {
        $output = $inspect;
        if ($inspect =~ /}/) {
          decrementIndent();
        }
        if ($inspect =~ /{/) {
          incrementIndent();
        }
      }
      elsif ($inspect =~ /}/) {
        $output = $inspect;
        decrementIndent();
      }
      elsif ($inspect =~ /\$(\w+)\+\+/) {
        $output = $inspect;
        $output =~ s/\$//g;
      }
      elsif ($inspect =~ /^exit/) {
        if ($inspect =~ /exit(\s*\(\s*\))?;/) {
          $inspect = "exit(0);";
        }
        $output = handleKeyword("exit", $inspect);
      }
      elsif ($inspect =~ /^unlink/) {
        $output = handleKeyword("unlink", $inspect);
      }
      elsif ($inspect =~ /^sleep/) {
        $output = handleKeyword("sleep", $inspect);
      }
      elsif ($inspect =~ /^close/) {
        $output = handleKeyword("close", $inspect);
      }
      elsif ($inspect =~ /^print[\s\(]/) {
        $output = handlePrint($inspect);
      }
      elsif ($inspect =~ /\->Spawn/) {
        my(@parts) = handleSpawn($inspect);
        $output = "$parts[0]$parts[1]";
      }
      elsif ($inspect =~ /->WaitKill/) {
        handleWaitKill($inspect);
      }
      elsif ($inspect =~ /->Kill/) {
        handleWaitKill($inspect);
      }
      elsif ($inspect =~ /->Arguments/) {
        $output = handleArguments($inspect);
      }
      elsif ($inspect =~ /local\s*\(/) {
        handleLocal($inspect);
      }
      elsif ($inspect =~ /sub\s+\w+\s*/) {
        $output = handleSubroutine($inspect);
      }
      elsif ($inspect =~ /open\s*\(/) {
        my(@parts) = handleOpen($inspect);
        $output = $parts[0];
      }
      else {
        if ($inspect ne "") {
          my($needError) = 1;
          $output = "// Needs to be implemented: $inspect";
          foreach my $sub (@subs) {
            if ($inspect =~ /$sub/) {
              $output = handleSubCall($inspect);
              $needError = 0;
              last;
            }
          }
          if ($needError) {
            generateError($UNSUPPORTED_CONSTRUCT, $inspect);
          }
        }
      }

      if (defined $output) {
        ## Post-processing
        $output = convertFILEtoInt($output);
        $output = convertAngleToRead($output);

        if ($needMain) {
          push(@cppbody, getMainBeginning());
          if (defined $injected) {
            push(@cppbody, $injected);
            decrementIndent();
          }
          $needMain = 0;
        }
        if ($output =~ /}\s*elsif/) {
          decrementIndent();
        }
        my($part) = ($output ne "" ? getIndent() : "") . "$output\n";
        if ($insub) {
          push(@cppsubs, $part);
          if (defined $injected) {
            push(@cppsubs, $injected);
            decrementIndent();
          }
        }
        else {
          if (!$firstExecutable) {
            push(@cppbody, $part);
            if (defined $injected) {
              push(@cppbody, $injected);
              decrementIndent();
            }
          }
          else {
            push(@cppheader, $part);
            if (defined $injected) {
              push(@cppheader, $injected);
              decrementIndent();
            }
          }
        }
        if ($insub == 1 && $indent == $indsc) {
          $insub = 0;
        }
        if ($output =~ /}\s*elsif/) {
          $indent++;
        }
      }
    }
    push(@cppbody, getMainEnding());

    ## Put in the definition of the timeout's now that
    ## the defaultTimeout is as large as it's going to get.
    foreach my $timeout (sort keys %timeoutVars) {
      my($val) = ($timeoutVars{$timeout} eq "" ? $defaultTimeout :
                                                 $timeoutVars{$timeout});
      unshift(@cppbody, "static int $timeout = ($val * $timefactor);\n");
    }

    foreach my $line (@cppheader, @cppsubs, @cppbody) {
      print $oh $line;
    }
  }
  else {
    print STDERR "Unable to open $ofile for output\n";
    $status++;
  }
  close($fh);
}
else {
  print STDERR "Unable to open $ifile for input\n";
  $status++;
}

## Remove the file if there was an error
if ($status) {
  unlink($ofile);
}

exit($status);
