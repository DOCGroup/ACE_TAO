#$Id$
# -*- perl -*-
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

unshift @INC, '../../../../bin';
require ACEutils;

$status  = system ($EXEPREFIX."FindPOA$Process::EXE_EXT");

exit $status;
