#!/pkg/gnu/bin/perl -pi
#
# You may want to run the "find" command with this script, which maybe something like this:
#
# find . -type f \( -name "*.i" -o -name "*.h" -o -name "*.C" -o -name "*.cc" -o -name "*.c" -o -name "*.cpp" -o -name "*.hpp" -o -name "*.ipp" \) -print | xargs $ACE_ROOT/bin/corba-boolean.perl
#
# And if your perl installation isn't in /pkg/gnu/bin/perl, 
# please make the change accordingly
#

s/CORBA::B_FALSE/CORBA::FALSE/g;
s/CORBA::B_TRUE/CORBA::TRUE/g;
