// -*- C++ -*-
//
// $Id$

#include "ORBInitInfo.h"
#include "ORB_Core.h"
#include "StringSeqC.h"
#include "CodecFactory.h"

ACE_RCSID (TAO,
           ORBInitInfo,
           "$Id$")


TAO_ORBInitInfo::TAO_ORBInitInfo (TAO_ORB_Core *orb_core,
                                  int  argc ,
                                  char *argv[])
  : orb_core_ (orb_core),
    argc_ (argc),
    argv_ (argv),
    codec_factory_ ()
{
}

TAO_ORBInitInfo::~TAO_ORBInitInfo (void)
{
}

CORBA::StringSeq *
TAO_ORBInitInfo::arguments (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->check_validity (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  // In accordance with the C++ mapping for sequences, it is up to the
  // caller to deallocate storage for returned sequences.

  CORBA::StringSeq *args = 0;
  ACE_NEW_THROW_EX (args,
                    CORBA::StringSeq,
                    CORBA::NO_MEMORY (
                      CORBA_SystemException::_tao_minor_code (
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
TAO_ORBInitInfo::orb_id (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->check_validity (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  // In accordance with the C++ mapping for strings, return a copy.

  return CORBA::string_dup (this->orb_core_->orbid ());
}

IOP::CodecFactory_ptr
TAO_ORBInitInfo::codec_factory (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->check_validity (ACE_TRY_ENV);
  ACE_CHECK_RETURN (IOP::CodecFactory::_nil ());

  return &(this->codec_factory_);
}

void
TAO_ORBInitInfo::register_initial_reference (
    const char * id,
    CORBA::Object_ptr obj,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ORBInitInfo::InvalidName))
{
  this->check_validity (ACE_TRY_ENV);
  ACE_CHECK;

  // This method is only valid during pre_init(), i.e. only before
  // TAO_ORB_Core::init() has been called.  The ORB Core starts out
  // in the "shutdown" state, so checking if it is in that state
  // would indicate that the ORB hasn't been initalized yet, at least
  // partially, meaning that initial reference registration can
  // proceed.
  if (!(this->orb_core_->has_shutdown ()))
    ACE_THROW (CORBA::BAD_INV_ORDER ());  // @@ Throw an exception?
                                          //    What about the minor
                                          //    code?

  if (id == 0)
    ACE_THROW (PortableInterceptor::ORBInitInfo::InvalidName ());
  else if (ACE_OS_String::strlen (id) == 0)
    ACE_THROW (PortableInterceptor::ORBInitInfo::InvalidName ());


  TAO_Object_Ref_Table &table = this->orb_core_->object_ref_table ();

  table.register_initial_reference (id, obj, ACE_TRY_ENV);
  ACE_CHECK;
}

CORBA::Object_ptr
TAO_ORBInitInfo::resolve_initial_references (
    const char * id,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ORBInitInfo::InvalidName))
{
  this->check_validity (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  // This method is only valid during post_init(), i.e. only after
  // TAO_ORB_Core::init() has been called.  The ORB Core starts out
  // in the "shutdown" state, so checking if it is in that state
  // would indicate that the ORB hasn't been initalized yet, at least
  // partially.
  this->orb_core_->check_shutdown (ACE_TRY_ENV);
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
    this->orb_core_->orb ()->resolve_initial_references (id,
                                                         ACE_TRY_ENV);
}

void
TAO_ORBInitInfo::add_client_request_interceptor (
    PortableInterceptor::ClientRequestInterceptor_ptr interceptor,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ORBInitInfo::DuplicateName))
{
# if TAO_HAS_INTERCEPTORS == 1
  this->check_validity (ACE_TRY_ENV);
  ACE_CHECK;

  this->orb_core_->add_interceptor (interceptor,
                                    ACE_TRY_ENV);
#else
  ACE_UNUSED_ARG (interceptor);
  ACE_THROW (CORBA::NO_IMPLEMENT (
               CORBA_SystemException::_tao_minor_code (
                 TAO_DEFAULT_MINOR_CODE,
                 ENOTSUP),
               CORBA::COMPLETED_NO));
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
}

void
TAO_ORBInitInfo::add_server_request_interceptor (
    PortableInterceptor::ServerRequestInterceptor_ptr interceptor,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ORBInitInfo::DuplicateName))
{
# if TAO_HAS_INTERCEPTORS == 1
  this->check_validity (ACE_TRY_ENV);
  ACE_CHECK;

  this->orb_core_->add_interceptor (interceptor,
                                    ACE_TRY_ENV);

#else
  ACE_UNUSED_ARG (interceptor);
  ACE_THROW (CORBA::NO_IMPLEMENT (
               CORBA_SystemException::_tao_minor_code (
                 TAO_DEFAULT_MINOR_CODE,
                 ENOTSUP),
               CORBA::COMPLETED_NO));
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
}

void
TAO_ORBInitInfo::add_ior_interceptor (
    PortableInterceptor::IORInterceptor_ptr interceptor,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ORBInitInfo::DuplicateName))
{
  this->check_validity (ACE_TRY_ENV);
  ACE_CHECK;

  this->orb_core_->add_interceptor (interceptor,
                                    ACE_TRY_ENV);
}

PortableInterceptor::SlotId
TAO_ORBInitInfo::allocate_slot_id (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->check_validity (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA_SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    0);
}

void
TAO_ORBInitInfo::register_policy_factory (
    CORBA::PolicyType type,
    PortableInterceptor::PolicyFactory_ptr policy_factory,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->check_validity (ACE_TRY_ENV);
  ACE_CHECK;

  TAO_PolicyFactory_Registry *registry =
    this->orb_core_->policy_factory_registry ();

  registry->register_policy_factory (type,
                                     policy_factory,
                                     ACE_TRY_ENV);
}

size_t
TAO_ORBInitInfo::allocate_tss_slot_id (ACE_CLEANUP_FUNC cleanup,
                                       CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->check_validity (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  size_t slot_id = 0;

  int result = this->orb_core_->add_tss_cleanup_func (cleanup,
                                                      slot_id);

  if (result != 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (
                        CORBA_SystemException::_tao_minor_code (
                          TAO_DEFAULT_MINOR_CODE,
                          errno),
                        CORBA::COMPLETED_NO),
                      0);

  return slot_id;
}

void
TAO_ORBInitInfo::check_validity (CORBA_Environment &ACE_TRY_ENV)
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

TAO_ORBInitInfo *
TAO_ORBInitInfo::_narrow (CORBA::Object_ptr obj,
                          CORBA::Environment &ACE_TRY_ENV)
{
  return TAO_ORBInitInfo::_unchecked_narrow (obj, ACE_TRY_ENV);
}

TAO_ORBInitInfo *
TAO_ORBInitInfo::_unchecked_narrow (CORBA::Object_ptr obj,
                                    CORBA::Environment &)
{
  if (CORBA::is_nil (obj))
    return 0;
  return
      ACE_reinterpret_cast (
        TAO_ORBInitInfo *,
        obj->_tao_QueryInterface (
          ACE_reinterpret_cast (ptr_arith_t,
                                &TAO_ORBInitInfo::_narrow)));
}

void *
TAO_ORBInitInfo::_tao_QueryInterface (ptr_arith_t type)
{
  void *retv = 0;
  if (type == ACE_reinterpret_cast (ptr_arith_t,
                                    &TAO_ORBInitInfo::_narrow))
    retv = ACE_reinterpret_cast (void*, this);
  else if (type == ACE_reinterpret_cast
    (ptr_arith_t,
      &::PortableInterceptor::ORBInitInfo::_narrow))
    retv = ACE_reinterpret_cast (
             void *,
             ACE_static_cast (PortableInterceptor::ORBInitInfo_ptr,
                              this));
  else if (type == ACE_reinterpret_cast (ptr_arith_t, &CORBA::Object::_narrow))
    retv = ACE_reinterpret_cast (void *,
      ACE_static_cast (CORBA::Object_ptr, this));

//   if (retv)
//     this->_add_ref ();
  return retv;
}
