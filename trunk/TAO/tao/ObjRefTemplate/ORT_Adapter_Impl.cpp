// $Id$

#include "tao/ObjRefTemplate/ORT_Adapter_Impl.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/CORBA_String.h"
#include "tao/ORB_Constants.h"
#include "tao/CORBA_methods.h"


ACE_RCSID (ObjRefTemplate,
           ORT_Adapter_Impl,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

char *
TAO::ORT_Adapter_Impl::tao_server_id (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // No need to duplicate, the ort_template_ method has to do the duplicate
  return this->ort_template_->server_id (ACE_ENV_SINGLE_ARG_PARAMETER);
}

char *
TAO::ORT_Adapter_Impl::tao_orb_id (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // No need to duplicate, the ort_template_ method has to do the duplicate
  return this->ort_template_->orb_id (ACE_ENV_SINGLE_ARG_PARAMETER);
}

PortableInterceptor::AdapterName *
TAO::ORT_Adapter_Impl::tao_adapter_name (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // No need to duplicate, the ort_template_ method has to do the duplicate
  return this->ort_template_->adapter_name (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Object_ptr
TAO::ORT_Adapter_Impl::make_object (const char *repo_id,
                                    const PortableInterceptor::ObjectId &id
                                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->ort_factory_->make_object (repo_id,
                                          id
                                          ACE_ENV_ARG_PARAMETER);
}

PortableInterceptor::ObjectReferenceTemplate *
TAO::ORT_Adapter_Impl::get_adapter_template (void)
{
  CORBA::add_ref (this->ort_template_.in ());

  return this->ort_template_;
}

PortableInterceptor::ObjectReferenceFactory *
TAO::ORT_Adapter_Impl::get_obj_ref_factory (void)
{
  CORBA::add_ref (this->ort_factory_.in ());

  return this->ort_factory_;
}

int
TAO::ORT_Adapter_Impl::set_obj_ref_factory (
  PortableInterceptor::ObjectReferenceFactory *cf
  ACE_ENV_ARG_DECL_NOT_USED)
{
  this->ort_factory_ = cf;

  CORBA::add_ref (this->ort_factory_.in ());

  return 0;
}

void
TAO::ORT_Adapter_Impl::release (
  PortableInterceptor::ObjectReferenceTemplate * t)
{
  CORBA::remove_ref (t);
}

int
TAO::ORT_Adapter_Impl::activate (
  const char *server_id,
  const char *orb_id,
  PortableInterceptor::AdapterName *adapter_name,
  PortableServer::POA_ptr poa
  ACE_ENV_ARG_DECL)
{
  // No need to lock here, there is one instance for each POA and
  // when the POA creates and actives an ORT_Adapter it will lock
  // itself. Create an ObjectReferenceTemplate for this POA.

  ObjectReferenceTemplate * t = 0;
  ACE_NEW_THROW_EX (t,
                    ObjectReferenceTemplate (server_id,
                                             orb_id,
                                             adapter_name,
                                             poa),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (-1);

  this->ort_template_ = t;

  // Must increase ref count since this->ort_factory_ will
  // decrease it upon destruction.
  CORBA::add_ref (t);
  this->ort_factory_ = t;

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
