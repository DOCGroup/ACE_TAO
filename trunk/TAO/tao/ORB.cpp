// $Id$

#include "tao/ORB.h"
#include "tao/Connector_Registry.h"

#include "ace/Dynamic_Service.h"
#include "ace/Service_Repository.h"
#include "ace/Object_Manager.h"
#include "ace/SOCK_Dgram_Mcast.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Thread_Manager.h"
#include "ace/Read_Buffer.h"
#include "ace/Auto_Ptr.h"
#include "ace/Arg_Shifter.h"

#include "tao/Object.h"
#include "tao/Typecode.h"
#include "tao/NVList.h"
#include "tao/Stub.h"
#include "tao/DynAny_i.h"
#include "tao/ORB_Core.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/debug.h"
#include "tao/TAO_Internal.h"
#include "tao/CDR.h"
#include "tao/IOR_LookupTable.h"
#include "tao/Object_Adapter.h"
#include "tao/POA.h"
#include "tao/Request.h"
#include "tao/MProfile.h"
#include "tao/Object_Loader.h"

#include "tao/RT_ORB.h"
#include "tao/Priority_Mapping_Manager.h"
#include "tao/RT_Current.h"

#if defined (TAO_HAS_VALUETYPE)
#  include "tao/ValueFactory_Map.h"
#endif /* TAO_HAS_VALUETYPE */

#include "tao/Messaging_Policy_i.h"
#include "tao/Client_Priority_Policy.h"
#include "tao/Buffering_Constraint_Policy.h"

#if defined (ACE_HAS_EXCEPTIONS)
# if defined (ACE_MVS)
#   include /**/ <unexpect.h>
# else
#  if defined (ACE_HAS_STANDARD_CPP_LIBRARY)
#   include /**/ <exception>
#   if !defined (_MSC_VER)
using std::set_unexpected;
#   endif /* !_MSC_VER */
#  else
#   include /**/ <exception.h>
#  endif /* ACE_HAS_STANDARD_CPP_LIBRARY */
# endif /* ACE_MVS */
#endif /* ACE_HAS_EXCEPTIONS */

#if !defined (__ACE_INLINE__)
# include "tao/ORB.i"
#endif /* ! __ACE_INLINE__ */


ACE_RCSID(tao, ORB, "$Id$")

static const char ior_prefix [] = "IOR:";
static const char file_prefix[] = "file://";
static const char dll_prefix[] = "DLL:";

// = Static initialization.

// Count of the number of ORBs.
int CORBA_ORB::orb_init_count_ = 0;

CORBA::Boolean
operator<< (TAO_OutputCDR &strm,
            const CORBA::ORB::InvalidName &_tao_aggregate)
{
  // first marshal the repository ID
  if (strm << _tao_aggregate._id ())
    return 1;
  else
    return 0;
}

CORBA::Boolean operator>> (TAO_InputCDR &,
                           CORBA::ORB::InvalidName &)
{
  return 1;
}

CORBA_ORB::InvalidName::InvalidName (void)
  : CORBA_UserException ("IDL:omg.org/CORBA/ORB/InvalidName:1.0")
{
}

CORBA::ORB::InvalidName::~InvalidName (void)
{
}

CORBA::ORB::InvalidName::InvalidName (const CORBA::ORB::InvalidName &_tao_excp)
  : CORBA_UserException (_tao_excp._id ())
{
}

// assignment operator
CORBA::ORB::InvalidName&
CORBA::ORB::InvalidName::operator= (const CORBA::ORB::InvalidName &_tao_excp)
{

  this->CORBA_UserException::operator= (_tao_excp);
  return *this;
}

CORBA_ORB::InvalidName*
CORBA_ORB::InvalidName::_downcast (CORBA_Exception *ex)
{
  if (!ACE_OS::strcmp ("IDL:omg.org/CORBA/ORB/InvalidName:1.0", ex->_id ()))
    return ACE_dynamic_cast (CORBA::ORB::InvalidName*, ex);
  else
    return 0;
}

void CORBA_ORB::InvalidName::_raise (void)
{
  TAO_RAISE(*this);
}

void CORBA_ORB::InvalidName::_tao_encode (TAO_OutputCDR &cdr,
                                          CORBA::Environment &ACE_TRY_ENV) const
{
  if (cdr << *this)
    return;
  ACE_THROW (CORBA::MARSHAL ());
}

void CORBA_ORB::InvalidName::_tao_decode (TAO_InputCDR &cdr,
                                          CORBA::Environment &ACE_TRY_ENV)
{
  if (cdr >> *this)
    return;
  ACE_THROW (CORBA::MARSHAL ());
}

// TAO extension - the _alloc method
CORBA::Exception *
CORBA::ORB::InvalidName::_alloc (void)
{
  CORBA::ORB::InvalidName *retval = 0;
  ACE_NEW_RETURN (retval,
                  CORBA::ORB::InvalidName,
                  0);
  return retval;
}

CORBA_ORB::CORBA_ORB (TAO_ORB_Core *orb_core)
  : lock_ (),
    refcount_ (1),
    orb_core_ (orb_core),
# if defined (TAO_HAS_VALUETYPE)
    valuetype_factory_map_ (0),
# endif /* TAO_HAS_VALUETYPE */
# if (TAO_HAS_INTERCEPTORS == 1)
    client_interceptor_ (),
    server_interceptor_ (),
# endif /* TAO_HAS_INTERCEPTORS */
    lookup_table_ (),
    ior_manipulation_ (),
    use_omg_ior_format_ (1)
{
}

CORBA_ORB::~CORBA_ORB (void)
{
  // This destructor is only invoked when the last ORB reference (not
  // instance) is being destroyed.

# ifdef TAO_HAS_VALUETYPE
  // delete valuetype_factory_map_;
  // @@ not really, its a singleton
# endif /* TAO_HAS_VALUETYPE */
}

void
CORBA_ORB::shutdown (CORBA::Boolean wait_for_completion,
                     CORBA::Environment &ACE_TRY_ENV)
{
  this->check_shutdown (ACE_TRY_ENV);
  ACE_CHECK;

  this->orb_core ()->shutdown (wait_for_completion,
                               ACE_TRY_ENV);
  ACE_CHECK;
}

void
CORBA_ORB::destroy (CORBA::Environment &ACE_TRY_ENV)
{
  if (this->orb_core () == 0)
    {
      // If the ORB_Core pointer is zero, assume that the ORB_Core has
      // been destroyed.

      // As defined by the CORBA 2.3 specification, throw a
      // CORBA::OBJECT_NOT_EXIST exception if the ORB has been
      // destroyed by the time an ORB function is called.

      ACE_THROW (CORBA::OBJECT_NOT_EXIST (TAO_DEFAULT_MINOR_CODE,
                                          CORBA::COMPLETED_NO));
    }

  if (TAO_debug_level >= 3)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("CORBA::ORB::destroy() has been called on ORB <%s>.\n"),
                  this->orb_core ()->orbid ()));
    }

  this->orb_core ()->destroy (ACE_TRY_ENV);
  ACE_CHECK;

  // Now invalidate the pointer to the ORB_Core that created this
  // ORB.
  this->orb_core_ = 0;
}

int
CORBA_ORB::run (CORBA::Environment &ACE_TRY_ENV)
{
  return this->run (0, ACE_TRY_ENV);
}

int
CORBA_ORB::run (ACE_Time_Value &tv, CORBA::Environment &ACE_TRY_ENV)
{
  return this->run (&tv, ACE_TRY_ENV);
}

