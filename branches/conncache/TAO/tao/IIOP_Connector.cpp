// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//
// = FILENAME
//
// = DESCRIPTION
//
// = AUTHOR
//
// ============================================================================

#include "tao/IIOP_Connector.h"
#include "tao/IIOP_Profile.h"
#include "tao/GIOP.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/Client_Strategy_Factory.h"
#include "tao/Environment.h"

ACE_RCSID(tao, IIOP_Connector, "$Id$")

// ****************************************************************

TAO_IIOP_Connect_Creation_Strategy::
  TAO_IIOP_Connect_Creation_Strategy (ACE_Thread_Manager* t,
                                      TAO_ORB_Core *orb_core)
    :  ACE_Creation_Strategy<TAO_IIOP_Client_Connection_Handler> (t),
       orb_core_ (orb_core)
{
}

int
TAO_IIOP_Connect_Creation_Strategy::make_svc_handler (
    TAO_IIOP_Client_Connection_Handler *&sh)
{
  if (sh == 0)
    {
      ACE_NEW_RETURN (sh,
                      TAO_IIOP_Client_Connection_Handler (
                        this->orb_core_->thr_mgr (),
                        this->orb_core_),
                      -1);
    }
  return 0;
}

// ****************************************************************

TAO_IIOP_Connector::TAO_IIOP_Connector (void)
  : TAO_Connector (TAO_IOP_TAG_INTERNET_IOP),
    base_connector_ (),
    orb_core_ (0)
{
}

int
TAO_IIOP_Connector::open (TAO_ORB_Core *orb_core)
{
  this->orb_core_ = orb_core;

  TAO_Cached_Connector_Lock *connector_lock = 0;
  ACE_NEW_RETURN (connector_lock,
                  TAO_Cached_Connector_Lock (this->orb_core_),
                  -1);

  int result = make_connection_caching_strategy ();
  if (result == -1)
    return -1;

   this->cached_connect_strategy_ =
    new TAO_CACHED_CONNECT_STRATEGY (*this->caching_strategy_,
        new TAO_IIOP_Connect_Creation_Strategy (this->orb_core_->thr_mgr (),
                                                this->orb_core_),
        0,
        0,
        connector_lock,
        1);

  return this->base_connector_.open (this->orb_core_->reactor (),
                                     &this->null_creation_strategy_,
                                     this->cached_connect_strategy_,
                                     &this->null_activation_strategy_);
}

// = Typedefs needed for connection caching strategies. 
typedef size_t ATTRIBUTES;
typedef ACE_Pair<TAO_IIOP_Client_Connection_Handler *, ATTRIBUTES>
        CACHED_HANDLER;
typedef ACE_Refcounted_Hash_Recyclable<ACE_INET_Addr>
        REFCOUNTED_HASH_RECYCLABLE_ADDR;
typedef ACE_Hash_Map_Manager_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Null_Mutex> 
        IIOP_HASH_MAP;
typedef ACE_Hash_Map_Iterator_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Null_Mutex> 
        IIOP_HASH_ITERATOR;
typedef ACE_Hash_Map_Reverse_Iterator_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Null_Mutex> 
        IIOP_HASH_REVERSE_ITERATOR;
typedef ACE_Recyclable_Handler_Caching_Utility<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, IIOP_HASH_MAP, IIOP_HASH_ITERATOR, ATTRIBUTES>
        IIOP_CACHING_UTILITY;

typedef ACE_LRU_Caching_Strategy<ATTRIBUTES, IIOP_CACHING_UTILITY>
        LRU_IIOP_CACHING_STRATEGY;

#if defined (ACE_HAS_BROKEN_EXTENDED_TEMPLATES)
  typedef LRU_IIOP_CACHING_STRATEGY
          IIOP_CACHING_STRATEGY;
