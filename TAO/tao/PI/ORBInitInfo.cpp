// $Id$

#include "tao/PI/ORBInitInfo.h"
#include "tao/ORB_Core.h"
#include "tao/ORB.h"
#include "tao/SystemException.h"
#include "tao/Object_Loader.h"
#include "tao/PolicyFactory_Registry_Adapter.h"

#include "ace/Dynamic_Service.h"
#include "ace/Service_Config.h"

#if TAO_HAS_INTERCEPTORS == 1
#include "tao/PI/PICurrent.h"
#endif  /* TAO_HAS_INTERCEPTORS == 1 */

ACE_RCSID (TAO,
           ORBInitInfo,
           "$Id$")

#if !defined (__ACE_INLINE__)
#include "tao/PI/ORBInitInfo.inl"
#endif /* defined INLINE */

#include "ace/OS_NS_string.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Traits specializations for TAO_ORBInitInfo.

TAO_ORBInitInfo_ptr
TAO::Objref_Traits<TAO_ORBInitInfo>::duplicate (
    TAO_ORBInitInfo_ptr p
  )
{
  return TAO_ORBInitInfo::_duplicate (p);
}

void
TAO::Objref_Traits<TAO_ORBInitInfo>::release (
    TAO_ORBInitInfo_ptr p
  )
{
  ::CORBA::release (p);
}

TAO_ORBInitInfo_ptr
TAO::Objref_Traits<TAO_ORBInitInfo>::nil (void)
{
  return TAO_ORBInitInfo::_nil ();
}

CORBA::Boolean
TAO::Objref_Traits<TAO_ORBInitInfo>::marshal (
    TAO_ORBInitInfo_ptr p,
    TAO_OutputCDR & cdr
  )
{
  return p->marshal (cdr);
}

