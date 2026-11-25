#!/usr/bin/env perl

use strict;
use warnings;
use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $target = PerlACE::TestTarget::create_target (1) || die "Create target failed\n";
my $test = $target->CreateProcess ('SFP_Peek_Test');
exit $test->SpawnWaitKill ($target->ProcessStartWaitInterval ());
