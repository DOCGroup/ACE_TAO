// $Id$

#include "orbsvcs/PortableGroup/UIPMC_Profile.h"
#include "orbsvcs/PortableGroup/UIPMC_Connector.h"

#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/Base_Transport_Property.h"
#include "tao/Protocols_Hooks.h"
#include "tao/Thread_Lane_Resources.h"

#include "ace/Connector.h"
#include "ace/OS_NS_strings.h"
#include "ace/os_include/os_netdb.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_UIPMC_Connector::TAO_UIPMC_Connector (void)
  : TAO_Connector (IOP::TAG_UIPMC)
{
}

TAO_UIPMC_Connector::~TAO_UIPMC_Connector (void)
{
}

int
TAO_UIPMC_Connector::open (TAO_ORB_Core *orb_core)
{
  this->orb_core (orb_core);

  // For the sake of uniformity do
  if (this->create_connect_strategy () == -1)
    return -1;

  return 0;
}

int
TAO_UIPMC_Connector::close (void)
{
  return 0;
}

int
TAO_UIPMC_Connector::set_validate_endpoint (TAO_Endpoint *endpoint)
{
  if (endpoint->tag () != IOP::TAG_UIPMC)
    return -1;

  TAO_UIPMC_Endpoint *uipmc_endpoint =
    dynamic_cast<TAO_UIPMC_Endpoint *> (endpoint);

  if (uipmc_endpoint == 0)
    return -1;

  const ACE_INET_Addr &remote_address =
    uipmc_endpoint->object_addr ();

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
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) UIPMC connection failed.\n")
                      ACE_TEXT ("TAO (%P|%t) This is most likely ")
                      ACE_TEXT ("due to a hostname lookup ")
                      ACE_TEXT ("failure.\n")));
        }

      return -1;
    }

  return 0;
}

TAO_Transport *
TAO_UIPMC_Connector::make_connection (TAO::Profile_Transport_Resolver *,
                                      TAO_Transport_Descriptor_Interface &desc,
                                      ACE_Time_Value *)
{
  TAO_UIPMC_Endpoint *uipmc_endpoint =
    dynamic_cast<TAO_UIPMC_Endpoint *> (desc.endpoint ());

  if (uipmc_endpoint == 0)
    return 0;

  const ACE_INET_Addr &remote_address =
    uipmc_endpoint->object_addr ();

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

          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO (%P|%t) - UIPMC_Connector::open, ")
                      ACE_TEXT ("invalid connection to IPv4 mapped IPv6 interface <%s>!\n"),
                      remote_as_string));
        }
      return 0;
    }
#endif /* ACE_HAS_IPV6 && ACE_HAS_IPV6_V6ONLY */

  TAO_UIPMC_Connection_Handler *svc_handler = 0;

  ACE_NEW_RETURN (svc_handler,
                  TAO_UIPMC_Connection_Handler (this->orb_core ()),
                  0);

  // Make sure that we always do a remove_reference
  ACE_Event_Handler_var svc_handler_auto_ptr (svc_handler);

  u_short port = 0;
  const ACE_UINT32 ia_any = INADDR_ANY;
  ACE_INET_Addr local_addr(port, ia_any);

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
          ACE_ERROR ((LM_ERROR,
                      "TAO (%P|%t) - UIPMC_Connector::make_connection, "
                      "could not make a new connection\n"));
        }

      return 0;
    }

  if (TAO_debug_level > 2)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO (%P|%t) - UIPMC_Connector::make_connection, ")
                ACE_TEXT ("new connection on HANDLE %d\n"),
                svc_handler->get_handle ()));

  UIPMC_TRANSPORT *transport =
    dynamic_cast<UIPMC_TRANSPORT *> (svc_handler->transport ());

  // In case of errors transport is zero
  if (transport == 0)
    {
      svc_handler->close ();

      // Give users a clue to the problem.
      if (TAO_debug_level > 3)
          ACE_ERROR ((LM_ERROR,
                      "TAO (%P|%t) - UIPMC_Connector::make_connection, "
                      "connection to <%C:%u> failed (%p)\n",
                      uipmc_endpoint->host (),
                      uipmc_endpoint->port (),
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
          ACE_ERROR ((LM_ERROR,
                      "TAO (%P|%t) - UIPMC_Connector::make_connection, "
                      "could not add the new connection to cache\n"));
        }

      return 0;
    }

  svc_handler_auto_ptr.release ();
  return transport;
}

TAO_Profile *
TAO_UIPMC_Connector::create_profile (TAO_InputCDR& cdr)
{
  TAO_Profile *pfile;
  ACE_NEW_RETURN (pfile,
                  TAO_UIPMC_Profile (this->orb_core ()),
                  0);

  int r = pfile->decode (cdr);
  if (r == -1)
    {
      pfile->_decr_refcnt ();
      pfile = 0;
    }

  return pfile;
}

TAO_Profile *
TAO_UIPMC_Connector::make_profile (void)
{
  // The endpoint should be of the form:
  //    N.n@host:port/object_key
  // or:
  //    host:port/object_key

  TAO_Profile *profile = 0;
  ACE_NEW_THROW_EX (profile,
                    TAO_UIPMC_Profile (this->orb_core ()),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  return profile;
}

int
TAO_UIPMC_Connector::check_prefix (const char *endpoint)
{
  // Check for a valid string
  if (!endpoint || !*endpoint)
    return -1;  // Failure

  static const char protocol[] = "miop";
  static size_t const len = sizeof (protocol) - 1;

  size_t const slot = ACE_OS::strchr (endpoint, ':') - endpoint;

  // Check for the proper prefix in the IOR.  If the proper prefix
  // isn't in the IOR then it is not an IOR we can use.
  if (slot == len
      && ACE_OS::strncasecmp (endpoint, protocol, len) == 0)
    return 0;

  return -1;
  // Failure: not a MIOP IOR
  // DO NOT throw an exception here.
}

char
TAO_UIPMC_Connector::object_key_delimiter (void) const
{
  return TAO_UIPMC_Profile::object_key_delimiter_;
}

int
TAO_UIPMC_Connector::cancel_svc_handler (
  TAO_Connection_Handler * /* svc_handler */)
{
  // Noop
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL

