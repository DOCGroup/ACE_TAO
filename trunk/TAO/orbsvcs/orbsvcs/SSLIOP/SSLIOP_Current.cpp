// -*- C++ -*-

#include "SSLIOP_Current.h"
#include "tao/debug.h"


ACE_RCSID (TAO_SSLIOP,
           SSLIOP_Current,
           "$Id$")


#if !defined (__ACE_INLINE__)
# include "SSLIOP_Current.inl"
#endif /* __ACE_INLINE__ */


int TAO_SSLIOP_Current::_tao_class_id = 0;


TAO_SSLIOP_Current::TAO_SSLIOP_Current (TAO_ORB_Core *orb_core)
  : tss_slot_ (0),
    orb_core_ (orb_core)
{
}

TAO_SSLIOP_Current::~TAO_SSLIOP_Current (void)
{
}

SSLIOP::ASN_1_Cert *
TAO_SSLIOP_Current::get_peer_certificate (
    ACE_ENV_SINGLE_ARG_DECL)
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
    ACE_ENV_SINGLE_ARG_DECL)
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
TAO_SSLIOP_Current::no_context (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return (this->implementation () == 0 ? 1 : 0);
}

void
TAO_SSLIOP_Current::setup (TAO_SSLIOP_Current_Impl *&prev_impl,
                           TAO_SSLIOP_Current_Impl *new_impl,
                           CORBA::Boolean &setup_done)
{
  // Set the current context and remember the old one.

  prev_impl = this->implementation ();

  (void) this->implementation (new_impl);  // Check for error?

  // Setup is complete.
  setup_done = 1;
}

void
TAO_SSLIOP_Current::teardown (TAO_SSLIOP_Current_Impl *prev_impl,
                              CORBA::Boolean &setup_done)
{
  if (setup_done)
    {
      // Reset the old context.
      (void) this->implementation (prev_impl);
      setup_done = 0;
    }
}

TAO_SSLIOP_Current_ptr
TAO_SSLIOP_Current::_narrow (
  CORBA::Object_ptr obj
  ACE_ENV_ARG_DECL)
{
  return TAO_SSLIOP_Current::_unchecked_narrow (obj ACE_ENV_ARG_PARAMETER);
}

TAO_SSLIOP_Current_ptr
TAO_SSLIOP_Current::_unchecked_narrow (
  CORBA::Object_ptr obj
  ACE_ENV_ARG_DECL_NOT_USED)
{
  if (CORBA::is_nil (obj))
    return TAO_SSLIOP_Current::_nil ();

  return
    ACE_reinterpret_cast
      (
         TAO_SSLIOP_Current_ptr,
         obj->_tao_QueryInterface
           (
             ACE_reinterpret_cast
             (
                ptr_arith_t,
                &TAO_SSLIOP_Current::_tao_class_id
             )
           )
       );
}

TAO_SSLIOP_Current_ptr
TAO_SSLIOP_Current::_duplicate (TAO_SSLIOP_Current_ptr obj)
{
  if (!CORBA::is_nil (obj))
    obj->_add_ref ();

  return obj;
}

void *
TAO_SSLIOP_Current::_tao_QueryInterface (ptr_arith_t type)
{
  void *retv = 0;
  if (type == ACE_reinterpret_cast
      (ptr_arith_t,
       &TAO_SSLIOP_Current::_tao_class_id))
    retv = ACE_reinterpret_cast (void*, this);
  else if (type == ACE_reinterpret_cast
           (ptr_arith_t,
            &::SSLIOP::Current::_tao_class_id))
    retv = ACE_reinterpret_cast
      (
       void *,
       ACE_static_cast
       (
        SSLIOP::Current_ptr,
        this
        )
       );
  else if (type == ACE_reinterpret_cast
           (ptr_arith_t,
            &::CORBA::Current::_tao_class_id))
    retv = ACE_reinterpret_cast
      (
       void *,
       ACE_static_cast
       (
        CORBA::Current_ptr,
        this
        )
       );
  else if (type == ACE_reinterpret_cast (ptr_arith_t,
                                         &CORBA::Object::_tao_class_id))
    retv = ACE_reinterpret_cast (void *,
                                 ACE_static_cast (CORBA::Object_ptr, this));

  if (retv)
    this->_add_ref ();

  return retv;
}

const char *
TAO_SSLIOP_Current::_interface_repository_id (void) const
{
  return "IDL:TAO_SSLIOP_Current:1.0";
}

// ----------------------------------------------------------------

TAO_SSLIOP_Current_ptr
tao_TAO_SSLIOP_Current_duplicate (
    TAO_SSLIOP_Current_ptr p
  )
{
  return TAO_SSLIOP_Current::_duplicate (p);
}

