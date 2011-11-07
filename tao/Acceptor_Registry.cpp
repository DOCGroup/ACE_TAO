// $Id$

#include "tao/Acceptor_Registry.h"
#include "tao/Profile.h"
#include "tao/Transport_Acceptor.h"
#include "tao/Protocol_Factory.h"
#include "tao/ORB_Core.h"
#include "tao/params.h"
#include "tao/MProfile.h"
#include "tao/debug.h"
#include "tao/Acceptor_Filter.h"
#include "tao/Endpoint.h"
#include "tao/ORB_Constants.h"
#include "tao/SystemException.h"
#if defined (ACE_WIN32) && defined (ACE_HAS_IPV6)
# include "tao/IIOP_Acceptor.h"
#endif /* ACE_WIN32 && ACE_HAS_IPV6 */

#include "ace/Auto_Ptr.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_ctype.h"

#if !defined(__ACE_INLINE__)
#include "tao/Acceptor_Registry.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Acceptor_Registry::TAO_Acceptor_Registry (void)
  : acceptors_ (0),
    size_ (0)
{
}

TAO_Acceptor_Registry::~TAO_Acceptor_Registry (void)
{
  this->close_all ();

  delete [] this->acceptors_;
}

size_t
TAO_Acceptor_Registry::endpoint_count (void)
{
  size_t count = 0;
  const TAO_AcceptorSetIterator end = this->end ();

  for (TAO_AcceptorSetIterator i = this->begin (); i != end; ++i)
    {
      count += (*i)->endpoint_count ();
    }

  return count;
}

bool
TAO_Acceptor_Registry::is_collocated (const TAO_MProfile &mprofile)
{
  TAO_AcceptorSetIterator const end = this->end ();
  CORBA::ULong const count = mprofile.profile_count ();

  // If at least one endpoint in one of the profiles matches one of
  // the acceptors, we are collocated.
  for (TAO_AcceptorSetIterator i = this->begin (); i != end; ++i)
    {
      for (TAO_PHandle j = 0; j != count; ++j)
        {
          const TAO_Profile *profile = mprofile.get_profile (j);

          // @@ We need to invoke a nonconst <endpoint> method on
          //    <profile>.  The content of profile/endpoint will not
          //    be modified.
          TAO_Profile *pf = const_cast<TAO_Profile *> (profile);

          // Check all endpoints for address equality.
          if ((*i)->tag () == pf->tag ())
            {
              // @note This can be a potentially expensive (O(n^2))
              //       operation if the below is_collocated() call
              //       also executes a loop.
              for (TAO_Endpoint *endp = pf->endpoint ();
                   endp != 0;
                   endp = endp->next ())
                {
                  if ((*i)->is_collocated (endp))
                    {
                      return true;
                    }
                }
            }
        }
    }

  return false;
}

TAO_Acceptor*
TAO_Acceptor_Registry::get_acceptor (CORBA::ULong tag)
{
  TAO_AcceptorSetIterator const end = this->end ();
  TAO_AcceptorSetIterator acceptor = this->begin ();

  for (; acceptor != end; ++acceptor)
    {
      if ((*acceptor)->tag () == tag)
        {
          return *acceptor;
        }
    }

  return 0;
}

int
TAO_Acceptor_Registry::open (TAO_ORB_Core *orb_core,
                             ACE_Reactor *reactor,
                             const TAO_EndpointSet &endpoint_set,
                             bool ignore_address)
{
  if (endpoint_set.is_empty ()
      // No endpoints were specified, we let each protocol pick its
      // own default.

      // All TAO pluggable protocols are expected to have the ability
      // to create a default endpoint.
      && this->open_default (orb_core, reactor, 0) == -1)
    {
      throw ::CORBA::INTERNAL (
        CORBA::SystemException::_tao_minor_code (
          TAO_ACCEPTOR_REGISTRY_OPEN_LOCATION_CODE,
          0),
        CORBA::COMPLETED_NO);
    }

  // Count the maximum number of endpoints in the set.  This will be
  // the maximum number of acceptors that need to be created.
  size_t acceptor_count = 0;
  TAO_EndpointSetIterator endpts (endpoint_set);

  for (ACE_CString *ep = 0;
       endpts.next (ep) != 0;
       endpts.advance ())
    {
      const ACE_CString &iop = (*ep);

      // IOP://address1,address2
      //    ^ slot
      ACE_CString::size_type const slot = iop.find ("://", 0);

      if (slot == iop.npos)
        {
          if (TAO_debug_level > 0)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("(%P|%t) Invalid endpoint ")
                          ACE_TEXT ("specification: <%C>.\n"),
                          iop.c_str ()));
            }

          throw ::CORBA::BAD_PARAM (
            CORBA::SystemException::_tao_minor_code (
              TAO_ACCEPTOR_REGISTRY_OPEN_LOCATION_CODE,
              EINVAL),
            CORBA::COMPLETED_NO);
        }

      ++acceptor_count;  // We've got at least one acceptor so far.
