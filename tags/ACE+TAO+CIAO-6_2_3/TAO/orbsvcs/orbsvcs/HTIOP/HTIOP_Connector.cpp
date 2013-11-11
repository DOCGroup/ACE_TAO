// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/HTIOP/HTIOP_Connector.h"
#include "orbsvcs/HTIOP/HTIOP_Profile.h"

#include "ace/HTBP/HTBP_Session.h"
#include "ace/HTBP/HTBP_ID_Requestor.h"
#include "ace/HTBP/HTBP_Environment.h"

#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/Client_Strategy_Factory.h"
#include "tao/SystemException.h"
#include "tao/Base_Transport_Property.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Connect_Strategy.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Profile_Transport_Resolver.h"
#include "ace/Strategies_T.h"
#include "ace/OS_NS_strings.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::HTIOP::Connector::Connector (ACE::HTBP::Environment *ht_env)
  : TAO_Connector (OCI_TAG_HTIOP_PROFILE),
    connect_creation_strategy_ (),
    concurrency_strategy_ (),
    ht_env_ (ht_env)
{
}

TAO::HTIOP::Connector::~Connector (void)
{
}

int
TAO::HTIOP::Connector::open (TAO_ORB_Core *orb_core)
{
  // Set the ORB Core
  this->orb_core (orb_core);

  // Create our connect strategy
  if (this->create_connect_strategy () == -1)
    return -1;

  /// Our connect creation strategy
  ACE_NEW_RETURN (this->connect_creation_strategy_,
                  CONNECT_CREATION_STRATEGY
                      (orb_core->thr_mgr (),
                       orb_core),
                  -1);


  ACE_NEW_RETURN (this->concurrency_strategy_,
                  CONNECT_CONCURRENCY_STRATEGY (orb_core),
                  -1);

  return 0;
}

int
TAO::HTIOP::Connector::close (void)
{
  delete this->concurrency_strategy_;
  delete this->connect_creation_strategy_;

  return 0;
}

int
TAO::HTIOP::Connector::set_validate_endpoint (TAO_Endpoint *endpoint)
{
  // Set and validate endpoint. We need to do this to initialize our
  // remote *_Addr's which have not been done during IOR decode.

  /**
   * @note The above comments talk about initialization. But, based on
   * code reading, this is more of a validation of the endpoint rather
   * than setting anything. So, nothing here needs to be modified for
   * TAO_HTIOP case.
   */
  TAO::HTIOP::Endpoint *htiop_endpoint =
    this->remote_endpoint (endpoint);

  if (htiop_endpoint == 0)
    return -1;

   const ACE::HTBP::Addr &remote_address =
     htiop_endpoint->object_addr ();

   // Verify that the remote ACE::HTBP::Addr was initialized properly.
   // Failure can occur if hostname lookup failed when initializing the
   // remote ACE::HTBP::Addr.
   if (remote_address.get_type () != AF_INET &&
       (remote_address.get_htid() == 0 ||
        ACE_OS::strlen(remote_address.get_htid()) == 0))
     {
       if (TAO_debug_level > 0)
         {
           ORBSVCS_DEBUG ((LM_DEBUG,
                       ACE_TEXT ("TAO (%P|%t) TAO_HTIOP connection failed.\n")
                       ACE_TEXT ("TAO (%P|%t) This is most likely ")
                       ACE_TEXT ("due to a hostname lookup ")
                       ACE_TEXT ("failure.\n")));
         }

       return -1;
     }

   return 0;
}

