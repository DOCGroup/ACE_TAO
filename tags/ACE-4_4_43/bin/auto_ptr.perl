#!/pkg/gnu/bin/perl -pi
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
s/auto_basic_ptr/ACE_Auto_Basic_Ptr/g;
s/auto_basic_array_ptr/ACE_Auto_Basic_Array_Ptr/g;
s/auto_array_ptr/ACE_Auto_Array_Ptr/g;
