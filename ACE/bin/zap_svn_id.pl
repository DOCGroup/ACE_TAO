eval '(exit $?0)' && eval 'exec perl -pi -S $0 ${1+"$@"}'
    & eval 'exec perl -0 -S $0 $argv:q'
    if 0;

# You may want to run the "find" command with this script, which maybe
# something like this:
#
# find . -type f \( -name "*.inl" -o -name "*.h" -o -name "*.cxx" -o -name "*.java" -o -name "*.l" -o -name "*.c" -o -name "*.mpd" -o -name "*.py" -o -name "*.cpp" -o -name "*.mpc" -o -name "*.idl" -o -name "*.conf" -o -name "*.mpb" -o -name "*.hpp" -o -name "*.pl"  -o -name "*.mpt"   -o -name "*.pm" -o -name "*.html" -o -name "*.xml" -o -name "*.y" \) -print | xargs $ACE_ROOT/bin/zap_svn_id.pl

# The first three lines above let this script run without specifying the
# full path to perl, as long as it is in the user's PATH.
# Taken from perlrun man page.

# Replace the old cvs-id tag with nothing
BEGIN{undef $/;} s///smg;
# Replace a single doxygen style with two spaces before the Id with one space
BEGIN{undef $/;} s/ \*\n \*  \$Id\$\n \*\n/ \*\n \* \$Id\$\n \*\n/smg;
# Replace a three line doxygen style Id tag with just line *
BEGIN{undef $/;} s/ \*\n \* \$Id\$\n \*\n/ \*\n/smg;
# Replace a three line doxygen style Id tag with just line *
BEGIN{undef $/;} s/\*\*\n \* \$Id\$\n \*\n/\*\*\n/smg;
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
BEGIN{undef $/;} s/\/\/ -\*- C\+\+ -\*-   \$Id\$\n/\/\/ -\*- C\+\+ -\*-\n/smg;
# Replace a one line start C++ line with Id with
BEGIN{undef $/;} s/\/\/ -\*- C\+\+ -\*-  \$Id\$\n/\/\/ -\*- C\+\+ -\*-\n/smg;
# Replace a one line start C++ line with Id with
BEGIN{undef $/;} s/\/\/ -\*- C\+\+ -\*- \$Id\$\n/\/\/ -\*- C\+\+ -\*-\n/smg;
# Replace a three line doxygen style Id tag with just line *
BEGIN{undef $/;} s/\/\*\n \* \$Id\$\n \*\n/\/\*\n/smg;
# Replace a one line c++-style id tag with an empty line below that with nothing
BEGIN{undef $/;} s/\/\* \$Id\$ \*\/\n\n//smg;
# Replace a one line c++-style id tag with an empty line below that with nothing
BEGIN{undef $/;} s/\/\* \$Id\$ \*\/\n//smg;
# Replace a three perl style Id tag with just line #
BEGIN{undef $/;} s/ \#\n \# \$Id\$\n \#\n/ \#\n/smg;
BEGIN{undef $/;} s/\#\n\# \$Id\$\n\#\n/\#\n/smg;
BEGIN{undef $/;} s/ \#\n \#\$Id\$\n \#\n/ \#\n/smg;
BEGIN{undef $/;} s/\#\n\#\$Id\$\n\#\n/\#\n/smg;
BEGIN{undef $/;} s/\#\$Id\$\n//smg;
BEGIN{undef $/;} s/\# \$Id\$\n//smg;
BEGIN{undef $/;} s/\$Id\$//smg;

# Trailing whitespaces
BEGIN{undef $/;} s/\#         \n//smg;
BEGIN{undef $/;} s/\*\* \n/\*\*\n/smg;
BEGIN{undef $/;} s/\/\*\n\*\*\n\*\*\n/\/\*\n/smg;
BEGIN{undef $/;} s/\/\* \n/\/\*\n/smg;
BEGIN{undef $/;} s/ \* \n/ \*\n/smg;
BEGIN{undef $/;} s/ \*  \n/ \*\n/smg;
BEGIN{undef $/;} s/\*\n\*  \n\*\n/\*\n/smg;
BEGIN{undef $/;} s/ \*\n \*\n \*\n/ \*\n/smg;
BEGIN{undef $/;} s/ \*\n \*\/\n/ \*\/\n/smg;
BEGIN{undef $/;} s/\/\/  \n/\/\/\n/smg;
BEGIN{undef $/;} s/ \*\n \*\n/ \*\n/smg;

BEGIN{undef $/;} s/\/\/\n\/\/\n/\/\/\n/smg;
BEGIN{undef $/;} s/ \n/\n/smg;

BEGIN{undef $/;} s/\/\/ cvs-id    :\n//smg;

# Empty header
BEGIN{undef $/;} s/\/\*\n \*\/\n//smg;
BEGIN{undef $/;} s/\/\*\*\n \*\/\n//smg;
BEGIN{undef $/;} s/\/\*\*\n \*\n \*\/\n//smg;
BEGIN{undef $/;} s/\/\*\*\n\*\n\*\/\n//smg;

