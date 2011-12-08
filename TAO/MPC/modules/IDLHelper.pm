# $Id$

package IDLHelper;

use strict;
use CommandHelper;
our @ISA = qw(CommandHelper);

sub get_output {
  my($self, $file, $flags) = @_;
  my @out;

  # handle generated executor files (if any)
  if ($flags =~ /-Gex/ && !($flags =~ /-oN/)) {
    my $gendir = '';
    if ($flags =~ /-oE\s*(\S+)/) {
      $gendir = $1;
      if (!($gendir =~ /(\\|\/)$/)) {
        $gendir .= '/';
      }
    }
    $file =~ /^(.*)(\.p?idl)$/;
    push(@out, $gendir.$1."_exec.h");
    push(@out, $gendir.$1."_exec.cpp");
  }
  if ($flags =~ /-Gxhex/ && $flags =~ /-Wb,exec_export_include=(\S*)/) {
    push(@out, $1);
  }
  if ($flags =~ /-Gxhst/) {
    if ($flags =~ /-Wb,stub_export_file=(\S*)/) {
      push(@out, $1);
    } elsif ($flags =~ /-Wb,stub_export_include=(\S*)/) {
      push(@out, $1);
    }
  }
  if ($flags =~ /-Gxhsk/) {
    if ($flags =~ /-Wb,skel_export_file=(\S*)/) {
      push(@out, $1);
    } elsif ($flags =~ /-Wb,skel_export_include=(\S*)/) {
      push(@out, $1);
    }
  }
  if ($flags =~ /-Gxhsv/ && $flags =~ /-Wb,svnt_export_include=(\S*)/) {
    push(@out, $1);
  }
  if ($flags =~ /-Gxhcn/ && $flags =~ /-Wb,conn_export_include=(\S*)/) {
    push(@out, $1);
  }

  return \@out;
}

sub get_outputexts {
  return ['\\.h']; #this is a regexp pattern, so . gets escaped
}

1;
