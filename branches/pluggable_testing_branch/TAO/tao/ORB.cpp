// $Id$

// @@ as with TAO, this has a strong coupling to the Internet ORB
// (IIOP) code.  We should make it know less about that protocol
// component and have a loose table-driven coupling to ORB/protocol
// library components.

#include "tao/ORB.h"

#include "ace/Dynamic_Service.h"
#include "ace/Service_Repository.h"
#include "ace/Object_Manager.h"
#include "ace/SOCK_Dgram_Mcast.h"
#include "ace/Thread_Manager.h"
#include "ace/Read_Buffer.h"
#include "ace/Auto_Ptr.h"

#include "tao/Object.h"
#include "tao/Typecode.h"
#include "tao/NVList.h"
#include "tao/Stub.h"
#include "tao/IIOP_Profile.h"
#include "tao/DynAny_i.h"
#include "tao/ORB_Core.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/CDR_Interpreter.h"
#include "tao/debug.h"
#include "tao/TAO_Internal.h"
#include "tao/CDR.h"
#include "tao/Marshal.h"
#include "tao/IOR_LookupTable.h"
#include "tao/GIOP.h"
#include "tao/IIOP_Acceptor.h"
#include "tao/Object_Adapter.h"
#include "tao/POA.h"
#include "tao/Request.h"
#ifdef TAO_HAS_VALUETYPE
#  include "tao/ValueFactory_Map.h"
#endif /* TAO_HAS_VALUETYPE */


#if !defined (__ACE_INLINE__)
# include "tao/ORB.i"
#endif /* ! __ACE_INLINE__ */

#include "tao/Timeprobe.h"

ACE_RCSID(tao, ORB, "$Id$")

static const char ior_prefix [] = "IOR:";
static const char iioploc_prefix [] = "iioploc:";
static const char file_prefix[] = "file://";

#if defined (ACE_ENABLE_TIMEPROBES)

static const char *TAO_ORB_Timeprobe_Description[] =
{
  "CORBA_ORB::run - start",
  "CORBA_ORB::run - end",
};

enum
{
  // Timeprobe description table start key
  TAO_CORBA_ORB_RUN_START = 0,
  TAO_CORBA_ORB_RUN_END
};

// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (TAO_ORB_Timeprobe_Description,
                                  TAO_CORBA_ORB_RUN_START);

#endif /* ACE_ENABLE_TIMEPROBES */

// = Static initialization.

// Count of the number of ORBs.
int CORBA_ORB::orb_init_count_ = 0;

// Pointer to the "default ORB."
CORBA::ORB_ptr CORBA::instance_ = 0;

// ****************************************************************

CORBA_ORB::InvalidName::InvalidName (void)
{
}

void
CORBA_ORB::InvalidName::_raise (void)
{
  TAO_RAISE(*this);
}

CORBA_ORB::InvalidName*
CORBA_ORB::InvalidName::_narrow (CORBA_Exception *ex)
{
  if (ex->_is_a ("IDL:omg.orb/CORBA/ORB/InvalidName:1.0"))
    return ACE_dynamic_cast (CORBA_ORB::InvalidName*, ex);
  return 0;
}

int
CORBA_ORB::InvalidName::_is_a (const char* interface_id) const
{
  return ((ACE_OS::strcmp (interface_id,
                          "IDL:omg.orb/CORBA/ORB/InvalidName:1.0") ==
           0)
          || CORBA_UserException::_is_a (interface_id));
}

CORBA_ORB::CORBA_ORB (TAO_ORB_Core* orb_core)
  : refcount_ (1),
    open_called_ (0),
    shutdown_lock_ (0),
    should_shutdown_ (0),
    name_service_ (CORBA_Object::_nil ()),
    schedule_service_ (CORBA_Object::_nil ()),
    event_service_ (CORBA_Object::_nil ()),
    trading_service_ (CORBA_Object::_nil ()),
    orb_core_ (orb_core),
# ifdef TAO_HAS_VALUETYPE
    valuetype_factory_map_ (0),
# endif /* TAO_HAS_VALUETYPE */
    use_omg_ior_format_ (1),
    optimize_collocation_objects_ (1)
{
  leader_follower_info_.leaders_ = 0;
  leader_follower_info_.leader_thread_ID_ =
    ACE_OS::NULL_thread;
  ACE_NEW (this->cond_become_leader_,
           ACE_SYNCH_CONDITION (leader_follower_info_.leader_follower_lock_));

  if (CORBA::instance_ == 0)
    CORBA::instance_ = this;
}

CORBA_ORB::~CORBA_ORB (void)
{
  this->orb_core_->fini ();

  ACE_MT (ACE_GUARD (ACE_Recursive_Thread_Mutex, tao_mon, *ACE_Static_Object_Lock::instance ()));

  CORBA_ORB::orb_init_count_--;

  if (CORBA_ORB::orb_init_count_ == 0)
    {
      // free up all the ORB owned Exceptions
      TAO_Exceptions::fini ();

      // free up all the ORB owned TypeCodes
      TAO_TypeCodes::fini ();

      // @@ Note that we shouldn't need to actually delete this
      // instance since it'll be handled by the destruction of the
      // instance via another mechanism.
      CORBA::instance_ = 0;
    }

  delete this->shutdown_lock_;
  this->shutdown_lock_ = 0;

  if (!CORBA::is_nil (this->name_service_))
    CORBA::release (this->name_service_);
  if (!CORBA::is_nil (this->schedule_service_))
    CORBA::release (this->schedule_service_);
  if (!CORBA::is_nil (this->event_service_))
    CORBA::release (this->event_service_);
  if (!CORBA::is_nil (this->trading_service_))
    CORBA::release (this->trading_service_);
# ifdef TAO_HAS_VALUETYPE
  // delete valuetype_factory_map_;
  // not really, its a singleton
# endif /* TAO_HAS_VALUETYPE */

  delete this->cond_become_leader_;
}

// Set up listening endpoints.

int
CORBA_ORB::open (void)
{
  // Double check pattern
  if (this->open_called_ == 1)
    return 1;

  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, tao_mon, this->open_lock_, -1);

  if (this->open_called_ == 1)
    return 1;

  this->open_called_ = 1;

  TAO_Server_Strategy_Factory *f =
    this->orb_core_->server_factory ();

  TAO_Acceptor_Registry *ar = this->orb_core_->acceptor_registry ();
  // get a reference to the acceptor_registry!

  // Initialize all the endpoints ... the registry will use the
  // orb_core_ to obtain a list of endpoints and strategies!

  if (ar->open (this->orb_core_) == -1)
    // Need to return an error somehow!!  Maybe set do_exit?
    return -1;

  return 0;
}

void
CORBA_ORB::shutdown (CORBA::Boolean wait_for_completion)
{
  // Set the shutdown flag
  this->should_shutdown (1);

  // Grab the thread manager
  ACE_Thread_Manager *tm = this->orb_core_->thr_mgr ();

  // Try to cancel all the threads in the ORB.
  tm->cancel_all ();

  // Wake up all waiting threads in the reactor.
  this->orb_core_->reactor ()->wakeup_all_threads ();

  // If <wait_for_completion> is set, wait for all threads to exit.
  if (wait_for_completion != 0)
    tm->wait ();

  return;
}

