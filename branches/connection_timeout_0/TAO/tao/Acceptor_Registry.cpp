// -*- C++ -*-
//
// $Id$

#include "tao/Acceptor_Registry.h"
#include "tao/Profile.h"
#include "tao/Pluggable.h"
#include "tao/Protocol_Factory.h"
#include "tao/Environment.h"
#include "tao/ORB_Core.h"
#include "tao/params.h"
#include "tao/MProfile.h"
#include "tao/debug.h"
#include "tao/Acceptor_Filter.h"
#include "tao/Endpoint.h"

#include "ace/Auto_Ptr.h"

#if !defined(__ACE_INLINE__)
#include "tao/Acceptor_Registry.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao,
           Acceptor_Registry,
           "$Id$")

TAO_Acceptor_Registry::TAO_Acceptor_Registry (void)
  : acceptors_ (0),
    size_ (0)
{
}

TAO_Acceptor_Registry::~TAO_Acceptor_Registry (void)
{
  this->close_all ();

  delete [] this->acceptors_;
  this->acceptors_ = 0;
  this->size_ = 0;
}

size_t
TAO_Acceptor_Registry::endpoint_count (void)
{
  int count = 0;
  TAO_AcceptorSetIterator end = this->end ();

  for (TAO_AcceptorSetIterator i = this->begin (); i != end; ++i)
    count += (*i)->endpoint_count ();

  return count;
}

int
TAO_Acceptor_Registry::is_collocated (const TAO_MProfile &mprofile)
{
  TAO_AcceptorSetIterator end = this->end ();

  // If at least one endpoint in one of the profiles matches one of
  // the acceptors, we are collocated.
  for (TAO_AcceptorSetIterator i = this->begin (); i != end; ++i)
    {
      for (TAO_PHandle j = 0;
           j != mprofile.profile_count ();
           ++j)
        {
          const TAO_Profile *profile = mprofile.get_profile (j);

          // @@ We need to invoke a nonconst <endpoint> method on
          // <profile>.  The content of profile/endpoint
          // will not be modified.
          TAO_Profile *pf =
            ACE_const_cast (TAO_Profile *,
                            profile);

          // Check all endpoints for address equality.
          if ((*i)->tag () == pf->tag ())
            for (TAO_Endpoint *endp = pf->endpoint ();
                 endp != 0;
                 endp = endp->next ())
              {
                if ((*i)->is_collocated (pf->endpoint ()))
                  return 1;
              }
        }
    }

  return 0;
}

TAO_Acceptor*
TAO_Acceptor_Registry::get_acceptor (CORBA::ULong tag)
{
  TAO_AcceptorSetIterator end =
    this->end ();
  TAO_AcceptorSetIterator acceptor =
    this->begin ();

  for (;
       acceptor != end ;
       ++acceptor)
    {
      if ((*acceptor)->tag () == tag)
        return *acceptor;
    }

  return 0;
}

