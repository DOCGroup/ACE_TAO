// -*- C++ -*-
//
// $Id$

#include "ORBInitInfo.h"
#include "ORB_Core.h"
#include "StringSeqC.h"
#include "CodecFactory.h"

#if TAO_HAS_INTERCEPTORS == 1
#include "PICurrent.h"
#endif  /* TAO_HAS_INTERCEPTORS == 1 */

ACE_RCSID (TAO,
           ORBInitInfo,
           "$Id$")

#if !defined (__ACE_INLINE__)
#include "ORBInitInfo.inl"
#endif /* defined INLINE */

TAO_ORBInitInfo::TAO_ORBInitInfo (TAO_ORB_Core *orb_core,
                                  int  argc ,
                                  char *argv[])
  : orb_core_ (orb_core),
    argc_ (argc),
    argv_ (argv),
    codec_factory_ (),
    slot_count_ (0)
{
}

TAO_ORBInitInfo::~TAO_ORBInitInfo (void)
{
}

CORBA::StringSeq *
TAO_ORBInitInfo::arguments (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // In accordance with the C++ mapping for sequences, it is up to the
  // caller to deallocate storage for returned sequences.

  CORBA::StringSeq *args = 0;
  ACE_NEW_THROW_EX (args,
                    CORBA::StringSeq,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (0);

  CORBA::StringSeq_var safe_args (args);

  // Copy the argument vector to the string sequence.

  args->length (this->argc_);   // Not a problem if argc is zero.
  for (int i = 0; i < this->argc_; ++i)
    (*args)[i] = CORBA::string_dup (this->argv_[i]);

  return safe_args._retn ();
}

char *
TAO_ORBInitInfo::orb_id (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // In accordance with the C++ mapping for strings, return a copy.

  return CORBA::string_dup (this->orb_core_->orbid ());
}

IOP::CodecFactory_ptr
TAO_ORBInitInfo::codec_factory (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (CORBA::is_nil (this->codec_factory_.in ()))
    {
      // A new instance must be allocated since the application code
      // may have registered an ORBInitializer that requires a
      // CodecFactory before the CodecFactory is itself registered
      // with the ORB.
      IOP::CodecFactory_ptr codec_factory;
      ACE_NEW_THROW_EX (codec_factory,
                        TAO_CodecFactory (this->orb_core_),
                          CORBA::NO_MEMORY (
                            CORBA::SystemException::_tao_minor_code (
                              TAO_DEFAULT_MINOR_CODE,
                              ENOMEM),
                            CORBA::COMPLETED_NO));
      ACE_CHECK_RETURN (IOP::CodecFactory::_nil ());

      this->codec_factory_ = codec_factory;
    }

  return IOP::CodecFactory::_duplicate (this->codec_factory_.in ());
}

void
TAO_ORBInitInfo::register_initial_reference (
    const char * id,
    CORBA::Object_ptr obj
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ORBInitInfo::InvalidName))
{
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (id == 0)
    ACE_THROW (PortableInterceptor::ORBInitInfo::InvalidName ());
  else if (ACE_OS_String::strlen (id) == 0)
    ACE_THROW (PortableInterceptor::ORBInitInfo::InvalidName ());


  TAO_Object_Ref_Table &table = this->orb_core_->object_ref_table ();

  table.register_initial_reference (id, obj ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

CORBA::Object_ptr
TAO_ORBInitInfo::resolve_initial_references (
    const char * id
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ORBInitInfo::InvalidName))
{
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  if (id == 0)
    ACE_THROW_RETURN (PortableInterceptor::ORBInitInfo::InvalidName (),
                      CORBA::Object::_nil ());
  else if (ACE_OS_String::strlen (id) == 0)
    ACE_THROW_RETURN (PortableInterceptor::ORBInitInfo::InvalidName (),
                      CORBA::Object::_nil ());

  // The ORB is practically fully initialized by the time this point
  // is reached so just use the ORB's resolve_initial_references()
  // mechanism.
  return
    this->orb_core_->orb ()->resolve_initial_references (id
                                                          ACE_ENV_ARG_PARAMETER);
}

void
TAO_ORBInitInfo::add_client_request_interceptor (
    PortableInterceptor::ClientRequestInterceptor_ptr interceptor
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ORBInitInfo::DuplicateName))
{
# if TAO_HAS_INTERCEPTORS == 1
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->orb_core_->add_interceptor (interceptor
                                     ACE_ENV_ARG_PARAMETER);
#else
  ACE_UNUSED_ARG (interceptor);
  ACE_THROW (CORBA::NO_IMPLEMENT (
               CORBA::SystemException::_tao_minor_code (
                 TAO_DEFAULT_MINOR_CODE,
                 ENOTSUP),
               CORBA::COMPLETED_NO));
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
}