TAO_ORBInitInfo::TAO_ORBInitInfo (TAO_ORB_Core *orb_core,
                                  int argc,
                                  char *argv[],
                                  PortableInterceptor::SlotId slotid)
  : orb_core_ (orb_core),
    argc_ (argc),
    argv_ (argv),
    codec_factory_ (),
    slot_count_ (slotid)
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
                        0,
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
      TAO_Object_Loader *loader =
        ACE_Dynamic_Service<TAO_Object_Loader>::instance ("CodecFactory_Loader");

      if (loader == 0)
        {
          ACE_Service_Config::process_directive (
            ACE_DYNAMIC_SERVICE_DIRECTIVE("CodecFactory",
                                          "TAO_CodecFactory",
                                          "_make_TAO_CodecFactory_Loader",
                                          ""));
          loader =
            ACE_Dynamic_Service<TAO_Object_Loader>::instance ("CodecFactory_Loader");
        }

      if (loader != 0)
        {
          CORBA::Object_var cf =
            loader->create_object (this->orb_core_->orb (), 0, 0 ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (IOP::CodecFactory::_nil ());

          this->codec_factory_ = IOP::CodecFactory::_narrow (cf.in ());
        }
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

  if (id == 0 || ACE_OS::strlen (id) == 0)
    ACE_THROW (PortableInterceptor::ORBInitInfo::InvalidName ());
  ACE_CHECK;

  if (CORBA::is_nil (obj))
    ACE_THROW (CORBA::BAD_PARAM (CORBA::OMGVMCID | 27,
                                 CORBA::COMPLETED_NO));
  ACE_CHECK;

  TAO_Object_Ref_Table &table = this->orb_core_->object_ref_table ();

  if (table.register_initial_reference (id, obj) == -1)
    ACE_THROW (PortableInterceptor::ORBInitInfo::InvalidName ());
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

  if (id == 0 || ACE_OS::strlen (id) == 0)
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

  this->orb_core_->add_interceptor (interceptor ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
#else
  ACE_UNUSED_ARG (interceptor);
  ACE_THROW (CORBA::NO_IMPLEMENT (
               CORBA::SystemException::_tao_minor_code (
                 0,
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

  this->orb_core_->add_interceptor (interceptor ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

#else
  ACE_UNUSED_ARG (interceptor);
  ACE_THROW (CORBA::NO_IMPLEMENT (
               CORBA::SystemException::_tao_minor_code (
                 0,
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

void
TAO_ORBInitInfo::add_client_request_interceptor_with_policy (
    PortableInterceptor::ClientRequestInterceptor_ptr interceptor,
    const CORBA::PolicyList& policies
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ORBInitInfo::DuplicateName,
                   CORBA::PolicyError))
{
# if TAO_HAS_INTERCEPTORS == 1
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->orb_core_->add_interceptor (interceptor,
                                    policies
                                    ACE_ENV_ARG_PARAMETER);
#else
  ACE_UNUSED_ARG (interceptor);
  ACE_UNUSED_ARG (policies);
  ACE_THROW (CORBA::NO_IMPLEMENT (
               CORBA::SystemException::_tao_minor_code (
                 0,
                 ENOTSUP),
               CORBA::COMPLETED_NO));
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
}

void
TAO_ORBInitInfo::add_server_request_interceptor_with_policy (
    PortableInterceptor::ServerRequestInterceptor_ptr interceptor,
    const CORBA::PolicyList& policies
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ORBInitInfo::DuplicateName,
                   CORBA::PolicyError))
{
# if TAO_HAS_INTERCEPTORS == 1
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->orb_core_->add_interceptor (interceptor,
                                    policies
                                    ACE_ENV_ARG_PARAMETER);

#else
  ACE_UNUSED_ARG (interceptor);
  ACE_UNUSED_ARG (policies);
  ACE_THROW (CORBA::NO_IMPLEMENT (
               CORBA::SystemException::_tao_minor_code (
                 0,
                 ENOTSUP),
               CORBA::COMPLETED_NO));
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
}

void
TAO_ORBInitInfo::add_ior_interceptor_with_policy (
    PortableInterceptor::IORInterceptor_ptr interceptor,
    const CORBA::PolicyList& policies
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ORBInitInfo::DuplicateName,
                   CORBA::PolicyError))
{
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Since there are currently no PI Policies that pertain to
  // IOR Interceptors, we will always raise the NO_IMPLEMENT
  // CORBA System Exception here to indicate that this method
  // is currently not implemented/supported.
  ACE_UNUSED_ARG (interceptor);
  ACE_UNUSED_ARG (policies);
  ACE_THROW (CORBA::NO_IMPLEMENT (
               CORBA::SystemException::_tao_minor_code (
                 0,
                 ENOTSUP),
               CORBA::COMPLETED_NO));
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
                        0,
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

  TAO::PolicyFactory_Registry_Adapter *registry =
    this->orb_core_->policy_factory_registry ();

  if (registry == 0)
    {
      ACE_THROW (CORBA::INTERNAL ());
    }

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

  const int result = this->orb_core_->add_tss_cleanup_func (cleanup,
                                                            slot_id);

  if (result != 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (
                        CORBA::SystemException::_tao_minor_code (
                          0,
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
      ACE_THROW (CORBA::OBJECT_NOT_EXIST (0,
                                          CORBA::COMPLETED_NO));
    }
}

CORBA::ORB_ptr
TAO_ORBInitInfo::_get_orb (ACE_ENV_SINGLE_ARG_DECL)
{
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ORB::_nil ());

  return CORBA::ORB::_duplicate (this->orb_core_->orb ());
}

TAO_ORBInitInfo_ptr TAO_ORBInitInfo::_narrow (
    CORBA::Object_ptr _tao_objref
    ACE_ENV_ARG_DECL_NOT_USED
  )
{
  if (CORBA::is_nil (_tao_objref))
    {
      return TAO_ORBInitInfo::_nil ();
    }

  TAO_ORBInitInfo_ptr proxy =
    dynamic_cast<TAO_ORBInitInfo_ptr> (_tao_objref);

  return TAO_ORBInitInfo::_duplicate (proxy);
}

TAO_ORBInitInfo_ptr
TAO_ORBInitInfo::_duplicate (TAO_ORBInitInfo_ptr obj)
{
  if (!CORBA::is_nil (obj))
    {
      obj->_add_ref ();
    }

  return obj;
}

const char* TAO_ORBInitInfo::_interface_repository_id (void) const
{
  return "IDL:TAO_ORBInitInfo:1.0";
}

TAO_END_VERSIONED_NAMESPACE_DECL
