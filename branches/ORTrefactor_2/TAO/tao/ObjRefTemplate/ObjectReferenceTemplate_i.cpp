#include "ObjectReferenceTemplate_i.h"
#include "tao/PortableServer/POA.h"

#include "tao/CORBA_String.h"
#include "tao/ORB_Constants.h"

ACE_RCSID (ORT,
           ObjectReferenceTemplate_i,
           "$Id$")

#if !defined (__ACE_INLINE__)
# include "ObjectReferenceTemplate_i.inl"
#endif /* ! __ACE_INLINE__ */

namespace TAO
{
  ObjectReferenceTemplate::ObjectReferenceTemplate (
  const char *server_id,
  const char *orb_id,
  PortableInterceptor::AdapterName *adapter_name,
  TAO_POA * poa)
  : server_id_ (server_id),
    orb_id_ (orb_id),
    adapter_name_ (adapter_name),
    poa_ (poa)
  {
  }

  ObjectReferenceTemplate::~ObjectReferenceTemplate (void)
  {
  }

  char *
  ObjectReferenceTemplate::server_id (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return CORBA::string_dup (this->server_id_);
  }

  char *
  ObjectReferenceTemplate::orb_id (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return CORBA::string_dup (this->orb_id_);
  }

  PortableInterceptor::AdapterName *
  ObjectReferenceTemplate::adapter_name (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    PortableInterceptor::AdapterName *adapter_name = 0;

    ACE_NEW_THROW_EX (adapter_name,
                      PortableInterceptor::AdapterName (
                                                        *(this->adapter_name_)),
                      CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                              TAO_DEFAULT_MINOR_CODE,
                              ENOMEM),
                          CORBA::COMPLETED_NO));
    ACE_CHECK_RETURN (0);

    return adapter_name;
  }

  CORBA::Object_ptr
  ObjectReferenceTemplate::make_object (
      const char *,
      const PortableInterceptor::ObjectId &
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    if (this->poa_ == 0)
      ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (), CORBA::Object::_nil ());

    return this->poa_->invoke_key_to_object (ACE_ENV_SINGLE_ARG_PARAMETER);
  }

  void
  ObjectReferenceTemplate::poa (TAO_POA * poa)
  {
    // @@ Johnny, Why aren't we duplicating this?
    this->poa_ = poa;
  }

}
