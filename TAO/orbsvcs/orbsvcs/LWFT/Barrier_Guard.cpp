// -*- C++ -*-
// $Id$

#include "ace/Barrier.h"

#include "Barrier_Guard.h"

Barrier_Guard::Barrier_Guard (ACE_Barrier &barrier)
  : barrier_ (barrier)
{
}

Barrier_Guard::~Barrier_Guard (void)
{
  barrier_.wait ();
}
