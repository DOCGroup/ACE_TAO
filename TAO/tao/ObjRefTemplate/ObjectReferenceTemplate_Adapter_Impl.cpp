// $Id$

#include "tao/PortableServer/POA.h"

#include "tao/CORBA_String.h"
#include "tao/ORB_Constants.h"
#include "tao/CORBA_methods.h"

ACE_RCSID (ORT,
           ObjectReferenceTemplate_Adapater_Impl,
           "$Id$")


#if !defined (__ACE_INLINE__)
# include "ObjectReferenceTemplate_Adapter_Impl.h"
#endif /* ! __ACE_INLINE__ */


TAO_ObjectReferenceTemplate_Adapter_Impl::
  TAO_ObjectReferenceTemplate_Adapter_Impl()
{
}

TAO_ObjectReferenceTemplate_Adapter_Impl::
  ~TAO_ObjectReferenceTemplate_Adapter_Impl (void)
{
}

char *
TAO_ObjectReferenceTemplate_Adapter_Impl::server_id (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->ort_template_->server_id (ACE_ENV_SINGLE_ARG_PARAMETER);
}

char *
TAO_ObjectReferenceTemplate_Adapter_Impl::orb_id (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->ort_template_->orb_id (ACE_ENV_SINGLE_ARG_PARAMETER);
}

PortableInterceptor::AdapterName *
TAO_ObjectReferenceTemplate_Adapter_Impl::adapter_name (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->ort_template_->adapter_name (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Object_ptr
TAO_ObjectReferenceTemplate_Adapter_Impl::make_object (
    const char *repository_id,
    const PortableInterceptor::ObjectId &id
  ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  return this->ort_factory_->make_object (repository_id,
                                          id
                                          ACE_ENV_ARG_PARAMETER);
}

PortableInterceptor::ObjectReferenceTemplate *
TAO_ObjectReferenceTemplate_Adapter_Impl::get_adapter_template (void)
{
  CORBA::add_ref (ort_template_.in());

  return ort_template_;
}

PortableInterceptor::ObjectReferenceFactory *
TAO_ObjectReferenceTemplate_Adapter_Impl::get_obj_ref_factory (void)
{
  CORBA::add_ref (ort_factory_.in());

  return ort_factory_;
}

int
TAO_ObjectReferenceTemplate_Adapter_Impl::set_obj_ref_factory (
  PortableInterceptor::ObjectReferenceFactory *current_factory
  ACE_ENV_ARG_DECL)
{
  ort_factory_ = current_factory;

  CORBA::add_ref (ort_factory_.in());

  return 0;
}

int
TAO_ObjectReferenceTemplate_Adapter_Impl::activate (
  const char *server_id,
  const char *orb_id,
  PortableInterceptor::AdapterName *adapter_name,
  TAO_POA *poa
  ACE_ENV_ARG_DECL)
{
  // Create an ObjectReferenceTemplate for this POA.
  ACE_NEW_THROW_EX (this->tao_ort_template,
                    TAO_ObjectReferenceTemplate (
                      server_id,
                      orb_id,
                      adapter_name,
                      poa),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (-1);

  this->ort_template_ = this->tao_ort_template;

  // Must increase ref count since this->obj_ref_factory_ will
  // descrease it upon destruction.
  CORBA::add_ref (this->ort_template_.in ());
  this->ort_factory_ = this->ort_template_;

  return 0;
}

void
TAO_ObjectReferenceTemplate_Adapter_Impl::poa (TAO_POA * poa)
{
  this->tao_ort_template->poa (poa);
}
