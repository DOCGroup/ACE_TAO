#include "ObjectReferenceTemplate.h"
#include "POA.h"

ACE_RCSID (PortableServer,
           ObjectReferenceTemplate,
           "$Id$")


#if !defined (__ACE_INLINE__)
# include "ObjectReferenceTemplate.inl"
#endif /* ! __ACE_INLINE__ */


TAO_ObjectReferenceTemplate::TAO_ObjectReferenceTemplate (
  const char *server_id,
  const char *orb_id,
  TAO_POA * poa)
  : server_id_ (server_id),
    orb_id_ (orb_id),
    poa_ (poa)
{
}

TAO_ObjectReferenceTemplate::~TAO_ObjectReferenceTemplate (void)
{
}

char *
TAO_ObjectReferenceTemplate::server_id (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->server_id_);
}

char *
TAO_ObjectReferenceTemplate::orb_id (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->orb_id_);
}

PortableInterceptor::AdapterName *
TAO_ObjectReferenceTemplate::adapter_name (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableInterceptor::AdapterName *adapter_name = 0;

  if (this->adapter_name_ != 0)
    {
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
  else
    {
      if (this->poa_ == 0)
        ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (), 0);

      this->adapter_name_ =
        this->poa_->adapter_name (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

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
}

CORBA::Object_ptr
TAO_ObjectReferenceTemplate::make_object (
    const char *,
    const PortableInterceptor::ObjectId &
  ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  if (this->poa_ == 0)
    ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (), CORBA::Object::_nil ());

  return this->poa_->invoke_key_to_object (ACE_ENV_SINGLE_ARG_PARAMETER);
}