void
tao_TAO_SSLIOP_Current_release (
    TAO_SSLIOP_Current_ptr p
  )
{
  CORBA::release (p);
}

TAO_SSLIOP_Current_ptr
tao_TAO_SSLIOP_Current_nil (
    void
  )
{
  return TAO_SSLIOP_Current::_nil ();
}

TAO_SSLIOP_Current_ptr
tao_TAO_SSLIOP_Current_narrow (
    CORBA::Object *p
    ACE_ENV_ARG_DECL
  )
{
  return TAO_SSLIOP_Current::_narrow (p ACE_ENV_ARG_PARAMETER);
}

CORBA::Object *
tao_TAO_SSLIOP_Current_upcast (
    void *src
  )
{
  TAO_SSLIOP_Current **tmp =
    ACE_static_cast (TAO_SSLIOP_Current **, src);
  return *tmp;
}

// *************************************************************
// Operations for class TAO_SSLIOP_Current_var
// *************************************************************

TAO_SSLIOP_Current_var::TAO_SSLIOP_Current_var (void) // default constructor
  : ptr_ (TAO_SSLIOP_Current::_nil ())
{
}

::TAO_SSLIOP_Current_ptr
TAO_SSLIOP_Current_var::ptr (void) const
{
  return this->ptr_;
}

TAO_SSLIOP_Current_var::TAO_SSLIOP_Current_var (
  const ::TAO_SSLIOP_Current_var &p)
  : TAO_Base_var (),
    ptr_ (TAO_SSLIOP_Current::_duplicate (p.ptr ()))
{
}

TAO_SSLIOP_Current_var::~TAO_SSLIOP_Current_var (void)
{
  CORBA::release (this->ptr_);
}

TAO_SSLIOP_Current_var &
TAO_SSLIOP_Current_var::operator= (TAO_SSLIOP_Current_ptr p)
{
  CORBA::release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

TAO_SSLIOP_Current_var &
TAO_SSLIOP_Current_var::operator= (const ::TAO_SSLIOP_Current_var &p)
{
  if (this != &p)
  {
    CORBA::release (this->ptr_);
    this->ptr_ = ::TAO_SSLIOP_Current::_duplicate (p.ptr ());
  }
  return *this;
}

TAO_SSLIOP_Current_var::operator const ::TAO_SSLIOP_Current_ptr &() const
{
  return this->ptr_;
}

TAO_SSLIOP_Current_var::operator ::TAO_SSLIOP_Current_ptr &()
{
  return this->ptr_;
}

::TAO_SSLIOP_Current_ptr
TAO_SSLIOP_Current_var::operator-> (void) const
{
  return this->ptr_;
}

::TAO_SSLIOP_Current_ptr
TAO_SSLIOP_Current_var::in (void) const
{
  return this->ptr_;
}

::TAO_SSLIOP_Current_ptr &
TAO_SSLIOP_Current_var::inout (void)
{
  return this->ptr_;
}

::TAO_SSLIOP_Current_ptr &
TAO_SSLIOP_Current_var::out (void)
{
  CORBA::release (this->ptr_);
  this->ptr_ = ::TAO_SSLIOP_Current::_nil ();
  return this->ptr_;
}

::TAO_SSLIOP_Current_ptr
TAO_SSLIOP_Current_var::_retn (void)
{
  // yield ownership of managed obj reference
  ::TAO_SSLIOP_Current_ptr val = this->ptr_;
  this->ptr_ = ::TAO_SSLIOP_Current::_nil ();
  return val;
}

::TAO_SSLIOP_Current_ptr
TAO_SSLIOP_Current_var::tao_duplicate (TAO_SSLIOP_Current_ptr p)
{
  return ::TAO_SSLIOP_Current::_duplicate (p);
}

void
TAO_SSLIOP_Current_var::tao_release (TAO_SSLIOP_Current_ptr p)
{
  CORBA::release (p);
}

::TAO_SSLIOP_Current_ptr
TAO_SSLIOP_Current_var::tao_nil (void)
{
  return ::TAO_SSLIOP_Current::_nil ();
}

::TAO_SSLIOP_Current_ptr
TAO_SSLIOP_Current_var::tao_narrow (
    CORBA::Object *p
    ACE_ENV_ARG_DECL
  )
{
  return ::TAO_SSLIOP_Current::_narrow (p ACE_ENV_ARG_PARAMETER);
}

CORBA::Object *
TAO_SSLIOP_Current_var::tao_upcast (void *src)
{
  TAO_SSLIOP_Current **tmp =
    ACE_static_cast (TAO_SSLIOP_Current **, src);
  return *tmp;
}
