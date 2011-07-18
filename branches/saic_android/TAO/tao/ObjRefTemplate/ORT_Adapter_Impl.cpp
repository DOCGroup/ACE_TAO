// $Id$

#include "tao/ObjRefTemplate/ORT_Adapter_Impl.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/CORBA_String.h"
#include "tao/ORB_Constants.h"
#include "tao/CORBA_methods.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

char *
TAO::ORT_Adapter_Impl::tao_server_id (void)
{
  // No need to duplicate, the ort_template_ method has to do the duplicate
  return this->ort_template_->server_id ();
}

char *
TAO::ORT_Adapter_Impl::tao_orb_id (void)
{
  // No need to duplicate, the ort_template_ method has to do the duplicate
  return this->ort_template_->orb_id ();
}

PortableInterceptor::AdapterName *
TAO::ORT_Adapter_Impl::tao_adapter_name (void)
{
  // No need to duplicate, the ort_template_ method has to do the duplicate
  return this->ort_template_->adapter_name ();
}

CORBA::Object_ptr
TAO::ORT_Adapter_Impl::make_object (const char *repo_id,
                                    const PortableInterceptor::ObjectId &id)
{
  return this->ort_factory_->make_object (repo_id, id);
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
  PortableInterceptor::ObjectReferenceFactory *cf)
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
  PortableServer::POA_ptr poa)
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

  this->ort_template_ = t;

  // Must increase ref count since this->ort_factory_ will
  // decrease it upon destruction.
  CORBA::add_ref (t);
  this->ort_factory_ = t;

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