void
TAO_ORBInitInfo::add_server_request_interceptor (
    PortableInterceptor::ServerRequestInterceptor_ptr interceptor
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ORBInitInfo::DuplicateName))
{
# if TAO_HAS_INTERCEPTORS == 1
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->orb_core_->add_interceptor (interceptor
                                     ACE_ENV_ARG_PARAMETER);

#else
  ACE_UNUSED_ARG (interceptor);
  ACE_THROW (CORBA::NO_IMPLEMENT (
               CORBA::SystemException::_tao_minor_code (
                 TAO_DEFAULT_MINOR_CODE,
                 ENOTSUP),
               CORBA::COMPLETED_NO));
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
}

void
TAO_ORBInitInfo::add_ior_interceptor (
    PortableInterceptor::IORInterceptor_ptr interceptor
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ORBInitInfo::DuplicateName))
{
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->orb_core_->add_interceptor (interceptor
                                     ACE_ENV_ARG_PARAMETER);
}

PortableInterceptor::SlotId
TAO_ORBInitInfo::allocate_slot_id (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

#if TAO_HAS_INTERCEPTORS == 1
  // No need to acquire a lock.  This only gets called during ORB
  // initialization.  ORB initialization is already atomic.
  return this->slot_count_++;
#else
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    0);
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
}

void
TAO_ORBInitInfo::register_policy_factory (
    CORBA::PolicyType type,
    PortableInterceptor::PolicyFactory_ptr policy_factory
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  TAO_PolicyFactory_Registry *registry =
    this->orb_core_->policy_factory_registry ();

  registry->register_policy_factory (type,
                                     policy_factory
                                      ACE_ENV_ARG_PARAMETER);
}

size_t
TAO_ORBInitInfo::allocate_tss_slot_id (ACE_CLEANUP_FUNC cleanup
                                       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  size_t slot_id = 0;

  int result = this->orb_core_->add_tss_cleanup_func (cleanup,
                                                      slot_id);

  if (result != 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (
                        CORBA::SystemException::_tao_minor_code (
                          TAO_DEFAULT_MINOR_CODE,
                          errno),
                        CORBA::COMPLETED_NO),
                      0);

  return slot_id;
}

void
TAO_ORBInitInfo::check_validity (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->orb_core_ == 0)
    {
      // As defined by the Portable Interceptor specification, throw a
      // CORBA::OBJECT_NOT_EXIST exception after CORBA::ORB_init() has
      // completed.  CORBA::ORB_init() sets the ORB core pointer in
      // this instance to zero when it is done initializing the ORB,
      // which is why we base "existence" on the validity of the ORB
      // core pointer.
      ACE_THROW (CORBA::OBJECT_NOT_EXIST (TAO_DEFAULT_MINOR_CODE,
                                          CORBA::COMPLETED_NO));
    }
}

TAO_ORBInitInfo_ptr TAO_ORBInitInfo::_narrow (
    CORBA::Object_ptr obj
    ACE_ENV_ARG_DECL
  )
{
  return TAO_ORBInitInfo::_unchecked_narrow (obj ACE_ENV_ARG_PARAMETER);
}

TAO_ORBInitInfo_ptr TAO_ORBInitInfo::_unchecked_narrow (
    CORBA::Object_ptr obj
    ACE_ENV_ARG_DECL_NOT_USED
  )
{
  if (CORBA::is_nil (obj))
    return TAO_ORBInitInfo::_nil ();
  return
      ACE_reinterpret_cast
        (
          TAO_ORBInitInfo_ptr,
            obj->_tao_QueryInterface
              (
                ACE_reinterpret_cast
                  (
                    ptrdiff_t,
                    &TAO_ORBInitInfo::_narrow
                  )
              )
        );
}

TAO_ORBInitInfo_ptr
TAO_ORBInitInfo::_duplicate (TAO_ORBInitInfo_ptr obj)
{
  if (!CORBA::is_nil (obj))
    obj->_add_ref ();
  return obj;
}

void *TAO_ORBInitInfo::_tao_QueryInterface (ptrdiff_t type)
{
  void *retv = 0;
  if (type == ACE_reinterpret_cast
    (ptrdiff_t,
      &TAO_ORBInitInfo::_narrow))
    retv = ACE_reinterpret_cast (void*, this);
  else if (type == ACE_reinterpret_cast
    (ptrdiff_t,
      &::PortableInterceptor::ORBInitInfo::_narrow))
    retv = ACE_reinterpret_cast
      (
        void *,
        ACE_static_cast
          (
            PortableInterceptor::ORBInitInfo_ptr,
            this
          )
      );
  else if (type == ACE_reinterpret_cast (ptrdiff_t, &CORBA::Object::_narrow))
    retv = ACE_reinterpret_cast (void *,
      ACE_static_cast (CORBA::Object_ptr, this));

  if (retv)
    this->_add_ref ();
  return retv;
}