#else
  typedef ACE_Caching_Strategy<ATTRIBUTES, IIOP_CACHING_UTILITY>
          IIOP_CACHING_STRATEGY;
  typedef ACE_LFU_Caching_Strategy<ATTRIBUTES, IIOP_CACHING_UTILITY>
          LFU_IIOP_CACHING_STRATEGY;
  typedef ACE_FIFO_Caching_Strategy<ATTRIBUTES, IIOP_CACHING_UTILITY>
          FIFO_IIOP_CACHING_STRATEGY;
  typedef ACE_Null_Caching_Strategy<ATTRIBUTES, IIOP_CACHING_UTILITY>
          NULL_IIOP_CACHING_STRATEGY;
  typedef ACE_Caching_Strategy_Adapter<ATTRIBUTES, IIOP_CACHING_UTILITY, LRU_IIOP_CACHING_STRATEGY>
          LRU_IIOP_CACHING_STRATEGY_ADAPTER;
  typedef ACE_Caching_Strategy_Adapter<ATTRIBUTES, IIOP_CACHING_UTILITY, LFU_IIOP_CACHING_STRATEGY>
          LFU_IIOP_CACHING_STRATEGY_ADAPTER;
  typedef ACE_Caching_Strategy_Adapter<ATTRIBUTES, IIOP_CACHING_UTILITY, FIFO_IIOP_CACHING_STRATEGY>
          FIFO_IIOP_CACHING_STRATEGY_ADAPTER;
  typedef ACE_Caching_Strategy_Adapter<ATTRIBUTES, IIOP_CACHING_UTILITY, NULL_IIOP_CACHING_STRATEGY>
          NULL_IIOP_CACHING_STRATEGY_ADAPTER;
#endif /* ACE_HAS_BROKEN_EXTENDED_TEMPLATES */

typedef ACE_Cached_Connect_Strategy_Ex<TAO_IIOP_Client_Connection_Handler, ACE_SOCK_CONNECTOR, IIOP_CACHING_STRATEGY, ATTRIBUTES, TAO_Cached_Connector_Lock>
        TAO_CACHED_CONNECT_STRATEGY;

TAO_CACHED_CONNECT_STRATEGY &
TAO_IIOP_Connector::cached_connect_strategy (void)
{
  return *this->cached_connect_strategy_;
}

int 
TAO_IIOP_Connector::make_connection_caching_strategy (void)
{

#if defined (ACE_HAS_BROKEN_EXTENDED_TEMPLATES)
   if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "\nLRU_Caching_Strategy\n\n"));
      ACE_NEW_RETURN (this->caching_strategy_,
                      IIOP_CACHING_STRATEGY,
                      -1);
#else

  this->caching_strategy_ = 0;

  switch (this->orb_core_->connection_caching_strategy_type ())
    {
    case TAO_NULL: 
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "\nNull_Caching_Strategy\n\n"));
      ACE_NEW_RETURN (this->caching_strategy_,
                      NULL_IIOP_CACHING_STRATEGY_ADAPTER,
                      -1);
      break;

    default:
    case TAO_LRU:
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "\nLRU_Caching_Strategy\n\n"));
      ACE_NEW_RETURN (this->caching_strategy_,
                      LRU_IIOP_CACHING_STRATEGY_ADAPTER,
                      -1);
      break;

    case TAO_LFU:
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "\nLFU_Caching_Strategy\n\n"));
      ACE_NEW_RETURN (this->caching_strategy_,
                      LFU_IIOP_CACHING_STRATEGY_ADAPTER,
                      -1);
      break;

    case TAO_FIFO:
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "\nFIFO_Caching_Strategy\n\n"));
       ACE_NEW_RETURN (this->caching_strategy_,
                       FIFO_IIOP_CACHING_STRATEGY_ADAPTER,
                      -1);
      break;

    }
#endif /* ACE_HAS_BROKEN_EXTENDED_TEMPLATES */ 

  this->caching_strategy_->purge_percent (this->orb_core_->purge_percentage ());
  return 0;
}