#if !defined (TAO_HAS_MINIMUM_CORBA)

void
CORBA_ORB::create_list (CORBA::Long count,
                        CORBA::NVList_ptr &new_list,
                        CORBA_Environment &)
{
  assert (CORBA::ULong (count) <= UINT_MAX);

  // Create an empty list
  new_list = new CORBA::NVList;

  // If count is greater than 0, create a list of NamedValues.
  if (count != 0)
    {
      new_list->max_ = (CORBA::ULong) count;

      for (CORBA::Long i=0; i < count; i++)
        {
          CORBA::NamedValue_ptr nv = new CORBA::NamedValue;
          new_list->values_.enqueue_tail (nv);
        }
    }
}

int
CORBA_ORB::perform_work (const ACE_Time_Value &tv)
{
  ACE_Reactor *r = this->orb_core_->reactor ();

  // Set the owning thread of the Reactor to the one which we're
  // currently in.  This is necessary b/c it's possible that the
  // application is calling us from a thread other than that in which
  // the Reactor's CTOR (which sets the owner) was called.
  r->owner (ACE_Thread::self ());

  ACE_Time_Value tmp_tv (tv);

  return r->handle_events (tmp_tv);
}

CORBA::Boolean
CORBA_ORB::work_pending (void)
{
  // There's ALWAYS work to do ;-)
  return 1;
}

// The following functions are not implemented - they just throw
// CORBA::NO_IMPLEMENT.

void
CORBA_ORB::create_exception_list (CORBA::ExceptionList_ptr &,
                                  CORBA_Environment &ACE_TRY_ENV)
{
  ACE_THROW (CORBA::NO_IMPLEMENT (TAO_DEFAULT_MINOR_CODE,
                                  CORBA::COMPLETED_NO));
}

void
CORBA_ORB::create_environment (CORBA::Environment_ptr &,
                               CORBA_Environment &ACE_TRY_ENV)
{
  ACE_THROW (CORBA::NO_IMPLEMENT (TAO_DEFAULT_MINOR_CODE,
                                  CORBA::COMPLETED_NO));
}

CORBA::Boolean
CORBA_ORB::get_service_information (CORBA::ServiceType service_type,
                                    CORBA::ServiceInformation_out service_information,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  return 0;
}

void
CORBA_ORB::create_named_value (CORBA::NamedValue_ptr &,
                               CORBA_Environment &ACE_TRY_ENV)
{
  ACE_THROW (CORBA::NO_IMPLEMENT (TAO_DEFAULT_MINOR_CODE,
                                  CORBA::COMPLETED_NO));
}

void
CORBA_ORB::create_context_list (CORBA::ContextList_ptr &,
                                CORBA_Environment &ACE_TRY_ENV)
{
  ACE_THROW (CORBA::NO_IMPLEMENT (TAO_DEFAULT_MINOR_CODE,
                                  CORBA::COMPLETED_NO));
}

void
CORBA_ORB::get_default_context (CORBA::Context_ptr &,
                                CORBA_Environment &ACE_TRY_ENV)
{
  ACE_THROW (CORBA::NO_IMPLEMENT (TAO_DEFAULT_MINOR_CODE,
                                  CORBA::COMPLETED_NO));
}

void
CORBA_ORB::send_multiple_requests_oneway (const CORBA_ORB_RequestSeq,
                                          CORBA_Environment &ACE_TRY_ENV)
{
  ACE_THROW (CORBA::NO_IMPLEMENT (TAO_DEFAULT_MINOR_CODE,
                                  CORBA::COMPLETED_NO));
}

void
CORBA_ORB::send_multiple_requests_deferred (const CORBA_ORB_RequestSeq,
                                            CORBA_Environment &ACE_TRY_ENV)
{
  ACE_THROW (CORBA::NO_IMPLEMENT (TAO_DEFAULT_MINOR_CODE,
                                  CORBA::COMPLETED_NO));
}

void
CORBA_ORB::get_next_response (CORBA_Request_ptr &,
                              CORBA_Environment &ACE_TRY_ENV)
{
  ACE_THROW (CORBA::NO_IMPLEMENT (TAO_DEFAULT_MINOR_CODE,
                                  CORBA::COMPLETED_NO));
}

CORBA::Boolean
CORBA_ORB::poll_next_response (CORBA_Environment &ACE_TRY_ENV)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (TAO_DEFAULT_MINOR_CODE,
                                         CORBA::COMPLETED_NO),
                    0);
}

#endif /* TAO_HAS_MINIMUM_CORBA */

int
CORBA_ORB::run (ACE_Time_Value *tv,
                int break_on_timeouts)
{
  ACE_FUNCTION_TIMEPROBE (TAO_CORBA_ORB_RUN_START);

  {
    ACE_Guard<ACE_SYNCH_MUTEX> g (this->orb_core_->leader_follower_lock ());

    while (this->orb_core_->leader_available ())
      {
        // @@ Why do this on every iteration, shouldn't this be done
        // just once?
        if (this->orb_core_->add_follower (this->cond_become_leader_) == -1)
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) ORB::run: Failed to add a follower thread\n"));
        this->cond_become_leader_->wait ();
      }
    this->orb_core_->set_leader_thread ();
  }

  ACE_Reactor *r = this->orb_core_->reactor ();

  // Set the owning thread of the Reactor to the one which we're
  // currently in.  This is necessary b/c it's possible that the
  // application is calling us from a thread other than that in which
  // the Reactor's CTOR (which sets the owner) was called.
  r->owner (ACE_Thread::self ());

  // This method should only be called by servers, so now we set up
  // for listening!
  if (this->open () == -1)
    return -1;

#if 0
  const int max_iterations = 100;
  int counter = 0;
#endif /* 0 */

  int result = 1;
  // 1 to detect that nothing went wrong

  // Loop "forever" handling client requests.
  while (this->should_shutdown () == 0)
    {
#if 0
      counter++;
      if (counter == max_iterations)
        {
          ACE_TIMEPROBE_PRINT;
          ACE_TIMEPROBE_RESET;
          counter = 0;
        }

      ACE_FUNCTION_TIMEPROBE (TAO_CORBA_ORB_RUN_START);
#endif /* 0 */

      switch (r->handle_events (tv))
        {
        case 0: // Timed out, so we return to caller.
          if (break_on_timeouts)
            result = 0;
          break;
          /* NOTREACHED */
        case -1: // Something else has gone wrong, so return to caller.
          result = -1;
          break;
          /* NOTREACHED */
        default:
          // Some handlers were dispatched, so keep on processing
          // requests until we're told to shutdown .
          break;
          /* NOTREACHED */
        }
      if (result == 0 || result == -1)
        break;
    }

  if (result != -1)
    {
      if (this->orb_core_->unset_leader_wake_up_follower () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) ORB::run: Failed to add a follower thread\n"),
                          -1);
      return 0;
      // nothing went wrong
    }
  else
    return result;
}

int
CORBA_ORB::run (ACE_Time_Value &tv)
{
  return this->run (&tv, 1);
}

