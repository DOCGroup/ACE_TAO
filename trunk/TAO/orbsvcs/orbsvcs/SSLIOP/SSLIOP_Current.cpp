#include "SSLIOP_Current.h"
#include "tao/debug.h"


ACE_RCSID (SSLIOP,
           SSLIOP_Current,
           "$Id$")


#if !defined (__ACE_INLINE__)
# include "SSLIOP_Current.inl"
#endif /* __ACE_INLINE__ */


TAO::SSLIOP::Current::Current (TAO_ORB_Core *orb_core)
  : tss_slot_ (0),
    orb_core_ (orb_core)
{
}

TAO::SSLIOP::Current::~Current (void)
{
}

::SSLIOP::ASN_1_Cert *
TAO::SSLIOP::Current::get_peer_certificate (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   SSLIOP::Current::NoContext))
{
  TAO::SSLIOP::Current_Impl *impl = this->implementation ();

  // If the implementation pointer returned from TSS is zero, then
  // we're not in the middle of a request or an upcall.  Throw an
  // exception to indicate that.
  if (impl == 0)
    ACE_THROW_RETURN (::SSLIOP::Current::NoContext (), 0);

  // A valid value must always be returned, so instantiate a sequence
  // regardless of whether or not it is populated with certificates.
  ::SSLIOP::ASN_1_Cert *c = 0;
  ACE_NEW_THROW_EX (c,
                    ::SSLIOP::ASN_1_Cert,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (0);

  ::SSLIOP::ASN_1_Cert_var certificate = c;

  // Populate the sequence with the DER encoded certificate.
  impl->get_peer_certificate (c);

  return certificate._retn ();
}

SSLIOP::SSL_Cert *
TAO::SSLIOP::Current::get_peer_certificate_chain (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   SSLIOP::Current::NoContext))
{
  TAO::SSLIOP::Current_Impl *impl = this->implementation ();

  // If the implementation pointer returned from TSS is zero, then
  // we're not in the middle of a request or an upcall.  Throw an
  // exception to indicate that.
  if (impl == 0)
    ACE_THROW_RETURN (SSLIOP::Current::NoContext (), 0);

  // A valid value must always be returned, so instantiate a sequence
  // regardless of whether or not it is populated with certificates.
  ::SSLIOP::SSL_Cert *c = 0;
  ACE_NEW_THROW_EX (c,
                    ::SSLIOP::SSL_Cert,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (0);

  ::SSLIOP::SSL_Cert_var cert_chain = c;

  // Populate the sequence with the chain of DER encoded certificates.
  impl->get_peer_certificate_chain (c);

  return cert_chain._retn ();
}

CORBA::Boolean
TAO::SSLIOP::Current::no_context (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return (this->implementation () == 0 ? 1 : 0);
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
TAO::SSLIOP::Current::_narrow (
  CORBA::Object_ptr obj
  ACE_ENV_ARG_DECL_NOT_USED)
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
tao_TAO_SSLIOP_Current_duplicate (
    TAO::SSLIOP::Current_ptr p
  )
{
  return TAO::SSLIOP::Current::_duplicate (p);
}

void
tao_TAO_SSLIOP_Current_release (
    TAO::SSLIOP::Current_ptr p
  )
{
  CORBA::release (p);
}

TAO::SSLIOP::Current_ptr
tao_TAO_SSLIOP_Current_nil (
    void
  )
{
  return TAO::SSLIOP::Current::_nil ();
}

TAO::SSLIOP::Current_ptr
tao_TAO_SSLIOP_Current_narrow (
    CORBA::Object *p
    ACE_ENV_ARG_DECL
  )
{
  return TAO::SSLIOP::Current::_narrow (p ACE_ENV_ARG_PARAMETER);
}

CORBA::Object *
tao_TAO_SSLIOP_Current_upcast (
    void *src
  )
{
  TAO::SSLIOP::Current **tmp =
    static_cast<TAO::SSLIOP::Current **> (src);
  return *tmp;
}

// *************************************************************
// Operations for class TAO::SSLIOP::Current_var
// *************************************************************

TAO::SSLIOP::Current_var::Current_var (void) // default constructor
  : ptr_ (TAO::SSLIOP::Current::_nil ())
{
}

::TAO::SSLIOP::Current_ptr
TAO::SSLIOP::Current_var::ptr (void) const
{
  return this->ptr_;
}

TAO::SSLIOP::Current_var::Current_var (
  const ::TAO::SSLIOP::Current_var &p)
  : TAO_Base_var (),
    ptr_ (TAO::SSLIOP::Current::_duplicate (p.ptr ()))
{
}

TAO::SSLIOP::Current_var::~Current_var (void)
{
  CORBA::release (this->ptr_);
}

TAO::SSLIOP::Current_var &
TAO::SSLIOP::Current_var::operator= (TAO::SSLIOP::Current_ptr p)
{
  CORBA::release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

TAO::SSLIOP::Current_var &
TAO::SSLIOP::Current_var::operator= (const ::TAO::SSLIOP::Current_var &p)
{
  if (this != &p)
  {
    CORBA::release (this->ptr_);
    this->ptr_ = ::TAO::SSLIOP::Current::_duplicate (p.ptr ());
  }
  return *this;
}

TAO::SSLIOP::Current_var::operator const ::TAO::SSLIOP::Current_ptr &() const
{
  return this->ptr_;
}

TAO::SSLIOP::Current_var::operator ::TAO::SSLIOP::Current_ptr &()
{
  return this->ptr_;
}

::TAO::SSLIOP::Current_ptr
TAO::SSLIOP::Current_var::operator-> (void) const
{
  return this->ptr_;
}

::TAO::SSLIOP::Current_ptr
TAO::SSLIOP::Current_var::in (void) const
{
  return this->ptr_;
}

::TAO::SSLIOP::Current_ptr &
TAO::SSLIOP::Current_var::inout (void)
{
  return this->ptr_;
}

::TAO::SSLIOP::Current_ptr &
TAO::SSLIOP::Current_var::out (void)
{
  CORBA::release (this->ptr_);
  this->ptr_ = ::TAO::SSLIOP::Current::_nil ();
  return this->ptr_;
}

::TAO::SSLIOP::Current_ptr
TAO::SSLIOP::Current_var::_retn (void)
{
  // yield ownership of managed obj reference
  ::TAO::SSLIOP::Current_ptr val = this->ptr_;
  this->ptr_ = ::TAO::SSLIOP::Current::_nil ();
  return val;
}

::TAO::SSLIOP::Current_ptr
TAO::SSLIOP::Current_var::tao_duplicate (TAO::SSLIOP::Current_ptr p)
{
  return ::TAO::SSLIOP::Current::_duplicate (p);
}

void
TAO::SSLIOP::Current_var::tao_release (TAO::SSLIOP::Current_ptr p)
{
  CORBA::release (p);
}

::TAO::SSLIOP::Current_ptr
TAO::SSLIOP::Current_var::tao_nil (void)
{
  return ::TAO::SSLIOP::Current::_nil ();
}

::TAO::SSLIOP::Current_ptr
TAO::SSLIOP::Current_var::tao_narrow (
    CORBA::Object *p
    ACE_ENV_ARG_DECL
  )
{
  return ::TAO::SSLIOP::Current::_narrow (p ACE_ENV_ARG_PARAMETER);
}

CORBA::Object *
TAO::SSLIOP::Current_var::tao_upcast (void *src)
{
  TAO::SSLIOP::Current **tmp =
    static_cast<TAO::SSLIOP::Current **> (src);
  return *tmp;
}
