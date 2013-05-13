// $Id$

#include "tao/Strategies/UIOP_Endpoint.h"
#include "tao/Strategies/UIOP_Connection_Handler.h"
#include "tao/ORB_Constants.h"
#include "ace/OS_NS_string.h"

#if TAO_HAS_UIOP == 1

#if !defined (__ACE_INLINE__)
# include "tao/Strategies/UIOP_Endpoint.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_UIOP_Endpoint::TAO_UIOP_Endpoint (const ACE_UNIX_Addr &addr,
                                      CORBA::Short priority)
  : TAO_Endpoint (TAO_TAG_UIOP_PROFILE, priority)
    , object_addr_ (addr)
    , next_ (0)
{
}

TAO_UIOP_Endpoint::TAO_UIOP_Endpoint (void)
  : TAO_Endpoint (TAO_TAG_UIOP_PROFILE)
    , object_addr_ ()
    , next_ (0)
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
                  TAO_UIOP_Endpoint (this->object_addr_,
                                     this->priority ()),
                  0);

  return endpoint;
}

CORBA::Boolean
TAO_UIOP_Endpoint::is_equivalent (const TAO_Endpoint *other_endpoint)
{
  TAO_Endpoint *endpt = const_cast<TAO_Endpoint *> (other_endpoint);

  TAO_UIOP_Endpoint *endpoint = dynamic_cast<TAO_UIOP_Endpoint *> (endpt);

  if (endpoint == 0)
    return 0;

  return ACE_OS::strcmp (this->rendezvous_point (),
                         endpoint->rendezvous_point ()) == 0;
}

CORBA::ULong
TAO_UIOP_Endpoint::hash (void)
{
  if (this->hash_val_ != 0)
    return this->hash_val_;

  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                      guard,
                      this->addr_lookup_lock_,
                      this->hash_val_);
    // .. DCL
    if (this->hash_val_ != 0)
      return this->hash_val_;

    this->hash_val_ =
      ACE::hash_pjw (this->rendezvous_point ());
  }

  return this->hash_val_;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_HAS_UIOP == 1 */