int
CORBA_ORB::run (ACE_Time_Value *tv,
                CORBA::Environment &ACE_TRY_ENV)
{
  this->check_shutdown (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  return this->orb_core ()->run (tv, 0, ACE_TRY_ENV);
}

int
CORBA_ORB::perform_work (CORBA::Environment &ACE_TRY_ENV)
{
  return this->perform_work (0, ACE_TRY_ENV);
}

int
CORBA_ORB::perform_work (ACE_Time_Value &tv, CORBA::Environment &ACE_TRY_ENV)
{
  return this->perform_work (&tv, ACE_TRY_ENV);
}

int
CORBA_ORB::perform_work (ACE_Time_Value *tv,
                         CORBA::Environment &ACE_TRY_ENV)
{
  // This method should not be called if the ORB has been shutdown.
  this->check_shutdown (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  return this->orb_core ()->run (tv, 1, ACE_TRY_ENV);
}

CORBA::Boolean
CORBA_ORB::work_pending (CORBA_Environment &ACE_TRY_ENV)
{
  // This method should not be called if the ORB has been shutdown.
  this->check_shutdown (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  int result = this->orb_core_->reactor ()->work_pending ();
  if (result == 0)
    return 0;

  if (result == -1)
    ACE_THROW_RETURN (CORBA::INTERNAL (), 0);

  return 1;
}

#if (TAO_HAS_MINIMUM_CORBA == 0)

void
CORBA_ORB::create_list (CORBA::Long count,
                        CORBA::NVList_ptr &new_list,
                        CORBA_Environment &ACE_TRY_ENV)
{
  ACE_ASSERT (CORBA::ULong (count) <= UINT_MAX);

  // Create an empty list
  ACE_NEW_THROW_EX (new_list,
                    CORBA::NVList,
                    CORBA::NO_MEMORY (
                      CORBA_SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  // If count is greater than 0, create a list of NamedValues.
  if (count != 0)
    {
      new_list->max_ = (CORBA::ULong) count;

      for (CORBA::Long i=0; i < count; i++)
        {
          CORBA::NamedValue_ptr nv = 0;
          ACE_NEW_THROW_EX (nv,
                            CORBA::NamedValue,
                            CORBA::NO_MEMORY (
                              CORBA_SystemException::_tao_minor_code (
                                TAO_DEFAULT_MINOR_CODE,
                                ENOMEM),
                              CORBA::COMPLETED_NO));
          ACE_CHECK;

          new_list->values_.enqueue_tail (nv);
        }
    }
}

// The following functions are not implemented - they just throw
// CORBA::NO_IMPLEMENT.

void
CORBA_ORB::create_exception_list (CORBA::ExceptionList_ptr &list,
                                  CORBA_Environment &ACE_TRY_ENV)
{
  ACE_NEW_THROW_EX (list, CORBA::ExceptionList (),
                    CORBA::NO_MEMORY (
                      CORBA_SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
}

void
CORBA_ORB::create_environment (CORBA::Environment_ptr &environment,
                               CORBA_Environment &ACE_TRY_ENV)
{
  ACE_NEW_THROW_EX (environment, CORBA::Environment (),
                    CORBA::NO_MEMORY (
                      CORBA_SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
}

CORBA::Boolean
CORBA_ORB::get_service_information (CORBA::ServiceType /* service_type */,
                                    CORBA::ServiceInformation_out /* service_information */,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA_SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    0);
}

void
CORBA_ORB::create_named_value (CORBA::NamedValue_ptr &nv,
                               CORBA_Environment &ACE_TRY_ENV)
{
  ACE_NEW_THROW_EX (nv,
                    CORBA::NamedValue,
                    CORBA::NO_MEMORY (
                      CORBA_SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
}

void
CORBA_ORB::create_context_list (CORBA::ContextList_ptr &,
                                CORBA_Environment &ACE_TRY_ENV)
{
  ACE_THROW (CORBA::NO_IMPLEMENT (
               CORBA_SystemException::_tao_minor_code (
                 TAO_DEFAULT_MINOR_CODE,
                 ENOTSUP),
               CORBA::COMPLETED_NO));
}

void
CORBA_ORB::get_default_context (CORBA::Context_ptr &,
                                CORBA_Environment &ACE_TRY_ENV)
{
  ACE_THROW (CORBA::NO_IMPLEMENT (
               CORBA_SystemException::_tao_minor_code (
                 TAO_DEFAULT_MINOR_CODE,
                 ENOTSUP),
               CORBA::COMPLETED_NO));
}

void
CORBA_ORB::send_multiple_requests_oneway (const CORBA_ORB_RequestSeq,
                                          CORBA_Environment &ACE_TRY_ENV)
{
  ACE_THROW (CORBA::NO_IMPLEMENT (
               CORBA_SystemException::_tao_minor_code (
                 TAO_DEFAULT_MINOR_CODE,
                 ENOTSUP),
               CORBA::COMPLETED_NO));
}

void
CORBA_ORB::send_multiple_requests_deferred (const CORBA_ORB_RequestSeq,
                                            CORBA_Environment &ACE_TRY_ENV)
{
  ACE_THROW (CORBA::NO_IMPLEMENT (
               CORBA_SystemException::_tao_minor_code (
                 TAO_DEFAULT_MINOR_CODE,
                 ENOTSUP),
               CORBA::COMPLETED_NO));
}

void
CORBA_ORB::get_next_response (CORBA_Request_ptr &,
                              CORBA_Environment &ACE_TRY_ENV)
{
  ACE_THROW (CORBA::NO_IMPLEMENT (
               CORBA_SystemException::_tao_minor_code (
                 TAO_DEFAULT_MINOR_CODE,
                 ENOTSUP),
               CORBA::COMPLETED_NO));
}

CORBA::Boolean
CORBA_ORB::poll_next_response (CORBA_Environment &ACE_TRY_ENV)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA_SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    0);
}

#endif /* TAO_HAS_MINIMUM_CORBA */

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
CORBA_ORB::resolve_poa_current (CORBA::Environment &)
{
  // Return the pointer to the POA Current.
  return CORBA_Object::_duplicate (&this->orb_core_->poa_current ());
}

CORBA_Object_ptr
CORBA_ORB::resolve_policy_manager (CORBA::Environment &)
{

#if (TAO_HAS_CORBA_MESSAGING == 1)

  TAO_Policy_Manager *policy_manager =
    this->orb_core_->policy_manager ();
  if (policy_manager == 0)
    return CORBA_Object::_nil ();

  return CORBA_Object::_duplicate (policy_manager);

#else

  return CORBA_Object::_nil ();

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

}

CORBA_Object_ptr
CORBA_ORB::resolve_rt_current (CORBA::Environment &/*ACE_TRY_ENV*/)
{

#if (TAO_HAS_RT_CORBA == 1)

  return this->orb_core_->rt_current ();

#else

  return CORBA_Object::_nil ();

#endif /* TAO_HAS_RT_CORBA == 1 */

}

CORBA_Object_ptr
CORBA_ORB::resolve_priority_mapping_manager (CORBA::Environment &/*ACE_TRY_ENV*/)
{

#if (TAO_HAS_RT_CORBA == 1)

    return this->orb_core_->priority_mapping_manager ();

#else

  return CORBA_Object::_nil ();

#endif /* TAO_HAS_RT_CORBA == 1 */

}

CORBA_Object_ptr
CORBA_ORB::resolve_rt_orb (CORBA::Environment &/*ACE_TRY_ENV*/)
{

#if (TAO_HAS_RT_CORBA == 1)

  return this->orb_core_->rt_orb ();

#else

  return CORBA_Object::_nil ();

#endif /* TAO_HAS_RT_CORBA == 1 */

}

CORBA_Object_ptr
CORBA_ORB::resolve_policy_current (CORBA::Environment &)
{

#if (TAO_HAS_CORBA_MESSAGING == 1)

  TAO_Policy_Current &policy_current = this->orb_core_->policy_current ();
  return CORBA_Object::_duplicate (&policy_current);

#else

  return CORBA_Object::_nil ();

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

}

CORBA_Object_ptr
CORBA_ORB::resolve_ior_manipulation (CORBA::Environment&)
{
  return TAO_IOP::TAO_IOR_Manipulation::_duplicate (&this->ior_manipulation_);
}

CORBA_Object_ptr
CORBA_ORB::resolve_service (MCAST_SERVICEID mcast_service_id,
                            ACE_Time_Value *timeout,
                            CORBA::Environment& ACE_TRY_ENV)
{
  const char * env_service_ior [] =
  {
    "NameServiceIOR",
    "TradingServiceIOR",
    "ImplRepoServiceIOR",
    "InterfaceRepoServiceIOR"
  };

  const char * env_service_port [] =
  {
    "NameServicePort",
    "TradingServicePort",
    "ImplRepoServicePort",
    "InterfaceRepoServicePort"
  };

 u_short default_service_port [] =
 {
   TAO_DEFAULT_NAME_SERVER_REQUEST_PORT,
   TAO_DEFAULT_TRADING_SERVER_REQUEST_PORT,
   TAO_DEFAULT_IMPLREPO_SERVER_REQUEST_PORT,
   TAO_DEFAULT_INTERFACEREPO_SERVER_REQUEST_PORT
 };

 const char * service_objid [] =
 {
   TAO_OBJID_NAMESERVICE,
   TAO_OBJID_TRADINGSERVICE,
   TAO_OBJID_IMPLREPOSERVICE,
   TAO_OBJID_INTERFACEREP
 };

 CORBA_Object_var return_value = CORBA_Object::_nil ();

 // By now, the table filled in with -ORBInitRef arguments has been
 // checked.  We only get here if the table didn't contain an initial
 // reference for the requested Service.

 // Check to see if the user has an environment variable.
 ACE_CString service_ior = ACE_OS::getenv (env_service_ior[mcast_service_id]);

 if (service_ior.length () != 0)
   {
     return_value =
       this->string_to_object (service_ior.c_str (),
                               ACE_TRY_ENV);
     ACE_CHECK_RETURN (CORBA_Object::_nil ());
   }
 else
   {
     // First, determine if the port was supplied on the command line
     u_short port =
       this->orb_core_->orb_params ()->service_port (mcast_service_id);

     if (port == 0)
       {
         // Look for the port among our environment variables.
         const char *port_number =
           ACE_OS::getenv (env_service_port[mcast_service_id]);

         if (port_number != 0)
           port = (u_short) ACE_OS::atoi (port_number);
         else
           port = default_service_port[mcast_service_id];
       }

     return_value =
       this->multicast_to_service (service_objid[mcast_service_id],
                                   port,
                                   timeout,
                                   ACE_TRY_ENV);
     ACE_CHECK_RETURN (CORBA_Object::_nil ());
   }

 // Return ior.
 return return_value._retn ();
}

int
CORBA_ORB::multicast_query (char *&buf,
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
                  ACE_TEXT ("acceptor.open () || ")
                  ACE_TEXT ("acceptor.get_local_addr () failed")));
      result = -1;
    }
  else
    {
      ACE_INET_Addr multicast_addr (port,
                                    ACE_DEFAULT_MULTICAST_ADDR);
      // Set the address if multicast_discovery_endpoint option
      // is specified for the Naming Service.
      ACE_CString mde (this->orb_core_->orb_params ()
                       ->mcast_discovery_endpoint ());

      if (ACE_OS::strcasecmp (service_name,
                              "NameService") == 0
          && mde.length () != 0)
        if (multicast_addr.set (mde.c_str()) == -1)
          {
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT("ORB.cpp: Multicast address setting failed\n")));
            stream.close ();
            dgram.close ();
            acceptor.close ();
            return -1;
          }

      // Open the datagram.
      if (dgram.open (ACE_Addr::sap_any) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Unable to open the Datagram!\n")));
          result = -1;
        }
      else
        {
          // Convert the acceptor port into network byte order.
          ACE_UINT16 response_port =
            (ACE_UINT16) ACE_HTONS (my_addr.get_port_number ());

          // Length of service name we will send.
          CORBA::Short data_len =
            (CORBA::Short) ACE_HTONS (ACE_OS::strlen (service_name) + 1);

          // Vector we will send.  It contains: 1) length of service
          // name string, 2)port on which we are listening for
          // replies, and 3) name of service we are looking for.
          const int iovcnt = 3;
          iovec iovp[iovcnt];

          // The length of service name string.
          iovp[0].iov_base = (char *) &data_len;
          iovp[0].iov_len  = sizeof (CORBA::Short);

          // The port at which we are listening.
          iovp[1].iov_base = (char *) &response_port;
          iovp[1].iov_len  = sizeof (ACE_UINT16);

          // The service name string.
          iovp[2].iov_base = (char *) service_name;
          iovp[2].iov_len  = ACE_OS::strlen (service_name) + 1;

          // Send the multicast.
          result = dgram.send (iovp,
                               iovcnt,
                               multicast_addr);

          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("\nsent multicast request.")));

          // Check for errors.
          if (result == -1)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("%p\n"),
                        ACE_TEXT ("error sending IIOP multicast")));
          else
            {
              if (TAO_debug_level > 0)
                ACE_DEBUG ((LM_DEBUG,
                            ACE_TEXT ("\n%s; Sent multicast.")
                            ACE_TEXT ("# of bytes sent is %d.\n"),
                            __FILE__,
                            result));
              // Wait for response until timeout.
              ACE_Time_Value tv (
                timeout == 0
                ? ACE_Time_Value (TAO_DEFAULT_SERVICE_RESOLUTION_TIMEOUT)
                : *timeout);

              // Accept reply connection from server.
              if (acceptor.accept (stream,
                                   0,
                                   &tv) == -1)
                {
                  ACE_ERROR ((LM_ERROR,
                              ACE_TEXT ("%p\n"),
                              ACE_TEXT ("multicast_query: unable to accept")));
                  result = -1;
                }
              else
                {
                  // Receive the IOR.

                  // IOR length.
                  CORBA::Short ior_len;
                  result = stream.recv_n (&ior_len,
                                          sizeof ior_len,
                                          0,
                                          &tv);
                  if (result != sizeof (ior_len))
                    {
                      ACE_ERROR ((LM_ERROR,
                                  ACE_TEXT ("%p\n"),
                                  ACE_TEXT ("multicast_query: unable to receive ")
                                  ACE_TEXT ("ior length")));
                      result = -1;
                    }
                  else
                    {
                      // Allocate more space for the ior if we don't
                      // have enough.
                      ior_len = (CORBA::Short) ACE_NTOHS (ior_len);
                      if (ior_len > TAO_DEFAULT_IOR_SIZE)
                        {
                          buf = CORBA::string_alloc (ior_len);
                          if (buf == 0)
                            {
                              ACE_ERROR ((LM_ERROR,
                                          ACE_TEXT ("%p\n"),
                                          ACE_TEXT ("multicast_query: unable to ")
                                          ACE_TEXT ("allocate memory")));
                              result = -1;
                            }
                        }

                      if (result != -1)
                        {
                          // Receive the ior.
                          result = stream.recv_n (buf,
                                                  ior_len,
                                                  0,
                                                  &tv);
                          if (result == -1)
                            ACE_ERROR ((LM_ERROR,
                                        ACE_TEXT ( "%p\n"),
                                        ACE_TEXT ("error reading ior")));
                          else if (TAO_debug_level > 0)
                            ACE_DEBUG ((LM_DEBUG,
                                        ACE_TEXT ("%s: service resolved to IOR <%s>\n"),
                                        __FILE__,
                                        buf));
                        }
                    }
                }
            }
        }
    }

  // Clean up.
  stream.close ();
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
CORBA_ORB::multicast_to_service (const char *service_name,
                                 u_short port,
                                 ACE_Time_Value *timeout,
                                 CORBA::Environment& ACE_TRY_ENV)
{
  char buf[TAO_DEFAULT_IOR_SIZE];
  char *ior = buf;
  CORBA::String_var cleaner;

  CORBA_Object_var return_value =
    CORBA_Object::_nil ();

  // Use UDP multicast to locate the  service.
  int result = this->multicast_query (ior,
                                      service_name,
                                      port,
                                      timeout);

  // If the IOR didn't fit into <buf>, memory for it was dynamically
  // allocated - make sure it gets deallocated.
  if (ior != buf)
    cleaner = ior;

  if (result == 0)
    {
      // Convert IOR to an object reference.
      return_value =
        this->string_to_object (ior,
                                ACE_TRY_ENV);
      ACE_CHECK_RETURN (CORBA_Object::_nil ());
    }

  // Return object reference.
  return return_value._retn ();
}

