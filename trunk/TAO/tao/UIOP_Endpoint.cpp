// This may look like C, but it's really -*- C++ -*-
// $Id$


#include "tao/UIOP_Endpoint.h"
#include "tao/UIOP_Connect.h"

#if TAO_HAS_UIOP == 1

#include "tao/debug.h"

ACE_RCSID(tao, UIOP_Endpoint, "$Id$")

#if !defined (__ACE_INLINE__)
# include "tao/UIOP_Endpoint.i"
#endif /* __ACE_INLINE__ */

TAO_UIOP_Endpoint::TAO_UIOP_Endpoint (const ACE_UNIX_Addr &addr)
  : TAO_Endpoint (TAO_TAG_UIOP_PROFILE),
    object_addr_ (addr),
    hint_ (0),
    next_ (0)
{
}

TAO_UIOP_Endpoint::TAO_UIOP_Endpoint (void)
  : TAO_Endpoint (TAO_TAG_UIOP_PROFILE),
    object_addr_ (),
    hint_ (0),
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
  if (this->hint_)
    this->hint_->cleanup_hint ((void **) &this->hint_);
}

#endif  /* TAO_HAS_UIOP == 1 */
