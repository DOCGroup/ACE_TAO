// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "SSLIOP_Profile.h"
#include "SSLIOP_Connect.h"
#include "tao/GIOP.h"
#include "tao/CDR.h"
#include "tao/Environment.h"
#include "tao/ORB.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"

ACE_RCSID(tao, SSLIOP_Profile, "$Id$")

#if !defined (__ACE_INLINE__)
# include "SSLIOP_Profile.i"
#endif /* __ACE_INLINE__ */

TAO_SSLIOP_Profile::TAO_SSLIOP_Profile (const ACE_INET_Addr &addr,
                                        const TAO_ObjectKey &object_key,
                                        const TAO_GIOP_Version &version,
                                        TAO_ORB_Core *orb_core,
                                        u_short ssl_port)
  : TAO_IIOP_Profile (addr,
                      object_key,
                      version,
                      orb_core),
    ssl_hint_ (0)
{
  this->ssl_component_.port = ssl_port;
}

TAO_SSLIOP_Profile::TAO_SSLIOP_Profile (const char* host,
                                        CORBA::UShort port,
                                        const TAO_ObjectKey &object_key,
                                        const ACE_INET_Addr &addr,
                                        const TAO_GIOP_Version &version,
                                        TAO_ORB_Core *orb_core,
                                        u_short ssl_port)
  : TAO_IIOP_Profile (host,
                      port,
                      object_key,
                      addr,
                      version,
                      orb_core),
    ssl_hint_ (0)
{
  this->ssl_component_.port = ssl_port;
}

TAO_SSLIOP_Profile::TAO_SSLIOP_Profile (const char *string,
                                        TAO_ORB_Core *orb_core,
                                        u_short ssl_port,
                                        CORBA::Environment &ACE_TRY_ENV)
  : TAO_IIOP_Profile (string, orb_core, ACE_TRY_ENV),
    ssl_hint_ (0)
{
  this->ssl_component_.port = ssl_port;
}

TAO_SSLIOP_Profile::TAO_SSLIOP_Profile (TAO_ORB_Core *orb_core)
  : TAO_IIOP_Profile (orb_core),
    ssl_hint_ (0)
{
  this->ssl_component_.port = 0;
}

TAO_SSLIOP_Profile::~TAO_SSLIOP_Profile (void)
{
}

void
TAO_SSLIOP_Profile::reset_hint (void)
{
  this->TAO_IIOP_Profile::reset_hint ();
  // @@ Who is doing the locking here!
  if (this->ssl_hint_)
    {
      this->ssl_hint_->cleanup_hint ();
      this->ssl_hint_ = 0;
    }
}

// return codes:
// -1 -> error
//  0 -> can't understand this version
//  1 -> success.
int
TAO_SSLIOP_Profile::decode (TAO_InputCDR& cdr)
{
  int r = this->TAO_IIOP_Profile::decode (cdr);
  if (r != 1)
    return r;

  IOP::TaggedComponent component;
  component.tag = IOP::TAG_SSL_SEC_TRANS;
  if (this->tagged_components ().get_component (component) == 0)
    {
      this->ssl_component_.port = 0;
      return 1;
    }
  else
    {
      TAO_InputCDR cdr (
          ACE_reinterpret_cast (const char*,
                                component.component_data.get_buffer ()),
          component.component_data.length ());
      CORBA::Boolean byte_order;
      if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
        return -1;
      cdr.reset_byte_order (ACE_static_cast(int,byte_order));

      cdr >> this->ssl_component_;
    }

  return 1;
}

CORBA::Boolean
TAO_SSLIOP_Profile::is_equivalent (const TAO_Profile *other_profile)
{
  if (this->TAO_IIOP_Profile::is_equivalent (other_profile) == 0)
    return 0;

  const TAO_SSLIOP_Profile *op =
    ACE_dynamic_cast (const TAO_SSLIOP_Profile *, other_profile);

  if (this->ssl_component_.port != 0
      && op->ssl_component_.port != 0
      && this->ssl_component_.port != op->ssl_component_.port)
    return 0;
  // @@ Anything else to check????

  return 1;
}
