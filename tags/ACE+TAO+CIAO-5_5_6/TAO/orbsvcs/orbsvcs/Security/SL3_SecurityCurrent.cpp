// $Id$

#include "orbsvcs/Security/SL3_SecurityCurrent.h"


ACE_RCSID (Security,
           SL3_SecurityCurrent,
           "$Id$")


#if !defined (__ACE_INLINE__)
# include "orbsvcs/Security/SL3_SecurityCurrent.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::SL3::SecurityCurrent::SecurityCurrent (size_t tss_slot,
                                            TAO_ORB_Core * oc)
  : tss_slot_ (tss_slot),
    orb_core_ (oc)
{
}


TAO::SL3::SecurityCurrent::~SecurityCurrent (void)
{
}

SecurityLevel3::ClientCredentials_ptr
TAO::SL3::SecurityCurrent::client_credentials (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO::SL3::SecurityCurrent_Impl * impl = this->implementation ();

  // If the implementation pointer returned from TSS is zero, then
  // we're not in the middle of a request/upcall.  Throw an exception
  // to indicate that.
  if (impl == 0)
    ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (),
                      SecurityLevel3::ClientCredentials::_nil ());

  return impl->client_credentials ();
}

CORBA::Boolean
TAO::SL3::SecurityCurrent::request_is_local (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO::SL3::SecurityCurrent_Impl * impl = this->implementation ();

  // If the implementation pointer returned from TSS is zero, then
  // we're not in the middle of a request/upcall.  Throw an exception
  // to indicate that.
  if (impl == 0)
    ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (), false);

  return impl->request_is_local ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