CORBA_Object_ptr
CORBA_ORB::resolve_initial_references (const char *name,
                                       CORBA_Environment &ACE_TRY_ENV)
{
  return this->resolve_initial_references (name,
                                           0,
                                           ACE_TRY_ENV);
}

CORBA_Object_ptr
CORBA_ORB::resolve_initial_references (const char *name,
                                       ACE_Time_Value *timeout,
                                       CORBA_Environment &ACE_TRY_ENV)
{
  // This method should not be called if the ORB has been shutdown.
  this->check_shutdown (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  if (ACE_OS::strcmp (name, TAO_OBJID_ROOTPOA) == 0)
    return this->resolve_root_poa (ACE_TRY_ENV);

  else if (ACE_OS::strcmp (name, TAO_OBJID_POACURRENT) == 0)
    return this->resolve_poa_current (ACE_TRY_ENV);

  else if (ACE_OS::strcmp (name, TAO_OBJID_POLICYMANAGER) == 0)
    return this->resolve_policy_manager (ACE_TRY_ENV);

  else if (ACE_OS::strcmp (name, TAO_OBJID_POLICYCURRENT) == 0)
    return this->resolve_policy_current (ACE_TRY_ENV);

  else if (ACE_OS::strcmp (name, TAO_OBJID_IORMANIPULATION) == 0)
    return this->resolve_ior_manipulation (ACE_TRY_ENV);

  else if (ACE_OS::strcmp (name, TAO_OBJID_TYPECODEFACTORY) == 0)
    {
      CORBA::Object_var obj =
        this->orb_core ()->typecode_factory ();
      if (CORBA::is_nil (obj.in ()))
        {
          ACE_Service_Config::process_directive (
              "dynamic TypeCodeFactory Service_Object * TypeCodeFactory_DLL:_make_TCF_Loader()"
            );

          CORBA::Object_var tf =
            this->dll_string_to_object (name, ACE_TRY_ENV);
          ACE_CHECK_RETURN (CORBA::Object::_nil ());

          obj = CORBA::Object::_duplicate (tf.in ());
          this->orb_core ()->typecode_factory (tf._retn ());
        }

      return obj._retn ();
    }

  else if (ACE_OS::strcmp (name, TAO_OBJID_RTORB) == 0)
    return this->resolve_rt_orb (ACE_TRY_ENV);

  else if (ACE_OS::strcmp (name, TAO_OBJID_PRIORITYMAPPINGMANAGER) == 0)
    return this->resolve_priority_mapping_manager (ACE_TRY_ENV);

  else if (ACE_OS::strcmp (name, TAO_OBJID_RTCURRENT) == 0)
    return this->resolve_rt_current (ACE_TRY_ENV);

  // Is not one of the well known services, try to find it in the
  // InitRef table....

  // Get the table of initial references specified through
  // -ORBInitRef.
  TAO_IOR_LookupTable *table =
    this->orb_core ()->orb_params ()->ior_lookup_table ();

  ACE_CString ior;
  ACE_CString object_id ((const char *) name);

  // Is the service name in the IOR Table.
  if (table->find_ior (object_id, ior) == 0)
    return this->string_to_object (ior.c_str (), ACE_TRY_ENV);
  if (this->lookup_table_.find_ior (object_id, ior) == 0)
    return this->string_to_object (ior.c_str (), ACE_TRY_ENV);
  else
    {
      // Get the list of initial reference prefixes specified through
      // -ORBDefaultInitRef.
      char * default_init_ref =
        this->orb_core ()->orb_params ()->default_init_ref ();

      // Check if a DefaultInitRef was specified.
      if (ACE_OS::strlen (default_init_ref) != 0)
        {
          ACE_CString list_of_profiles (default_init_ref);

          // Clean up.
          delete [] default_init_ref;

          // Obtain the appropriate object key delimiter for the
          // specified protocol.
          const char object_key_delimiter =
            this->orb_core ()->connector_registry ()->object_key_delimiter (
                                                  list_of_profiles.c_str ());

          // Make sure that the default initial reference doesn't end
          // with the object key delimiter character.
          if (list_of_profiles[list_of_profiles.length() - 1] !=
              object_key_delimiter)
            list_of_profiles += ACE_CString (object_key_delimiter);

          list_of_profiles += object_id;

          return this->string_to_object (list_of_profiles.c_str (),
                                         ACE_TRY_ENV);
        }
      else
        {
          // Clean up.
          delete [] default_init_ref;
        }
    }

  // Did not find it in the InitRef table, or in the DefaultInitRef
  // entry.... Try the hard-coded ways to find the basic services...

  if (ACE_OS::strcmp (name, TAO_OBJID_NAMESERVICE) == 0)
    return this->resolve_service (NAMESERVICE, timeout, ACE_TRY_ENV);

  else if (ACE_OS::strcmp (name, TAO_OBJID_TRADINGSERVICE) == 0)
    return this->resolve_service (TRADINGSERVICE, timeout, ACE_TRY_ENV);

  else if (ACE_OS::strcmp (name, TAO_OBJID_IMPLREPOSERVICE) == 0)
    return this->resolve_service (IMPLREPOSERVICE, timeout, ACE_TRY_ENV);

  else if (ACE_OS::strcmp (name, TAO_OBJID_INTERFACEREP) == 0)
    return this->resolve_service (INTERFACEREPOSERVICE, timeout, ACE_TRY_ENV);

  else
    ACE_THROW_RETURN (CORBA::ORB::InvalidName (), CORBA::Object::_nil ());

}

CORBA_ORB_ObjectIdList_ptr
CORBA_ORB::list_initial_services (CORBA::Environment &ACE_TRY_ENV)
{
  this->check_shutdown (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  TAO_IOR_LookupTable *table =
    this->orb_core ()->orb_params ()->ior_lookup_table ();

  return table->list_initial_services (ACE_TRY_ENV);
}

TAO_Stub *
CORBA_ORB::create_stub_object (const TAO_ObjectKey &key,
                               const char *type_id,
                               CORBA::PolicyList *policy_list,
                               CORBA::Environment &ACE_TRY_ENV)
{
  this->check_shutdown (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  return this->orb_core_->create_stub_object (key,
                                              type_id,
                                              policy_list,
                                              ACE_TRY_ENV);
}

// Create an objref

CORBA::Object_ptr
CORBA_ORB::key_to_object (const TAO_ObjectKey &key,
                          const char *type_id,
                          CORBA::PolicyList *policy_list,
                          TAO_ServantBase *servant,
                          CORBA::Boolean collocated,
                          CORBA::Environment &ACE_TRY_ENV)
{
  this->check_shutdown (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  TAO_Stub *data = this->create_stub_object (key,
                                             type_id,
                                             policy_list,
                                             ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  TAO_Stub_Auto_Ptr safe_data (data);

  // Create the CORBA level proxy
  CORBA::Object_var new_obj =
    this->orb_core_->optimize_collocation_objects () ?
    new CORBA::Object (safe_data.get (), servant, collocated) :
    new CORBA::Object (safe_data.get (), 0, 0);

  // Clean up in case of errors.
  if (CORBA::is_nil (new_obj.in ()))
    ACE_THROW_RETURN (CORBA::INTERNAL (), CORBA::Object::_nil ());

  // @@ Do not duplicate the ORB here!  TAO_Stub::servant_orb()
  //    duplicates it.
  //       -Ossama
  safe_data.get ()->servant_orb (this);

  data = safe_data.release ();

  return new_obj._retn ();
}

void
CORBA_ORB::check_shutdown (CORBA_Environment &ACE_TRY_ENV)
{
  if (this->orb_core () != 0)
    {
      this->orb_core ()->check_shutdown (ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      // If the ORB_Core pointer is zero, assume that the ORB_Core has
      // been destroyed.

      // As defined by the CORBA 2.3 specification, throw a
      // CORBA::OBJECT_NOT_EXIST exception if the ORB has been
      // destroyed by the time an ORB function is called.

      ACE_THROW (CORBA::OBJECT_NOT_EXIST (TAO_DEFAULT_MINOR_CODE,
                                          CORBA::COMPLETED_NO));
    }
}

#if (TAO_HAS_MINIMUM_CORBA == 0)

// Dynamic Any factory functions.

CORBA_DynAny_ptr
CORBA_ORB::create_dyn_any       (const CORBA_Any& any,
                                 CORBA::Environment& ACE_TRY_ENV)
{
  return TAO_DynAny_i::create_dyn_any (any, ACE_TRY_ENV);
}

CORBA_DynAny_ptr
CORBA_ORB::create_basic_dyn_any (CORBA_TypeCode_ptr tc,
                                 CORBA::Environment& ACE_TRY_ENV)
{
  return TAO_DynAny_i::create_dyn_any (tc, ACE_TRY_ENV);
}

CORBA_DynStruct_ptr
CORBA_ORB::create_dyn_struct    (CORBA_TypeCode_ptr tc,
                                 CORBA::Environment& ACE_TRY_ENV)
{
  return TAO_DynAny_i::create_dyn_struct (tc, ACE_TRY_ENV);
}

CORBA_DynSequence_ptr
CORBA_ORB::create_dyn_sequence  (CORBA_TypeCode_ptr tc,
                                 CORBA::Environment& ACE_TRY_ENV)
{
  return TAO_DynAny_i::create_dyn_sequence (tc, ACE_TRY_ENV);
}

CORBA_DynArray_ptr
CORBA_ORB::create_dyn_array     (CORBA_TypeCode_ptr tc,
                                 CORBA::Environment& ACE_TRY_ENV)
{
  return TAO_DynAny_i::create_dyn_array (tc, ACE_TRY_ENV);
}

CORBA_DynUnion_ptr
CORBA_ORB::create_dyn_union     (CORBA_TypeCode_ptr tc,
                                 CORBA::Environment& ACE_TRY_ENV)
{
  return TAO_DynAny_i::create_dyn_union (tc, ACE_TRY_ENV);
}

CORBA_DynEnum_ptr
CORBA_ORB::create_dyn_enum      (CORBA_TypeCode_ptr tc,
                                 CORBA::Environment& ACE_TRY_ENV)
{
  return TAO_DynAny_i::create_dyn_enum (tc, ACE_TRY_ENV);
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
  CORBA::string_free (this->ptr_);
  this->ptr_ = 0;
}

CORBA_String_var &
CORBA_String_var::operator= (char *p)
{
  if (this->ptr_ != p)
    {
      CORBA::string_free (this->ptr_);
      this->ptr_ = p;
    }
  return *this;
}

CORBA_String_var &
CORBA_String_var::operator= (const char *p)
{
  CORBA::string_free (this->ptr_);

  this->ptr_ = CORBA::string_dup (p);
  return *this;
}

CORBA_String_var &
CORBA_String_var::operator= (const CORBA_String_var& r)
{
  if (this != &r)
    {
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
  CORBA::wstring_free (this->ptr_);
  this->ptr_ = 0;
}

CORBA_WString_var &
CORBA_WString_var::operator= (CORBA::WChar *p)
{
  if (this->ptr_ != p)
    {
      CORBA::wstring_free (this->ptr_);
      this->ptr_ = p;
    }
  return *this;
}

CORBA_WString_var &
CORBA_WString_var::operator= (const CORBA::WChar *p)
{
  CORBA::wstring_free (this->ptr_);

  this->ptr_ = CORBA::wstring_dup (p);
  return *this;
}

CORBA_WString_var &
CORBA_WString_var::operator= (const CORBA_WString_var& r)
{
  if (this != &r)
    {
      CORBA::wstring_free (this->ptr_);
      this->ptr_ = CORBA::wstring_dup (r.ptr_);
    }
  return *this;
}

// ****************************************************************

void
CORBA_ORB::init_orb_globals (CORBA::Environment &ACE_TRY_ENV)
{
  // This method should be invoked atomically.  It is the caller's
  // responsibility to ensure that this condition is satisfied.

  // Prevent multiple initializations.
  if (CORBA::ORB::orb_init_count_ != 0)
    return;
  else
    CORBA::ORB::orb_init_count_++;

#if defined(ACE_HAS_EXCEPTIONS)
  set_unexpected (CORBA_ORB::_tao_unexpected_exception);
#endif /* ACE_HAS_EXCEPTIONS */

  // initialize the system TypeCodes
  TAO_TypeCodes::init ();

  // initialize the system exceptions
  TAO_Exceptions::init (ACE_TRY_ENV);
  ACE_CHECK;

  // Verify some of the basic implementation requirements.  This test
  // gets optimized away by a decent compiler (or else the rest of the
  // routine does).
  //
  // NOTE:  we still "just" assume that native floating point is IEEE.
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

void CORBA_ORB::_tao_unexpected_exception (void)
{
#if defined(ACE_HAS_EXCEPTIONS)
  throw CORBA::UNKNOWN ();
#else
  // Nothing to do, this will be handled by the ORB core when sending
  // the exception back to the client...
#endif /* ACE_HAS_EXCEPTIONS */
}

// ****************************************************************

// ORB initialization, per OMG document 98-12-01.

CORBA::ORB_ptr
CORBA::ORB_init (int &argc,
                 char *argv[],
                 const char *orb_name)
{
  return CORBA::ORB_init (argc,
                          argv,
                          orb_name,
                          TAO_default_environment ());
}

CORBA::ORB_ptr
CORBA::ORB_init (int &argc,
                 char *argv[],
                 const char *orbid,
                 CORBA_Environment &ACE_TRY_ENV)
{
  // Using ACE_Static_Object_Lock::instance() precludes <ORB_init>
  // from being called within a static object CTOR.
  ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard,
                            *ACE_Static_Object_Lock::instance (),
                            CORBA::ORB::_nil ()));

  // Make sure TAO's singleton manager is initialized.
  if (TAO_Singleton_Manager::instance ()->init () == -1)
    return CORBA::ORB::_nil ();

  CORBA_ORB::init_orb_globals (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::ORB::_nil ());

  // Make sure the following is done after the global ORB
  // initialization since we need to have exceptions initialized.

  // It doesn't make sense for argc to be zero and argv to be
  // non-empty/zero, or for argc to be greater than zero and argv be
  // zero.
  size_t argv0_len = (argv ? (*argv ? ACE_OS::strlen (*argv) : 0) : 0);
  if ((argc == 0 && argv0_len != 0) ||
      (argc != 0 && (argv == 0 || argv[0] == 0)))
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (
                          CORBA_SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            EINVAL),
                          CORBA::COMPLETED_NO),
                        CORBA::ORB::_nil ());
    }

  if (orbid == 0 || ACE_OS::strcmp (orbid, "") == 0)
    {
      orbid = "";
      ACE_Arg_Shifter arg_shifter (argc, argv);
      while (arg_shifter.is_anything_left ())
        {
          char *current_arg = arg_shifter.get_current ();

          const char orbid_opt[] = "-ORBid";
          const int orbid_len = sizeof (orbid_opt) - 1;
          if (ACE_OS::strcasecmp (current_arg,
                                  orbid_opt) == 0)
            {
              arg_shifter.consume_arg ();
              if (arg_shifter.is_parameter_next ())
                {
                  orbid = arg_shifter.get_current ();
                  arg_shifter.consume_arg ();
                }
            }
          else if (ACE_OS::strncasecmp (current_arg, orbid_opt,
                                        orbid_len) == 0)
            {
              arg_shifter.consume_arg ();
              // The rest of the argument is the ORB id...
              orbid = orbid_opt + orbid_len;
              // but we should skip an optional space...
              if (orbid[0] == ' ')
                orbid++;
            }
          else
            arg_shifter.ignore_arg ();
        }
    }

  // Get ORB Core
  TAO_ORB_Core *oc =
    TAO_ORB_Table::instance ()->find (orbid);

  // The ORB was initialized already, just return that one!
  if (oc != 0)
    {
      if (oc->has_shutdown ())
        {
          // As defined by the CORBA 2.3 specification, throw a
          // CORBA::BAD_INV_ORDER exception with minor code 4 if the ORB
          // has shutdown by the time an ORB function is called.

          // @@ Does the BAD_INV_ORDER exception apply here?
          //       -Ossama

          ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (4, CORBA::COMPLETED_NO),
                            CORBA::ORB::_nil ());
        }

      return CORBA::ORB::_duplicate (oc->orb ());

    }

  // An ORB corresponding to the desired ORBid doesn't exist so create
  // a new one.
  ACE_NEW_THROW_EX (oc,
                    TAO_ORB_Core (orbid),
                    CORBA::NO_MEMORY (
                      CORBA_SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::ORB::_nil ());

  TAO_ORB_Core_Auto_Ptr safe_oc (oc);

  // Initialize the ORB Core instance.
  int result = safe_oc.get ()->init (argc, argv, ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::ORB::_nil ());

  // Check for errors and return 0 if error.
  if (result == -1)
      ACE_THROW_RETURN (CORBA::BAD_PARAM (), CORBA::ORB::_nil ());

  if (TAO_debug_level >= 3)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO (%P|%t) created new ORB <%s>\n"),
                orbid));

  // Before returning remember to store the ORB into the table...
  if (TAO_ORB_Table::instance ()->bind (orbid, oc) != 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (TAO_DEFAULT_MINOR_CODE,
                                       CORBA::COMPLETED_NO),
                      CORBA::ORB::_nil ());

  // Return a duplicate since the ORB_Core should release the last
  // reference to the ORB.
  return CORBA::ORB::_duplicate (oc->orb ());
}

// ****************************************************************

// Objref stringification.
char *
CORBA_ORB::object_to_string (CORBA::Object_ptr obj,
                             CORBA::Environment &ACE_TRY_ENV)
{
  // This method should not be called if the ORB has been shutdown.
  this->check_shutdown (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  // Application writer controls what kind of objref strings they get,
  // maybe along with other things, by how they initialize the ORB.

  if (use_omg_ior_format_)
    {
      // By default, orbs use IOR strings; these are ugly (and error
      // prone) but specified by CORBA.
      //
      // XXX there should be a simple way to reuse this code in other
      // ORB implementations ...

      char buf [ACE_CDR::DEFAULT_BUFSIZE];
#if defined(ACE_HAS_PURIFY)
      (void) ACE_OS::memset (buf, '\0', sizeof(buf));
#endif /* ACE_HAS_PURIFY */

      TAO_OutputCDR cdr (buf,  sizeof buf,
                         TAO_ENCAP_BYTE_ORDER,
                         this->orb_core_->output_cdr_buffer_allocator (),
                         this->orb_core_->output_cdr_dblock_allocator (),
                         this->orb_core_->orb_params ()->cdr_memcpy_tradeoff (),
                         this->orb_core_->to_iso8859 (),
                         this->orb_core_->to_unicode ());

      // support limited oref ACE_OS::strcmp.
      (void) ACE_OS::memset (buf, 0, sizeof(buf));

      // Marshal the objref into an encapsulation bytestream.
      (void) cdr.write_octet (TAO_ENCAP_BYTE_ORDER);
      if ((cdr << obj) == 0)
        ACE_THROW_RETURN (CORBA::MARSHAL (), 0);

      // Now hexify the encapsulated CDR data into a string, and
      // return that string.

      size_t total_len = cdr.total_length ();

      char *cp;
      ACE_ALLOCATOR_RETURN (cp,
                            CORBA::string_alloc (sizeof ior_prefix
                                                 + 2 * total_len),
                            0);

      CORBA::String_var string = cp;

      ACE_OS::strcpy (cp, ior_prefix);
      cp += sizeof (ior_prefix) - 1;

      for (const ACE_Message_Block *i = cdr.begin ();
           i != 0;
           i = i->cont ())
        {
          const char *bytes = i->rd_ptr ();
          size_t len = i->length ();

          while (len--)
            {
              *cp++ = ACE::nibble2hex ((*bytes) >> 4);
              *cp++ = ACE::nibble2hex (*bytes);
              bytes++;
            }
        }
      // Null terminate the string..
      *cp = 0;

      return string._retn ();
    }
  else
    {
      if (obj->_stubobj () == 0)
        {
          if (TAO_debug_level > 0)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("TAO_Stub pointer in CORBA::ORB::object_to_string() ")
                        ACE_TEXT ("is zero.\n")));

          ACE_THROW_RETURN (CORBA::MARSHAL (
                              CORBA_SystemException::_tao_minor_code (
                                TAO_DEFAULT_MINOR_CODE,
                                EINVAL),
                              CORBA::COMPLETED_NO),
                            0);
        }

      // @@ According to Carlos, we shouldn't be using
      //    profile_in_use(). Instead we should use the first profile
      //    in the MProfile instead, for example.
      //
      //    For now, I'll just throw an exception since I was getting
      //    segmentation faults.
      //             -Ossama
      if (obj->_stubobj ()->profile_in_use () == 0)
        {
          if (TAO_debug_level > 0)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("TAO_Profile pointer in ")
                        ACE_TEXT ("CORBA::ORB::object_to_string() is zero.\n")));

          ACE_THROW_RETURN (CORBA::MARSHAL (
                              CORBA_SystemException::_tao_minor_code (
                                TAO_DEFAULT_MINOR_CODE,
                                EINVAL),
                              CORBA::COMPLETED_NO),
                            0);
        }

      return obj->_stubobj ()->profile_in_use ()->to_string (ACE_TRY_ENV);
    }
}