#if defined (ACE_WIN32) && defined (ACE_HAS_IPV6)
      // Since Win32 has single-stack IPv4/IPv6 we need an additional
      // acceptor if an iiop protocol without endpoints is specified
      // to open explicitly on IPv6 ANY *and* IPv4 ANY.

      // Now get the list of available protocol factories.
      const TAO_ProtocolFactorySetItor end =
        orb_core->protocol_factories ()->end ();

      // extract the protocol prefix
      const ACE_CString prefix (iop.substring (0, slot));

      for (TAO_ProtocolFactorySetItor factory =
             orb_core->protocol_factories ()->begin ();
           factory != end;
           ++factory)
        {
          if ((*factory)->factory ()->match_prefix (prefix))
            {
              if ((*factory)->factory ()->tag () == IOP::TAG_INTERNET_IOP)
                {
                  // just add additional space to cover for possibility
                  ++acceptor_count;
                }
              break; // we found the protocol, no need to go on
            }
        }
#endif /* ACE_WIN32 && ACE_HAS_IPV6 */

      // Now count the number of commas.  That number will be the
      // remaining number of endpoints in the current endpoint
      // specification.
      const char *ep_end =
        ep->c_str () + ACE_OS::strlen (ep->c_str ());

      for (const char *e = ACE_OS::strchr (ep->c_str (), ',');
           e != 0 && e != ep_end;
           e = ACE_OS::strchr (e, ','))
        {
          ++acceptor_count;
          ++e;
        }
    }

  // The array containing the TAO_Acceptors will never contain more
  // than the number of endpoints stored in TAO_ORB_Parameters.
  if (this->acceptors_ == 0)
    {
      ACE_NEW_THROW_EX (this->acceptors_,
                        TAO_Acceptor *[acceptor_count],
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO_ACCEPTOR_REGISTRY_OPEN_LOCATION_CODE,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
    }

  TAO_EndpointSetIterator endpoints (endpoint_set);

  for (ACE_CString *endpoint = 0;
       endpoints.next (endpoint) != 0;
       endpoints.advance ())
    {
      const ACE_CString &iop = (*endpoint);

      // IOP://address1,address2
      //    ^ slot
      ACE_CString::size_type const slot = iop.find ("://", 0);

      if (slot == iop.npos)
        {
          if (TAO_debug_level > 0)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("(%P|%t) Invalid endpoint ")
                          ACE_TEXT ("specification: <%C>.\n"),
                          iop.c_str ()));
            }

          throw ::CORBA::BAD_PARAM (
            CORBA::SystemException::_tao_minor_code (
              TAO_ACCEPTOR_REGISTRY_OPEN_LOCATION_CODE,
              EINVAL),
            CORBA::COMPLETED_NO);
        }

      const ACE_CString prefix (iop.substring (0, slot));

      // @@ We could move the protocol factory loop to the outermost
      //    level but for now we leave it inside the endpoint loop
      //    under the assumption that there will be more endpoints
      //    than protocols.

      // Now get the list of available protocol factories.
      const TAO_ProtocolFactorySetItor end =
        orb_core->protocol_factories ()->end ();

      bool found = false;
      // If usable protocol (factory) is found then this will be
      // set equal to true.

      for (TAO_ProtocolFactorySetItor factory =
             orb_core->protocol_factories ()->begin ();
           factory != end;
           ++factory)
        {
          if ((*factory)->factory ()->match_prefix (prefix))
            {
              // increment slot past the "://" (i.e. add 3)
              ACE_CString addrs = iop.substring (slot + 3);

              int const result = this->open_i (orb_core,
                                               reactor,
                                               addrs,
                                               factory,
                                               ignore_address);

              if (result != 0)
                {
                  return -1;
                }

              found = true;  // A usable protocol was found.
            }
          else
            {
              continue;
            }
        }

      if (found == false)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO (%P|%t) ")
                      ACE_TEXT ("no usable transport protocol ")
                      ACE_TEXT ("was found.\n")));

          throw ::CORBA::BAD_PARAM (
            CORBA::SystemException::_tao_minor_code (
              TAO_ACCEPTOR_REGISTRY_OPEN_LOCATION_CODE,
              EINVAL),
            CORBA::COMPLETED_NO);
        }
    }

  return 0;
}