int
CORBA_ORB::run (ACE_Time_Value *tv)
{
  return this->run (tv, 1);
}

int
CORBA_ORB::run (void)
{
  return this->run (0, 0);
}

CORBA_Object_ptr
CORBA_ORB::resolve_root_poa (CORBA::Environment &ACE_TRY_ENV,
                             const char *adapter_name,
                             TAO_POA_Manager *poa_manager,
                             const TAO_POA_Policies *policies)
{
  return this->orb_core_->root_poa_reference (ACE_TRY_ENV,
                                              adapter_name,
                                              poa_manager,
                                              policies);
}

CORBA_Object_ptr
CORBA_ORB::resolve_poa_current (CORBA::Environment &ACE_TRY_ENV)
{
  // Return the pointer to this thread's POACurrent.

  TAO_POA_Current *poa_current = this->orb_core_->poa_current ();
  if (poa_current == 0)
    return CORBA_Object::_nil ();

  return poa_current->_this (ACE_TRY_ENV);
}

CORBA_Object_ptr
CORBA_ORB::resolve_policy_manager (CORBA::Environment& ACE_TRY_ENV)
{
#if defined (TAO_HAS_CORBA_MESSAGING)
  TAO_Policy_Manager *policy_manager =
    this->orb_core_->policy_manager ();
  if (policy_manager == 0)
    return CORBA_Object::_nil ();

  return policy_manager->_this (ACE_TRY_ENV);
#else
  return CORBA_Object::_nil ();
#endif /* TAO_HAS_CORBA_MESSAGING */
}

CORBA_Object_ptr
CORBA_ORB::resolve_policy_current (CORBA::Environment& ACE_TRY_ENV)
{
#if defined (TAO_HAS_CORBA_MESSAGING)
  TAO_Policy_Current *policy_current =
    this->orb_core_->policy_current ();
  if (policy_current == 0)
    return CORBA_Object::_nil ();

  return policy_current->_this (ACE_TRY_ENV);
#else
  return CORBA_Object::_nil ();
#endif /* TAO_HAS_CORBA_MESSAGING */
}

CORBA_Object_ptr
CORBA_ORB::resolve_service (const char *service_name,
                            ACE_Time_Value *timeout,
                            CORBA::Environment& ACE_TRY_ENV)
{
  CORBA_Object_ptr return_value = CORBA_Object::_nil ();

  // First check to see if we've already initialized this.
  if (this->name_service_ != CORBA_Object::_nil ())
    {
      // @@ Someone please double-check this ;-)
      return_value = this->name_service_;
    }
  else
    {
      ACE_CString name_service_ior =
        this->orb_core_->orb_params ()->name_service_ior ();

      // Second, check to see if the user has give us a parameter on
      // the command-line.
      if (name_service_ior.length () == 0)
        // Third, check to see if the user has an environment variable.
        name_service_ior = ACE_OS::getenv ("NameServiceIOR");

      if (name_service_ior.length () != 0)
        {
          this->name_service_ =
            this->string_to_object (name_service_ior.c_str (),
                                    ACE_TRY_ENV);

          // check for errors
          if (ACE_TRY_ENV.exception () != 0)
            this->name_service_ = CORBA_Object::_nil ();
        }
      else
        {
          // First, determine if the port was supplied on the command line
          u_short port =
            this->orb_core_->orb_params ()->name_service_port ();

          if (port == 0)
            {
              // Look for the port among our environment variables.
              const char *port_number =
                ACE_OS::getenv ("NameServicePort");

              if (port_number != 0)
                port = ACE_OS::atoi (port_number);
              else
                port = TAO_DEFAULT_NAME_SERVER_REQUEST_PORT;
            }

          this->name_service_ =
            this->multicast_to_service (service_name,
                                        port,
                                        timeout,
                                        ACE_TRY_ENV);
        }
    }

  // Return ior.
  return_value = this->name_service_;
  return CORBA_Object::_duplicate (return_value);
}

CORBA_Object_ptr
CORBA_ORB::resolve_trading_service (ACE_Time_Value *timeout,
                                    CORBA::Environment& ACE_TRY_ENV)
{
  CORBA_Object_ptr return_value = CORBA_Object::_nil ();

  // First check to see if we've already initialized this.
  if (this->trading_service_ != CORBA_Object::_nil ())
    {
      // @@ Someone please double-check this ;-)
      return_value = this->trading_service_;
    }
  else
    {
      ACE_CString trading_service_ior =
        this->orb_core_->orb_params ()->trading_service_ior ();

      // Second, check to see if the user has give us a parameter on
      // the command-line.
      if (trading_service_ior.length () == 0)
        // Third, check to see if the user has an environment variable.
        trading_service_ior = ACE_OS::getenv ("TradingServiceIOR");

      if (trading_service_ior.length () != 0)
        {
          this->trading_service_ =
            this->string_to_object (trading_service_ior.c_str (), ACE_TRY_ENV);

          // check for errors
          if (ACE_TRY_ENV.exception () != 0)
            this->trading_service_ = CORBA_Object::_nil ();
        }
      else
        {
          // First, determine if the port was supplied on the command line
          // @@ FRED: need a generic rep for this!
          u_short port =
            this->orb_core_->orb_params ()->trading_service_port ();

          if (port == 0)
            {
              // Look for the port among our environment variables.
              const char *port_number = ACE_OS::getenv ("TradingServicePort");

              if (port_number != 0)
                port = ACE_OS::atoi (port_number);
              else
                port = TAO_DEFAULT_TRADING_SERVER_REQUEST_PORT;
            }

          this->trading_service_ =
            this->multicast_to_service ("TradingService",
                                        port,
                                        timeout,
                                        ACE_TRY_ENV);
        }
    }

  return_value = this->trading_service_;
  return CORBA_Object::_duplicate (return_value);
}