// Destringify arbitrary objrefs.  This method is called from
// <resolve_name_service> with an IOR <multicast_to_service>.

CORBA::Object_ptr
CORBA_ORB::string_to_object (const char *str,
                             CORBA::Environment &ACE_TRY_ENV)
{
  // This method should not be called if the ORB has been shutdown.
  this->check_shutdown (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  // Check for NULL pointer
  if (str == 0)
    ACE_THROW_RETURN (CORBA::INV_OBJREF (
                          CORBA_SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            EINVAL),
                          CORBA::COMPLETED_NO),
                      CORBA::Object::_nil ());


  if (ACE_OS::strncmp (str,
                       file_prefix,
                       sizeof file_prefix - 1) == 0)
    return this->file_string_to_object (str + sizeof file_prefix - 1,
                                        ACE_TRY_ENV);
  else if (ACE_OS::strncmp (str,
                            ior_prefix,
                            sizeof ior_prefix - 1) == 0)
    return this->ior_string_to_object (str + sizeof ior_prefix - 1,
                                       ACE_TRY_ENV);
  else if (ACE_OS::strncmp (str,
                            dll_prefix,
                            sizeof dll_prefix - 1) == 0)
    return this->dll_string_to_object (str + sizeof dll_prefix - 1,
                                       ACE_TRY_ENV);
  else
    return this->url_ior_string_to_object (str, ACE_TRY_ENV);
}

