#!/bin/sh -- # -*- perl -*-
eval 'exec perl -pi.Array.$$ -S $0 ${1+"$@"}'
             if 0;

# $Id$

#
# After the 4.6.10 release the template instantiations for ACE_Array
# have changed, the class is implemented in terms of ACE_Array_Base;
# this script fixes the template instantiations if needed.
#
# It changes instantiations of:
#
# ACE_Array<T>
#
# into:
#
# ACE_Array<T>
# ACE_Array_Base<T>
#

# Notice the use of the -pi options: the while(<>) loop is implicit,
# printing the current line is also implicit as well as fixing the
# file in place.

if (m/template class\s+ACE_Array\s*<(.*)>\s*;\s*/) {
    print "template class ACE_Array_Base<", $1, ">;\n";
} elsif (m/#pragma instantiate\s+ACE_Array\s*<(.*)>\s*$/) {
    print "#pragma instantiate ACE_Array_Base<", $1, ">\n";
}