int
TAO_IIOP_Connector::close (void)
{
  TAO_CACHED_CONNECT_STRATEGY *cached_connect_strategy =
    ACE_dynamic_cast (TAO_CACHED_CONNECT_STRATEGY *,
                      this->base_connector_.connect_strategy ());

  // Zap the creation strategy that we created earlier
  delete this->cached_connect_strategy_->creation_strategy ();
  delete this->cached_connect_strategy_;
  delete this->caching_strategy_;

  this->base_connector_.close ();
  return 0;
}

int
TAO_IIOP_Connector::connect (TAO_Profile *profile,
                             TAO_Transport *& transport,
                             ACE_Time_Value *max_wait_time)
{
  if (profile->tag () != TAO_IOP_TAG_INTERNET_IOP)
    return -1;

  TAO_IIOP_Profile *iiop_profile =
    ACE_dynamic_cast (TAO_IIOP_Profile *, profile);

  if (iiop_profile == 0)
    return -1;

  const ACE_INET_Addr &oa = iiop_profile->object_addr ();

  ACE_Synch_Options synch_options;
  if (max_wait_time != 0)
    {
      synch_options.set (ACE_Synch_Options::USE_TIMEOUT,
                         *max_wait_time);
    }

  TAO_IIOP_Client_Connection_Handler* result;

  // the connect call will set the hint () stored in the Profile
  // object; but we obtain the transport in the <result>
  // variable. Other threads may modify the hint, but we are not
  // affected.
  errno = 0;
  if (this->base_connector_.connect (iiop_profile->hint (),
                                     result,
                                     oa,
                                     synch_options) == -1)
    { // Give users a clue to the problem.
      if (TAO_orbdebug)
        {
          char buffer [MAXNAMELEN * 2];
          profile->addr_to_string (buffer, (MAXNAMELEN * 2) - 1);
          ACE_DEBUG ((LM_ERROR, "(%P|%t) %s:%u, connection to "
                      "%s failed (%p)\n",
                      __FILE__,
                      __LINE__,
                      buffer,
                      "errno"));
        }

      return -1;
    }

  transport = result->transport ();

  return 0;
}

int
TAO_IIOP_Connector::preconnect (const char *preconnects)
{
  // Check for the proper protocol prefix.
  if (this->check_prefix (preconnects) != 0)
    return 0; // Failure: zero successful preconnections

  const char *protocol_removed = ACE_OS::strstr (preconnects, "://") + 3;
  // "+ 3" since strlen of "://" is 3.

  char *preconnections = ACE_OS::strdup (protocol_removed);

  int successes = 0;
  if (preconnections)
    {
      ACE_INET_Addr dest;
      ACE_Unbounded_Stack<ACE_INET_Addr> dests;

      size_t num_connections;

      char *nextptr = 0;
      char *where = 0;
      for (where = ACE::strsplit_r (preconnections, ",", nextptr);
           where != 0;
           where = ACE::strsplit_r (0, ",", nextptr))
        {
          int version_offset = 0;
          // Additional offset to remove version from preconnect, if it exists.

          if (isdigit (where[0]) &&
              where[1] == '.' &&
              isdigit (where[2]) &&
              where[3] == '@')
            version_offset = 4;

          // @@ For now, we just drop the version prefix.  However, at
          //    some point in the future the version may become useful.

          char *tport = 0;
          char *thost = where + version_offset;
          char *sep = ACE_OS::strchr (where, ':');

          if (sep)
            {
              *sep = '\0';
              tport = sep + 1;

              dest.set (ACE_OS::atoi (tport), thost);
              dests.push (dest);
            }
          else
            {
              // No port was specified so assume that the port will be the
              // IANA assigned port for IIOP.
              //
              //    IIOP:           683
              //    IIOP over SSL:  684

              dest.set (683, thost);
              dests.push (dest);

              if (TAO_debug_level > 0)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "TAO (%P|%t) No port specified for <%s>.  "
                              "Using <%d> as default port.\n",
                              where,
                              dest.get_port_number ()));
                }
            }
        }

      // Create an array of addresses from the stack, as well as an
      // array of eventual handlers.
      num_connections = dests.size ();
      ACE_INET_Addr *remote_addrs = 0;
      TAO_IIOP_Client_Connection_Handler **handlers = 0;
      char *failures = 0;

      ACE_NEW_RETURN (remote_addrs,
                      ACE_INET_Addr[num_connections],
                      -1);
      ACE_NEW_RETURN (handlers,
                      TAO_IIOP_Client_Connection_Handler *[num_connections],
                      -1);
      ACE_NEW_RETURN (failures,
                      char[num_connections],
                      -1);

      size_t slot = 0;

      // Fill in the remote address array
      while (dests.pop (remote_addrs[slot]) == 0)
        handlers[slot++] = 0;

      // Finally, try to connect.
      this->base_connector_.connect_n (num_connections,
                                       handlers,
                                       remote_addrs,
                                       failures);
      // Loop over all the failures and set the handlers that
      // succeeded to idle state.
      for (slot = 0;
           slot < num_connections;
           slot++)
        {
          if (!failures[slot])
            {
              handlers[slot]->idle ();
              successes++;

              if (TAO_debug_level > 0)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "TAO (%P|%t) Preconnection <%s:%d> "
                              "succeeded.\n",
                              remote_addrs[slot].get_host_name (),
                              remote_addrs[slot].get_port_number ()));
                }
            }
          else
            {
              if (TAO_debug_level > 0)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "TAO (%P|%t) Preconnection <%s:%d> failed.\n",
                              remote_addrs[slot].get_host_name (),
                              remote_addrs[slot].get_port_number ()));
                }
            }
        }

      ACE_OS::free (preconnections);

      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) IIOP preconnections: %d successes and "
                      "%d failures.\n",
                      successes,
                      num_connections - successes));
        }
    }

  return successes;
}

