// $Id$

#include "ORT_Adapter_Impl.h"
#include "tao/PortableServer/POA.h"
#include "tao/CORBA_String.h"
#include "tao/ORB_Constants.h"
#include "tao/CORBA_methods.h"

ACE_RCSID (ORT,
           ORT_Adapter_Impl,
           "$Id$")

namespace TAO
{
  ORT_Adapter_Impl::ORT_Adapter_Impl (void)
    : tao_ort_template_ (0)
  {
  }

  ORT_Adapter_Impl::~ORT_Adapter_Impl (void)
  {
  }

  char *
  ORT_Adapter_Impl::tao_server_id (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    // No need to duplicate, the ort_template_ method has to do the duplicate
    return this->ort_template_->server_id (ACE_ENV_SINGLE_ARG_PARAMETER);
  }

  char *
  ORT_Adapter_Impl::tao_orb_id (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    // No need to duplicate, the ort_template_ method has to do the duplicate
    return this->ort_template_->orb_id (ACE_ENV_SINGLE_ARG_PARAMETER);
  }

  PortableInterceptor::AdapterName *
  ORT_Adapter_Impl::tao_adapter_name (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    // No need to duplicate, the ort_template_ method has to do the duplicate
    return this->ort_template_->adapter_name (ACE_ENV_SINGLE_ARG_PARAMETER);
  }

  CORBA::Object_ptr
  ORT_Adapter_Impl::make_object (const char *repo_id,
                                 const PortableInterceptor::ObjectId &id
                                 ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return this->ort_factory_->make_object (repo_id,
                                            id
                                            ACE_ENV_ARG_PARAMETER);
  }

  PortableInterceptor::ObjectReferenceTemplate *
  ORT_Adapter_Impl::get_adapter_template (void)
  {
    CORBA::add_ref (ort_template_.in ());

    return ort_template_;
  }

  PortableInterceptor::ObjectReferenceFactory *
  ORT_Adapter_Impl::get_obj_ref_factory (void)
  {
    CORBA::add_ref (ort_factory_.in ());

    return ort_factory_;
  }

  int
  ORT_Adapter_Impl::set_obj_ref_factory (
    PortableInterceptor::ObjectReferenceFactory *cf
    ACE_ENV_ARG_DECL_NOT_USED)
  {
    ort_factory_ = cf;

    CORBA::add_ref (ort_factory_.in ());

    return 0;
  }

  int
  ORT_Adapter_Impl::activate (const char *server_id,
                              const char *orb_id,
                              PortableInterceptor::AdapterName *adapter_name,
                              PortableServer::POA_ptr poa
                              ACE_ENV_ARG_DECL)
  {
    // No need to lock here, there is one instance for each POA and
    // when the POA creates and actives an ORT_Adapter it will lock
    // itself. Create an ObjectReferenceTemplate for this POA.

    ACE_NEW_THROW_EX (this->tao_ort_template_,
                      ObjectReferenceTemplate (server_id,
                                               orb_id,
                                               adapter_name,
                                               poa),
                      CORBA::NO_MEMORY ());
    ACE_CHECK_RETURN (-1);

    this->ort_template_ = this->tao_ort_template_;

    // Must increase ref count since this->obj_ref_factory_ will
    // decrease it upon destruction.
    CORBA::add_ref (this->ort_template_.in ());
    this->ort_factory_ = this->ort_template_;

    return 0;
  }
}
