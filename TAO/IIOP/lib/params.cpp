#include "params.h"
#include "connect.h"
#include "objtable.h"

ROA_Parameters::ROA_Parameters()
  : using_threads_(0),
    thread_flags_(THR_NEW_LWP),
    context_p_(0),
    upcall_(0),
    forwarder_(0),
    oa_(0)
{
}

// Determine the appropriate default thread flags, based on system.
// When I put the concurrency strategy into the factory, then this will
// go away b/c the concurrency strategy will do this appropriate
// for each platform!
#  if defined(linux)
#    define ROA_DEFAULT_THREADFLAGS  (THR_DETACHED)
#  elif defined(WIN32)
#    define ROA_DEFAULT_THREADFLAGS  (THR_DETACHED)
#  elif defined(sparc)
#    define ROA_DEFAULT_THREADFLAGS  (THR_DETACHED|THR_SCOPE_PROCESS)
#  else
#    define ROA_DEFAULT_THREADFLAGS  (THR_DETACHED)
#  endif

ROA_Factory::CONCURRENCY_STRATEGY*
ROA_Factory::server_concurrency_strategy()
{
  ROA_Parameters* p = ROA_PARAMS::instance();


  if (p->using_threads())
    {
      // Set the strategy parameters
      threaded_strategy_.open(ACE_Service_Config::thr_mgr(), ROA_DEFAULT_THREADFLAGS);
      server_concurrency_strategy_ = &threaded_strategy_;
    }
  else
    {
      reactive_strategy_.open(ACE_Service_Config::reactor());
      server_concurrency_strategy_ = &reactive_strategy_;
    }

  return server_concurrency_strategy_;
}

TAO_Object_Table* ROA_Factory::objlookup_strategy()
{
  ROA_Parameters* p = ROA_PARAMS::instance();

  // Since these are dynamically created, when do they get destroyed?
  switch(p->demux_strategy())
    {
    case ROA_Parameters::TAO_LINEAR:
      this->objtable_ = new TAO_Linear_ObjTable (p->tablesize());
      break;
    case ROA_Parameters::TAO_USER_DEFINED:
      // it is assumed that the user would have used the hooks to supply a
      // user-defined instance of the object table
      ACE_ASSERT(this->objtable_ != 0);
      break;
    case ROA_Parameters::TAO_ACTIVE_DEMUX:
      break;
    case ROA_Parameters::TAO_DYNAMIC_HASH:
    default:
      this->objtable_ = new TAO_Dynamic_Hash_ObjTable (p->tablesize());
    }
  return this->objtable_;
}

void ROA_Factory::set_userdef_objtable(TAO_Object_Table *ot)
{
  this->objtable_ = ot;  // we assume ownership
}

ROA_Factory::ROA_Factory()
  : server_concurrency_strategy_(0),
    objtable_(0),
    client_concurrency_strategy_(0)
{
  // When should I do this open()?  It seems like this is way too early,
  // but doing it in the accessor for connector() seems like it would be
  // too late as well.
  connector_.open(ACE_Service_Config::reactor(),
		  &null_creation_strategy_,
		  &caching_connect_strategy_,
		  client_concurrency_strategy_());
}


#if !defined(__ACE_INLINE__)
#  include "params.i"
#endif

#if defined(ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Thread_Strategy<ROA_Handler>;
template class ACE_Concurrency_Strategy<ROA_Handler>;
template class ACE_Reactive_Strategy<ROA_Handler>;
template class ACE_Creation_Strategy<ROA_Handler>;
template class ACE_Scheduling_Strategy<ROA_Handler>;
template class ACE_Accept_Strategy<ROA_Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_Singleton<ROA_Parameters, ACE_Thread_Mutex>;
template class ACE_Singleton<ROA_Factory, ACE_Thread_Mutex>;
#endif