int
CORBA_ORB::multicast_query (char *buf,
                            const char *service_name,
                            u_short port,
                            ACE_Time_Value *timeout)
{
  ACE_INET_Addr my_addr;
  ACE_SOCK_Acceptor acceptor;
  ACE_SOCK_Stream stream;
  ACE_SOCK_Dgram dgram;

  ssize_t result = 0;

  // Bind listener to any port and then find out what the port was.
  if (acceptor.open (ACE_Addr::sap_any) == -1
      || acceptor.get_local_addr (my_addr) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "acceptor.open () || "
                  "acceptor.get_local_addr () failed"));
      result = -1;
    }
  else
    {
      // This starts out initialized to all zeros!
      ACE_INET_Addr multicast_addr (port,
                                    ACE_DEFAULT_MULTICAST_ADDR);
      // Open the datagram.
      if (dgram.open (ACE_Addr::sap_any) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "Unable to open the Datagram!\n"));
          result = -1;
        }
      else
        {
          // Convert the port we're listening on into network byte
          // order.
          ACE_UINT16 response_port =
            ACE_HTONS (my_addr.get_port_number ());

          // Length of data to be sent. This is sent as a header.
          CORBA::Short data_len =
            ACE_HTONS (sizeof (ACE_UINT16)
                       + ACE_OS::strlen (service_name));

          // Vector to be sent.
          const int iovcnt = 3;
          iovec iovp[iovcnt];

          // The length of data to be sent.
          iovp[0].iov_base = (char *) &data_len;
          iovp[0].iov_len  = sizeof (CORBA::Short);

          // The port at which we are listening.
          iovp[1].iov_base = (char *) &response_port;
          iovp[1].iov_len  = sizeof (ACE_UINT16);

          // The service name string.
          iovp[2].iov_base = (char *) service_name;
          iovp[2].iov_len  = ACE_OS::strlen (service_name);

          // Send the multicast.
          result = dgram.send (iovp,
                               iovcnt,
                               multicast_addr);

          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "\nsent multicast request."));

          // Check for errors.
          if (result == -1)
            ACE_ERROR ((LM_ERROR,
                        "%p\n",
                        "error sending IIOP multicast"));
          else
            {
              if (TAO_debug_level > 0)
                ACE_DEBUG ((LM_DEBUG,
                            "\n%s; Sent multicast."
                            "# of bytes sent is %d.\n",
                            __FILE__,
                            result));
              // Wait for response until
              // TAO_DEFAULT_SERVICE_RESOLUTION_TIMEOUT.
              ACE_Time_Value tv (timeout == 0
                                 ? ACE_Time_Value (TAO_DEFAULT_SERVICE_RESOLUTION_TIMEOUT)
                                 : *timeout);
              // Accept reply connection from server.
              if (acceptor.accept (stream,
                                   0,
                                   &tv) == -1)
                {
                  ACE_ERROR ((LM_ERROR,
                              "%p\n",
                              "multicast_query: unable to accept"));
                  result = -1;
                }
              else
                {
                  // Receive the IOR.
                  result = stream.recv (buf,
                                        BUFSIZ,
                                        0,
                                        timeout);
                  // Close socket now.
                  stream.close ();

                  // Check for errors.
                  if (result == -1)
                    ACE_ERROR ((LM_ERROR,
                                "%p\n",
                                "error reading IIOP multicast response"));
                  else
                    {
                      // Null terminate message.
                      buf[result] = 0;

                      if (TAO_debug_level > 0)
                        ACE_DEBUG ((LM_DEBUG,
                                    "%s: service resolved to IOR <%s>\n",
                                    __FILE__,
                                    buf));
                    }
                }
            }
        }
    }

  // We don't need the dgram or acceptor anymore.
  dgram.close ();
  acceptor.close ();

  return result == -1 ? -1 : 0;
}
// @@ This will have to be sanitized of transport specific calls
//    in order to support pluggable protocols!  But, it does use
//    UDP and multicast.  Not all transport protocols may support
//    this, connectionless and multicast. fredk
// @@ FRED: Should define a flag, something like Protocol_Has_Multicast
//    If there is no multicast, then this functionality is not available
//    and we return NULL.

CORBA_Object_ptr
CORBA_ORB::multicast_to_service (const char * service_name,
                                 u_short port,
                                 ACE_Time_Value *timeout,
                                 CORBA::Environment& ACE_TRY_ENV)
{
  char buf[BUFSIZ + 1];

  // Use UDP multicast to locate the  service.
  CORBA_Object_ptr return_value =
    CORBA_Object::_nil ();

  if (this->multicast_query (buf,
                             service_name,
                             port,
                             timeout) == 0)
    {
      // Convert IOR to an object reference.
      CORBA_Object_ptr objectified_ior =
        this->string_to_object ((CORBA::String) buf,
                                ACE_TRY_ENV);

      // Check for errors.
      if (ACE_TRY_ENV.exception () == 0)
        return_value = objectified_ior;
    }

  // Return ior.
  return return_value;
}

CORBA_Object_ptr
CORBA_ORB::resolve_initial_references (const char *name,
                                       CORBA_Environment &TAO_IN_ENV)
{
  return this->resolve_initial_references (name,
                                           0,
                                           TAO_IN_ENV);
}

CORBA_Object_ptr
CORBA_ORB::resolve_initial_references (const char *name,
                                       ACE_Time_Value *timeout,
                                       CORBA_Environment &ACE_TRY_ENV)
{
  // Get the table of initial references specified through
  // -ORBInitRef.
  TAO_IOR_LookupTable *table =
    this->orb_core_->orb_params ()->ior_lookup_table ();

  ACE_CString ior;
  ACE_CString object_id ((const char *) name);

  // Is the service name in the IOR Table.
  if (table->find_ior (object_id, ior) == 0)
    return this->string_to_object (ior.c_str (), ACE_TRY_ENV);
  else
    {
      // Get the list of initial reference prefixes specified through
      // -ORBDefaultInitRef.
      char * default_init_ref =
        this->orb_core_->orb_params ()->default_init_ref ();

      // Check if a DefaultInitRef was specified.
      if (ACE_OS::strlen (default_init_ref) != 0)
        {
          ACE_CString list_of_profiles;

          // Used by the strtok_r.
          char *lasts = 0;

          // Append the given object ID to all the end-points of
          // Default Init Ref.
          for (char *str = ACE_OS::strtok_r (default_init_ref,
                                             ",",
                                             &lasts);
               str != 0 ;
               str = ACE_OS::strtok_r (0,
                                       ",",
                                       &lasts))
            {
              list_of_profiles += ACE_CString (str);
              list_of_profiles += ACE_CString ("/");
              list_of_profiles += object_id;
              list_of_profiles += ACE_CString (",");
            }

          // Clean up.
          delete [] default_init_ref;

          // Replace the last extra comma with a null.
          list_of_profiles[list_of_profiles.length () - 1] = '\0';

          return this->string_to_object (list_of_profiles.c_str (),
                                         ACE_TRY_ENV);
        }

      delete default_init_ref;
    }

  if (ACE_OS::strcmp (name, TAO_OBJID_NAMESERVICE) == 0)
    return this->resolve_service ("NameService", timeout, ACE_TRY_ENV);

  else if (ACE_OS::strcmp (name, TAO_OBJID_TRADINGSERVICE) == 0)
    return this->resolve_trading_service (timeout, ACE_TRY_ENV);

  else if (ACE_OS::strcmp (name, TAO_OBJID_ROOTPOA) == 0)
    return this->resolve_root_poa (ACE_TRY_ENV);

  else if (ACE_OS::strcmp (name, TAO_OBJID_POACURRENT) == 0)
    return this->resolve_poa_current (ACE_TRY_ENV);

  else if (ACE_OS::strcmp (name, TAO_OBJID_POLICYMANAGER) == 0)
    return this->resolve_policy_manager (ACE_TRY_ENV);

  else if (ACE_OS::strcmp (name, TAO_OBJID_POLICYCURRENT) == 0)
    return this->resolve_policy_current (ACE_TRY_ENV);

  else
    return this->resolve_service (name, timeout, ACE_TRY_ENV);
}

// Unimplemented at this time.
CORBA_ORB_ObjectIdList_ptr
CORBA_ORB::list_initial_services (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (TAO_DEFAULT_MINOR_CODE,
                                         CORBA::COMPLETED_NO),
                    0);
}

