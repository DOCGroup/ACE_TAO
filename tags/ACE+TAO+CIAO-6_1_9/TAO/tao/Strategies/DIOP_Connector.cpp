// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "tao/Strategies/DIOP_Connector.h"

#if defined (TAO_HAS_DIOP) && (TAO_HAS_DIOP != 0)

#include "ace/Connector.h"

#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/SystemException.h"
#include "tao/Base_Transport_Property.h"
#include "tao/Protocols_Hooks.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_string.h"
#include "tao/Thread_Lane_Resources.h"
#include "ace/os_include/os_netdb.h"

#include "tao/Strategies/DIOP_Profile.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_DIOP_Connector::TAO_DIOP_Connector (void)
  : TAO_Connector (TAO_TAG_DIOP_PROFILE)
{
}

TAO_DIOP_Connector::~TAO_DIOP_Connector (void)
{
}

int
TAO_DIOP_Connector::open (TAO_ORB_Core *orb_core)
{
  this->orb_core (orb_core);

  // Create our connect strategy
  if (this->create_connect_strategy () == -1)
    return -1;

  return 0;
}

int
TAO_DIOP_Connector::close (void)
{
  return 0;
}

int
TAO_DIOP_Connector::set_validate_endpoint (TAO_Endpoint *endpoint)
{
  TAO_DIOP_Endpoint *diop_endpoint =
    this->remote_endpoint (endpoint);

  if (diop_endpoint == 0)
    return -1;

  const ACE_INET_Addr &remote_address =
    diop_endpoint->object_addr ();

  // Verify that the remote ACE_INET_Addr was initialized properly.
  // Failure can occur if hostname lookup failed when initializing the
  // remote ACE_INET_Addr.
#if defined (ACE_HAS_IPV6)
   if (remote_address.get_type () != AF_INET &&
       remote_address.get_type () != AF_INET6)
#else /* ACE_HAS_IPV6 */
  if (remote_address.get_type () != AF_INET)
#endif /* !ACE_HAS_IPV6 */
    {
      if (TAO_debug_level > 0)
        {
          TAOLIB_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - DIOP connection failed.\n")
                      ACE_TEXT ("TAO (%P|%t) This is most likely ")
                      ACE_TEXT ("due to a hostname lookup ")
                      ACE_TEXT ("failure.\n")));
        }

      return -1;
    }

  return 0;
}

TAO_Transport *
TAO_DIOP_Connector::make_connection (TAO::Profile_Transport_Resolver *,
                                     TAO_Transport_Descriptor_Interface &desc,
                                     ACE_Time_Value * /*max_wait_time*/)
{
  TAO_DIOP_Endpoint *diop_endpoint =
    this->remote_endpoint (desc.endpoint ());

  if (diop_endpoint == 0)
    return 0;

  const ACE_INET_Addr &remote_address =
    diop_endpoint->object_addr ();

#if defined (ACE_HAS_IPV6) && !defined (ACE_HAS_IPV6_V6ONLY)
  // Check if we need to invalidate accepted connections
  // from IPv4 mapped IPv6 addresses
  if (this->orb_core ()->orb_params ()->connect_ipv6_only () &&
      remote_address.is_ipv4_mapped_ipv6 ())
    {
      if (TAO_debug_level > 0)
        {
          ACE_TCHAR remote_as_string[MAXHOSTNAMELEN + 16];

          (void) remote_address.addr_to_string (remote_as_string,
                                                sizeof remote_as_string);

          TAOLIB_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO (%P|%t) - DIOP_Connection_Handler::open, ")
                      ACE_TEXT ("invalid connection to IPv4 mapped IPv6 interface <%s>!\n"),
                      remote_as_string));
        }
      return 0;
    }
#endif /* ACE_HAS_IPV6 && ACE_HAS_IPV6_V6ONLY */

  TAO_DIOP_Connection_Handler *svc_handler = 0;

  ACE_NEW_RETURN (svc_handler,
                  TAO_DIOP_Connection_Handler (this->orb_core ()),
                  0);

  // Make sure that we always do a remove_reference
  ACE_Event_Handler_var svc_handler_auto_ptr (svc_handler);

  u_short port = 0;
  const ACE_UINT32 ia_any = INADDR_ANY;
  ACE_INET_Addr local_addr (port, ia_any);

#if defined (ACE_HAS_IPV6)
  if (remote_address.get_type () == AF_INET6)
    local_addr.set (port,
                    ACE_IPV6_ANY);
