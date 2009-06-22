package IDLHelper;

use strict;
use CommandHelper;
our @ISA = qw(CommandHelper);

sub get_output {
  my($self, $file, $flags) = @_;
  my $tsidl = $file;
  my @out;

  if ($flags =~ /-Gxhex/ && $flags =~ /-Wb,exec_export_include=(\S*)\s/) {
    my $xhex = $1;
    push(@out, $xhex);
  }
  if ($flags =~ /-Gxhst/ && $flags =~ /-Wb,stub_export_include=(\S*)\s/) {
    my $xhex = $1;
    push(@out, $xhex);
  }
  if ($flags =~ /-Gxhsk/ && $flags =~ /-Wb,skel_export_include=(\S*)\s/) {
    my $xhex = $1;
    push(@out, $xhex);
  }
  if ($flags =~ /-Gxhsv/ && $flags =~ /-Wb,snvt_export_include=(\S*)\s/) {
    my $xhex = $1;
    push(@out, $xhex);
  }

  return \@out;
}

sub get_outputexts {
  return ['\\.h']; #this is a regexp pattern, so . gets escaped
}

1;