TAO_Stub *
CORBA_ORB::create_stub_object (const TAO_ObjectKey &key,
                               const char *type_id,
                               CORBA::Environment &ACE_TRY_ENV)
{
  if (this->open () == -1)
    ACE_THROW_RETURN (CORBA::INTERNAL (), 0);

  CORBA::String id = 0;

  if (type_id)
    id = CORBA::string_dup (type_id);

  TAO_Stub *stub = 0;

  // First we create a profile list, well actually a list of one!
  // @@ should go to the acceptor for this, the orb delegates to the acceptor
  // to create Profiles!
  TAO_MProfile mp (1);

  TAO_ORB_Parameters *orb_params =
    this->orb_core_->orb_params ();

  // @@ Ug, broken.  Again, we need to go to the acceptor registry
  //    (when we got one) for this!!! [fredk]
  // @@ Fred&Ossama: You guys have one now ;-)  I still believe that
  //    the right way to do this is something along these lines:
  //
  // size_t pfile_count =
  //     this->orb_core_->acceptor_registry ()->count_profiles ();
  // TAO_MProfile mp (pfile_count);
  // this->orb_core_->acceptor_registry ()->fill_mprofile (key); 
  //
  //    What do you think?

  TAO_IIOP_Profile *pfile;
  ACE_NEW_THROW_EX (pfile,
                    TAO_IIOP_Profile (orb_params->host (),
                                      orb_params->addr ().get_port_number (),
                                      key,
                                      orb_params->addr ()),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_MAYBE));
  ACE_CHECK_RETURN (stub);
  // we give up the profile to the mp object.  If the mp object is deleted,
  // the pfile will be taken care of (reference decremented by one!).
  mp.give_profile (pfile);

  ACE_NEW_THROW_EX (stub,
                    TAO_Stub (id, mp, this->orb_core_),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE));
  ACE_CHECK_RETURN (stub);

  return stub;
}

// Create an objref

CORBA::Object_ptr
CORBA_ORB::key_to_object (const TAO_ObjectKey &key,
                          const char *type_id,
                          CORBA::Environment &env)
{
  TAO_Stub *data = this->create_stub_object (key, type_id, env);

  if (env.exception () != 0)
    return CORBA::Object::_nil ();

  // Create the CORBA level proxy
  CORBA_Object *new_obj = new CORBA_Object (data);

  // Clean up in case of errors.
  if (CORBA::is_nil (new_obj))
    {
      data->_decr_refcnt ();
      env.exception (new CORBA::INTERNAL ());
      return CORBA::Object::_nil ();
    }

  return new_obj;
}

// Get access to the leader_follower_info.

TAO_Leader_Follower_Info &
CORBA_ORB::leader_follower_info (void)
{
  return leader_follower_info_;
}

#if !defined (TAO_HAS_MINIMUM_CORBA)

// Dynamic Any factory functions.

ACE_INLINE
CORBA_DynAny_ptr
CORBA_ORB::create_dyn_any       (const CORBA_Any& any,
                                 CORBA::Environment& env)
{
  return TAO_DynAny_i::create_dyn_any (any, env);
}

ACE_INLINE
CORBA_DynAny_ptr
CORBA_ORB::create_basic_dyn_any (CORBA_TypeCode_ptr tc,
                                 CORBA::Environment& env)
{
  return TAO_DynAny_i::create_dyn_any (tc, env);
}

ACE_INLINE
CORBA_DynStruct_ptr
CORBA_ORB::create_dyn_struct    (CORBA_TypeCode_ptr tc,
                                 CORBA::Environment& env)
{
  return TAO_DynAny_i::create_dyn_struct (tc, env);
}

ACE_INLINE
CORBA_DynSequence_ptr
CORBA_ORB::create_dyn_sequence  (CORBA_TypeCode_ptr tc,
                                 CORBA::Environment& env)
{
  return TAO_DynAny_i::create_dyn_sequence (tc, env);
}

ACE_INLINE
CORBA_DynArray_ptr
CORBA_ORB::create_dyn_array     (CORBA_TypeCode_ptr tc,
                                 CORBA::Environment& env)
{
  return TAO_DynAny_i::create_dyn_array (tc, env);
}

ACE_INLINE
CORBA_DynUnion_ptr
CORBA_ORB::create_dyn_union     (CORBA_TypeCode_ptr tc,
                                 CORBA::Environment& env)
{
  return TAO_DynAny_i::create_dyn_union (tc, env);
}

ACE_INLINE
CORBA_DynEnum_ptr
CORBA_ORB::create_dyn_enum      (CORBA_TypeCode_ptr tc,
                                 CORBA::Environment& env)
{
  return TAO_DynAny_i::create_dyn_enum (tc, env);
}

#endif /* TAO_HAS_MINIMUM_CORBA */

// ****************************************************************

// String utility support; this needs to be integrated with the ORB's
// own memory allocation subsystem.

CORBA::Char*
CORBA::string_copy (const CORBA::Char *str)
{
  return CORBA::string_dup (str);
}

CORBA_String_var::CORBA_String_var (char *p)
  : ptr_ (p)
{
  // NOTE: According to the CORBA spec this string must *not* be
  // copied, but it is non-compliant to use it/release it in the
  // calling code.  argument is consumed. p should never be NULL
}

CORBA_String_var::CORBA_String_var (const CORBA_String_var& r)
{
  this->ptr_ = CORBA::string_dup (r.ptr_);
}

CORBA_String_var::~CORBA_String_var (void)
{
  if (this->ptr_ != 0)
    {
      CORBA::string_free (this->ptr_);
      this->ptr_ = 0;
    }
}

CORBA_String_var &
CORBA_String_var::operator= (char *p)
{
  if (this->ptr_ != p)
    {
      if (this->ptr_ != 0)
        CORBA::string_free (this->ptr_);
      this->ptr_ = p;
    }
  return *this;
}

CORBA_String_var &
CORBA_String_var::operator= (const char *p)
{
  if (this->ptr_ != 0)
    CORBA::string_free (this->ptr_);

  this->ptr_ = CORBA::string_dup (p);
  return *this;
}

CORBA_String_var &
CORBA_String_var::operator= (const CORBA_String_var& r)
{
  if (this != &r)
    {
      if (this->ptr_ != 0)
        CORBA::string_free (this->ptr_);
      this->ptr_ = CORBA::string_dup (r.ptr_);
    }
  return *this;
}

// ****************************************************************

CORBA_WString_var::CORBA_WString_var (CORBA::WChar *p)
  : ptr_ (p)
{
  // NOTE: According to the CORBA spec this string must *not* be
  // copied, but it is non-compliant to use it/release it in the
  // calling code.  argument is consumed. p should never be NULL
}

CORBA_WString_var::CORBA_WString_var (const CORBA_WString_var& r)
{
  this->ptr_ = CORBA::wstring_dup (r.ptr_);
}

CORBA_WString_var::~CORBA_WString_var (void)
{
  if (this->ptr_ != 0)
    {
      CORBA::wstring_free (this->ptr_);
      this->ptr_ = 0;
    }
}

CORBA_WString_var &
CORBA_WString_var::operator= (CORBA::WChar *p)
{
  if (this->ptr_ != p)
    {
      if (this->ptr_ != 0)
        CORBA::wstring_free (this->ptr_);
      this->ptr_ = p;
    }
  return *this;
}

