eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../bin';
require ACEutils;

$status  = system ($EXEPREFIX."OctetSeq$Process::EXE_EXT -n 16 -l 32 -h 512 -s 4");

exit $status;