// Iterate through the loaded transport protocols and create a default
// server for each protocol.
int TAO_Acceptor_Registry::open_default (TAO_ORB_Core *orb_core,
                                         ACE_Reactor *reactor,
                                         const char *options)
{
  // Flag that indicates at least one endpoint was opened.  If one
  // wasn't opened then there is a problem.
  bool opened_endpoint = false;

#if defined (ACE_WIN32) && defined (ACE_HAS_IPV6)
  if (!orb_core->orb_params ()->connect_ipv6_only () && this->acceptors_ == 0)
    {
      // Since Win32 has single-stack IPv4/IPv6 we need an additional
      // acceptor to open explicitly on IPv6 ANY *and* IPv4 ANY.
      // This code takes care of that.

      TAO_ProtocolFactorySet *pfs = orb_core->protocol_factories ();

      // Number of acceptors to set up
      size_t acceptor_count = 0;

      const TAO_ProtocolFactorySetItor end = pfs->end ();

      // Loop through all the loaded protocols...
      for (TAO_ProtocolFactorySetItor i = pfs->begin (); i != end; ++i)
        {
          if (!(*i)->factory ()->requires_explicit_endpoint ())
            {
              if ((*i)->factory ()->tag () == IOP::TAG_INTERNET_IOP)
                acceptor_count += 2; // IPv4 AND IPv6 endpoints
              else
                acceptor_count += 1;
            }
        }

      // Allocate TAO_Acceptor array
      ACE_NEW_RETURN (this->acceptors_,
                      TAO_Acceptor *[acceptor_count],
                      -1);

      // Loop through all the loaded protocols...
      for (TAO_ProtocolFactorySetItor i = pfs->begin (); i != end; ++i)
        {
          // If the protocol requires an explicit -ORBEndpoint option then
          // don't use it, otherwise open a default endpoint for that
          // protocol, this solves the problem with persistent endpoints
          // (such as UNIX domain rendesvouz points) that are not cleaned
          // up if the server crashes.
          if (!(*i)->factory ()->requires_explicit_endpoint ())
            {
              // Make an acceptor
              TAO_Acceptor *acceptor =
                (*i)->factory ()->make_acceptor ();

              if (acceptor == 0)
                {
                  if (TAO_debug_level > 0)
                    {
                      ACE_ERROR ((
                          LM_ERROR,
                          ACE_TEXT ("TAO (%P|%t) unable to create ")
                          ACE_TEXT ("an acceptor for <%C>\n"),
                          (*i)->protocol_name ().c_str ()
                        ));
                    }

                  return -1;
                }

              if ((*i)->factory ()->tag () == IOP::TAG_INTERNET_IOP)
                {
                  // Open first acceptor on IPv4 ANY
                  ACE_INET_Addr addr(static_cast<unsigned short> (0));

                  TAO_IIOP_Acceptor* iiop_acceptor =
                    dynamic_cast<TAO_IIOP_Acceptor*> (acceptor);

                  if (!iiop_acceptor)
                    return -1;

                  iiop_acceptor->set_default_address (addr);

                  if (this->open_default_i (orb_core,
                                            reactor,
                                            TAO_DEF_GIOP_MAJOR,
                                            TAO_DEF_GIOP_MINOR,
                                            i,
                                            acceptor,
                                            options) != 0)
                    {
                      return -1;
                    }

                  // record the port chosen for the IPv4 acceptor
                  u_short port =
                    iiop_acceptor->default_address ().get_port_number ();

                  // Create second acceptor for IPV6 traffic
                  acceptor =
                    (*i)->factory ()->make_acceptor ();

                  if (acceptor == 0)
                    {
                      if (TAO_debug_level > 0)
                        {
                          ACE_ERROR ((
                              LM_ERROR,
                              ACE_TEXT ("TAO (%P|%t) unable to create ")
                              ACE_TEXT ("an acceptor for <%C>\n"),
                              (*i)->protocol_name ().c_str ()
                            ));
                        }

                      return -1;
                    }

                  if (ACE::ipv6_enabled() &&
                      addr.set (port, ACE_IPV6_ANY, 1, AF_INET6) == 0)
                    {
                      iiop_acceptor =
                        dynamic_cast<TAO_IIOP_Acceptor*> (acceptor);

                      if (!iiop_acceptor)
                        return -1;

                      iiop_acceptor->set_default_address (addr);

                      if (this->open_default_i (orb_core,
                          reactor,
                          TAO_DEF_GIOP_MAJOR,
                          TAO_DEF_GIOP_MINOR,
                          i,
                          acceptor,
                          options) != 0)
                        {
                          return -1;
                        }
                    }
                }
              else
                {
                  if (this->open_default_i (orb_core,
                                            reactor,
                                            TAO_DEF_GIOP_MAJOR,
                                            TAO_DEF_GIOP_MINOR,
                                            i,
                                            acceptor,
                                            options) != 0)
                    {
                      return -1;
                    }
                }

              opened_endpoint = true;
            }
        }
    }
  else
    {
#endif /* ACE_WIN32 && ACE_HAS_IPV6 */
  TAO_ProtocolFactorySet *pfs = orb_core->protocol_factories ();

  // If the TAO_Acceptor array is zero by the time we get here then no
  // endpoints were specified by the user, meaning that the number of
  // acceptors will never be more than the number of loaded protocols
  // in the ORB core.
  if (this->acceptors_ == 0)
    {
      ACE_NEW_RETURN (this->acceptors_,
                      TAO_Acceptor *[pfs->size ()],
                      -1);
    }

  const TAO_ProtocolFactorySetItor end = pfs->end ();

  // Loop through all the loaded protocols...
  for (TAO_ProtocolFactorySetItor i = pfs->begin (); i != end; ++i)
    {
      // If the protocol requires an explicit -ORBEndpoint option then
      // don't use it, otherwise open a default endpoint for that
      // protocol, this solves the problem with persistent endpoints
      // (such as UNIX domain rendesvouz points) that are not cleaned
      // up if the server crashes.
      if (!(*i)->factory ()->requires_explicit_endpoint ())
        {
          if (this->open_default (orb_core,
                                  reactor,
                                  TAO_DEF_GIOP_MAJOR,  // default major
                                  TAO_DEF_GIOP_MINOR,  // default minor
                                  i,
                                  options) != 0)
            {
              return -1;
            }

          opened_endpoint = true;
        }
    }
#if defined (ACE_WIN32) && defined (ACE_HAS_IPV6)
    }
#endif /* ACE_WIN32) && ACE_HAS_IPV6 */

  if (!opened_endpoint)
    {
      if (TAO_debug_level > 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO (%P|%t) No default endpoints ")
                      ACE_TEXT ("opened.\n")
                      ACE_TEXT ("Please specify one or more using ")
                      ACE_TEXT ("the \"-ORBListenEndpoints\" ")
                      ACE_TEXT ("option.\n")));
        }

      return -1;
    }

  return 0;
}

