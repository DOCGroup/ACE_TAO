# -*- perl -*-
# $Id$
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

$status = system ("EC_Basic".$Process::EXE_EXT, "");

# @@ Capture any errors from the server too.
exit $status;
