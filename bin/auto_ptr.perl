#!/usr/bin/perl -pi
#
#
# You may want to run the "find" command with this script, which maybe something like this:
#
# find . -type f \( -name "*.i" -o -name "*.h" -o -name "*.C" -o -name "*.cc" -o -name "*.c" -o -name "*.cpp" -o -name "*.hpp" -o -name "*.ipp" \) -print | xargs $ACE_ROOT/bin/auto_ptr.perl
#
# And if your perl installation isn't in /pkg/gnu/bin/perl,
# please make the change accordingly
#
#
s/__TEXT/ACE_TEXT/g;
