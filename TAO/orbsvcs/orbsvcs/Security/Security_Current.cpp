// -*- C++ -*-
#include "orbsvcs/Security/Security_Current.h"
#include "tao/debug.h"

#if !defined (__ACE_INLINE__)
# include "orbsvcs/Security/Security_Current.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Security_Current::TAO_Security_Current (size_t tss_slot,
                                            char const *orb_id)
  : tss_slot_ (tss_slot),
    orb_id_ (orb_id),
    orb_core_ (nullptr)
{
}

Security::AttributeList *
TAO_Security_Current::get_attributes (
    Security::AttributeTypeList const & attributes)
{
  TAO::Security::Current_Impl *impl = this->implementation ();

  // If the implementation pointer returned from TSS is zero, then
  // we're not in the middle of a request/upcall.  Throw an exception
  // to indicate that.
  if (impl == nullptr)
    throw CORBA::BAD_INV_ORDER ();

  return impl->get_attributes (attributes);
}

SecurityLevel2::ReceivedCredentials_ptr
TAO_Security_Current::received_credentials ()
{
  TAO::Security::Current_Impl *impl = this->implementation ();

  // If the implementation pointer returned from TSS is zero, then
  // we're not in the middle of a request/upcall.  Throw an exception
  // to indicate that.
  if (impl == nullptr)
    throw CORBA::BAD_INV_ORDER ();

  return impl->received_credentials ();
}

int
TAO_Security_Current::init ()
{
  int result {};

  try
    {
      int argc {};
      ACE_TCHAR **argv = nullptr;
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            this->orb_id_.in ());

      this->orb_core_ = orb.in ()->orb_core ();

      // No longer need the ORBid, so reclaim the memory it was
      // occupying.
      (void) this->orb_id_.out ();
    }
  catch (CORBA::Exception const & ex)
    {
      if (TAO_debug_level >= 1)
        ex._tao_print_exception ("Could not initialize SecurityCurrent:");

      result = -1;
    }

  return result;
}

TAO_END_VERSIONED_NAMESPACE_DECL
