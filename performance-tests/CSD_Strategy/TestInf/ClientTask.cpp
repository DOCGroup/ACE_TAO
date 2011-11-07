// $Id$
// This may look like C, but it's really -*- C++ -*-

#include "ClientTask.h"
#include "tao/Exception.h"
#include "ace/SString.h"

ClientTask::ClientTask()
  : failure_count_(0),
    num_loops_(1)
{
}

ClientTask::~ClientTask()
{
}

void
ClientTask::add_engine(ClientEngine* engine)
{
  // Pass in false so that _add_ref() is called.
  ClientEngine_Handle engine_handle(engine,false);
  this->engines_.push_back(engine_handle);
}

void
ClientTask::num_loops(unsigned num_loops)
{
  this->num_loops_ = num_loops;
}

int
ClientTask::open(void*)
{
  size_t const num_threads = this->engines_.size();

  if (num_threads == 0)
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        "(%P|%t) ClientTask cannot activate 0 threads.\n"),
                       -1);
    }

  if (this->activate(THR_NEW_LWP | THR_JOINABLE, num_threads) != 0)
    {
      // Assumes that when activate returns non-zero return code that
      // no threads were activated.
      ACE_ERROR_RETURN((LM_ERROR,
                        "(%P|%t) ClientTask failed to activate "
                        "the %d client threads.\n", num_threads),
                       -1);
    }

  return 0;
}


int
ClientTask::svc()
{
  ClientEngine_Handle engine;
  unsigned num_loops;

  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);
    this->engines_.get(engine, this->engines_.size() - 1);
    this->engines_.pop_back();
    num_loops = this->num_loops_;
  }

  try
  {
    if (engine->execute(num_loops) == false)
      {
        ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);
        ++this->failure_count_;
      }
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception (
      "ClientTask::svc Caught exception from execute():");

    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);
    ++this->failure_count_ ;
  }
  catch (...)
  {
    ACE_ERROR((LM_ERROR,
               "(%P|%t) ClientTask::svc caught unknown (...) exception "\
               "in execute() " ));
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);
    ++this->failure_count_;
  }

  return 0;
}


int
ClientTask::close(u_long)
{
  return 0;
}


unsigned
ClientTask::failure_count() const
{
  return this->failure_count_;
}