// Used when no endpoints were specified.  Open a default server for
// the indicated protocol.
int
TAO_Acceptor_Registry::open_default (TAO_ORB_Core *orb_core,
                                     ACE_Reactor *reactor,
                                     int major,
                                     int minor,
                                     TAO_ProtocolFactorySetItor &factory,
                                     const char *options)
{
  // No endpoints were specified, we let each protocol pick its own
  // default endpoint.

  // Make an acceptor
  TAO_Acceptor *acceptor = (*factory)->factory ()->make_acceptor ();

  if (acceptor == 0)
    {
      if (TAO_debug_level > 0)
        {
          ACE_ERROR ((
              LM_ERROR,
              ACE_TEXT ("TAO (%P|%t) unable to create ")
              ACE_TEXT ("an acceptor for <%C>\n"),
              (*factory)->protocol_name ().c_str ()
            ));
        }

      return -1;
    }

  return this->open_default_i (orb_core,
                               reactor,
                               major,
                               minor,
                               factory,
                               acceptor,
                               options);
}

// Open a default server.
int
TAO_Acceptor_Registry::open_default_i (TAO_ORB_Core *orb_core,
                                       ACE_Reactor *reactor,
                                       int major,
                                       int minor,
                                       TAO_ProtocolFactorySetItor &factory,
                                       TAO_Acceptor* acceptor,
                                       const char *options)
{
  // Initialize the acceptor to listen on a default endpoint.
  if (acceptor->open_default (orb_core, reactor, major, minor, options) == -1)
    {
      delete acceptor;

      if (TAO_debug_level > 0)
        {
          ACE_ERROR ((
              LM_ERROR,
              ACE_TEXT ("TAO (%P|%t) - Unable to open ")
              ACE_TEXT ("default acceptor for <%C>%p\n"),
              (*factory)->protocol_name ().c_str (),
              ACE_TEXT ("")
            ));
        }

      return -1;
    }

  this->acceptors_[this->size_++] = acceptor;

  return 0;
}

