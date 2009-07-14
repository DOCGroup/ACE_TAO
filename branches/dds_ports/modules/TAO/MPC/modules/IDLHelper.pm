package IDLHelper;

use strict;
use CommandHelper;
our @ISA = qw(CommandHelper);

sub get_output {
  my($self, $file, $flags) = @_;
  my $tsidl = $file;
  my @out;

  if ($flags =~ /-Gxhex/ && $flags =~ /-Wb,exec_export_include=(\S*)/) {
    push(@out, $1);
  }
  if ($flags =~ /-Gxhst/ && $flags =~ /-Wb,stub_export_include=(\S*)/) {
    push(@out, $1);
  }
  if ($flags =~ /-Gxhsk/ && $flags =~ /-Wb,skel_export_include=(\S*)/) {
    push(@out, $1);
  }
  if ($flags =~ /-Gxhsv/ && $flags =~ /-Wb,svnt_export_include=(\S*)/) {
    push(@out, $1);
  }

  return \@out;
}

sub get_outputexts {
  return ['\\.h']; #this is a regexp pattern, so . gets escaped
}

1;
