//
// $Id$
//
#include "Controller.h"

ACE_RCSID(LongUpcalls, Controller, "$Id$")

Controller::Controller (void)
  :  start_count_ (0)
  ,  finish_count_ (0)
{
}

void
Controller::dump_results ()
{
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->mutex_);
  if (this->start_count_ != this->finish_count_)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR - mismatched count! (%d,%d)\n",
                  this->start_count_, this->finish_count_));
    }
}

void
Controller::worker_started (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->mutex_);
  this->start_count_++;

  // ACE_DEBUG ((LM_DEBUG, "Received worker start report\n"));
}

void
Controller::worker_finished (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->mutex_);
  this->finish_count_++;

  // ACE_DEBUG ((LM_DEBUG, "Received worker completion report\n"));
}