TAO_Transport *
TAO::HTIOP::Connector::make_connection (TAO::Profile_Transport_Resolver *,
                                  TAO_Transport_Descriptor_Interface &desc,
                                  ACE_Time_Value *timeout)
{

  /**
   * \par
   * Connector is used only in the processes inside the firewall
   * This can connect to the remote address as it does always.
   * If it is needed that the client first connect to the firewall
   * and the firewall connects to the remote address, the code here
   * should be modified to accomodate the requirement. And this would
   * mean that we need to know the firewall address as well in this
   * method.
   * Probable checks would be
   * 1. Check if the endpoint is outside the firewall
   * 2. If so, connect to the firewall
   * 3. Make the firewall connect to the outside server.
   *
   * \par Dated Oct 15:
   */

  TAO::HTIOP::Endpoint *htiop_endpoint =
    this->remote_endpoint (desc.endpoint ());

  if (htiop_endpoint == 0)
    return 0;


  ACE::HTBP::Session_Id_t session_id;
  ACE_TString proxy_host;
  unsigned proxy_port;

  int const port_set = this->ht_env_->get_proxy_port(proxy_port);
  int const host_set = this->ht_env_->get_proxy_host(proxy_host);
  if (port_set != 0 ||
      host_set != 0)
    {
      proxy_port = htiop_endpoint->port();
      proxy_host = ACE_TEXT_CHAR_TO_TCHAR(htiop_endpoint->host());
    }
  else
    {
      ACE::HTBP::ID_Requestor req(ht_env_);
      session_id.local_ = ACE_TEXT_ALWAYS_CHAR(req.get_HTID());
    }

  if (proxy_port == 0)
    return 0;

  session_id.peer_ = htiop_endpoint->object_addr ();
  session_id.id_ = ACE::HTBP::Session::next_session_id();

  if (TAO_debug_level > 2)
    {
      char buffer[BUFSIZ];
      htiop_endpoint->addr_to_string(buffer,BUFSIZ);
    ORBSVCS_DEBUG ((LM_DEBUG,
                "TAO (%P|%t) - TAO::HTIOP::Connector::make_connection, "
                "to <%s>\n",
                buffer));
    }

  // Get the right synch options
  ACE_Synch_Options synch_options;

  this->active_connect_strategy_->synch_options (timeout,
                                                 synch_options);

  // The code used to set the timeout to zero, with the intent of
  // polling the reactor for connection completion. However, the side-effect
  // was to cause the connection to timeout immediately.

  // This is where we need to set the ACE::HTBP::Stream to the connection
  // handler.
  TAO::HTIOP::Connection_Handler *svc_handler = 0;

  // In more traditional connectors, the following code would be part of a
  // strategized connector's connect() method. This include making the
  // handler, attaching the session, and activating the handler.

  // Instantiate a ACE::HTBP::Session
  ACE::HTBP::Session *session = 0;
  if (ACE::HTBP::Session::find_session (session_id, session) == -1)
    {
      ACE_INET_Addr *proxy = 0;
      ACE_NEW_RETURN (proxy,
                      ACE_INET_Addr(static_cast<u_short> (proxy_port),proxy_host.c_str()),
                      0);
      ACE_NEW_RETURN (session, ACE::HTBP::Session (session_id,proxy, 1), 0);
      if (ACE::HTBP::Session::add_session (session) == -1)
        ORBSVCS_ERROR_RETURN ((LM_ERROR,
                           "ACE::HTBP::Initial_Filter::recv_data_header %p",
                           "add_session"),
                          0);
    }

  // Make the svc_handler
  this->connect_creation_strategy_->make_svc_handler (svc_handler);
  // we now have a connection handler that has an unconnected stream

  // Make sure that we always do a remove_reference
  ACE_Event_Handler_var svc_handler_auto_ptr (svc_handler);

  svc_handler->peer().session(session);
  session->handler(svc_handler);
  ACE::HTBP::Channel *outbound = session->outbound();
  ACE_UNUSED_ARG (outbound);

  // Activate immediately if we are connected.
  this->concurrency_strategy_->activate_svc_handler (svc_handler,0);

  // The above code is a rendering of connect() that allows the session to
  // reacquire a stream rather than opening a new one.

  int closed = svc_handler->is_closed();
  // At this point, the IIOP Connector has a result from an asynch connect
  // strategy, which does not apply here. Therefore the whole bit of logic
  // of dealing with a failed wait but an unclosed svc_handler is skipped.

  if (closed) // would be result == -1 in IIOP_Connector
    {
      if (TAO_debug_level)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
                      ACE_TEXT("(%P|%t) - TAO::HTIOP::Connector::make_connection, ")
                      ACE_TEXT("connection to  <%s:%d> failed (%p)\n"),
                       htiop_endpoint->host (), htiop_endpoint->port (),
                       "errno"));
        }
      return 0;
    }

  if (TAO_debug_level > 2)
    ORBSVCS_DEBUG ((LM_DEBUG,
                ACE_TEXT("(%P|%t) - TAO::HTIOP::Connector::make_connection, ")
                ACE_TEXT("new connection to <%s:%d> on Transport[%d]\n"),
                htiop_endpoint->host (), htiop_endpoint->port (),
                svc_handler->peer ().get_handle ()));

  TAO_Transport *transport = svc_handler->transport ();

   // Add the handler to Cache
   // @todo several other things need to be done regarding the following
   // code.
   // @note The cache_transport would probably need to cache the session id
   // with the local and remote addresses.
  int retval =
    this->orb_core ()->lane_resources ().
    transport_cache ().cache_transport (&desc, transport);

  if (retval == -1)
    {
      svc_handler->close();
      if (TAO_debug_level > 0)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
                      ACE_TEXT("(%P|%t) - TAO::HTIOP::Connector::make_connection, ")
                      ACE_TEXT("could not add the new")
                      ACE_TEXT(" connection to cache\n")));
        }
      return 0;
    }

   // If the wait strategy wants us to be registered with the reactor
   // then we do so.
  retval =  transport->wait_strategy ()->register_handler ();
  if (retval != 0)
    {
      transport->purge_entry ();
      transport->close_connection();

      if (TAO_debug_level > 0)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
                      ACE_TEXT("(%P|%t) - TAO::HTIOP::Connector::make_connection, ")
                      ACE_TEXT("could not register the new connection")
                      ACE_TEXT(" in the reactor\n")));
        }
      return 0;
    }

  svc_handler_auto_ptr.release ();
  return transport;
}


