eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-
# $Id$

$DESTINATION = shift;

use File::Basename;

# Compute the dependencies using lorder
@DEPS = ();
@FILES = ();

open(LORDER, "lorder ".join(' ',@ARGV)." |")
  || die "Cannot run lorder\n";
while(<LORDER>) {
  chomp;
  @files = split ' ';

  my ($source,,) = fileparse($files[0],"\.o");
  my ($target,,) = fileparse($files[1],"\.o");
  push @FILES, $source;
  push @FILES, $target;
  push @DEPS, join(':', $source, $target);
}
close (LORDER);

@DEPS = &unique(@DEPS);
@FILES = &unique(@FILES);

#foreach $i (@DEPS) {
#  print $i, "\n";
#}

my $doxfont_source = $ENV{'HOME'};
system ("cp $doxfont_source/doxfont.ttf doxfont.ttf");

foreach $file (@FILES) {
  my @dotfile = &gen_deps_dot_file ($file, '_deps', @DEPS);
  &gen_files ($file, '_deps',
	      $file.' Direct Dependencies', @dotfile);
}
my @fulldotfile = &gen_dot_file('index', "_deps", @DEPS);
&gen_files ('index', '_deps',
	    'Full Dependencies for All Files', @fulldotfile);

foreach $file (@FILES) {
  my @dotfile = &gen_closure_dot_file ($file, '_close', @DEPS);
  &gen_files ($file, '_close',
              $file.' Direct and Indirect Dependencies', @dotfile);
}

if (0) {
@fulldotfile = &gen_dot_file('AllFiles', "_close", @DEPS);
&gen_files ('AllFiles', '_close',
	    'Full Dependencies for All Files', @fulldotfile);
}

################################################################

sub unique {
  my $last = '';
  my @uniq = ();
  foreach my $i (sort @_) {
    if ($i eq $last) {
      next;
    }
    push @uniq, $i;
    $last = $i;
  }
  return @uniq;
}
  
sub deps_for_file {
  my $file = shift;

  my @deps = ();
  foreach my $i (@_) {
    if ($i =~ m/^$file\:/
	|| $i =~ m/:$file$/) {
      push @deps, $i;
    }
  }
  return @deps;
}

sub gen_deps_dot_file {
  my $file = shift;
  my $suffix = shift;

  my @related = &deps_for_file ($file, @_);

  return &gen_dot_file ($file, $suffix, @related);
}

sub file_deps {
  my $file = shift;

  my @deps = ();
  foreach my $i (@_) {
    if ($i =~ m/^$file\:/) {
      push @deps, $i;
    }
  }
  return @deps;
}

sub gen_closure_dot_file {
  my $file = shift;
  my $suffix = shift;

  my @related = &file_deps ($file, @_);

  my $count = $#related;
  for (;;) {
    my @list = @related;
    foreach my $i (@related) {
      my @f = split ':', $i;
      push @list, &file_deps ($f[0], @_);
      push @list, &file_deps ($f[1], @_);
    }
    @related = &unique (sort @list);
    if ($count == $#related) {
      last;
    }
    $count = $#related;
  }

  foreach my $i (@_) {
   if ($i =~ m/:$file$/) {
      push @related, $i;
    }
  }
  @related = &unique (sort @related);
  
  return &gen_dot_file ($file, $suffix, @related);
}

sub gen_dot_file {
  my $file = shift;
  my $suffix = shift;
  my @related = @_;

  my @dot_file = ();
  push @dot_file, ("digraph $file {\n",
		   "  size=\"50,50\";\n",
		   "  rankdir=LR;\n",
		   "  node [fontname=doxfont,shape=box,"
		   ."color=green,style=filled];\n",
		   "  $file [color=red];\n");

  my @other_files = &unique(map {split ':'} @related);

  foreach my $other (@other_files) {
    my $htmlother = $other.$suffix.".html";
    if ($other eq $file) {
      my $alt = '_deps';
      if ($suffix eq '_deps') {
	$alt = '_close';
      }
      $htmlother = $other.$alt.".html";
    }
    push @dot_file, "  $other [URL=\"$htmlother\"];\n";
  }
  foreach my $d (@related) {
    my @f = split ':', $d;
    if ($f[0] eq $f[1]) {
      next;
    }
    push @dot_file, "  ".$f[0].' -> '.$f[1].";\n";
  }
  push @dot_file, "}\n";
  return @dot_file;
}

sub gen_files {
  my $file = shift;
  my $suffix = shift;
  my $title = shift;
  my @dotfile = @_;

  my $htmlfile = $DESTINATION."/".$file.$suffix.".html";
  my $ismapfile = $DESTINATION."/".$file.$suffix.".ismap";
  my $gifbase = $file.$suffix.".gif";
  my $giffile = $DESTINATION."/".$gifbase;

  #print "######\n";
  #print @dotfile;
  #print "######\n";
  open (DOT, "|/usr/local/bin/dot -Tgif -o $giffile")
    || die "Cannot run 'dot' to generate the GIF file for $file\n";
  print DOT @dotfile;
  close(DOT)
    || die "Error when closing 'dot' for $file GIF's\n";

  open (DOT, "|/usr/local/bin/dot -Tismap -o $ismapfile")
    || die "Cannot run 'dot' to generate the ISMAP file for $file\n";
  print DOT @dotfile;
  close(DOT)
    || die "Error when closing 'dot' for $file ISMAP's\n";

  open (HTML, ">".$htmlfile)
    || die "Cannot open HTML output for $file\n";
  print HTML <<_HEADER_;
<!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML//EN">
<html>
  <head>
    <title>$title</title>
  </head>

  <body>
    <CENTER>
      <h1>$title</h1>

      <IMG SRC="$gifbase" USEMAP="#dependencies_map">
    </CENTER>
    <MAP NAME="dependencies_map">
_HEADER_

  open (ISMAP, $ismapfile)
    || die "Cannot open $file ISMAP for reading\n";
  while (<ISMAP>) {
    chomp;
    s/\(//g;
    s/\)//g;
    s/,/ /g;
    my @f = split ' ';
    print HTML "<AREA HREF=\"".$f[5]."\"\n";
    print HTML "  SHAPE=\"RECT\"\n";
    print HTML "  COORDS=\"$f[1],$f[4],$f[3],$f[2]\">\n\n";
  }
  close (ISMAP)
    || die "Error closing $file ISMAP\n";
  print HTML <<_FOOTER_;
  </MAP>
  </body>
</html>
_FOOTER_
  close(HTML)
    || die "Error closing HTML output for $file\n";
  
}