int
TAO_Acceptor_Registry::open (TAO_ORB_Core *orb_core,
                             ACE_Reactor *reactor,
                             int ignore_address
                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // protocol_factories is in the following form
  //   IOP1://addr1,addr2,...,addrN/;IOP2://addr1,...addrM/;...
  TAO_EndpointSet endpoint_set = orb_core->orb_params ()->endpoints ();

  if (endpoint_set.is_empty ())
    {
      // No endpoints were specified, we let each protocol pick its
      // own default.

      // All TAO pluggable protocols are expected to have the ability
      // to create a default endpoint.
      if (this->open_default (orb_core,
                              reactor,
                              0) == -1)
        ACE_THROW_RETURN (CORBA::INTERNAL (
                            CORBA_SystemException::_tao_minor_code (
                              TAO_ACCEPTOR_REGISTRY_OPEN_LOCATION_CODE,
                              0),
                            CORBA::COMPLETED_NO),
                          -1);
    }

  // Count the maximum number of endpoints in the set.  This will be
  // the maximum number of acceptors that need to be created.
  size_t acceptor_count = 0;
  TAO_EndpointSetIterator endpts = endpoint_set.begin ();
  for (ACE_CString *ep = 0;
       endpts.next (ep) != 0;
       endpts.advance ())
    {
      const ACE_CString &iop = (*ep);

      // IOP://address1,address2
      //    ^ slot
      int slot = iop.find ("://", 0);

      if (slot == iop.npos)
        {
          if (TAO_debug_level > 0)
            ACE_ERROR ((LM_ERROR,
                        ACE_LIB_TEXT ("(%P|%t) Invalid endpoint specification: ")
                        ACE_LIB_TEXT ("<%s>.\n"),
                        ACE_TEXT_CHAR_TO_TCHAR(iop.c_str ())));

          ACE_THROW_RETURN (CORBA::BAD_PARAM (
              CORBA_SystemException::_tao_minor_code (
                TAO_ACCEPTOR_REGISTRY_OPEN_LOCATION_CODE,
                EINVAL),
              CORBA::COMPLETED_NO),
            -1);
        }

      ++acceptor_count;  // We've got at least one acceptor so far.

      // Now count the number of commas.  That number will be the
      // remaining number of endpoints in the current endpoint
      // specification.
      const char *ep_end =
        ep->c_str () + ACE_OS_String::strlen (ep->c_str ());
      for (const char *e = ACE_OS_String::strchr (ep->c_str (), ',');
           e != 0 && e != ep_end;
           e = ACE_OS_String::strchr (e, ','))
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
                          CORBA_SystemException::_tao_minor_code (
                            TAO_ACCEPTOR_REGISTRY_OPEN_LOCATION_CODE,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_CHECK_RETURN (-1);
    }

  TAO_EndpointSetIterator endpoints = endpoint_set.begin ();
  for (ACE_CString *endpoint = 0;
       endpoints.next (endpoint) != 0;
       endpoints.advance ())
    {
      const ACE_CString &iop = (*endpoint);

      // IOP://address1,address2
      //    ^ slot
      int slot = iop.find ("://", 0);

      if (slot == iop.npos)
        {
          if (TAO_debug_level > 0)
            ACE_ERROR ((LM_ERROR,
                        ACE_LIB_TEXT ("(%P|%t) Invalid endpoint specification: ")
                        ACE_LIB_TEXT ("<%s>.\n"),
                        ACE_TEXT_CHAR_TO_TCHAR(iop.c_str ())));

          ACE_THROW_RETURN (CORBA::BAD_PARAM (
              CORBA_SystemException::_tao_minor_code (
                TAO_ACCEPTOR_REGISTRY_OPEN_LOCATION_CODE,
                EINVAL),
              CORBA::COMPLETED_NO),
            -1);
        }

      ACE_CString prefix = iop.substring (0, slot);

      // @@ We could move the protocol factory loop to the outermost
      //    level but for now we leave it inside the endpoint loop
      //    under the assumption that there will be more endpoints
      //    than protocols.

      // Now get the list of available protocol factories.
      TAO_ProtocolFactorySetItor end =
        orb_core->protocol_factories ()->end ();

      int found = 0;
      // If usable protocol (factory) is found then this will be
      // set equal to 1.

      for (TAO_ProtocolFactorySetItor factory =
             orb_core->protocol_factories ()->begin ();
           factory != end;
           ++factory)
        {
          if ((*factory)->factory ()->match_prefix (prefix))
            {
              // increment slot past the "://" (i.e. add 3)
              ACE_CString addrs = iop.substring (slot + 3);

              int result = this->open_i (orb_core,
                                         reactor,
                                         addrs,
                                         factory,
                                         ignore_address
                                          ACE_ENV_ARG_PARAMETER);
              ACE_CHECK_RETURN (-1);

              if (result != 0)
                return -1;

              found = 1;  // A usable protocol was found.
            }
          else
            continue;
        }

      if (found == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_LIB_TEXT ("TAO (%P|%t) ")
                      ACE_LIB_TEXT ("no usable transport protocol ")
                      ACE_LIB_TEXT ("was found.\n")));

          ACE_THROW_RETURN (CORBA::BAD_PARAM (
              CORBA_SystemException::_tao_minor_code (
                TAO_ACCEPTOR_REGISTRY_OPEN_LOCATION_CODE,
                EINVAL),
              CORBA::COMPLETED_NO),
            -1);
        }
    }

  // No longer need the endpoint set since all associated acceptors
  // have been opened by now.  Reclaim the memory used by the endpoint
  // set.
  endpoint_set.reset ();

  return 0;
}

