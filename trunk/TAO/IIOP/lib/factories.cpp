#include "factories.h"

#if !defined(__ACE_INLINE__)
#  include "factories.i"
#endif

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

TAO_Server_Factory::CONCURRENCY_STRATEGY*
TAO_Server_Factory::server_concurrency_strategy()
{
  OA_Parameters* p = OA_PARAMS::instance();


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

TAO_Object_Table*
TAO_Server_Factory::object_lookup_strategy()
{
  OA_Parameters* p = OA_PARAMS::instance();

  // Since these are dynamically created, when do they get destroyed?
  switch(p->demux_strategy())
    {
    case OA_Parameters::TAO_LINEAR:
      this->objtable_ = new TAO_Linear_ObjTable (p->tablesize());
      break;
    case OA_Parameters::TAO_USER_DEFINED:
      // it is assumed that the user would have used the hooks to supply a
      // user-defined instance of the object table
      ACE_ASSERT(this->objtable_ != 0);
      break;
    case OA_Parameters::TAO_ACTIVE_DEMUX:
      break;
    case OA_Parameters::TAO_DYNAMIC_HASH:
    default:
      this->objtable_ = new TAO_Dynamic_Hash_ObjTable (p->tablesize());
    }
  return this->objtable_;
}

TAO_Client_Factory::TAO_Client_Factory()
{
  // When should I do this open()?  It seems like this is way too early,
  // but doing it in the accessor for connector() seems like it would be
  // too late as well.
  connector_.open(ACE_Service_Config::reactor(),
		  &null_creation_strategy_,
		  &caching_connect_strategy_,
		  client_concurrency_strategy_());
}

#if defined(ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Thread_Strategy<ROA_Handler>;
template class ACE_Concurrency_Strategy<ROA_Handler>;
template class ACE_Reactive_Strategy<ROA_Handler>;
template class ACE_Creation_Strategy<ROA_Handler>;
template class ACE_Scheduling_Strategy<ROA_Handler>;
template class ACE_Accept_Strategy<ROA_Handler, ACE_SOCK_ACCEPTOR>;
#endif