// ****************************************************************

#if (TAO_HAS_CORBA_MESSAGING == 1)

CORBA::Policy_ptr
CORBA_ORB::create_policy (CORBA::PolicyType type,
                          const CORBA::Any& val,
                          CORBA::Environment &ACE_TRY_ENV)
{
  this->check_shutdown (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  PortableServer::POA_var root_poa =
    this->orb_core_->root_poa_reference (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  switch (type)
    {

#if (TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1)

    case TAO_MESSAGING_RELATIVE_RT_TIMEOUT_POLICY_TYPE:
      return TAO_RelativeRoundtripTimeoutPolicy::create (val,
                                                         ACE_TRY_ENV);

#endif /* TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1 */

#if (TAO_HAS_CLIENT_PRIORITY_POLICY == 1)

    case TAO_CLIENT_PRIORITY_POLICY_TYPE:
      return TAO_Client_Priority_Policy::create (val,
                                                 ACE_TRY_ENV);

#endif /* TAO_HAS_CLIENT_PRIORITY_POLICY == 1 */

#if (TAO_HAS_SYNC_SCOPE_POLICY == 1)

    case TAO_MESSAGING_SYNC_SCOPE_POLICY_TYPE:
      return TAO_Sync_Scope_Policy::create (val,
                                            ACE_TRY_ENV);

#endif /* TAO_HAS_SYNC_SCOPE_POLICY == 1 */

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

    case TAO_BUFFERING_CONSTRAINT_POLICY_TYPE:
      return TAO_Buffering_Constraint_Policy::create (val,
                                                      ACE_TRY_ENV);

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

    case TAO_MESSAGING_REBIND_POLICY_TYPE:
    case TAO_MESSAGING_REQUEST_PRIORITY_POLICY_TYPE:
    case TAO_MESSAGING_REPLY_PRIORITY_POLICY_TYPE:
    case TAO_MESSAGING_REQUEST_START_TIME_POLICY_TYPE:
    case TAO_MESSAGING_REQUEST_END_TIME_POLICY_TYPE:
    case TAO_MESSAGING_REPLY_START_TIME_POLICY_TYPE:
    case TAO_MESSAGING_REPLY_END_TIME_POLICY_TYPE:
    case TAO_MESSAGING_RELATIVE_REQ_TIMEOUT_POLICY_TYPE:
    case TAO_MESSAGING_ROUTING_POLICY_TYPE:
    case TAO_MESSAGING_MAX_HOPS_POLICY_TYPE:
    case TAO_MESSAGING_QUEUE_ORDER_POLICY_TYPE:
      ACE_THROW_RETURN (CORBA::PolicyError (CORBA::UNSUPPORTED_POLICY),
                        CORBA::Policy::_nil ());

    default:
      break;
    }
  ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY),
                    CORBA::Policy::_nil ());
}

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

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
CORBA_ORB::file_string_to_object (const char *filename,
                                  CORBA::Environment& ACE_TRY_ENV)
{
  FILE *file = ACE_OS::fopen (filename, "r");

  if (file == 0)
    return CORBA::Object::_nil ();

  ACE_Read_Buffer reader (file, 1);

  char *string = reader.read ();

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
      ACE_RE_THROW;
    }
  ACE_ENDTRY;

  return object;
}