int
TAO::HTIOP::Connector::cancel_svc_handler (
  TAO_Connection_Handler * svc_handler)
{
  TAO::HTIOP::Connection_Handler* handler=
    dynamic_cast<TAO::HTIOP::Connection_Handler*> (svc_handler);

  if (handler)
      // Cancel from the connector
      //      this->base_connector_.cancel (handler);
    return 0;

  return -1;

}


/**
 * @brief Create a profile and initialize it based on the
 * encapsulation in <cdr>
 */
TAO_Profile *
TAO::HTIOP::Connector::create_profile (TAO_InputCDR& cdr)
{
  TAO_Profile *pfile;
  ACE_NEW_RETURN (pfile,
                  TAO::HTIOP::Profile (this->orb_core ()),
                  0);

  int r = pfile->decode (cdr);
  if (r == -1)
    {
      pfile->_decr_refcnt ();
      pfile = 0;
    }

  return pfile;
}

/**
 * @brief Create a profile with a given endpoint.
 */
TAO_Profile *
TAO::HTIOP::Connector::make_profile (void)
{
  // The endpoint should be of the form:
  //    N.n@host:port/object_key
  // or:
  //    host:port/object_key

  TAO_Profile *profile = 0;
  ACE_NEW_THROW_EX (profile,
                    TAO::HTIOP::Profile (this->orb_core ()),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  return profile;
}

int
TAO::HTIOP::Connector::check_prefix (const char *endpoint)
{
  // Check for a valid string
  if (!endpoint || !*endpoint)
    return -1;  // Failure

  const char *protocol[] = { "htiop" };

  size_t slot = ACE_OS::strchr (endpoint, ':') - endpoint;

  size_t len0 = ACE_OS::strlen (protocol[0]);

  // Check for the proper prefix in the IOR.  If the proper prefix
  // isn't in the IOR then it is not an IOR we can use.
  if (slot == len0
      && ACE_OS::strncasecmp (endpoint, protocol[0], len0) == 0)
    return 0;

  return -1;
  // Failure: not an TAO_HTIOP IOR
  // DO NOT throw an exception here.
}

char
TAO::HTIOP::Connector::object_key_delimiter (void) const
{
  return TAO::HTIOP::Profile::object_key_delimiter_;
}

TAO::HTIOP::Endpoint *
TAO::HTIOP::Connector::remote_endpoint (TAO_Endpoint *endpoint)
{
  if (endpoint->tag () != OCI_TAG_HTIOP_PROFILE)
    return 0;

  TAO::HTIOP::Endpoint *htiop_endpoint =
    dynamic_cast<TAO::HTIOP::Endpoint *> (endpoint );

 if (htiop_endpoint == 0)
    return 0;

  return htiop_endpoint;
}

TAO_END_VERSIONED_NAMESPACE_DECL
