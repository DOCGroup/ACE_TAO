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
BEGIN{undef $/;} s/ \*\n \* \$Id\$\n \*\n/ \*\n/smg