int
TAO_Acceptor_Registry::close_all (void)
{
  const TAO_AcceptorSetIterator end = this->end ();

  for (TAO_AcceptorSetIterator i = this->begin (); i != end; ++i)
    {
      if (*i != 0)
        {
          (*i)->close ();
          delete *i;
        }
    }

  this->size_ = 0;
  return 0;
}

void
TAO_Acceptor_Registry::extract_endpoint_options (ACE_CString &addrs,
                                                 ACE_CString &options,
                                                 TAO_Protocol_Factory *factory)
{
  ACE_CString::size_type const options_index =
    addrs.find (factory->options_delimiter ());

  if (options_index == addrs.length () - 1)
    {
      // Get rid of trailing option delimiter.
      addrs = addrs.substring (0, addrs.length () - 1);
    }
  else if (options_index != ACE_CString::npos)
    {
      options = addrs.substring (options_index + 1);
      addrs = addrs.substring (0, options_index);
    }
}

void
TAO_Acceptor_Registry::extract_endpoint_version (ACE_CString &address,
                                                 int &major,
                                                 int &minor)
{
  // Check if an "N.n@" version prefix was
  // specified.
  major = TAO_DEF_GIOP_MAJOR;
  minor = TAO_DEF_GIOP_MINOR;

  if (ACE_OS::ace_isdigit (address[0])
      && address[1] == '.'
      && ACE_OS::ace_isdigit (address[2])
      && address[3] == '@')
    {
      major = address[0] - '0';
      minor = address[2] - '0';
      address = address.substring (4);
    }
}