// Iterate through the loaded transport protocols and create a default
// server for each protocol.
int TAO_Acceptor_Registry::open_default (TAO_ORB_Core *orb_core,
                                         ACE_Reactor *reactor,
                                         const char *options)
{
  TAO_ProtocolFactorySet *pfs = orb_core->protocol_factories ();

  // If the TAO_Acceptor array is zero by the time we get here then no
  // endpoints were specified by the user, meaning that the number of
  // acceptors will never be more than the number of loaded protocols
  // in the ORB core.
  if (this->acceptors_ == 0)
    ACE_NEW_RETURN (this->acceptors_,
                    TAO_Acceptor *[pfs->size ()],
                    -1);

  TAO_ProtocolFactorySetItor end = pfs->end ();

  // Flag that indicates at least one endpoint was opened.  If one
  // wasn't opened then there is a problem.
  int opened_endpoint = 0;

  // Loop through all the loaded protocols...
  for (TAO_ProtocolFactorySetItor i = pfs->begin ();
       i != end;
       ++i)
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
            return -1;

          opened_endpoint = 1;
        }
    }

  if (!opened_endpoint)
    {
      if (TAO_debug_level > 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_LIB_TEXT ("TAO (%P|%t) No default endpoints ")
                      ACE_LIB_TEXT ("opened.\n")
                      ACE_LIB_TEXT ("Please specify one or more using ")
                      ACE_LIB_TEXT ("the \"-ORBEndpoint\" option.\n")));
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
  TAO_Acceptor *acceptor =
    (*factory)->factory ()->make_acceptor ();

  if (acceptor == 0)
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    ACE_LIB_TEXT ("TAO (%P|%t) unable to create ")
                    ACE_LIB_TEXT ("an acceptor for <%s>\n"),
                    ACE_TEXT_CHAR_TO_TCHAR((*factory)->protocol_name ().c_str ())));

      return -1;
    }

  // Initialize the acceptor to listen on a default endpoint.
  if (acceptor->open_default (orb_core,
                              reactor,
                              major,
                              minor,
                              options) == -1)
    {
      delete acceptor;

      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    ACE_LIB_TEXT ("TAO (%P|%t) unable to open ")
                    ACE_LIB_TEXT ("default acceptor for <%s>%p\n"),
                    ACE_TEXT_CHAR_TO_TCHAR((*factory)->protocol_name ().c_str ()),
                    ACE_LIB_TEXT ("")));

      return -1;
    }

  this->acceptors_[this->size_++] = acceptor;

  return 0;
}

int
TAO_Acceptor_Registry::close_all (void)
{
  TAO_AcceptorSetIterator end = this->end ();

  for (TAO_AcceptorSetIterator i = this->begin ();
       i != end;
       ++i)
    {
      if (*i == 0)
        continue;

      (*i)->close ();

      delete *i;
    }

  this->size_ = 0;

  return 0;
}