// ****************************************************************

CORBA::Object_ptr
CORBA_ORB::dll_string_to_object (const char *str,
                                 CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Object_Loader *loader =
    ACE_Dynamic_Service<TAO_Object_Loader>::instance (str);
  if (loader == 0)
    {
      ACE_THROW_RETURN
        (CORBA::INV_OBJREF
         (CORBA_SystemException::_tao_minor_code (
            TAO_DEFAULT_MINOR_CODE,
            EINVAL),
          CORBA::COMPLETED_NO),
         CORBA::Object::_nil ());
    }

  return loader->create_object (this, ACE_TRY_ENV);
}

// ****************************************************************

// Convert an URL style IOR in an object

CORBA::Object_ptr
CORBA_ORB::url_ior_string_to_object (const char* str,
                                     CORBA::Environment& ACE_TRY_ENV)
{
  TAO_MProfile mprofile;
  // It is safe to declare this on the stack since the contents of
  // mprofile get copied.  No memory is allocated for profile storage
  // here.  The Connector Registry will determine the exact number
  // of profiles and tell the MProfile object to allocate enough memory
  // to hold them all.

  int retv =
    this->orb_core_->connector_registry ()->make_mprofile (str,
                                                           mprofile,
                                                           ACE_TRY_ENV);

  ACE_CHECK_RETURN (CORBA::Object::_nil ());   // Return nil.

  if (retv != 0)
    {
      ACE_THROW_RETURN (CORBA::INV_OBJREF (
                          CORBA_SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            EINVAL),
                          CORBA::COMPLETED_NO),
                        CORBA::Object::_nil ());
    }

  // Now make the TAO_Stub.
  TAO_Stub *data = 0;
  ACE_NEW_THROW_EX (data,
                    TAO_Stub ((char *) 0, mprofile, this->orb_core_),
                    CORBA::NO_MEMORY (
                      CORBA_SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  TAO_Stub_Auto_Ptr safe_data (data);

  // Figure out if the servant is collocated.
  TAO_ServantBase *servant = 0;
  TAO_SERVANT_LOCATION servant_location =
    this->_get_collocated_servant (safe_data.get (),
                                   servant);

  int collocated = 0;
  if (servant_location != TAO_SERVANT_NOT_FOUND)
    collocated = 1;

  CORBA::Object_ptr obj = CORBA::Object::_nil ();

  // Create the CORBA level proxy.  This will increase the ref_count
  // on data by one
  ACE_NEW_THROW_EX (obj,
                    CORBA_Object (safe_data.get (),
                                  servant,
                                  (CORBA::Boolean) collocated),
                    CORBA::NO_MEMORY (
                      CORBA_SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  // All is well, so release the stub object from its auto_ptr.
  data = safe_data.release ();

  return obj;
}

// ****************************************************************

void
CORBA_ORB::_optimize_collocation_objects (CORBA::Boolean opt)
{
  if (this->orb_core_ != 0)
    this->orb_core_->optimize_collocation_objects (opt);
}

CORBA::Boolean
CORBA_ORB::_optimize_collocation_objects (void) const
{
  if (this->orb_core_ != 0)
    return this->orb_core_->optimize_collocation_objects ();
  else
    return 0; // Need to return something
}

TAO_SERVANT_LOCATION
CORBA_ORB::_get_collocated_servant (TAO_Stub *sobj,
                                    TAO_ServantBase *&servant)
{
  if (sobj == 0)
    return TAO_SERVANT_NOT_FOUND;

  // @@ What about forwarding.  With this approach we are never forwarded
  //    when we use collocation!
  const TAO_MProfile &mprofile = sobj->base_profiles ();

  {
    // @@ Ossama: maybe we need another lock for the table, to
    //    reduce contention on the Static_Object_Lock below, if so
    //    then we need to use that lock in the ORB_init() function.
    ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard,
                              *ACE_Static_Object_Lock::instance (),
                              TAO_SERVANT_NOT_FOUND));

    TAO_ORB_Table *table = TAO_ORB_Table::instance ();
    TAO_ORB_Table::Iterator end = table->end ();
    for (TAO_ORB_Table::Iterator i = table->begin ();
         i != end;
         ++i)
      {
        TAO_SERVANT_LOCATION servant_location =
          this->_find_collocated_servant (sobj,
                                          (*i).int_id_,
                                          servant,
                                          mprofile);
        if (servant_location != TAO_SERVANT_NOT_FOUND)
          return servant_location;
      }
  }
  // If we don't find one by this point, we return 0.
  return TAO_SERVANT_NOT_FOUND;
}

TAO_SERVANT_LOCATION
CORBA_ORB::_find_collocated_servant (TAO_Stub *sobj,
                                     TAO_ORB_Core *orb_core,
                                     TAO_ServantBase *&servant,
                                     const TAO_MProfile &mprofile)
{
  if (!orb_core->optimize_collocation_objects ())
    return TAO_SERVANT_NOT_FOUND;

  if (!orb_core->use_global_collocation () && orb_core != this->orb_core_)
    return TAO_SERVANT_NOT_FOUND;

  if (!orb_core->is_collocated (mprofile))
    return TAO_SERVANT_NOT_FOUND;

  TAO_Object_Adapter *oa = orb_core->object_adapter ();

  for (TAO_PHandle j = 0;
       j != mprofile.profile_count ();
       ++j)
    {
      const TAO_Profile *profile = mprofile.get_profile (j);
      TAO_ObjectKey_var objkey = profile->_key ();

      ACE_DECLARE_NEW_CORBA_ENV;
      ACE_TRY
        {
          TAO_SERVANT_LOCATION servant_location =
            oa->find_servant (objkey.in (),
                              servant,
                              ACE_TRY_ENV);
          ACE_TRY_CHECK;

          if (servant_location != TAO_SERVANT_NOT_FOUND)
            {
              // Found collocated object.  Perhaps we can get around
              // by simply setting the servant_orb, but let get this
              // to work first.

              // There could only be one ORB which is us.

              // @@ Do not duplicate the ORB here!
              //    TAO_Stub::servant_orb()  duplicates it.
              //       -Ossama
              sobj->servant_orb (orb_core->orb ());

              return servant_location;
            }
        }
      ACE_CATCHANY
        {
          // Ignore the exception and continue with the next one.
        }
      ACE_ENDTRY;
    }

  return TAO_SERVANT_NOT_FOUND;
}

// ****************************************************************

// Add a mapping ObjectID->IOR to the table.
int
CORBA_ORB::_tao_add_to_IOR_table (const ACE_CString &object_id,
                                  CORBA::Object_ptr obj)
{
  if (CORBA::is_nil (obj))
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("TAO (%P|%t): Cannot add nil object to table <%s>\n"),
                       object_id.c_str ()),
                      -1);

  CORBA::String_var string =
    this->object_to_string (obj);

  if (string.in () == 0 || string[0u] == '\0')
    return -1;

  ACE_CString ior (string.in ());

  if (this->lookup_table_.add_ior (object_id, ior) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("TAO (%P|%t): Unable to add IOR to table <%s>\n"),
                       object_id.c_str ()),
                      -1);

  return 0;
}

