eval '(exit $?0)' && eval 'exec perl -w -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

require "getopts.pl";
&Getopts("f:o:");

$opt_f = "Makefile" if( ! $opt_f );
$opt_o = ".depend"  if( ! $opt_o );

	# Open the Makefile that has been mangled by 'make depend'
	# and suck it into a perl array.
open(IF,"<$opt_f") || die;
@makefile = <IF>;
close(IF);

	# Now open our .depend file and a temporary Makefile.
	# We'll split the original Makefile between these two.
open(DF,">$opt_o") || die;
open(MF,">$opt_f.tmp") || die;

	# For each line we read out of the original file...
foreach (@makefile) {

	# If we're into the dependency section, write the line
	# into the .depend file.
	#
	if( $depend ) {
		print DF $_;
	}
	else {
		# If we haven't gotten to the dependency section yet
		# then see if the current line is the separator that
		# "make depend" causes to be inserted.
		#
		if( m/^\Q# DO NOT DELETE THIS LINE -- g++dep uses it.\E/ ) {

			# If so, change our "mode" and skip this line.
			++$depend;
			next;
		}

		# Also skip the "include .depend" that we insert.  If we
		# don't do this, it is possible to have a bunch of these
		# inserted into the output when we read an unmangled Makefile
		next if( m/^include $opt_o/ );

		# Print the non-dependency info to the temporary Makefile
		print MF $_;
	}
}

# Tell our new Makefile to include the dependency file
print MF "include $opt_o\n";

# Close the two output files...
close(DF);
close(MF);

# Unlink (remove) the original Makefile and rename our
# temporary file.  There's obviously room for error checking
# here but we've got the Makefile checked into some revision
# control system anyway.  Don't we?

unlink("$opt_f");
rename("$opt_f.tmp","$opt_f");

exit(0);