int
TAO_Acceptor_Registry::open_i (TAO_ORB_Core *orb_core,
                               ACE_Reactor *reactor,
                               ACE_CString &addrs,
                               TAO_ProtocolFactorySetItor &factory,
                               bool ignore_address)
{
  ACE_CString options_tmp;
  this->extract_endpoint_options (addrs, options_tmp, (*factory)->factory ());

  const char *options = 0;

  if (options_tmp.length () > 0)
    {
      options = options_tmp.c_str ();
    }

  char *last_addr = 0;
  ACE_Auto_Basic_Array_Ptr<char> addr_str (addrs.rep ());

  const char *astr = ACE_OS::strtok_r (addr_str.get (), ",", &last_addr);

  // Iterate over the addrs specified in the endpoint.

  do
    {
      // For the first time only through the loop, it is
      // possible for astr to be 0.  This indicates that
      // the user is requesting the default endpoint for
      // the specified protocol.
      ACE_CString address (astr == 0 ? "" : astr);

      TAO_Acceptor *acceptor = (*factory)->factory ()->make_acceptor ();

      if (acceptor != 0)
        {
          // Extract the desired endpoint/protocol version if one
          // exists.
          int major = TAO_DEF_GIOP_MAJOR;
          int minor = TAO_DEF_GIOP_MINOR;
          this->extract_endpoint_version (address, major, minor);

          // Check for existence of endpoint.
          if (ignore_address || address.length () == 0)
            {
              // Protocol prefix was specified without any endpoints.
              // All TAO pluggable protocols are expected to have the
              // ability to create a default endpoint.
#if defined (ACE_WIN32) && defined (ACE_HAS_IPV6)
              if ((*factory)->factory ()->tag () == IOP::TAG_INTERNET_IOP)
                {
                  // Open first acceptor on IPv4 ANY
                  ACE_INET_Addr addr(static_cast<unsigned short> (0)); // IPv4 ANY

                  TAO_IIOP_Acceptor* iiop_acceptor = dynamic_cast<TAO_IIOP_Acceptor*> (acceptor);

                  if (!iiop_acceptor)
                    return -1;

                  iiop_acceptor->set_default_address (addr);

                  if (this->open_default_i (orb_core,
                                            reactor,
                                            major,
                                            minor,
                                            factory,
                                            acceptor,
                                            options) == 0)
                    {
                      // record the port chosen for the IPv4 acceptor
                      u_short port = iiop_acceptor->default_address ().get_port_number ();

                      // Create second acceptor for IPV6 traffic
                      acceptor =
                        (*factory)->factory ()->make_acceptor ();

                      if (acceptor == 0)
                        {
                          if (TAO_debug_level > 0)
                            {
                              ACE_ERROR ((
                                  LM_ERROR,
                                  ACE_TEXT ("TAO (%P|%t) unable to create ")
                                  ACE_TEXT ("an acceptor for <%C>\n"),
                                  (*factory)->protocol_name ().c_str ()
                                ));
                            }

                          throw ::CORBA::NO_MEMORY (
                            CORBA::SystemException::_tao_minor_code (
                              TAO_ACCEPTOR_REGISTRY_OPEN_LOCATION_CODE,
                              ENOMEM),
                            CORBA::COMPLETED_NO);
                        }

                      addr.set (port, ACE_IPV6_ANY, AF_INET6); // IPv6 ANY on specified port

                      iiop_acceptor = dynamic_cast<TAO_IIOP_Acceptor*> (acceptor);
                      if (!iiop_acceptor)
                        return -1;

                      iiop_acceptor->set_default_address (addr);

                      if (this->open_default_i (orb_core,
                                                reactor,
                                                major,
                                                minor,
                                                factory,
                                                acceptor,
                                                options) == 0)
                        {
                          continue;
                        }
                    }

                }
              else
                {
                  if (this->open_default_i (orb_core,
                                            reactor,
                                            major,
                                            minor,
                                            factory,
                                            acceptor,
                                            options) == 0)
                    {
                      continue;
                    }
                }

              // Could not open a default endpoint, nor an explicit
              // one.
              throw ::CORBA::INTERNAL (
                CORBA::SystemException::_tao_minor_code (
                  TAO_ACCEPTOR_REGISTRY_OPEN_LOCATION_CODE,
                  0),
                CORBA::COMPLETED_NO);
#else /* ACE_WIN32 && ACE_HAS_IPV6 */
              if (this->open_default_i (orb_core,
                                        reactor,
                                        major,
                                        minor,
                                        factory,
                                        acceptor,
                                        options) == 0)
                continue;

              // Could not open a default endpoint, nor an explicit
              // one.
              else
                {
                  throw ::CORBA::INTERNAL (
                    CORBA::SystemException::_tao_minor_code (
                      TAO_ACCEPTOR_REGISTRY_OPEN_LOCATION_CODE,
                      0),
                    CORBA::COMPLETED_NO);
                }
#endif /* !ACE_WIN32 || !ACE_HAS_IPV6 */
            }
          // An explicit endpoint was provided.
          else if (acceptor->open (orb_core,
                                   reactor,
                                   major,
                                   minor,
                                   address.c_str (),
                                   options) == -1)
            {
              /* Need to save the errno value from the acceptor->open(),
               * because errno will get reset when we delete acceptor */
              int const errno_value = errno;
              delete acceptor;

              if (TAO_debug_level > 0)
                {
                  ACE_ERROR ((LM_ERROR,
                              ACE_TEXT ("TAO (%P|%t) - ")
                              ACE_TEXT ("Unable to open acceptor ")
                              ACE_TEXT ("for <%C>%p\n"),
                              address.c_str (),
                              ACE_TEXT ("")));
                }

              throw ::CORBA::BAD_PARAM (
                CORBA::SystemException::_tao_minor_code (
                  TAO_ACCEPTOR_REGISTRY_OPEN_LOCATION_CODE,
                  errno_value),
                CORBA::COMPLETED_NO);
            }

          // add acceptor to list
          this->acceptors_[this->size_++] = acceptor;
        }
      else
        {
          if (TAO_debug_level > 0)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("TAO (%P|%t) unable to create ")
                          ACE_TEXT ("an acceptor for <%C>.\n"),
                          address.c_str ()));
            }

          throw ::CORBA::NO_MEMORY (
            CORBA::SystemException::_tao_minor_code (
              TAO_ACCEPTOR_REGISTRY_OPEN_LOCATION_CODE,
              ENOMEM),
            CORBA::COMPLETED_NO);
        }
    }
  while (astr != 0 && (astr = ACE_OS::strtok_r (0, ",", &last_addr)) != 0);

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
