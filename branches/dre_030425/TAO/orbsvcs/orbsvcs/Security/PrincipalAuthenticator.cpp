// -*- C++ -*-

#include "PrincipalAuthenticator.h"
#include "SecurityManager.h"

ACE_RCSID (Security,
           PrincipalAuthenticator,
           "$Id$")


TAO_PrincipalAuthenticator::TAO_PrincipalAuthenticator (
  TAO_SecurityManager *manager)
  : vaults_ (),
    security_manager_ (manager)
{
  // Make sure the SecurityManager exists at least as long as this
  // PrincipalAuthenticator.
  (void) SecurityLevel2::SecurityManager::_duplicate (manager);
}

TAO_PrincipalAuthenticator::~TAO_PrincipalAuthenticator (void)
{
  size_t count = this->vaults_.size ();

  for (size_t i = 0; i < count; ++i)
    CORBA::release (ACE_static_cast (SecurityReplaceable::Vault_ptr,
                                     this->vaults_[i]));

  CORBA::release (this->security_manager_);
}

Security::AuthenticationMethodList *
TAO_PrincipalAuthenticator::get_supported_authen_methods (
    const char * /* mechanism */
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
#if 0
  Security::AuthenticationMethodList *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    Security::AuthenticationMethodList,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (0);

  Security::AuthenticationMethodList_var list = tmp;

WHAT DO WE DO WITH THE "mechanism" parameter!

//   CORBA::ULong count = this->vaults_.size ();
//   list->length (count);

//   for (CORBA::ULong i = 0; i < count; ++i)
//     list[i] = this->vaults_[i]->authentication_method ();

//   return list._retn ();
#else
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    0);
#endif  /* 0 */
}