CORBA_WString_var &
CORBA_WString_var::operator= (const CORBA::WChar *p)
{
  if (this->ptr_ != 0)
    CORBA::wstring_free (this->ptr_);

  this->ptr_ = CORBA::wstring_dup (p);
  return *this;
}

CORBA_WString_var &
CORBA_WString_var::operator= (const CORBA_WString_var& r)
{
  if (this != &r)
    {
      if (this->ptr_ != 0)
        CORBA::wstring_free (this->ptr_);
      this->ptr_ = CORBA::wstring_dup (r.ptr_);
    }
  return *this;
}

// ****************************************************************

void
CORBA_ORB::init_orb_globals (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_MT (ACE_GUARD (ACE_Recursive_Thread_Mutex, tao_mon, *ACE_Static_Object_Lock::instance ()));

  // Put these initializations here so that exceptions are enabled
  // immediately.

  if (CORBA_ORB::orb_init_count_ == 0)
    {
      // initialize the system TypeCodes
      TAO_TypeCodes::init ();

      // initialize the factory for marshaling
      TAO_Marshal::init ();

      // initialize the interpreter
      TAO_CDR_Interpreter::init ();

      // initialize the system exceptions
      TAO_Exceptions::init (ACE_TRY_ENV);
      ACE_CHECK;

      // Verify some of the basic implementation requirements.  This
      // test gets optimized away by a decent compiler (or else the
      // rest of the routine does).
      //
      // NOTE:  we still "just" assume that native floating point is
      // IEEE.

      if (sizeof (CORBA::Short) != 2
          || sizeof (CORBA::Long) != 4
          || sizeof (CORBA::LongLong) != 8
          || sizeof (CORBA::Float) != 4
          || sizeof (CORBA::Double) != 8
          || sizeof (CORBA::LongDouble) != 16
          || sizeof (CORBA::WChar) < 2
          || sizeof (void *) != ACE_SIZEOF_VOID_P)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "%s; ERROR: unexpected basic type size; "
                      "s:%d l:%d ll:%d f:%d d:%d ld:%d wc:%d v:%d\n"
                      "please reconfigure TAO\n",
                      __FILE__,
                      sizeof (CORBA::Short),
                      sizeof (CORBA::Long),
                      sizeof (CORBA::LongLong),
                      sizeof (CORBA::Float),
                      sizeof (CORBA::Double),
                      sizeof (CORBA::LongDouble),
                      sizeof (CORBA::WChar),
                      sizeof (void *)));

          ACE_THROW (CORBA::INITIALIZE ());
        }
    }
  CORBA_ORB::orb_init_count_++;
}

// ORB initialisation, per OMG document 94-9-46.
//

CORBA::ORB_ptr
CORBA::ORB_init (int &argc,
                 char *const *argv,
                 const char * orb_name)
{
  return CORBA::ORB_init (argc, argv, orb_name,
                          CORBA::default_environment ());
}

CORBA::ORB_ptr
CORBA::ORB_init (int &argc,
                 char *const *argv,
                 const char * /* orb_name */,
                 CORBA::Environment &ACE_TRY_ENV)
{
  // Using ACE_Static_Object_Lock::instance() precludes <ORB_init>
  // from being called within a static object CTOR.
  ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard,
                            *ACE_Static_Object_Lock::instance (), 0));

  // Get ORB Core
  // @@ As part of the ORB re-architecture this will the point where
  //    we locate the right ORB (from a table) and use that one
  //    instead of just creating a new one every time.
  TAO_ORB_Core *oc;

  ACE_NEW_RETURN (oc, TAO_ORB_Core, CORBA::ORB::_nil ());

  // Initialize the ORB Core instance.
  int result = oc->init (argc, (char **) argv);

  // Check for errors and return 0 if error.
  if (result == -1)
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);
    }

  // This (init_orb_globals) must come *after* ORB Core initialization.
  // Make sure initialization of TAO globals only occurs once.
  CORBA_ORB::init_orb_globals (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  // @@ We may only want to set this if ORB_init() has 0 argc/argv
  // parameters.
  if (CORBA::instance_ == 0)
    CORBA::instance_ = oc->orb ();

  return oc->orb ();
}

// ****************************************************************

// Objref stringification.
CORBA::String
CORBA_ORB::object_to_string (CORBA::Object_ptr obj,
                             CORBA::Environment &env)
{
  env.clear ();

  // Application writer controls what kind of objref strings they get,
  // maybe along with other things, by how they initialize the ORB.

  if (use_omg_ior_format_)
    {
      // By default, orbs use IOR strings; these are ugly (and error
      // prone) but specified by CORBA.
      //
      // XXX there should be a simple way to reuse this code in other
      // ORB implementations ...

      // @@ Is BUFSIZ the right size here?
      char buf [BUFSIZ];
      TAO_OutputCDR cdr (buf,
                         sizeof buf,
                         TAO_ENCAP_BYTE_ORDER);

      // support limited oref ACE_OS::strcmp.
      (void) ACE_OS::memset (buf, 0, BUFSIZ);

      // Marshal the objref into an encapsulation bytestream.
      (void) cdr.write_octet (TAO_ENCAP_BYTE_ORDER);
      if ((cdr << obj) == 0)
        return 0;

      // Now hexify the encapsulated CDR data into a string, and
      // return that string.

      CORBA::String cp;
      size_t len = cdr.length ();

      CORBA::String string;
      ACE_ALLOCATOR_RETURN (string,
                            CORBA::string_alloc (sizeof ior_prefix + 2 * len),
                            0);

      ACE_OS::strcpy ((char *) string,
                      ior_prefix);

      const char *bytes = cdr.buffer ();

      for (cp = (CORBA::String) ACE_OS::strchr ((char *) string, ':') + 1;
           len--;
           bytes++)
        {
          *cp++ = ACE::nibble2hex ((*bytes) >> 4);
          *cp++ = ACE::nibble2hex (*bytes);
        }

      *cp = 0;
      return string;
    }
  else
    {
      // The "internet" ORB uses readable URL style objrefs, as used
      // in the World Wide Web.
      //
      // This only works for IIOP objrefs.  If we're handed an objref
      // that's not an IIOP objref, fail -- application must use an
      // ORB that's configured differently.  @@ Is this true? FRED

      if (obj->_stubobj () == 0)
        return CORBA::string_copy ((CORBA::String) TAO_IIOP_Profile::prefix ());
        // @@ This should be some sort of default prefix, not
        // hardcoded to IIOP!! FRED

      CORBA::String buf =
        obj->_stubobj ()->profile_in_use ()->to_string (env);
      return buf;
    }
}

// Destringify arbitrary objrefs.  This method is called from
// <resolve_name_service> with an IOR <multicast_to_service>.

