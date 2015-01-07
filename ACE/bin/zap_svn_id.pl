eval '(exit $?0)' && eval 'exec perl -pi -S $0 ${1+"$@"}'
    & eval 'exec perl -0 -S $0 $argv:q'
    if 0;

# $Id$
#
# You may want to run the "find" command with this script, which maybe
# something like this:
#
# find . -type f \( -name "*.inl" -o -name "*.h" -o -name "*.cpp" -o -name "*.mpc" -o -name "*.idl" -o -name "*.conf" \) -print | xargs $ACE_ROOT/bin/zap_svn_id.pl

# The first three lines above let this script run without specifying the
# full path to perl, as long as it is in the user's PATH.
# Taken from perlrun man page.

#undef $/; $s = <>; $s =~ s/ \*\n \* \$Id\$\n \*\n/ \*\n/m; print $s
# Replace a single doxygen style with two spaces before the Id with one space
BEGIN{undef $/;} s/ \*\n \*  \$Id\$\n \*\n/ \*\n \* \$Id\$\n \*\n/smg;
# Replace a three line doxygen style Id tag with just line *
BEGIN{undef $/;} s/ \*\n \* \$Id\$\n \*\n/ \*\n/smg;
# Replace a three line c-style id tag with an empty line below that with nothing
BEGIN{undef $/;} s/\/\/\n\/\/ \$Id\$\n\/\/\n\n//smg;
# Replace a three line c-style id tag with nothing
BEGIN{undef $/;} s/\/\/\n\/\/ \$Id\$\n\/\/\n//smg;
# Replace a two line c-style id tag with an empty line below that with nothing
BEGIN{undef $/;} s/\/\/\n\/\/ \$Id\$\n\n//smg;
# Replace a two line c-style id tag with nothing
BEGIN{undef $/;} s/\/\/\n\/\/ \$Id\$\n//smg;
# Replace a one line c-style id tag with an empty line below that with nothing
BEGIN{undef $/;} s/\/\/ \$Id\$\n\n//smg;
# Replace a one line c-style id tag with nothing
BEGIN{undef $/;} s/\/\/ \$Id\$\n//smg;
# Replace a one line c-style id tag with an empty line below that with nothing
BEGIN{undef $/;} s/\/\/\$Id\$\n\n//smg;
# Replace a one line c-style id tag with nothing
BEGIN{undef $/;} s/\/\/\$Id\$\n//smg;
# Replace a one line start C++ line with Id with
BEGIN{undef $/;} s/\/\/ -\*- C\+\+ -\*-  \$Id\$\n/\/\/ -\*- C\+\+ -\*-\n/smg;
# Replace a one line start C++ line with Id with
BEGIN{undef $/;} s/\/\/ -\*- C\+\+ -\*- \$Id\$\n/\/\/ -\*- C\+\+ -\*-\n/smg;
