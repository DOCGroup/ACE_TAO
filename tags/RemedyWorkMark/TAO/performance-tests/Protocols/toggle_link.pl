eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use Time::localtime;
use Time::Local;

# amount of delay between running the servers
$sleeptime = 2;
$phasetime = 3;

$project = "pces/sctp";

$L1_DOWN = "tevc -e $project now link1 down &";
$L1_UP =   "tevc -e $project now link1 up &";
$L2_DOWN = "tevc -e $project now link2 down &";
$L2_UP =   "tevc -e $project now link2 up &";

$now = localtime;
$start_seconds = timelocal ($now->sec, $now->min, $now->hour, $now->mday, $now->mon, $now->year);

print_with_time ("L1 UP");
system ($L1_UP);

print_with_time ("L2 DOWN");
system ($L2_DOWN);

select undef, undef, undef, $phasetime;

for (;;)
{
    print_with_time ("L2 UP");

    system ($L2_UP);

    select undef, undef, undef, $sleeptime / 2;

    print_with_time ("L1 DOWN");

    system ($L1_DOWN);

    select undef, undef, undef, $sleeptime;

    print_with_time ("L1 UP");

    system ($L1_UP);

    select undef, undef, undef, $sleeptime / 2;

    print_with_time ("L2 Down");

    system ($L2_DOWN);

    select undef, undef, undef, $sleeptime;
}

sub print_with_time
{
    $now = localtime;
    $now_seconds = timelocal ($now->sec, $now->min, $now->hour, $now->mday, $now->mon, $now->year);

    print "@_ at ", $now_seconds - $start_seconds, "\n";
}