CORBA::Object_ptr
CORBA_ORB::string_to_object (const char *str,
                             CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Object_ptr obj = CORBA::Object::_nil ();

  if (ACE_OS::strncmp (str,
                       file_prefix,
                       sizeof file_prefix - 1) == 0)
    obj = this->file_string_to_object (str + sizeof file_prefix - 1,
                                       ACE_TRY_ENV);
  else if (ACE_OS::strncmp (str,
                            ior_prefix,
                            sizeof ior_prefix - 1) == 0)
    obj = this->ior_string_to_object (str + sizeof ior_prefix - 1,
                                      ACE_TRY_ENV);
  else
    { 
      // @@ Fred&Ossama: Is there anyway to initialize the mprofile in 
      //    such a way that it does not allocate memory?
      //    The connector registry could count how many profiles are
      //    there (if any) and then allocate all the memory in one
      //    call, saving precious microseconds in an area of the code
      //    that is invoked only once ;-)

      TAO_MProfile mprofile (1);
      // It is safe to declare this on the stack since the contents of
      // mprofile get copied.

      if (this->orb_core_->connector_registry ()->make_mprofile (str,
                                                                 mprofile,
                                                                 ACE_TRY_ENV)
          != 0)
        {
          ACE_THROW_RETURN (CORBA::MARSHAL (), CORBA::Object::_nil ());
        }

      // Now make the TAO_Stub.
      TAO_Stub *data;
      ACE_NEW_RETURN (data,
                      TAO_Stub ((char *) 0, mprofile, this->orb_core_),
                      CORBA::Object::_nil ());

      // Create the CORBA level proxy.
      TAO_ServantBase *servant = this->_get_collocated_servant (data);

      // This will increase the ref_count on data by one
      ACE_NEW_RETURN (obj,
                      CORBA_Object (data,
                                    servant,
                                    servant != 0),
                      CORBA::Object::_nil ());
    }

  return obj;
}

// ****************************************************************

// Destringify OMG-specified "IOR" string.
//
// XXX there should be a simple way to reuse this code in other ORB
// implementations ...

CORBA::Object_ptr
CORBA_ORB::ior_string_to_object (const char *str,
                                 CORBA::Environment &ACE_TRY_ENV)
{
  // Unhex the bytes, and make a CDR deencapsulation stream from the
  // resulting data.
  ACE_Message_Block mb (ACE_OS::strlen ((char *) str) / 2 + 1
                        + ACE_CDR::MAX_ALIGNMENT + 1);

  ACE_CDR::mb_align (&mb);

  char *buffer = mb.rd_ptr ();

  const char *tmp = str;
  size_t len = 0;

  while (tmp [0] && tmp [1])
    {
      u_char byte;

      if (!(isxdigit (tmp [0]) && isxdigit (tmp [1])))
        break;

      byte = (u_char) (ACE::hex2byte (tmp [0]) << 4);
      byte |= ACE::hex2byte (tmp [1]);

      buffer [len++] = byte;
      tmp += 2;
    }

  if (tmp [0] && !isspace (tmp [0]))
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (),
                        CORBA::Object::_nil ());
    }

  // Create deencapsulation stream ... then unmarshal objref from that
  // stream.

  int byte_order = *(mb.rd_ptr ());
  mb.rd_ptr (1);
  mb.wr_ptr (len);
  TAO_InputCDR stream (&mb, byte_order, this->orb_core_);

  CORBA::Object_ptr objref = CORBA::Object::_nil ();
  stream >> objref;

  return objref;
}

CORBA::Object_ptr
CORBA_ORB::file_string_to_object (const char* filename,
                                  CORBA::Environment& ACE_TRY_ENV)
{
  FILE* file = ACE_OS::fopen (filename, "r");

  if (file == 0)
    return CORBA::Object::_nil ();

  ACE_Read_Buffer reader (file, 1);

  char* string = reader.read ();

  if (string == 0)
    return CORBA::Object::_nil ();

  CORBA::Object_ptr object = CORBA::Object::_nil ();
  ACE_TRY
    {
      object = this->string_to_object (string, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      reader.alloc ()->free (string);
    }
  ACE_CATCHANY
    {
      reader.alloc ()->free (string);
      ACE_RETHROW;
    }
  ACE_ENDTRY;

  return object;
}

// ****************************************************************

TAO_ServantBase *
CORBA_ORB::_get_collocated_servant (TAO_Stub *sobj)
{
  // ACE_DEBUG ((LM_DEBUG, "CORBA_ORB: get_collocated_servant\n"));

  if (this->optimize_collocation_objects_ && sobj != 0)
    {
      TAO_Profile *pfile = sobj->profile_in_use ();

      // Make sure users passed in a valid TAO_Stub otherwise, we
      // don't know what to do next.
      if (pfile == 0)
        {
          // @@ Fred, can you please either keep these debugging
          // statements in or remove them, but please don't leave the
          // #if 0's around!
#if 0
          ACE_ERROR ((LM_ERROR,
                      "%p: Passing IIOP ORB and non-IIOP object\n",
                      "_get_collocated_object"));
#endif
          // Something must be wrong!
          return 0;
        }
#if 0
      ACE_DEBUG ((LM_DEBUG,
                  "CORBA_ORB: checking collocation for <%s>\n",
                  pfile->addr_to_string ()));
#endif

      // @@EXC@@ We should receive the <env> from the command line.
      // @@ Fred: why do we need an environment for the
      // Profile::_key() method?

      CORBA::Environment env;
      TAO_ObjectKey_var objkey = pfile->_key (env);

      if (env.exception ())
        {
#if 0
          ACE_DEBUG ((LM_DEBUG,
                      "CORBA_ORB: cannot find key for <%s>\n",
                      pfile->addr_to_string ()));
#endif
          return 0;
        }

      // Check if the object requested is a collocated object.
      // @@ FRED - can we make this more generic!!
      // @@ Fred: this is a question that the Acceptor registry must
      //    answer, in other words: we should go to the connector
      //    registry with the Stub object and ask if that object is
      //    collocated or not.  Then we simply lookup in the POA to
      //    find the servant.
      //    How does the Acceptor Registry answer the question:
      //    - For each profile in the Stub it iterates over the
      //      acceptors, if the profile and the acceptor have the same
      //      protocol then we pass the profile to the acceptor.
      //    - The acceptor downcast the profile, the derived class
      //      contains a method that returns the address in the
      //      favourite format for that protocol (i.e. this is not a
      //      method of the base class); it uses that address to match
      //      with the addresses it is accepting on.
      //    - A policy question: do we return when the first acceptor
      //      matches or if they all match?
      //      Before answering: "only when they all match" think about
      //      a server that is restarted with more endpoints (or
      //      less), shouldn't old, persistent IORs, still be treated
      //      as collocated?
      TAO_Object_Adapter *object_adapter = 0;
      if (pfile->tag () == TAO_IOP_TAG_INTERNET_IOP)
        {
          const ACE_INET_Addr &addr =
            ACE_dynamic_cast (TAO_IIOP_Profile *, pfile)->object_addr ();

          object_adapter = this->orb_core_->get_collocated_object_adapter (addr);
        }
      else
        ACE_ERROR ((LM_ERROR,
                    "get_collocated_poa NOT Supported for NON-IIOP profile!\n"));

      if (object_adapter != 0)
        {
          PortableServer::Servant servant =
            object_adapter->find_servant (objkey.in (), env);
          if (env.exception ())
            {
#if 0
              ACE_DEBUG ((LM_DEBUG,
                          "CORBA_ORB: cannot find servant for <%s>\n",
                          pfile->addr_to_string ()));
#endif
              return 0;
            }

#if 0
          ACE_DEBUG ((LM_DEBUG,
                      "CORBA_ORB: object at <%s> is collocated\n",
                      pfile->addr_to_string ()));
#endif
          return servant;
        }
    }

#if 0
  ACE_DEBUG ((LM_DEBUG,
              "CORBA_ORB: collocation failed for \n"));
#endif

  return 0;
}

// ****************************************************************

// Add a mapping ObjectID->IOR to the table.
int
CORBA_ORB::_tao_add_to_IOR_table (ACE_CString &object_id,
                                  CORBA::Object_ptr obj)
{
  if (CORBA::is_nil (obj))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to add IOR to table\n"),
                      -1);

  CORBA::String_var string =
    this->object_to_string (obj);

  if (string.in () == 0 || string.in ()[0] == '\0')
    return -1;

  ACE_CString ior (string.in ());

  if (this->lookup_table_.add_ior (object_id, ior) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to add IOR to table\n"),
                      -1);

  return 0;
}

