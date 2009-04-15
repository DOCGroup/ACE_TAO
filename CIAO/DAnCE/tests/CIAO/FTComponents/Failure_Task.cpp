// -*- C++ -*-

//=============================================================================
/**
 *  @file   Failure_Task.cpp
 *
 *  $Id$
 *
 *  @author Friedhelm Wolf <fwolf@dre.vanderbilt.edu>
 */
//=============================================================================

#include "Failure_Task.h"

Failure_Task::Failure_Task (long & count)
  : condition_ (lock_),
    orb_ (CORBA::ORB::_nil ()),
    limit_ (0),
    count_ (count),
    stop_ (false)
{
}

void
Failure_Task::init (CORBA::ORB_ptr orb,
                    long limit)
{
  orb_ = orb;
  limit_ = limit;
}

int
Failure_Task::svc (void)
{
  ACE_Guard <ACE_Thread_Mutex> guard (lock_);

  while (((limit_ == 0) || (count_ < limit_)) && !stop_)
    {
      condition_.wait ();
    }

  // don't call shutdown on regular stop
  orb_->shutdown (true);

  return 0;
}

void 
Failure_Task::signal (void)
{
  condition_.signal ();
}

void
Failure_Task::stop (void)
{
  stop_ = true;
  condition_.signal ();
}