#endif /* ACE_HAS_IPV6 */

  svc_handler->local_addr (local_addr);
  svc_handler->addr (remote_address);

  int retval = svc_handler->open (0);

  // Failure to open a connection.
  if (retval != 0)
    {
      svc_handler->close ();

      if (TAO_debug_level > 0)
        {
          TAOLIB_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO (%P|%t) - DIOP_Connector::make_connection, ")
                      ACE_TEXT ("could not make a new connection\n")));
        }

      return 0;
    }

  if (TAO_debug_level > 2)
    TAOLIB_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO (%P|%t) - DIOP_Connector::connect, ")
                ACE_TEXT ("new connection on HANDLE %d\n"),
                svc_handler->peer ().get_handle ()));

  TAO_DIOP_Transport *transport =
    dynamic_cast <TAO_DIOP_Transport *> (svc_handler->transport ());

  // In case of errors transport is zero
  if (transport == 0)
    {
      svc_handler->close ();

      // Give users a clue to the problem.
      if (TAO_debug_level > 3)
        TAOLIB_DEBUG ((LM_ERROR,
                    ACE_TEXT ("TAO (%P|%t) - DIOP_Connector::make_connection, ")
                    ACE_TEXT ("connection to <%C:%u> failed (%p)\n"),
                    diop_endpoint->host (),
                    diop_endpoint->port (),
                    ACE_TEXT ("errno")));

      return 0;
    }

  // Add the handler to Cache
  retval =
    this->orb_core ()->lane_resources ().transport_cache ().cache_transport (&desc,
                                                                             transport);

  // Failure in adding to cache.
  if (retval == -1)
    {
      svc_handler->close ();

      if (TAO_debug_level > 0)
        {
          TAOLIB_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO (%P|%t) - DIOP_Connector::make_connection, ")
                      ACE_TEXT ("could not add the new connection to cache\n")));
        }

      return 0;
    }

  svc_handler_auto_ptr.release ();
  return transport;
}

TAO_Profile *
TAO_DIOP_Connector::create_profile (TAO_InputCDR& cdr)
{
  TAO_Profile *pfile = 0;
  ACE_NEW_RETURN (pfile,
                  TAO_DIOP_Profile (this->orb_core ()),
                  0);

  if (pfile->decode (cdr) == -1)
    {
      pfile->_decr_refcnt ();
      pfile = 0;
    }

  return pfile;
}

TAO_Profile *
TAO_DIOP_Connector::make_profile (void)
{
  // The endpoint should be of the form:
  //    N.n@host:port/object_key
  // or:
  //    host:port/object_key

  TAO_Profile *profile = 0;
  ACE_NEW_THROW_EX (profile,
                    TAO_DIOP_Profile (this->orb_core ()),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  return profile;
}

int
TAO_DIOP_Connector::check_prefix (const char *endpoint)
{
  // Check for a valid string
  if (!endpoint || !*endpoint)
    return -1;  // Failure

  const char *protocol[] = { "diop", "dioploc" };

  size_t const slot = ACE_OS::strchr (endpoint, ':') - endpoint;

  size_t const len0 = ACE_OS::strlen (protocol[0]);
  size_t const len1 = ACE_OS::strlen (protocol[1]);

  // Check for the proper prefix in the IOR.  If the proper prefix
  // isn't in the IOR then it is not an IOR we can use.
  if (slot == len0
      && ACE_OS::strncasecmp (endpoint, protocol[0], len0) == 0)
    return 0;
  else if (slot == len1
           && ACE_OS::strncasecmp (endpoint, protocol[1], len1) == 0)
    return 0;

  return -1;
  // Failure: not an DIOP IOR
  // DO NOT throw an exception here.
}

char
TAO_DIOP_Connector::object_key_delimiter (void) const
{
  return TAO_DIOP_Profile::object_key_delimiter_;
}

TAO_DIOP_Endpoint *
TAO_DIOP_Connector::remote_endpoint (TAO_Endpoint *endpoint)
{
  if (endpoint->tag () != TAO_TAG_DIOP_PROFILE)
    return 0;

  return dynamic_cast<TAO_DIOP_Endpoint *> (endpoint);
}

int
TAO_DIOP_Connector::cancel_svc_handler (
  TAO_Connection_Handler * /* svc_handler */)
{
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_DIOP && TAO_HAS_DIOP != 0 */