// Find an IOR in the table for the given ObjectID.
int
CORBA_ORB::_tao_find_in_IOR_table (ACE_CString &object_id,
                                   CORBA::Object_ptr &obj)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "TAO (%P|%t): lookup service ID <%s>\n",
                object_id.c_str ()));

  ACE_CString ior;

  if (this->lookup_table_.find_ior (object_id, ior) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "No match for the given ObjectID\n"),
                      -1);

  obj = this->string_to_object (ior.c_str ());

  return 0;
}

// ****************************************************************

CORBA::ORB_ptr
CORBA::instance (void)
{
  if (CORBA::instance_ == 0)
    {
      ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard,
                                *ACE_Static_Object_Lock::instance (), 0));
      if (CORBA::instance_ == 0)
        {
          int argc = 0;
          char *const *argv = 0;

          // Note that CORBA::ORB_init() will also acquire the static
          // lock, but that's ok since it's a recursive lock.
          CORBA::Environment ACE_TRY_ENV;
          CORBA::instance_ = CORBA::ORB_init (argc, argv, "",
                                              ACE_TRY_ENV);
        }
    }
  return CORBA::instance_;
}

void
CORBA::instance (CORBA::ORB_ptr orb)
{
  ACE_MT (ACE_GUARD (ACE_SYNCH_RECURSIVE_MUTEX, guard,
                     *ACE_Static_Object_Lock::instance ()));
  CORBA::instance_ = orb;
}

// *************************************************************
// Inline operators for TAO_opaque encoding and decoding
// *************************************************************

CORBA::Boolean
operator<< (TAO_OutputCDR& cdr, const TAO_opaque& x)
{
  CORBA::ULong length = x.length ();
  cdr.write_ulong (length);
#if !defined (TAO_NO_COPY_OCTET_SEQUENCES)
  cdr.write_octet_array (x.get_buffer (), length);
#else
  if (x.mb () == 0)
    cdr.write_octet_array (x.get_buffer (), length);
  else
    cdr.write_octet_array_mb (x.mb ());
#endif /* TAO_NO_COPY_OCTET_SEQUENCES */
  return cdr.good_bit ();
}

CORBA::Boolean
operator>>(TAO_InputCDR& cdr, TAO_opaque& x)
{
  CORBA::ULong length;
  cdr.read_ulong (length);
#if !defined (TAO_NO_COPY_OCTET_SEQUENCES)
  x.length (length);
  cdr.read_octet_array (x.get_buffer (), length);
#else
  x.replace (length, cdr.start ());
  cdr.skip_bytes (length);
#endif /* TAO_NO_COPY_OCTET_SEQUENCES */
  return cdr.good_bit ();
}

// *************************************************************
// Valuetype factory operations
// *************************************************************

#ifdef TAO_HAS_VALUETYPE

CORBA::ValueFactory_ptr
CORBA_ORB::register_value_factory (
                            const char *repository_id,
                            CORBA::ValueFactory_ptr factory,
                            CORBA_Environment &ACE_TRY_ENV)
{
// %! guard, and ACE_Null_Mutex in the map
// do _add_ref here not in map->rebind

  if (valuetype_factory_map_ == 0)
    {
      // currently the ValueFactory_Map is a singleton and not per ORB
      // as in the OMG specs
      valuetype_factory_map_ = TAO_VALUEFACTORY_MAP::instance ();
      if (valuetype_factory_map_ == 0)
        ACE_THROW_RETURN (CORBA::INTERNAL (), 0);
    }

  int result = valuetype_factory_map_->rebind (repository_id, factory);
  if (result == -1)
    {
      // Error on bind.
      ACE_THROW_RETURN (CORBA::INTERNAL (), 0);
    }
  if (result == 1)
    {
      return factory;    // previous factory was found
    }
  return 0;
}

void
CORBA_ORB::unregister_value_factory (const char * repository_id,
                                     CORBA_Environment &ACE_TRY_ENV)
{
  ACE_ERROR((LM_ERROR, "(%N:%l) function not implemented\n"));
  // %! TODO
}

CORBA::ValueFactory_ptr
CORBA_ORB::lookup_value_factory (const char *repository_id,
                                 CORBA_Environment &ACE_TRY_ENV)
{
// %! guard
// do _add_ref here not in map->find
  if (valuetype_factory_map_)
    {
      CORBA::ValueFactory_ptr factory;
      int result = valuetype_factory_map_->find (repository_id, factory);
      if (result == -1)
        factory = 0;  // %! raise exception !
      return factory;
    }
  else
    {
      return 0; // %! raise exception !
    }
}

#endif /* TAO_HAS_VALUETYPE */


// ****************************************************************

// @@ Fred&Ossama: I'm convinced that you ended up moving template
//    instantiations from here to the Connect.cpp file.

#define CACHED_CONNECT_STRATEGY ACE_Cached_Connect_Strategy<TAO_Client_Connection_Handler, TAO_SOCK_CONNECTOR, TAO_Cached_Connector_Lock>
#define REFCOUNTED_HASH_RECYCLABLE_ADDR ACE_Refcounted_Hash_Recyclable<ACE_INET_Addr>

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#if !defined (TAO_NO_COPY_OCTET_SEQUENCES)
template class TAO_Unbounded_Sequence<CORBA::Octet>;
#endif /* defined (TAO_NO_COPY_OCTET_SEQUENCES) */

template class ACE_Dynamic_Service<TAO_Server_Strategy_Factory>;
template class ACE_Dynamic_Service<TAO_Client_Strategy_Factory>;
template class ACE_Guard<TAO_Cached_Connector_Lock>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#if !defined (TAO_NO_COPY_OCTET_SEQUENCES)
#pragma instantiate TAO_Unbounded_Sequence<CORBA::Octet>
#endif /* defined (TAO_NO_COPY_OCTET_SEQUENCES) */

#pragma instantiate ACE_Dynamic_Service<TAO_Server_Strategy_Factory>
#pragma instantiate ACE_Dynamic_Service<TAO_Client_Strategy_Factory>
#pragma instantiate ACE_Guard<TAO_Cached_Connector_Lock>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