TAO_Profile*
TAO_IIOP_Connector::create_profile (TAO_InputCDR& cdr)
{
  TAO_Profile* pfile;
  ACE_NEW_RETURN (pfile,
                  TAO_IIOP_Profile (this->orb_core_),
                  0);

  int r = pfile->decode (cdr);
  if (r == -1)
    {
      pfile->_decr_refcnt ();
      pfile = 0;
    }

  return pfile;
}

void
TAO_IIOP_Connector::make_profile (const char *endpoint,
                                  TAO_Profile *&profile,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  // The endpoint should be of the form:
  //
  //    N.n@host:port/object_key
  //
  // or:
  //
  //    host:port/object_key

  ACE_NEW_THROW_EX (profile,
                    TAO_IIOP_Profile (endpoint,
                                      this->orb_core_,
                                      ACE_TRY_ENV),
                    CORBA::NO_MEMORY ());

  ACE_CHECK;
}


int
TAO_IIOP_Connector::check_prefix (const char *endpoint)
{
  // Check for a valid string
  if (!endpoint || !*endpoint)
    return -1;  // Failure

  const char *protocol[] = { "iiop", "iioploc" };
  // This is valid for any protocol beginning with `iiop' or `iioploc'.

  // Check for the proper prefix in the IOR.  If the proper prefix isn't
  // in the IOR then it is not an IOR we can use.
  if (ACE_OS::strncasecmp (endpoint,
                           protocol[0],
                           ACE_OS::strlen (protocol[0])) == 0 ||
      ACE_OS::strncasecmp (endpoint,
                           protocol[1],
                           ACE_OS::strlen (protocol[1])) == 0)
    {
      return 0;  // Success
    }

  return -1;
  // Failure: not an IIOP IOR
  // DO NOT throw an exception here.
}

char
TAO_IIOP_Connector::object_key_delimiter (void) const
{
  return TAO_IIOP_Profile::object_key_delimiter;
}


