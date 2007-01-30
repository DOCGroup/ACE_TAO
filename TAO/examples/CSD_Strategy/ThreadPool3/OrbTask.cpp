// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    OrbTask.cpp
 *
 *  $Id$
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#include "OrbTask.h"

namespace { enum { MAX_ORB_TASK_WORKER_THREADS = 20 }; }


OrbTask::OrbTask(CORBA::ORB_ptr orb, unsigned num_threads)
  : orb_ (CORBA::ORB::_duplicate(orb)),
    num_threads_(num_threads)
{
}


OrbTask::~OrbTask()
{
}


int
OrbTask::open(void*)
{
  if (this->num_threads_ < 1)
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        "(%P|%t) OrbTask failed to open.  "
                        "num_threads_ (%d) is less-than 1.\n",
                        this->num_threads_),
                       -1);
    }

  if (this->num_threads_ > MAX_ORB_TASK_WORKER_THREADS)
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        "(%P|%t) OrbTask failed to open.  "
                        "num_threads_ (%d) is too large.  Max is %d.\n",
                        this->num_threads_, MAX_ORB_TASK_WORKER_THREADS),
                       -1);
    }

  if (CORBA::is_nil(this->orb_.in()))
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        "(%P|%t) OrbTask failed to open.  "
                        "ORB object reference is nil.\n"),
                        -1);
    }

  if (this->activate(THR_NEW_LWP | THR_JOINABLE, this->num_threads_) != 0)
    {
      // Assumes that when activate returns non-zero return code that
      // no threads were activated.
      ACE_ERROR_RETURN((LM_ERROR,
                        "(%P|%t) OrbTask failed to activate "
                        "(%d) worker threads.\n",
                        this->num_threads_),
                       -1);
    }

  return 0;
}


int
OrbTask::svc()
{
  try
  {
    this->orb_->run();
  }
  catch (...)
  {
    ACE_ERROR((LM_ERROR,
               "(%P|%t) Exception raised by ORB::run() method. "
               "OrbTask is stopping.\n"));
  }

  return 0;
}


int
OrbTask::close(u_long)
{
  return 0;
}
