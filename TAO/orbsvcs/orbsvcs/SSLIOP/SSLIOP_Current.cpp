// -*- C++ -*-
//
// $Id$

#include "SSLIOP_Current.h"

ACE_RCSID (TAO_SSLIOP, SSLIOP_Current, "$Id$")

#if !defined (__ACE_INLINE__)
# include "SSLIOP_Current.inl"
#endif /* __ACE_INLINE__ */

TAO_SSLIOP_Current::TAO_SSLIOP_Current (size_t tss_slot,
                                        const char *orb_id)
  : tss_slot_ (tss_slot),
    orb_id_ (orb_id),
    orb_core_ (0),
    setup_done_ (0),
    previous_current_impl_ (0)
{
}

TAO_SSLIOP_Current::~TAO_SSLIOP_Current (void)
{
}

SSLIOP::ASN_1_Cert *
TAO_SSLIOP_Current::get_peer_certificate (
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   SSLIOP::Current::NoContext))
{
  TAO_SSLIOP_Current_Impl *impl = this->implementation ();

  // If the implementation pointer returned from TSS is zero, then
  // we're not in the middle of a request or an upcall.  Throw an
  // exception to indicate that.
  if (impl == 0)
    ACE_THROW_RETURN (SSLIOP::Current::NoContext (), 0);

  // A valid value must always be returned, so instantiate a sequence
  // regardless of whether or not it is populated with certificates.
  SSLIOP::ASN_1_Cert *c = 0;
  ACE_NEW_THROW_EX (c,
                    SSLIOP::ASN_1_Cert,
                    CORBA::NO_MEMORY (
                      CORBA_SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (0);

  SSLIOP::ASN_1_Cert_var certificate = c;

  // Populate the sequence with the DER encoded certificate.
  impl->get_peer_certificate (c);

  return certificate._retn ();
}

SSLIOP::SSL_Cert *
TAO_SSLIOP_Current::get_peer_certificate_chain (
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   SSLIOP::Current::NoContext))
{
  TAO_SSLIOP_Current_Impl *impl = this->implementation ();

  // If the implementation pointer returned from TSS is zero, then
  // we're not in the middle of a request or an upcall.  Throw an
  // exception to indicate that.
  if (impl == 0)
    ACE_THROW_RETURN (SSLIOP::Current::NoContext (), 0);

  // A valid value must always be returned, so instantiate a sequence
  // regardless of whether or not it is populated with certificates.
  SSLIOP::SSL_Cert *c = 0;
  ACE_NEW_THROW_EX (c,
                    SSLIOP::SSL_Cert,
                    CORBA::NO_MEMORY (
                      CORBA_SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (0);

  SSLIOP::SSL_Cert_var cert_chain = c;

  // Populate the sequence with the chain of DER encoded certificates.
  impl->get_peer_certificate_chain (c);

  return cert_chain._retn ();
}

CORBA::Boolean
TAO_SSLIOP_Current::no_context (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return (this->implementation () == 0 ? 1 : 0);
}

void
TAO_SSLIOP_Current::setup (TAO_SSLIOP_Current_Impl *impl)
{
  // Set the current context and remember the old one.

  this->previous_current_impl_ = this->implementation ();

  (void) this->implementation (impl);  // Check for error?

  // Setup is complete.
  this->setup_done_ = 1;
}

void
TAO_SSLIOP_Current::teardown (void)
{
  if (this->setup_done_)
    {
      // Reset the old context.
      (void) this->implementation (this->previous_current_impl_);
      this->setup_done_ = 0;
    }
}

int
TAO_SSLIOP_Current::init (void)
{
  int result = 0;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      int argc = 0;
      char **argv = 0;
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            this->orb_id_.in (),
                                            ACE_TRY_ENV);
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
                             "Could not initialize SSLIOP::Current");

      result = -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return result;
}
