// -*- C++ -*-
//
// $Id$

#include "tao/ORBInitInfo.h"
#include "tao/ORB_Core.h"

ACE_RCSID(tao, ORBInitInfo, "$Id$")


TAO_ORBInitInfo::TAO_ORBInitInfo (TAO_ORB_Core *orb_core,
                                  int argc,
                                  char *argv[])
  : orb_core_ (orb_core),
    arguments_ ()
{
  if (argc > 0)
    {
      this->arguments_.length (argc);
      for (int i = 0; i < argc; ++i)
        this->arguments_[i] = CORBA::string_dup (argv[i]);
    }
}

TAO_ORBInitInfo::~TAO_ORBInitInfo (void)
{
  /// The string sequence automatically calls the destructor on its
  /// elements so we do not have to explicitly deallocate the
  /// duplicated strings.
}

CORBA::StringSeq *
TAO_ORBInitInfo::arguments (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return &(this->arguments_);
}

char *
TAO_ORBInitInfo::orb_id (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->orb_core_->orbid ());
}

void
TAO_ORBInitInfo::register_initial_reference (
    const char * id,
    CORBA::Object_ptr /* obj */,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ORBInitInfo::InvalidName))
{
  if (id == 0)
    ACE_THROW (PortableInterceptor::ORBInitInfo::InvalidName ());
  else if (ACE_OS_String::strlen (id) == 0)
    ACE_THROW (PortableInterceptor::ORBInitInfo::InvalidName ());

  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

CORBA::Object_ptr
TAO_ORBInitInfo::resolve_initial_references (
    const char * id,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ORBInitInfo::InvalidName))
{
  /// This method is only valid during post_init(), i.e. only after
  /// TAO_ORB_Core::init() has been called.  The ORB Core starts out
  /// in the "shutdown" state, so checking if it is in that state
  /// would indicate that the ORB hasn't been initalized yet, at least
  /// partially.
  this->orb_core_->check_shutdown (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  if (id == 0)
    ACE_THROW_RETURN (PortableInterceptor::ORBInitInfo::InvalidName (),
                      CORBA::Object::_nil ());
  else if (ACE_OS_String::strlen (id) == 0)
    ACE_THROW_RETURN (PortableInterceptor::ORBInitInfo::InvalidName (),
                      CORBA::Object::_nil ());

  // @@ REMOVE THIS ONCE IMPLEMENTED.
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    CORBA::Object::_nil ());
}

void
TAO_ORBInitInfo::add_client_request_interceptor (
    PortableInterceptor::ClientRequestInterceptor_ptr interceptor,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ORBInitInfo::DuplicateName))
{
# if TAO_HAS_INTERCEPTORS == 1
  this->orb_core_->add_interceptor (interceptor,
                                    ACE_TRY_ENV);
#else
  ACE_UNUSED_ARG (interceptor);
  ACE_THROW (CORBA::NO_IMPLEMENT ());
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
  this->orb_core_->add_interceptor (interceptor,
                                    ACE_TRY_ENV);

#else
  ACE_UNUSED_ARG (interceptor);
  ACE_THROW (CORBA::NO_IMPLEMENT ());
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
}

void
TAO_ORBInitInfo::add_ior_interceptor (
    PortableInterceptor::IORInterceptor_ptr /* interceptor */,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ORBInitInfo::DuplicateName))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

PortableInterceptor::SlotId
TAO_ORBInitInfo::allocate_slot_id (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    0);
}

void
TAO_ORBInitInfo::register_policy_factory (
    CORBA::PolicyType type,
    PortableInterceptor::PolicyFactory_ptr policy_factory,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_PolicyFactory_Registry *registry =
    this->orb_core_->policy_factory_registry ();

  registry->register_policy_factory (type,
                                     policy_factory,
                                     ACE_TRY_ENV);
}
