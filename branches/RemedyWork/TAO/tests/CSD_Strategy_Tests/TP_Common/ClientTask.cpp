// $Id$
#include "ClientTask.h"
#include "ClientEngine.h"
#include "AppShutdown.h"
#include "ace/SString.h"


ClientTask::ClientTask(bool shutdown_after_done)
: shutdown_after_done_(shutdown_after_done),
  failure_count_(0)
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


int
ClientTask::open(void*)
{
  size_t num_threads = this->engines_.size();

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

  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->lock_, 0);
    this->engines_.get(engine, this->engines_.size() - 1);
    this->engines_.pop_back();
  }

  try
  {
    bool exec_ret = engine->execute();
    if (exec_ret == false)
      {
        ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->lock_, 0);
        this->failure_count_ ++;
      }
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception (
      "ClientTask::svc Caught exception from execute():");

    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->lock_, 0);
    this->failure_count_ ++;
  }
  catch (...)
  {
    ACE_ERROR((LM_ERROR,
               "(%P|%t) ClientTask::svc caught unknown (...) exception "\
               "in execute() " ));
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->lock_, 0);
    this->failure_count_ ++;
  }

  if(this->shutdown_after_done_)
    {
      // This is used to shutdown orb for a client application
      // with an orb running.
      TheAppShutdown->client_done ();
    }

  return 0;
}

int
ClientTask::close(u_long)
{
  return 0;
}

unsigned
ClientTask::failure_count () const
{
  return this->failure_count_;
}
