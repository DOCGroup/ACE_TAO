#! /usr/bin/perl
eval '(exit $?0)' && eval 'exec perl -w -S $0 ${1+"$@"}'
    & eval 'exec perl -w -S $0 $argv:q'
    if 0;

# ******************************************************************
#      Author: Chad Elliott
#        Date: 2/16/2006
#         $Id$
# ******************************************************************

# ******************************************************************
# Pragma Section
# ******************************************************************

use strict;
use FileHandle;
use FindBin;
use File::Spec;
use File::Basename;

my $basePath = $FindBin::Bin;
if ($^O eq 'VMS') {
  $basePath = File::Spec->rel2abs(dirname($0)) if ($basePath eq '');
  $basePath = VMS::Filespec::unixify($basePath);
}
$basePath = dirname($basePath);
unshift(@INC, $basePath . '/modules');

require ProjectCreator;
require TemplateParser;

# ******************************************************************
# Data Section
# ******************************************************************

my %keywords;
my %arrow_op;
my $ifmod     = 0;
my $formod    = 0;
my $cmod      = 50;
my %keycolors = (0 => [160, 32, 240],
                 1 => [255, 50, 50],
                 2 => [50, 50, 255],
                );
my $version   = '1.3';

# ******************************************************************
# Subroutine Section
# ******************************************************************

sub setup_keywords {
  ## Get the main MPC keywords
  my $keywords = ProjectCreator::getKeywords();
  foreach my $key (keys %$keywords) {
    $keywords{$key} = 0;
  }

  ## Get the pseudo template variables
  my $pjc = new ProjectCreator();
  $keywords = $pjc->get_command_subs();
  foreach my $key (keys %$keywords) {
    $keywords{$key} = 0;
  }

  ## Get the template function names
  $keywords = TemplateParser::getKeywords();
  foreach my $key (keys %$keywords) {
    $keywords{$key} = 0;
  }

  ## Get the template parser arrow operator keys
  $keywords = TemplateParser::getArrowOp();
  foreach my $key (keys %$keywords) {
    $arrow_op{$key} = 0;
  }

  ## These TemplateParser keywords need special values so
  ## that the color coding will recognize these as different
  ## from the rest of the keywords
  foreach my $key ('if', 'else', 'endif') {
    $keywords{$key} = 1;
  }
  foreach my $key ('foreach', 'forfirst',
                   'fornotfirst', 'fornotlast', 'forlast', 'endfor') {
    $keywords{$key} = 2;
  }
}


sub convert_to_html {
  my $line = shift;
  $line =~ s/&/&amp;/g;
  $line =~ s/</&lt;/g;
  $line =~ s/>/&gt;/g;
  $line =~ s/"/&quot;/g;
  $line =~ s/ /&nbsp;/g;
  $line =~ s/\t/&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;/g;
  $line =~ s/\n/<br>/;
  return $line;
}


sub usageAndExit {
  print "highlight_template.pl v$version\n",
        "Usage: ", basename($0), " <template> [html output]\n\n",
        "This script will color highlight the template provided using\n",
        "varying colors for the different keywords, variables and text.\n",
        "Nested if's and foreach's will have slightly different colors.\n";
  exit(0);
}

# ******************************************************************
# Main Section
# ******************************************************************

my $status = 0;
my $fh     = new FileHandle();
my $input  = $ARGV[0];
my $output = $ARGV[1];

usageAndExit() if (!defined $input || $input =~ /^-/);

if (!defined $output) {
  $output = $input;
  $output =~ s/\.mpd$//;
  $output .= '.html';
}

if (open($fh, $input)) {
  setup_keywords();

  my $deftxt = 'black';
  my @codes;
  while(<$fh>) {
    my $len = length($_);
    for(my $start = 0; $start < $len;) {
      my $sindex = index($_, '<%', $start);
      if ($sindex >= 0) {
        my $left = substr($_, $start, $sindex - $start);
        if ($left ne '') {
          push(@codes, [$deftxt, $left]);
        }
        my $eindex = index($_, '%>', $sindex);
        if ($eindex >= $sindex) {
          $eindex += 2;
        }
        else {
          $eindex = $len;
        }

        my $part  = substr($_, $sindex, $eindex - $sindex);
        my $key   = substr($part, 2, length($part) - 4);
        my $name  = $key;
        my $color = 'green';
        my @entry;
        if ($key =~ /^([^\(]+)\(.*\)/) {
          $name = $1;
          if (defined $keywords{$name}) {
            @entry = @{$keycolors{$keywords{$1}}};
          }
        }
        elsif (defined $keywords{$key}) {
          @entry = @{$keycolors{$keywords{$key}}};
        }
        else {
          foreach my $ao (keys %arrow_op) {
            if ($key =~ /^$ao/) {
              @entry = @{$keycolors{$arrow_op{$ao}}};
              last;
            }
          }
        }

        if (defined $entry[0]) {
          if ($name eq 'if') {
            $ifmod++;
            $entry[0] -= ($cmod * ($ifmod - 1));
          }
          elsif ($name eq 'endif') {
            $entry[0] -= ($cmod * ($ifmod - 1));
            $ifmod-- if ($ifmod > 0);
          }
          elsif (defined $keywords{$name} &&
                 $keywords{$name} == $keywords{'if'}) {
            $entry[0] -= ($cmod * ($ifmod - 1));
          }
          elsif ($name eq 'foreach') {
            $formod++;
            $entry[2] -= ($cmod * ($formod - 1));
          }
          elsif ($name eq 'endfor') {
            $entry[2] -= ($cmod * ($formod - 1));
            $formod-- if ($formod > 0);
          }
          elsif (defined $keywords{$name} &&
                 $keywords{$name} == $keywords{'foreach'}) {
            $entry[2] -= ($cmod * ($formod - 1));
          }
          foreach my $entry (@entry) {
            $entry = 0 if ($entry < 0);
          }
          $color = '#' . sprintf("%02x%02x%02x", @entry);
        }

        push(@codes, [$color, $part]);
        $start = $eindex;
      }
      else {
        my $part = substr($_, $start, $len - $start);
        push(@codes, [$deftxt, $part]);
        $start += ($len - $start);
      }
    }
  }
  close($fh);

  if (open($fh, ">$output")) {
    print $fh "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n",
              "<html><head><title>", basename($input), "</title></head>\n",
              "<body>\n";
    foreach my $code (@codes) {
      $$code[1] = convert_to_html($$code[1]);
      my $newline = ($$code[1] =~ s/<br>//);
      print $fh ($$code[1] ne '' ?
                   "<font color=\"$$code[0]\">$$code[1]</font>" : ''),
                ($newline ? "<br>\n" : '');
    }
    print $fh "</body></html>\n";
  }
  else {
    print STDERR "ERROR: Unable to open $output for writing\n";
    ++$status;
  }
}
else {
  print STDERR "ERROR: Unable to open $input for reading\n";
  ++$status;
}

exit($status);

