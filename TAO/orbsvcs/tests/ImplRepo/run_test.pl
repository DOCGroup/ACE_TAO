# $Id$
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# This is a Perl script that tests the ImplRepo

unshift @INC, '../../../../bin';
require ACEutils;

$IR = Process::Create ("../../ImplRepo_Service/ImplRepo.exe", "-ORBobjrefstyle url");

sleep $ACE::sleeptime;

system ("server -r -o svr.ior -ORBobjrefstyle url");

$SV = Process::Create ("server.exe", "-ORBobjrefstyle url");

sleep $ACE::sleeptime;

system ("client -n 2 -f svr.ior -ORBobjrefstyle url");

