// $Id$

#include "tao/PortableServer/POA.h"

#include "tao/CORBA_String.h"
#include "tao/ORB_Constants.h"


ACE_RCSID (ORT,
           ObjectReferenceTemplate_Adapater_Impl,
           "$Id$")


#if !defined (__ACE_INLINE__)
# include "ObjectReferenceTemplate_Adapter_Impl.h"
#endif /* ! __ACE_INLINE__ */


TAO_ObjectReferenceTemplate_Adapter_Impl::TAO_ObjectReferenceTemplate_Adapter_Impl()
{}

TAO_ObjectReferenceTemplate_Adapter_Impl::TAO_ObjectReferenceTemplate_Adapter_Impl (
  const char *server_id,
  const char *orb_id,
  PortableInterceptor::AdapterName *adapter_name,
  TAO_POA * poa)
  : server_id_ (server_id),
    orb_id_ (orb_id),
    poa_ (poa)
{
}

TAO_ObjectReferenceTemplate_Adapter_Impl::~TAO_ObjectReferenceTemplate_Adapter_Impl (void)
{
}

char *
TAO_ObjectReferenceTemplate_Adapter_Impl::server_id (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->server_id_);
}

char *
TAO_ObjectReferenceTemplate_Adapter_Impl::orb_id (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->orb_id_);
}

PortableInterceptor::AdapterName *
TAO_ObjectReferenceTemplate_Adapter_Impl::adapter_name (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableInterceptor::AdapterName *adapter_name;
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
TAO_ObjectReferenceTemplate_Adapter_Impl::make_object (
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

int
TAO_ObjectReferenceTemplate_Adapter_Impl::destroy (void)
{
//  delete this;
  return 0;
}

PortableInterceptor::ObjectReferenceTemplate *
TAO_ObjectReferenceTemplate_Adapter_Impl::get_adapter_template (void)
{
  this->_add_ref();
  return this;
}

PortableInterceptor::ObjectReferenceFactory *
TAO_ObjectReferenceTemplate_Adapter_Impl::get_obj_ref_factory (void)
{
  this->_add_ref();
  return this;
}

int
TAO_ObjectReferenceTemplate_Adapter_Impl::activate (PortableInterceptor::ObjectReferenceFactory *current_factory,
                        TAO_POA *poa)
{
  poa_ = poa;
  return 0;
}

int
TAO_ObjectReferenceTemplate_Adapter_Impl::activate (const char *server_id,
                        const char *orb_id,
                        PortableInterceptor::AdapterName *adapter_name,
                        TAO_POA *poa)
{
  server_id_ = server_id;
  orb_id_ = orb_id;
  adapter_name_ = adapter_name;
  poa_ = poa;

  return 0;
}

void
TAO_ObjectReferenceTemplate_Adapter_Impl::poa (TAO_POA * poa)
{
  poa_ = poa;
}
