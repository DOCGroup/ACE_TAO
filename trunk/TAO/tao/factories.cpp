#include "tao/factories.h"

#if !defined(__ACE_INLINE__)
#  include "factories.i"
#endif

// Template specializations which allow the cached connection manager
// to work better.

size_t
ACE_Hash_Addr<ACE_INET_Addr, TAO_Client_Connection_Handler>::hash_i (const ACE_INET_Addr &addr) const
{
  return addr.get_ip_address () + addr.get_port_number ();
}

int
ACE_Hash_Addr<ACE_INET_Addr, TAO_Client_Connection_Handler>::compare_i (const ACE_INET_Addr &a1, 
                                                                        const ACE_INET_Addr &a2) const
{
  return a1 != a2;
}

int
TAO_Client_Connection_Handler::open(void *)
{
  // Here is where we could enable all sorts of things such as
  // nonblock I/O, sock buf sizes, TCP no-delay, etc.
  
  const int MAX_SOCK_BUF_SIZE = 65536;
  if (this->peer ().set_option(SOL_SOCKET, SO_SNDBUF, (void
						       *)&MAX_SOCK_BUF_SIZE,
			       sizeof(MAX_SOCK_BUF_SIZE)) == -1) 
    return -1;
  if (this->peer ().set_option(SOL_SOCKET, SO_RCVBUF, (void
						       *)&MAX_SOCK_BUF_SIZE,
			       sizeof (MAX_SOCK_BUF_SIZE)) == -1)
    return -1;

  // For now, we just return success
  return 0;
}

// Determine the appropriate default thread flags, based on system.
// When I put the concurrency strategy into the factory, then this
// will go away b/c the concurrency strategy will do this appropriate
// for each platform!
#  if defined(linux)
#    define ROA_DEFAULT_THREADFLAGS  (THR_DETACHED)
#  elif defined(WIN32)
#    define ROA_DEFAULT_THREADFLAGS  (THR_DETACHED)
#  elif defined(sparc)
#    define ROA_DEFAULT_THREADFLAGS  (THR_DETACHED | THR_BOUND)
#  else
#    define ROA_DEFAULT_THREADFLAGS  (THR_DETACHED)
#  endif

TAO_Server_Factory::CONCURRENCY_STRATEGY*
TAO_Server_Factory::concurrency_strategy ()
{
  TAO_OA_Parameters* p = TAO_OA_PARAMS::instance ();


  if (p->using_threads ())
    {
      // Set the strategy parameters
      threaded_strategy_.open (ACE_Service_Config::thr_mgr (),
			       ROA_DEFAULT_THREADFLAGS);
      concurrency_strategy_ = &threaded_strategy_;
    }
  else
    {
      reactive_strategy_.open (ACE_Service_Config::reactor ());
      concurrency_strategy_ = &reactive_strategy_;
    }

  return concurrency_strategy_;
}

TAO_Object_Table *
TAO_Server_Factory::object_lookup_strategy (void)
{
  TAO_OA_Parameters* p = TAO_OA_PARAMS::instance ();

  // Since these are dynamically created, when do they get destroyed?
  switch (p->demux_strategy ())
    {
    case TAO_OA_Parameters::TAO_LINEAR:
      ACE_NEW_RETURN (this->objtable_,
		      TAO_Linear_ObjTable (p->tablesize ()),
					   0);
      break;
    case TAO_OA_Parameters::TAO_USER_DEFINED:
      // it is assumed that the user would have used the hooks to supply a
      // user-defined instance of the object table
      ACE_ASSERT (this->objtable_ != 0);
      break;
    case TAO_OA_Parameters::TAO_ACTIVE_DEMUX:
      break;
    case TAO_OA_Parameters::TAO_DYNAMIC_HASH:
    default:
      ACE_NEW_RETURN (this->objtable_,
		      TAO_Dynamic_Hash_ObjTable (p->tablesize ()),
		      0);
    }
  return this->objtable_;
}

TAO_Client_Factory::TAO_Client_Factory (void)
{
  // When should I do this open ()?  It seems like this is way too
  // early, but doing it in the accessor for connector () seems like
  // it would be too late as well.
  connector_.open (ACE_Service_Config::reactor (),
		   &null_creation_strategy_,
		   &caching_connect_strategy_,
#if defined (TAO_HAS_CLIENT_CONCURRENCY)
		   concurrency_strategy_ ()
#else
		   0
#endif /* TAO_HAS_CLIENT_CONCURRENCY */
		   );
}

TAO_Client_Factory::~TAO_Client_Factory (void)
{}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Thread_Strategy<TAO_OA_Connection_Handler>;
template class ACE_Concurrency_Strategy<TAO_OA_Connection_Handler>;
template class ACE_Reactive_Strategy<TAO_OA_Connection_Handler>;
template class ACE_Creation_Strategy<TAO_OA_Connection_Handler>;
template class ACE_Scheduling_Strategy<TAO_OA_Connection_Handler>;
template class ACE_Accept_Strategy<TAO_OA_Connection_Handler, ACE_SOCK_ACCEPTOR>;

template class ACE_Creation_Strategy<TAO_Client_Connection_Handler>;
template class ACE_Connect_Strategy<TAO_Client_Connection_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_Concurrency_Strategy<TAO_Client_Connection_Handler>;
template class ACE_Cached_Connect_Strategy<TAO_Client_Connection_Handler, ACE_SOCK_CONNECTOR, ACE_SYNCH_RW_MUTEX>;
template class ACE_NOOP_Creation_Strategy<TAO_Client_Connection_Handler>;
template class ACE_Strategy_Connector<TAO_Client_Connection_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_Connector<TAO_Client_Connection_Handler, ACE_SOCK_CONNECTOR>;
#define TAO_HASH_ADDR ACE_Hash_Addr<ACE_INET_Addr, TAO_Client_Connection_Handler>
template class TAO_HASH_ADDR;
template class ACE_Hash_Map_Manager<TAO_HASH_ADDR, TAO_Client_Connection_Handler*, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Entry<TAO_HASH_ADDR, TAO_Client_Connection_Handler*>;
#define TAO_SVC_TUPLE ACE_Svc_Tuple<TAO_Client_Connection_Handler>
template class TAO_SVC_TUPLE;
template class ACE_Map_Manager<int, TAO_SVC_TUPLE*, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Iterator<int, TAO_SVC_TUPLE*, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Entry<int, TAO_SVC_TUPLE*>;

#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
