// This may look like C, but it's really -*- C++ -*-
// $Id$


#include "UIOP_Endpoint.h"
#include "UIOP_Connection_Handler.h"

#if TAO_HAS_UIOP == 1

ACE_RCSID(Strategies, UIOP_Endpoint, "$Id$")

#if !defined (__ACE_INLINE__)
# include "UIOP_Endpoint.i"
#endif /* __ACE_INLINE__ */

TAO_UIOP_Endpoint::TAO_UIOP_Endpoint (const ACE_UNIX_Addr &addr)
  : TAO_Endpoint (TAO_TAG_UIOP_PROFILE),
    object_addr_ (addr),
    next_ (0)
{
}

TAO_UIOP_Endpoint::TAO_UIOP_Endpoint (void)
  : TAO_Endpoint (TAO_TAG_UIOP_PROFILE),
    object_addr_ (),
    next_ (0)
{
}

TAO_UIOP_Endpoint::~TAO_UIOP_Endpoint (void)
{
}

int
TAO_UIOP_Endpoint::addr_to_string (char *buffer, size_t length)
{
  if (length < (ACE_OS::strlen (this->rendezvous_point ()) + 1))
    return -1;

  ACE_OS::strcpy (buffer, this->rendezvous_point ());

  return 0;
}

void
TAO_UIOP_Endpoint::reset_hint (void)
{
  //  if (this->hint_)
  //this->hint_->cleanup_hint ((void **) &this->hint_);
}

TAO_Endpoint *
TAO_UIOP_Endpoint::next (void)
{
  return this->next_;
}

TAO_Endpoint *
TAO_UIOP_Endpoint::duplicate (void)
{
  TAO_UIOP_Endpoint *endpoint = 0;
  ACE_NEW_RETURN (endpoint,
                  TAO_UIOP_Endpoint (this->object_addr_),
                  0);

  return endpoint;
}

CORBA::Boolean
TAO_UIOP_Endpoint::is_equivalent (const TAO_Endpoint *other_endpoint)
{
  TAO_Endpoint *endpt = ACE_const_cast (TAO_Endpoint *,
                                        other_endpoint);

  TAO_UIOP_Endpoint *endpoint = ACE_dynamic_cast (TAO_UIOP_Endpoint *,
                                                  endpt);
  if (endpoint == 0)
    return 0;

  return
    ACE_OS::strcmp (this->rendezvous_point (),
                    endpoint->rendezvous_point ()) == 0;
}

CORBA::ULong
TAO_UIOP_Endpoint::hash (void)
{
  return ACE::hash_pjw (this->rendezvous_point ());
}
#endif  /* TAO_HAS_UIOP == 1 */
