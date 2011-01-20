// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "tao/Strategies/COIOP_Connector.h"

#if defined (TAO_HAS_COIOP) && (TAO_HAS_COIOP != 0)

#include "ace/Connector.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/SystemException.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_string.h"
#include "tao/Strategies/COIOP_Profile.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_COIOP_Connector::TAO_COIOP_Connector (void)
  : TAO_Connector (TAO_TAG_COIOP_PROFILE)
{
}

TAO_COIOP_Connector::~TAO_COIOP_Connector (void)
{
}

int
TAO_COIOP_Connector::open (TAO_ORB_Core *orb_core)
{
  this->orb_core (orb_core);

  // Create our connect strategy
  if (this->create_connect_strategy () == -1)
    return -1;

  return 0;
}

int
TAO_COIOP_Connector::close (void)
{
  return 0;
}

int
TAO_COIOP_Connector::set_validate_endpoint (TAO_Endpoint *endpoint)
{
  TAO_COIOP_Endpoint *COIOP_endpoint =
    this->remote_endpoint (endpoint);

  if (COIOP_endpoint == 0)
    return -1;

  return 0;
}

TAO_Transport *
TAO_COIOP_Connector::make_connection (TAO::Profile_Transport_Resolver *,
                                      TAO_Transport_Descriptor_Interface &,
                                      ACE_Time_Value * /*max_wait_time*/)
{
  // No remote connection possible with COIOP
  return 0;
}

TAO_Profile *
TAO_COIOP_Connector::create_profile (TAO_InputCDR& cdr)
{
  TAO_Profile *pfile = 0;
  ACE_NEW_RETURN (pfile,
                  TAO_COIOP_Profile (this->orb_core ()),
                  0);

  int const r = pfile->decode (cdr);
  if (r == -1)
    {
      pfile->_decr_refcnt ();
      pfile = 0;
    }

  return pfile;
}

TAO_Profile *
TAO_COIOP_Connector::make_profile (void)
{
  // The endpoint should be of the form:
  //    N.n@uuid/object_key
  // or:
  //    uuid/object_key

  TAO_Profile *profile = 0;
  ACE_NEW_THROW_EX (profile,
                    TAO_COIOP_Profile (this->orb_core ()),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  return profile;
}

int
TAO_COIOP_Connector::check_prefix (const char *endpoint)
{
  // Check for a valid string
  if (!endpoint || !*endpoint)
    return -1;  // Failure

  const char *protocol[] = { "COIOP", "COIOPloc" };

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
  // Failure: not an COIOP IOR
  // DO NOT throw an exception here.
}

char
TAO_COIOP_Connector::object_key_delimiter (void) const
{
  return TAO_COIOP_Profile::object_key_delimiter_;
}

TAO_COIOP_Endpoint *
TAO_COIOP_Connector::remote_endpoint (TAO_Endpoint *endpoint)
{
  if (endpoint->tag () != TAO_TAG_COIOP_PROFILE)
    return 0;

  TAO_COIOP_Endpoint *COIOP_endpoint =
    dynamic_cast<TAO_COIOP_Endpoint *> (endpoint );

  return COIOP_endpoint;
}

int
TAO_COIOP_Connector::cancel_svc_handler (
  TAO_Connection_Handler * /* svc_handler */)
{
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_COIOP && TAO_HAS_COIOP != 0 */