// Find an IOR in the table for the given ObjectID.
int
CORBA_ORB::_tao_find_in_IOR_table (const ACE_CString &object_id,
                                   CORBA::Object_ptr &obj)
{
  // @@ This debugging output should *NOT* be used since the
  //    object key string is not null terminated, nor can it
  //    be null terminated without copying.  No copying should
  //    be done since performance is somewhat important here.
  //    So, just remove the debugging output entirely.
  //
  //   if (TAO_debug_level > 0)
  //     ACE_DEBUG ((LM_DEBUG, "TAO (%P|%t): lookup service ID <%s>\n",
  //                 object_id.c_str ()));

  ACE_CString ior;

  if (this->lookup_table_.find_ior (object_id, ior) != 0)
    {
      // @@ This debugging output should *NOT* be used since the
      //    object key string is not null terminated, nor can it
      //    be null terminated without copying.  No copying should
      //    be done since performance is somewhat important here.
      //    So, just remove the debugging output entirely.
      //
      //       ACE_ERROR_RETURN ((LM_ERROR,
      //                          "TAO (%P|%t) cannot find IOR for <%s>\n",
      //                          object_id.c_str ()),
      //                         -1);
      return -1;
    }

  obj = this->string_to_object (ior.c_str ());

  return 0;
}

