package AutomakeWorkspaceHelper;

# ************************************************************
# Description   : An Automake Workspace Helper
# Author        : Chad Elliott
# Create Date   : 9/01/2004
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;
use FileHandle;
use File::Basename;

use WorkspaceHelper;

use vars qw(@ISA);
@ISA = qw(WorkspaceHelper);

# ************************************************************
# Data Section
# ************************************************************

my(%vals)  = ('ACE_ROOT'      => '$(top_srcdir)',
              'TAO_ROOT'      => '$(top_srcdir)',
              'CIAO_ROOT'     => '$(top_srcdir)',
              'ACE_BUILDDIR'  => '$(top_builddir)',
              'TAO_BUILDDIR'  => '$(top_builddir)',
              'CIAO_BUILDDIR' => '$(top_builddir)',
              'TAO_IDL'       => 'ACE_ROOT=$(ACE_ROOT) TAO_ROOT=$(TAO_ROOT) $(TAO_BUILDDIR)/TAO_IDL/tao_idl' . "\n" .
				 'TAO_IDL_DEP = $(TAO_BUILDDIR)/TAO_IDL/tao_idl' . "\n" .
                                 'TAO_IDLFLAGS = -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -I$(TAO_ROOT) -I$(srcdir) -g $(ACE_BUILDDIR)/apps/gperf/src/ace_gperf',
             );

my(%addon) = ('ACE_ROOT'     => {'CIAO_ROOT'     => '/../..',
                                 'TAO_ROOT'      => '/..',
                                 'CIAO_BUILDDIR' => '/../..',
                                 'TAO_BUILDDIR'  => '/..'},
              'ACE_BUILDDIR' => {'CIAO_ROOT'     => '/../..',
                                 'TAO_ROOT'      => '/..',
                                 'CIAO_BUILDDIR' => '/../..',
                                 'TAO_BUILDDIR'  => '/..'},
              'TAO_ROOT'     => {'CIAO_ROOT'     => '/..',
                                 'CIAO_BUILDDIR' => '/..'},
              'TAO_BUILDDIR' => {'CIAO_ROOT'     => '/..',
                                 'CIAO_BUILDDIR' => '/..'},
             );

## These are required when processing the TAO workspace
my(%libdirs) = ('libACE_SSL.la'       => 'ace/SSL',
                'libACEXML.la'        => 'ACEXML/common',
                'libACEXML_Parser.la' => 'ACEXML/parser/parser',
                'libACE_TMCast.la'    => 'protocols/ace/TMCast',
                'libACE_RMCast.la'    => 'protocols/ace/RMCast',
                'libACE_HTBP.la'      => 'protocols/ace/HTBP',
		'libACE_ETCL.la'      => 'ace/ETCL',
                'libACE_ETCL_Parser.la' => 'ace/ETCL',
                'libACE_Monitor_Control.la' => 'ace/Monitor_Control',
                'libKokyu.la'         => 'Kokyu',
               );

# ************************************************************
# Subroutine Section
# ************************************************************

sub modify_value {
  my($self)  = shift;
  my($name)  = shift;
  my($value) = shift;

  if ($name eq 'includedir') {
    ## TAO/orbsvcs and TAO are like separate projects, so first
    ## remove the TAO/orbsvcs part and if that doesn't work try
    ## removing the TAO part.  The ACE related values don't need
    ## any modification.
    if (!($value =~ s/^\/orbsvcs\/orbsvcs$/\/orbsvcs/)) {
      if (!($value =~ s/^\/TAO\/orbsvcs//)) {
        if (!($value =~ s/^\/TAO//)) {
          ## These ACE related libraries need fixing too.
          $value =~ s/^\/protocols(\/ace)/$1/;
        }
      }
    }
  }
  elsif ($name eq 'amflags') {
    if (basename(Cwd::getcwd()) eq 'TAO') {
      $value .= ' -I ../m4';
    }
  }
  elsif ($name eq 'extra') {
    if (basename(Cwd::getcwd()) eq 'TAO') {
      $value .= "EXTRA_DIST =  \\\n" .
                "  PROBLEM-REPORT-FORM \\\n" .
                "  VERSION \\\n" .
                "  COPYING.sun \\\n" .
                "  LICENSE.sun \\\n" .
                "  README.sun\n\n" .
                "release:\n" .
                "\t\$(MAKE) dist\n\n".
                "TAO_EXTRA_DIST = docs\n\n" .
                "dist-hook:\n" .
                "\t(cd \$(top_srcdir); tar cf - \$(TAO_EXTRA_DIST)) | (cd \$(distdir); \\\n" .
                "\t  tar xfBp -)\n" .
                "\tlist=`find \$(distdir) -type d -name .svn -print`; for p in \$\$list; do \\\n" .
                "\t  rm -rf \$\$p; done\n";
    }
  }

  return $value;
}

sub modify_libpath {
  my($self)    = shift;
  my($str)     = shift;
  my($reldir)  = shift;
  my($libname) = shift;

  if ($libname =~ /^lib(ace)/i) {
    if (!defined $reldir || $reldir eq '') {
      if (defined $libdirs{$libname}) {
        $reldir = $libdirs{$libname};
      }
      else {
        $reldir = 'ace';
      }
    }
    $str =~ s!$libname!\$(ACE_BUILDDIR)/$reldir/$libname!;
    return $str;
  }
  elsif ($libname =~ /^lib(kokyu\.)/i) {
    if (!defined $reldir || $reldir eq '') {
      if (defined $libdirs{$libname}) {
        $reldir = $libdirs{$libname};
      }
      else {
        $reldir = 'Kokyu';
      }
    }
    $str =~ s!$libname!\$(ACE_BUILDDIR)/$reldir/$libname!;
    return $str;
  }
  elsif ($libname =~ /^lib(tao|kokyu)/i) {
    if (defined $reldir) {
      $reldir =~ s!TAO/!!;
      $str =~ s!$libname!\$(TAO_BUILDDIR)/$reldir/$libname!;
      return $str;
    }
  }
  elsif ($libname =~ /^libciao/i) {
    if (defined $reldir) {
      $reldir =~ s!TAO/CIAO/!!;
      $str =~ s!$libname!\$(CIAO_BUILDDIR)/$reldir/$libname!;
      return $str;
    }
  }

  return undef;
}

sub write_settings {
  my($self)   = shift;
  my($wsc)    = shift;
  my($fh)     = shift;
  my(@locals) = @_;
  my($status) = 1;
  my($error)  = undef;
  my($crlf)   = $wsc->crlf();
  my($pfh)    = new FileHandle();
  my(%seen)   = ();
  my($outdir) = $wsc->get_outdir();

  foreach my $local (reverse @locals) {
    if (open($pfh, "$outdir/$local")) {
      while(<$pfh>) {
        foreach my $key (keys %vals) {
          if (/\$\($key\)/) {
            $seen{$key} = $vals{$key};
          }
        }
      }
      close($pfh);
    }
    else {
      $status = 0;
      $error  = "Unable to open $local for reading.";
    }
  }

  foreach my $key (sort keys %seen) {
    print $fh "$key = $seen{$key}";
    if (defined $addon{$key}) {
      foreach my $add (
        sort { length($addon{$key}->{$b}) <=>
               length($addon{$key}->{$a}) } keys %{$addon{$key}}) {
        if ($seen{$add}) {
          print $fh $addon{$key}->{$add};
          last;
        }
      }
    }
    print $fh $crlf;
  }

  print $fh $crlf;

  return $status, $error;
}


1;
