$date = `/usr/bin/date +"%a %b %d %T %Y"`;
chop $date;
$VERSION = "VERSION";
$CHANGELOG = "ChangeLog";
$TAO_VERSION = '';
$MODNAME = 'TAO';

sub inplace {
    my($ext, @files) = @_;
    my(@nfiles) = ();
    my($nfile);
    $ext = '~' if ($ext eq '');
    foreach $file (@files) {
	$nfile = $file . $ext;
	if (rename ($file, $nfile)) {
	    push(@nfiles, $nfile);
	}
	else {
	    warn "Unable to rename $files[$i] for in-place editing: $!\n";
	}
    }
    @nfiles;
}

@inplace = inplace('.del', $VERSION, $CHANGELOG);

open (VERSION, $inplace[0])
    || die "Unable to open file $inplace[0] for inplace edit: $!\n";
open (VERSIONOUT, ">$VERSION")
    || die "Unable to open file $VERSION for writing: $!\n";

undef $version_number;
while (<VERSION>) {
    s/(TAO version \d+\.\d+\.)(\d+)/sprintf("$1%d",$2+1)/e;
    ($version_number = $_) =~ s/.*(\d+\.\d+\.\d+).*/$1/ if (!defined($version_number));
    if (s/(, released ).*/$1$date./) {
	($TAO_VERSION = $_) =~ s/^This is //;
    }
    print VERSIONOUT $_;
}

close (VERSIONOUT);
close (VERSION);


$message = $date."  ".$ENV{"SIGNATURE"}."  <".$ENV{"LOGNAME"}."\@cs.wustl.edu>\n\n\t* ".$TAO_VERSION."\n";
$message_printed = 0;

open (CHANGELOG, $inplace[1])
    || die "Unable to open file $inplace[1] for inplace edit: $!\n";
open (CHANGELOGOUT, ">$CHANGELOG")
    || die "Unable to open file $CHANGELOG for writing: $!\n";

while (<CHANGELOG>) {
    print CHANGELOGOUT "$message" if ( ! $message_printed++ );
    print CHANGELOGOUT $_;
}

close (CHANGELOG);
close (CHANGELOGOUT);

# Morph the version number
($version_tag = $MODNAME."-".$version_number) =~ s/\./_/g;

$CVSCOM = 'cvs';
$COM = qq/$CVSCOM commit -m'$TAO_VERSION' $VERSION $CHANGELOG && $CVSCOM rtag $version_tag $MODNAME/;
print $COM;
system $COM;

#unlink @inplace;