void
TAO_Acceptor_Registry::extract_endpoint_options (ACE_CString &addrs,
                                                 ACE_CString &options,
                                                 TAO_Protocol_Factory *factory)
{
  int options_index =
    addrs.find (factory->options_delimiter ());

  if (options_index == ACE_static_cast (int,
                                        addrs.length () - 1))
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
  if (isdigit (address[0])
      && address[1] == '.'
      && isdigit (address[2])
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
                               int ignore_address
                                ACE_ENV_ARG_DECL)
{
  ACE_CString options_tmp;
  this->extract_endpoint_options (addrs,
                                  options_tmp,
                                  (*factory)->factory ());

  const char *options = 0;
  if (options_tmp.length () > 0)
    options = options_tmp.c_str ();

  char *last_addr = 0;
  ACE_Auto_Basic_Array_Ptr <char> addr_str (addrs.rep ());

  const char *astr = ACE_OS::strtok_r (addr_str.get (),
                                       ",",
                                       &last_addr);

  // Iterate over the addrs specified in the endpoint.

  do
    {
      // For the first time only through the loop, it is
      // possible for astr to be 0.  This indicates that
      // the user is requesting the default endpoint for
      // the specified protocol.
      ACE_CString address (astr == 0 ? "" : astr);

      TAO_Acceptor *acceptor =
        (*factory)->factory ()->make_acceptor ();
      if (acceptor != 0)
        {
          // Extract the desired endpoint/protocol version if one
          // exists.
          int major = TAO_DEF_GIOP_MAJOR;
          int minor = TAO_DEF_GIOP_MINOR;
          this->extract_endpoint_version (address,
                                          major,
                                          minor);

          // Check for existence of endpoint.
          if (address.length () == 0 ||
              ignore_address)
            {
              // Protocol prefix was specified without any endpoints.
              // All TAO pluggable protocols are expected to have the
              // ability to create a default endpoint.
              if (this->open_default (orb_core,
                                      reactor,
                                      major,
                                      minor,
                                      factory,
                                      options) == 0)
                continue;

              // Could not open a default endpoint, nor an explicit
              // one.
              else
                ACE_THROW_RETURN (CORBA::INTERNAL (
                    CORBA_SystemException::_tao_minor_code (
                      TAO_ACCEPTOR_REGISTRY_OPEN_LOCATION_CODE,
                      0),
                    CORBA::COMPLETED_NO),
                  -1);
            }

          // An explicit endpoint was provided.
          else if (acceptor->open (orb_core,
                                   reactor,
                                   major,
                                   minor,
                                   address.c_str (),
                                   options) == -1)
            {
              delete acceptor;

              if (TAO_debug_level > 0)
                ACE_ERROR ((LM_ERROR,
                            ACE_LIB_TEXT ("TAO (%P|%t) ")
                            ACE_LIB_TEXT ("unable to open acceptor ")
                            ACE_LIB_TEXT ("for <%s>%p\n"),
                            ACE_TEXT_CHAR_TO_TCHAR(address.c_str ()),
                            ACE_LIB_TEXT ("")));

              ACE_THROW_RETURN (CORBA::BAD_PARAM (
                  CORBA_SystemException::_tao_minor_code (
                    TAO_ACCEPTOR_REGISTRY_OPEN_LOCATION_CODE,
                    EINVAL),
                  CORBA::COMPLETED_NO),
                -1);
            }

          // add acceptor to list
          this->acceptors_[this->size_++] = acceptor;
        }
      else
        {
          if (TAO_debug_level > 0)
            ACE_ERROR ((LM_ERROR,
                        ACE_LIB_TEXT ("TAO (%P|%t) unable to create ")
                        ACE_LIB_TEXT ("an acceptor for <%s>.\n"),
                        ACE_TEXT_CHAR_TO_TCHAR(address.c_str ())));

          ACE_THROW_RETURN (CORBA::NO_MEMORY (
              CORBA_SystemException::_tao_minor_code (
                TAO_ACCEPTOR_REGISTRY_OPEN_LOCATION_CODE,
                ENOMEM),
              CORBA::COMPLETED_NO),
            -1);
        }
    }
  while ((astr != 0) &&
         ((astr = ACE_OS::strtok_r (0,
                                    ",",
                                    &last_addr)) != 0));

  return 0;
}