const char* TAO_ORBInitInfo::_interface_repository_id (void) const
{
  return "IDL:TAO_ORBInitInfo:1.0";
}

// *************************************************************
TAO_ORBInitInfo_ptr
tao_TAO_ORBInitInfo_duplicate (
    TAO_ORBInitInfo_ptr p
  )
{
  return TAO_ORBInitInfo::_duplicate (p);
}

void
tao_TAO_ORBInitInfo_release (
    TAO_ORBInitInfo_ptr p
  )
{
  CORBA::release (p);
}

TAO_ORBInitInfo_ptr
tao_TAO_ORBInitInfo_nil (
    void
  )
{
  return TAO_ORBInitInfo::_nil ();
}

TAO_ORBInitInfo_ptr
tao_TAO_ORBInitInfo_narrow (
    CORBA::Object *p
    ACE_ENV_ARG_DECL
  )
{
  return TAO_ORBInitInfo::_narrow (p ACE_ENV_ARG_PARAMETER);
}

CORBA::Object *
tao_TAO_ORBInitInfo_upcast (
    void *src
  )
{
  TAO_ORBInitInfo **tmp =
    ACE_static_cast (TAO_ORBInitInfo **, src);
  return *tmp;
}

// *************************************************************

// *************************************************************
// Operations for class TAO_ORBInitInfo_var
// *************************************************************

TAO_ORBInitInfo_var::TAO_ORBInitInfo_var (void) // default constructor
  : ptr_ (TAO_ORBInitInfo::_nil ())
{}

::TAO_ORBInitInfo_ptr
TAO_ORBInitInfo_var::ptr (void) const
{
  return this->ptr_;
}

TAO_ORBInitInfo_var::TAO_ORBInitInfo_var (const ::TAO_ORBInitInfo_var &p)
  : TAO_Base_var (),
    ptr_ (TAO_ORBInitInfo::_duplicate (p.ptr ()))
{}

TAO_ORBInitInfo_var::~TAO_ORBInitInfo_var (void) // destructor
{
  CORBA::release (this->ptr_);
}

TAO_ORBInitInfo_var &
TAO_ORBInitInfo_var::operator= (TAO_ORBInitInfo_ptr p)
{
  CORBA::release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

TAO_ORBInitInfo_var &
TAO_ORBInitInfo_var::operator= (const ::TAO_ORBInitInfo_var &p)
{
  if (this != &p)
  {
    CORBA::release (this->ptr_);
    this->ptr_ = ::TAO_ORBInitInfo::_duplicate (p.ptr ());
  }
  return *this;
}

TAO_ORBInitInfo_var::operator const ::TAO_ORBInitInfo_ptr &() const
{
  return this->ptr_;
}

TAO_ORBInitInfo_var::operator ::TAO_ORBInitInfo_ptr &()
{
  return this->ptr_;
}

TAO_ORBInitInfo_ptr
TAO_ORBInitInfo_var::operator-> (void) const
{
  return this->ptr_;
}

TAO_ORBInitInfo_ptr
TAO_ORBInitInfo_var::in (void) const
{
  return this->ptr_;
}

TAO_ORBInitInfo_ptr &
TAO_ORBInitInfo_var::inout (void)
{
  return this->ptr_;
}

TAO_ORBInitInfo_ptr &
TAO_ORBInitInfo_var::out (void)
{
  CORBA::release (this->ptr_);
  this->ptr_ = ::TAO_ORBInitInfo::_nil ();
  return this->ptr_;
}

TAO_ORBInitInfo_ptr
TAO_ORBInitInfo_var::_retn (void)
{
  // yield ownership of managed obj reference
  ::TAO_ORBInitInfo_ptr val = this->ptr_;
  this->ptr_ = ::TAO_ORBInitInfo::_nil ();
  return val;
}

TAO_ORBInitInfo_ptr
TAO_ORBInitInfo_var::duplicate (TAO_ORBInitInfo_ptr p)
{
  return ::TAO_ORBInitInfo::_duplicate (p);
}

void
TAO_ORBInitInfo_var::release (TAO_ORBInitInfo_ptr p)
{
  CORBA::release (p);
}

TAO_ORBInitInfo_ptr
TAO_ORBInitInfo_var::nil (void)
{
  return ::TAO_ORBInitInfo::_nil ();
}

TAO_ORBInitInfo_ptr
TAO_ORBInitInfo_var::narrow (
    CORBA::Object *p
    ACE_ENV_ARG_DECL
  )
{
  return ::TAO_ORBInitInfo::_narrow (p ACE_ENV_ARG_PARAMETER);
}

CORBA::Object *
TAO_ORBInitInfo_var::upcast (void *src)
{
  TAO_ORBInitInfo **tmp =
    ACE_static_cast (TAO_ORBInitInfo **, src);
  return *tmp;
}
