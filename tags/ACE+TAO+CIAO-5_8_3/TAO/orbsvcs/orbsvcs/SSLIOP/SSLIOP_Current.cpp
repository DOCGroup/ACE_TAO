// $Id$

#include "orbsvcs/SSLIOP/SSLIOP_Current.h"
#include "tao/debug.h"

#if !defined (__ACE_INLINE__)
# include "orbsvcs/SSLIOP/SSLIOP_Current.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::SSLIOP::Current::Current (TAO_ORB_Core *orb_core)
  : tss_slot_ (0),
    orb_core_ (orb_core)
{
}

TAO::SSLIOP::Current::~Current (void)
{
}

::SSLIOP::ASN_1_Cert *
TAO::SSLIOP::Current::get_peer_certificate ()
{
  TAO::SSLIOP::Current_Impl *impl = this->implementation ();

  // If the implementation pointer returned from TSS is zero, then
  // we're not in the middle of a request or an upcall.  Throw an
  // exception to indicate that.
  if (impl == 0)
    throw ::SSLIOP::Current::NoContext ();

  // A valid value must always be returned, so instantiate a sequence
  // regardless of whether or not it is populated with certificates.
  ::SSLIOP::ASN_1_Cert *c = 0;
  ACE_NEW_THROW_EX (c,
                    ::SSLIOP::ASN_1_Cert,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  ::SSLIOP::ASN_1_Cert_var certificate = c;

  // Populate the sequence with the DER encoded certificate.
  impl->get_peer_certificate (c);

  return certificate._retn ();
}

SSLIOP::SSL_Cert *
TAO::SSLIOP::Current::get_peer_certificate_chain ()
{
  TAO::SSLIOP::Current_Impl *impl = this->implementation ();

  // If the implementation pointer returned from TSS is zero, then
  // we're not in the middle of a request or an upcall.  Throw an
  // exception to indicate that.
  if (impl == 0)
    throw SSLIOP::Current::NoContext ();

  // A valid value must always be returned, so instantiate a sequence
  // regardless of whether or not it is populated with certificates.
  ::SSLIOP::SSL_Cert *c = 0;
  ACE_NEW_THROW_EX (c,
                    ::SSLIOP::SSL_Cert,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  ::SSLIOP::SSL_Cert_var cert_chain = c;

  // Populate the sequence with the chain of DER encoded certificates.
  impl->get_peer_certificate_chain (c);

  return cert_chain._retn ();
}

CORBA::Boolean
TAO::SSLIOP::Current::no_context (void)
{
  return (this->implementation () == 0 ? true : false);
}

void
TAO::SSLIOP::Current::setup (TAO::SSLIOP::Current_Impl *&prev_impl,
                             TAO::SSLIOP::Current_Impl *new_impl,
                             bool &setup_done)
{
  // Set the current context and remember the old one.

  prev_impl = this->implementation ();

  (void) this->implementation (new_impl);  // Check for error?

  // Setup is complete.
  setup_done = true;
}

void
TAO::SSLIOP::Current::teardown (TAO::SSLIOP::Current_Impl *prev_impl,
                                bool &setup_done)
{
  if (setup_done)
    {
      // Reset the old context.
      (void) this->implementation (prev_impl);
      setup_done = false;
    }
}

TAO::SSLIOP::Current_ptr
TAO::SSLIOP::Current::_narrow (CORBA::Object_ptr obj)
{
  return  TAO::SSLIOP::Current::_duplicate (
              dynamic_cast<TAO::SSLIOP::Current *> (obj));
}

TAO::SSLIOP::Current_ptr
TAO::SSLIOP::Current::_duplicate (TAO::SSLIOP::Current_ptr obj)
{
  if (!CORBA::is_nil (obj))
    obj->_add_ref ();

  return obj;
}

const char *
TAO::SSLIOP::Current::_interface_repository_id (void) const
{
  return "IDL:TAO/SSLIOP/Current:1.0";
}

// ----------------------------------------------------------------

TAO::SSLIOP::Current_ptr
tao_TAO_SSLIOP_Current_duplicate (TAO::SSLIOP::Current_ptr p)
{
  return TAO::SSLIOP::Current::_duplicate (p);
}

void
tao_TAO_SSLIOP_Current_release (TAO::SSLIOP::Current_ptr p)
{
  CORBA::release (p);
}

TAO::SSLIOP::Current_ptr
tao_TAO_SSLIOP_Current_nil (void)
{
  return TAO::SSLIOP::Current::_nil ();
}

TAO::SSLIOP::Current_ptr
tao_TAO_SSLIOP_Current_narrow (CORBA::Object *p)
{
  return TAO::SSLIOP::Current::_narrow (p);
}

CORBA::Object *
tao_TAO_SSLIOP_Current_upcast (void *src)
{
  TAO::SSLIOP::Current **tmp =
    static_cast<TAO::SSLIOP::Current **> (src);
  return *tmp;
}

TAO_END_VERSIONED_NAMESPACE_DECL
