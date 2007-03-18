eval '(exit $?0)' && eval 'exec perl -pi -S $0 ${1+"$@"}'
    & eval 'exec perl -pi -S $0 $argv:q'
    if 0;

# $Id$
#
# You may want to run the "find" command with this script, which maybe
# something like this:
#
# find . -type f \( -name "*.i" -o -name "*.h" -o -name "*.C" -o -name "*.cc" -o -name "*.c" -o -name "*.cpp" -o -name "*.hpp" -o -name "*.ipp" \) -print | xargs $ACE_ROOT/bin/corba-boolean.perl

# The first three lines above let this script run without specifying the
# full path to perl, as long as it is in the user's PATH.
# Taken from perlrun man page.

s/CORBA::B_FALSE/0/g;
s/CORBA::B_TRUE/1/g;
s/CORBA_B_FALSE/0/g;
s/CORBA_B_TRUE/1/g;