#define TAO_SVC_TUPLE ACE_Svc_Tuple<TAO_IIOP_Client_Connection_Handler>
#define CACHED_CONNECT_STRATEGY_BASE ACE_Cached_Connect_Strategy<TAO_IIOP_Client_Connection_Handler, ACE_SOCK_CONNECTOR, TAO_Cached_Connector_Lock>

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Node<ACE_INET_Addr>;
template class ACE_Unbounded_Stack<ACE_INET_Addr>;
template class ACE_Unbounded_Stack_Iterator<ACE_INET_Addr>;

template class ACE_Creation_Strategy<TAO_IIOP_Client_Connection_Handler>;
template class ACE_Connect_Strategy<TAO_IIOP_Client_Connection_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_Strategy_Connector<TAO_IIOP_Client_Connection_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_NOOP_Creation_Strategy<TAO_IIOP_Client_Connection_Handler>;
template class ACE_Concurrency_Strategy<TAO_IIOP_Client_Connection_Handler>;
template class ACE_NOOP_Concurrency_Strategy<TAO_IIOP_Client_Connection_Handler>;
template class ACE_Recycling_Strategy<TAO_IIOP_Client_Connection_Handler>;
template class ACE_Connector<TAO_IIOP_Client_Connection_Handler, ACE_SOCK_CONNECTOR>;

template class ACE_Node<TAO_IIOP_Client_Connection_Handler *>;

template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>;
template class CACHED_CONNECT_STRATEGY_BASE;
template class ACE_Refcounted_Hash_Recyclable<ACE_INET_Addr>;
template class ACE_Map_Manager<int, TAO_SVC_TUPLE*, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Iterator_Base<int, TAO_SVC_TUPLE*, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Iterator<int, TAO_SVC_TUPLE*, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Reverse_Iterator<int, TAO_SVC_TUPLE*, ACE_SYNCH_RW_MUTEX>;
template class TAO_SVC_TUPLE;

template class ACE_Map_Entry<int, TAO_SVC_TUPLE*>;

template class ACE_Hash_Map_Entry<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *>;
template class ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>;
template class ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>;

#if defined(ACE_HAS_THREADS)
template class ACE_Hash_Map_Manager<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Manager_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Base_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Bucket_Iterator<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_MUTEX>;
#endif /* ACE_HAS_THREADS */

template class ACE_Hash_Map_Manager<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Hash_Map_Manager_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Hash_Map_Iterator_Base_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Hash_Map_Iterator<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Hash_Map_Bucket_Iterator<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_NULL_MUTEX>;

// = Caching Strategy 
template class ACE_Pair<TAO_IIOP_Client_Connection_Handler *, ATTRIBUTES>;
template class ACE_Reference_Pair<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *>;
template class ACE_Hash_Map_Entry<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER>;
template class ACE_Hash_Map_Manager_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Hash_Map_Iterator_Base_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Hash_Map_Manager<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Hash_Map_Iterator<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Hash_Map_Bucket_Iterator<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_NULL_MUTEX>;

template class ACE_Recyclable_Handler_Cleanup_Strategy<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, IIOP_HASH_MAP>;
template class ACE_Cleanup_Strategy<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, IIOP_HASH_MAP>;
template class ACE_Recyclable_Handler_Caching_Utility<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, IIOP_HASH_MAP, IIOP_HASH_ITERATOR, ATTRIBUTES>;

template class ACE_Hash_Cache_Map_Manager<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>, IIOP_CACHING_STRATEGY, ATTRIBUTES>;
template class ACE_LRU_Caching_Strategy<ATTRIBUTES, IIOP_CACHING_UTILITY>;

