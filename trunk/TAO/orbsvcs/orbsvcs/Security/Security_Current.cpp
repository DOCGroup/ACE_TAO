// -*- C++ -*-

#include "Security_Current.h"
#include "tao/debug.h"

ACE_RCSID (TAO_Security,
           Security_Current,
           "$Id$")

#if !defined (__ACE_INLINE__)
# include "Security_Current.inl"
#endif /* __ACE_INLINE__ */

TAO_Security_Current::TAO_Security_Current (size_t tss_slot,
                                            const char *orb_id)
  : tss_slot_ (tss_slot),
    orb_id_ (orb_id),
    orb_core_ (0)
{
}

TAO_Security_Current::~TAO_Security_Current (void)
{
}

Security::AttributeList *
TAO_Security_Current::get_attributes (
    const Security::AttributeTypeList &attributes
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_Security_Current_Impl *impl = this->implementation ();

  // If the implementation pointer returned from TSS is zero, then
  // we're not in the middle of a request/upcall.  Throw an exception
  // to indicate that.
  if (impl == 0)
    ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (), 0);

  return impl->get_attributes (attributes ACE_ENV_ARG_PARAMETER);
}

SecurityLevel2::ReceivedCredentials_ptr
TAO_Security_Current::received_credentials (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_Security_Current_Impl *impl = this->implementation ();

  // If the implementation pointer returned from TSS is zero, then
  // we're not in the middle of a request/upcall.  Throw an exception
  // to indicate that.
  if (impl == 0)
    ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (), 0);

  return impl->received_credentials (ACE_ENV_SINGLE_ARG_PARAMETER);
}

int
TAO_Security_Current::init (void)
{
  int result = 0;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      int argc = 0;
      char **argv = 0;
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            this->orb_id_.in ()
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->orb_core_ = orb.in ()->orb_core ();

      // No longer need the ORBid, so reclaim the memory it was
      // occupying.
      (void) this->orb_id_.out ();
    }
  ACE_CATCHANY
    {
      if (TAO_debug_level >= 1)
        ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                             "Could not initialize SecurityCurrent:");

      result = -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return result;
}