Security::AuthenticationStatus
TAO_PrincipalAuthenticator::authenticate (
    Security::AuthenticationMethod method,
    const char * mechanism,
    const char * security_name,
    const CORBA::Any & auth_data,
    const Security::AttributeList & privileges,
    SecurityLevel2::Credentials_out creds,
    CORBA::Any_out continuation_data,
    CORBA::Any_out auth_specific_data
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  Security::AuthenticationStatus status = Security::SecAuthFailure;

  size_t count = this->vaults_.size ();

  // Iterate over the registered vaults until one that can
  // authenticate the credentials is found.
  for (size_t i = 0; i < count; ++i)
    {
      SecurityReplaceable::Vault_ptr vault =
        ACE_static_cast (SecurityReplaceable::Vault_ptr,
                         this->vaults_[i]);

      status = vault->acquire_credentials (method,
                                           mechanism,
                                           security_name,
                                           auth_data,
                                           privileges,
                                           creds,
                                           continuation_data,
                                           auth_specific_data
                                           ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (Security::SecAuthFailure);

      if (status == Security::SecAuthSuccess
          || status == Security::SecAuthContinue)
        break;
    }

  if (status == Security::SecAuthSuccess)
    {
      this->security_manager_->add_own_credentials (creds
                                                    ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (Security::SecAuthFailure);
    }

  return status;
}

Security::AuthenticationStatus
TAO_PrincipalAuthenticator::continue_authentication (
    const CORBA::Any & response_data,
    SecurityLevel2::Credentials_ptr creds,
    CORBA::Any_out continuation_data,
    CORBA::Any_out auth_specific_data
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  Security::AuthenticationStatus status = Security::SecAuthFailure;

  size_t count = this->vaults_.size ();

  // Iterate over the registered vaults until one that can
  // authenticate the credentials is found.
  for (size_t i = 0; i < count; ++i)
    {
      SecurityReplaceable::Vault_ptr vault =
        ACE_static_cast (SecurityReplaceable::Vault_ptr,
                         this->vaults_[i]);

      status =
        vault->continue_credentials_acquisition (response_data,
                                                 creds,
                                                 continuation_data,
                                                 auth_specific_data
                                                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (Security::SecAuthFailure);

      if (status == Security::SecAuthSuccess
          || status == Security::SecAuthContinue)
        break;
    }

  if (status == Security::SecAuthSuccess)
    {
      this->security_manager_->add_own_credentials (creds
                                                    ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (Security::SecAuthFailure);
    }

  return status;
}

void
TAO_PrincipalAuthenticator::register_vault (
  SecurityReplaceable::Vault_ptr vault
  ACE_ENV_ARG_DECL)
{
  if (!CORBA::is_nil (vault))
    {
      size_t old_size = this->vaults_.size ();
      this->vaults_.size (old_size + 1);
      this->vaults_[old_size] =
        SecurityReplaceable::Vault::_duplicate (vault);
    }
  else
    ACE_THROW (CORBA::BAD_PARAM ());
}

int TAO_PrincipalAuthenticator::_tao_class_id = 0;

TAO_PrincipalAuthenticator_ptr
tao_TAO_PrincipalAuthenticator_duplicate (
    TAO_PrincipalAuthenticator_ptr p
  )
{
  return TAO_PrincipalAuthenticator::_duplicate (p);
}

void
tao_TAO_PrincipalAuthenticator_release (
    TAO_PrincipalAuthenticator_ptr p
  )
{
  CORBA::release (p);
}

TAO_PrincipalAuthenticator_ptr
tao_TAO_PrincipalAuthenticator_nil (
    void
  )
{
  return TAO_PrincipalAuthenticator::_nil ();
}

TAO_PrincipalAuthenticator_ptr
tao_TAO_PrincipalAuthenticator_narrow (
    CORBA::Object *p
    ACE_ENV_ARG_DECL
  )
{
  return TAO_PrincipalAuthenticator::_narrow (p ACE_ENV_ARG_PARAMETER);
}

CORBA::Object *
tao_TAO_PrincipalAuthenticator_upcast (
    void *src
  )
{
  TAO_PrincipalAuthenticator **tmp =
    ACE_static_cast (TAO_PrincipalAuthenticator **, src);
  return *tmp;
}

// *************************************************************
// Operations for class TAO_PrincipalAuthenticator_var
// *************************************************************

TAO_PrincipalAuthenticator_var::TAO_PrincipalAuthenticator_var (void) // default constructor
  : ptr_ (TAO_PrincipalAuthenticator::_nil ())
{}

::TAO_PrincipalAuthenticator_ptr
TAO_PrincipalAuthenticator_var::ptr (void) const
{
  return this->ptr_;
}

TAO_PrincipalAuthenticator_var::TAO_PrincipalAuthenticator_var (
  const ::TAO_PrincipalAuthenticator_var &p)
  : TAO_Base_var (),
    ptr_ (TAO_PrincipalAuthenticator::_duplicate (p.ptr ()))
{}

TAO_PrincipalAuthenticator_var::~TAO_PrincipalAuthenticator_var (void)
{
  CORBA::release (this->ptr_);
}

TAO_PrincipalAuthenticator_var &
TAO_PrincipalAuthenticator_var::operator= (TAO_PrincipalAuthenticator_ptr p)
{
  CORBA::release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

TAO_PrincipalAuthenticator_var &
TAO_PrincipalAuthenticator_var::operator= (const ::TAO_PrincipalAuthenticator_var &p)
{
  if (this != &p)
  {
    CORBA::release (this->ptr_);
    this->ptr_ = ::TAO_PrincipalAuthenticator::_duplicate (p.ptr ());
  }
  return *this;
}

TAO_PrincipalAuthenticator_var::operator const ::TAO_PrincipalAuthenticator_ptr &() const // cast
{
  return this->ptr_;
}

TAO_PrincipalAuthenticator_var::operator ::TAO_PrincipalAuthenticator_ptr &() // cast
{
  return this->ptr_;
}

::TAO_PrincipalAuthenticator_ptr
TAO_PrincipalAuthenticator_var::operator-> (void) const
{
  return this->ptr_;
}

::TAO_PrincipalAuthenticator_ptr
TAO_PrincipalAuthenticator_var::in (void) const
{
  return this->ptr_;
}

::TAO_PrincipalAuthenticator_ptr &
TAO_PrincipalAuthenticator_var::inout (void)
{
  return this->ptr_;
}

::TAO_PrincipalAuthenticator_ptr &
TAO_PrincipalAuthenticator_var::out (void)
{
  CORBA::release (this->ptr_);
  this->ptr_ = ::TAO_PrincipalAuthenticator::_nil ();
  return this->ptr_;
}

::TAO_PrincipalAuthenticator_ptr
TAO_PrincipalAuthenticator_var::_retn (void)
{
  // yield ownership of managed obj reference
  ::TAO_PrincipalAuthenticator_ptr val = this->ptr_;
  this->ptr_ = ::TAO_PrincipalAuthenticator::_nil ();
  return val;
}

::TAO_PrincipalAuthenticator_ptr
TAO_PrincipalAuthenticator_var::tao_duplicate (TAO_PrincipalAuthenticator_ptr p)
{
  return ::TAO_PrincipalAuthenticator::_duplicate (p);
}

void
TAO_PrincipalAuthenticator_var::tao_release (TAO_PrincipalAuthenticator_ptr p)
{
  CORBA::release (p);
}

::TAO_PrincipalAuthenticator_ptr
TAO_PrincipalAuthenticator_var::tao_nil (void)
{
  return ::TAO_PrincipalAuthenticator::_nil ();
}

::TAO_PrincipalAuthenticator_ptr
TAO_PrincipalAuthenticator_var::tao_narrow (
    CORBA::Object *p
    ACE_ENV_ARG_DECL
  )
{
  return ::TAO_PrincipalAuthenticator::_narrow (p ACE_ENV_ARG_PARAMETER);
}

CORBA::Object *
TAO_PrincipalAuthenticator_var::tao_upcast (void *src)
{
  TAO_PrincipalAuthenticator **tmp =
    ACE_static_cast (TAO_PrincipalAuthenticator **, src);
  return *tmp;
}

// *************************************************************
// Operations for class TAO_PrincipalAuthenticator_out
// *************************************************************

TAO_PrincipalAuthenticator_out::TAO_PrincipalAuthenticator_out (TAO_PrincipalAuthenticator_ptr &p)
  : ptr_ (p)
{
  this->ptr_ = ::TAO_PrincipalAuthenticator::_nil ();
}

TAO_PrincipalAuthenticator_out::TAO_PrincipalAuthenticator_out (TAO_PrincipalAuthenticator_var &p) // constructor from _var
  : ptr_ (p.out ())
{
  CORBA::release (this->ptr_);
  this->ptr_ = ::TAO_PrincipalAuthenticator::_nil ();
}

TAO_PrincipalAuthenticator_out::TAO_PrincipalAuthenticator_out (const ::TAO_PrincipalAuthenticator_out &p) // copy constructor
  : ptr_ (ACE_const_cast (TAO_PrincipalAuthenticator_out &, p).ptr_)
{}

::TAO_PrincipalAuthenticator_out &
TAO_PrincipalAuthenticator_out::operator= (const ::TAO_PrincipalAuthenticator_out &p)
{
  this->ptr_ = ACE_const_cast (TAO_PrincipalAuthenticator_out&, p).ptr_;
  return *this;
}

TAO_PrincipalAuthenticator_out &
TAO_PrincipalAuthenticator_out::operator= (const ::TAO_PrincipalAuthenticator_var &p)
{
  this->ptr_ = ::TAO_PrincipalAuthenticator::_duplicate (p.ptr ());
  return *this;
}

TAO_PrincipalAuthenticator_out &
TAO_PrincipalAuthenticator_out::operator= (TAO_PrincipalAuthenticator_ptr p)
{
  this->ptr_ = p;
  return *this;
}

TAO_PrincipalAuthenticator_out::operator ::TAO_PrincipalAuthenticator_ptr &() // cast
{
  return this->ptr_;
}

::TAO_PrincipalAuthenticator_ptr &
TAO_PrincipalAuthenticator_out::ptr (void) // ptr
{
  return this->ptr_;
}

::TAO_PrincipalAuthenticator_ptr
TAO_PrincipalAuthenticator_out::operator-> (void)
{
  return this->ptr_;
}


TAO_PrincipalAuthenticator_ptr
TAO_PrincipalAuthenticator::_narrow (
    CORBA::Object_ptr obj
    ACE_ENV_ARG_DECL)
{
  return
    TAO_PrincipalAuthenticator::_unchecked_narrow (obj
                                                   ACE_ENV_ARG_PARAMETER);
}

TAO_PrincipalAuthenticator_ptr
TAO_PrincipalAuthenticator::_unchecked_narrow (
      CORBA::Object_ptr obj
      ACE_ENV_ARG_DECL_NOT_USED
    )
{
  if (CORBA::is_nil (obj))
      return TAO_PrincipalAuthenticator::_nil ();
    return
        ACE_reinterpret_cast
          (
            TAO_PrincipalAuthenticator_ptr,
              obj->_tao_QueryInterface
                (
                  ACE_reinterpret_cast
                    (
                      ptr_arith_t,
                      &TAO_PrincipalAuthenticator::_tao_class_id
                    )
                )
          );
}

TAO_PrincipalAuthenticator_ptr
TAO_PrincipalAuthenticator::_duplicate (TAO_PrincipalAuthenticator_ptr obj)
{
  if (!CORBA::is_nil (obj))
    obj->_add_ref ();
  return obj;
}

void *
TAO_PrincipalAuthenticator::_tao_QueryInterface (ptr_arith_t type)
{
  void *retv = 0;
  if (type == ACE_reinterpret_cast
    (ptr_arith_t,
      &TAO_PrincipalAuthenticator::_tao_class_id))
    retv = ACE_reinterpret_cast (void*, this);
  else if (type == ACE_reinterpret_cast
    (ptr_arith_t,
      &::SecurityLevel2::PrincipalAuthenticator::_tao_class_id))
    retv = ACE_reinterpret_cast
      (
        void *,
        ACE_static_cast
          (
            SecurityLevel2::PrincipalAuthenticator_ptr,
            this
          )
      );
  else if (type == ACE_reinterpret_cast (ptr_arith_t, &CORBA::Object::_tao_class_id))
    retv = ACE_reinterpret_cast (void *,
      ACE_static_cast (CORBA::Object_ptr, this));

  if (retv)
    this->_add_ref ();
  return retv;
}

const char*
TAO_PrincipalAuthenticator::_interface_repository_id (void) const
{
  return "IDL:TAO_PrincipalAuthenticator:1.0";
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Array_Base<TAO_PrincipalAuthenticator_Impl *>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Array_Base<TAO_PrincipalAuthenticator_Impl *>

#endif  /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