#if !defined (ACE_HAS_BROKEN_EXTENDED_TEMPLATES)
template class ACE_Caching_Strategy<ATTRIBUTES, IIOP_CACHING_UTILITY>;
template class ACE_LFU_Caching_Strategy<ATTRIBUTES, IIOP_CACHING_UTILITY>;
template class ACE_FIFO_Caching_Strategy<ATTRIBUTES, IIOP_CACHING_UTILITY>;
template class ACE_Null_Caching_Strategy<ATTRIBUTES, IIOP_CACHING_UTILITY>;
template class ACE_Caching_Strategy_Adapter<ATTRIBUTES, IIOP_CACHING_UTILITY, LRU_IIOP_CACHING_STRATEGY>;
template class ACE_Caching_Strategy_Adapter<ATTRIBUTES, IIOP_CACHING_UTILITY, LFU_IIOP_CACHING_STRATEGY>;
template class ACE_Caching_Strategy_Adapter<ATTRIBUTES, IIOP_CACHING_UTILITY, FIFO_IIOP_CACHING_STRATEGY>;
template class ACE_Caching_Strategy_Adapter<ATTRIBUTES, IIOP_CACHING_UTILITY, NULL_IIOP_CACHING_STRATEGY>;

template class ACE_Cache_Map_Manager<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, IIOP_HASH_MAP, IIOP_HASH_ITERATOR, IIOP_HASH_REVERSE_ITERATOR, IIOP_CACHING_STRATEGY, ATTRIBUTES>;
template class ACE_Cache_Map_Iterator<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, IIOP_HASH_ITERATOR, IIOP_CACHING_STRATEGY, ATTRIBUTES>;
template class ACE_Cache_Map_Reverse_Iterator<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, IIOP_HASH_REVERSE_ITERATOR, IIOP_CACHING_STRATEGY, ATTRIBUTES>;

#else

template class ACE_Cache_Map_Manager<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, IIOP_HASH_MAP, IIOP_CACHING_STRATEGY, ATTRIBUTES>;

#endif /* ACE_HAS_BROKEN_EXTENDED_TEMPLATES */

template class ACE_Cached_Connect_Strategy_Ex<TAO_IIOP_Client_Connection_Handler, ACE_SOCK_CONNECTOR, IIOP_CACHING_STRATEGY, ATTRIBUTES, TAO_Cached_Connector_Lock>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Node<ACE_INET_Addr>
#pragma instantiate ACE_Unbounded_Stack<ACE_INET_Addr>
#pragma instantiate ACE_Unbounded_Stack_Iterator<ACE_INET_Addr>

#pragma instantiate ACE_Creation_Strategy<TAO_IIOP_Client_Connection_Handler>
#pragma instantiate ACE_Connect_Strategy<TAO_IIOP_Client_Connection_Handler, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_Strategy_Connector<TAO_IIOP_Client_Connection_Handler, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_NOOP_Creation_Strategy<TAO_IIOP_Client_Connection_Handler>
#pragma instantiate ACE_Concurrency_Strategy<TAO_IIOP_Client_Connection_Handler>
#pragma instantiate ACE_NOOP_Concurrency_Strategy<TAO_IIOP_Client_Connection_Handler>
#pragma instantiate ACE_Recycling_Strategy<TAO_IIOP_Client_Connection_Handler>
#pragma instantiate ACE_Connector<TAO_IIOP_Client_Connection_Handler, ACE_SOCK_CONNECTOR>

#pragma instantiate ACE_Node<TAO_IIOP_Client_Connection_Handler *>

#pragma instantiate ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
#pragma instantiate CACHED_CONNECT_STRATEGY_BASE
#pragma instantiate ACE_Refcounted_Hash_Recyclable<ACE_INET_Addr>
#pragma instantiate TAO_SVC_TUPLE
#pragma instantiate ACE_Map_Manager<int, TAO_SVC_TUPLE*, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Iterator_Base<int, TAO_SVC_TUPLE*, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Iterator<int, TAO_SVC_TUPLE*, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Reverse_Iterator<int, TAO_SVC_TUPLE*, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Entry<int, TAO_SVC_TUPLE*>

#pragma instantiate ACE_Hash_Map_Entry<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *>
#pragma instantiate ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>
#pragma instantiate ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>

#if defined(ACE_HAS_THREADS)
#pragma instantiate ACE_Hash_Map_Manager<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Bucket_Iterator<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_MUTEX>
#endif /* ACE_HAS_THREADS */

#pragma instantiate ACE_Hash_Map_Manager<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Hash_Map_Bucket_Iterator<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_NULL_MUTEX>