void
CORBA_ORB::_tao_register_IOR_table_callback (TAO_IOR_LookupTable_Callback *callback,
                                             int delete_callback)
{
  this->lookup_table_.register_callback (callback, delete_callback);
}

// *************************************************************
// Inline operators for TAO_opaque encoding and decoding
// *************************************************************

CORBA::Boolean
operator<< (TAO_OutputCDR& cdr, const TAO_opaque& x)
{
  CORBA::ULong length = x.length ();
  cdr.write_ulong (length);
#if (TAO_NO_COPY_OCTET_SEQUENCES == 0)
  cdr.write_octet_array (x.get_buffer (), length);
#else /* TAO_NO_COPY_OCTET_SEQUENCES == 1 */
  if (x.mb () == 0)
    cdr.write_octet_array (x.get_buffer (), length);
  else
    cdr.write_octet_array_mb (x.mb ());
#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 1 */
  return (CORBA::Boolean) cdr.good_bit ();
}

CORBA::Boolean
operator>>(TAO_InputCDR& cdr, TAO_opaque& x)
{
  CORBA::ULong length;
  cdr.read_ulong (length);
#if (TAO_NO_COPY_OCTET_SEQUENCES == 0)
  x.length (length);
  cdr.read_octet_array (x.get_buffer (), length);
#else /* TAO_NO_COPY_OCTET_SEQUENCES == 1 */
  x.replace (length, cdr.start ());
  x.mb ()->wr_ptr (x.mb ()->rd_ptr () + length);
  cdr.skip_bytes (length);
#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 1 */
  return (CORBA::Boolean) cdr.good_bit ();
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
CORBA_ORB::unregister_value_factory (const char * /* repository_id */,
                                     CORBA_Environment &)
{
  ACE_ERROR((LM_ERROR, ACE_TEXT ("(%N:%l) function not implemented\n")));
  // %! TODO
}

CORBA::ValueFactory_ptr
CORBA_ORB::lookup_value_factory (const char *repository_id,
                                 CORBA_Environment &)
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


// *************************************************************
// C++ iostream operators for (W)String_var and (W)String_out
// *************************************************************

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)

ostream &
operator<< (ostream &os, const CORBA::String_var &sv)
{
  os << sv.in ();
  return os;
}

istream &
operator>> (istream &is, CORBA::String_var &sv)
{
  is >> sv.inout ();
  return is;
}

ostream &
operator<< (ostream &os, CORBA::String_out &so)
{
  os << so.ptr ();
  return os;
}

istream &
operator>> (istream &is, CORBA::String_out &so)
{
  is >> so.ptr ();
  return is;
}

// Until we implement WString support for platforms with a
// 4-byte wchar_t, we just define the following to emit
// the CORBA::WChars one by one.

ostream &
operator<< (ostream &os, const CORBA::WString_var &wsv)
{
  CORBA::ULong len = ACE_OS::wslen (wsv.in ());
  for (CORBA::ULong i = 0; i < len; i++)
    {
      os << wsv[i];
    }
  return os;
}

istream &
operator>> (istream &is, CORBA::WString_var &wsv)
{
  CORBA::ULong i = 0;
  CORBA::WChar wc;
  is >> wc;
  while (wc)
    {
      wsv[i] = wc;
      i++;
      is >> wc;
    }
  wsv[i] = 0;
  return is;
}

ostream &
operator<< (ostream &os, CORBA::WString_out &wso)
{
  CORBA::WChar *tmp = wso.ptr ();
  CORBA::ULong len = ACE_OS::wslen (tmp);
  for (CORBA::ULong i = 0; i < len; i++)
    {
      os << tmp[i];
    }
  return os;
}

istream &
operator>> (istream &is, CORBA::WString_out &wso)
{
  CORBA::ULong i = 0;
  CORBA::WChar wc;
  is >> wc;
  while (wc)
    {
      wso.ptr ()[i] = wc;
      i++;
      is >> wc;
    }
  wso.ptr ()[i] = 0;
  return is;
}

#endif /* ACE_LACKS_IOSTREAM_TOTALLY */

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<TAO_Server_Strategy_Factory>;
template class ACE_Dynamic_Service<TAO_Client_Strategy_Factory>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<TAO_Server_Strategy_Factory>
#pragma instantiate ACE_Dynamic_Service<TAO_Client_Strategy_Factory>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