// = Caching Strategy
#pragma instantiate ACE_Pair<TAO_IIOP_Client_Connection_Handler *, ATTRIBUTES>
#pragma instantiate ACE_Reference_Pair<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *>
#pragma instantiate ACE_Hash_Map_Entry<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER>
#pragma instantiate ACE_Hash_Map_Manager_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_NULL_MUTEX> 
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_NULL_MUTEX> 
#pragma instantiate ACE_Hash_Map_Manager<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, ACE_SYNCH_NULL_MUTEX> 
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Hash_Map_Bucket_Iterator<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_SYNCH_NULL_MUTEX>

#pragma instantiate ACE_Recyclable_Handler_Cleanup_Strategy<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, IIOP_HASH_MAP>
#pragma instantiate ACE_Cleanup_Strategy<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, IIOP_HASH_MAP>
#pragma instantiate ACE_Recyclable_Handler_Caching_Utility<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, IIOP_HASH_MAP, IIOP_HASH_ITERATOR, ATTRIBUTES>

#pragma instantiate ACE_Hash_Cache_Map_Manager<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>, IIOP_CACHING_STRATEGY, ATTRIBUTES>
#pragma instantiatetiates ACE_LRU_Caching_Strategy<ATTRIBUTES, IIOP_CACHING_UTILITY>

#if !defined (ACE_HAS_BROKEN_EXTENDED_TEMPLATES)
#pragma instantiate ACE_Caching_Strategy<ATTRIBUTES, IIOP_CACHING_UTILITY>
#pragma instantiate ACE_LFU_Caching_Strategy<ATTRIBUTES, IIOP_CACHING_UTILITY>
#pragma instantiate ACE_FIFO_Caching_Strategy<ATTRIBUTES, IIOP_CACHING_UTILITY>
#pragma instantiate ACE_Null_Caching_Strategy<ATTRIBUTES, IIOP_CACHING_UTILITY>
#pragma instantiate ACE_Caching_Strategy_Adapter<ATTRIBUTES, IIOP_CACHING_UTILITY, LRU_IIOP_CACHING_STRATEGY>
#pragma instantiate ACE_Caching_Strategy_Adapter<ATTRIBUTES, IIOP_CACHING_UTILITY, LFU_IIOP_CACHING_STRATEGY>
#pragma instantiate ACE_Caching_Strategy_Adapter<ATTRIBUTES, IIOP_CACHING_UTILITY, FIFO_IIOP_CACHING_STRATEGY>
#pragma instantiate ACE_Caching_Strategy_Adapter<ATTRIBUTES, IIOP_CACHING_UTILITY, NULL_IIOP_CACHING_STRATEGY>

#pragma instantiate ACE_Cache_Map_Manager<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, IIOP_HASH_MAP, IIOP_HASH_ITERATOR, IIOP_HASH_REVERSE_ITERATOR, IIOP_CACHING_STRATEGY, ATTRIBUTES>
#pragma instantiate ACE_Cache_Map_Iterator<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, IIOP_HASH_ITERATOR, IIOP_CACHING_STRATEGY, ATTRIBUTES>
#pragma instantiate ACE_Cache_Map_Reverse_Iterator<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, IIOP_HASH_REVERSE_ITERATOR, IIOP_CACHING_STRATEGY, ATTRIBUTES>

#else

#pragma instantiate ACE_Cache_Map_Manager<REFCOUNTED_HASH_RECYCLABLE_ADDR, TAO_IIOP_Client_Connection_Handler *, IIOP_HASH_MAP, IIOP_CACHING_STRATEGY, ATTRIBUTES>

#endif /* ACE_HAS_BROKEN_EXTENDED_TEMPLATES */

#pragma instantiate ACE_Cached_Connect_Strategy_Ex<TAO_IIOP_Client_Connection_Handler, ACE_SOCK_CONNECTOR, IIOP_CACHING_STRATEGY, ATTRIBUTES, TAO_Cached_Connector_Lock>
       
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
