// $Id$

#include "ObjectReferenceTemplate_Adapter_Impl.h"
#include "tao/PortableServer/POA.h"
#include "tao/CORBA_String.h"
#include "tao/ORB_Constants.h"
#include "tao/CORBA_methods.h"

#if !defined (__ACE_INLINE__)
# include "ObjectReferenceTemplate_Adapter_Impl.inl"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID (ORT,
           ObjectReferenceTemplate_Adapater_Impl,
           "$Id$")

namespace TAO
{
  ObjectReferenceTemplate_Adapter_Impl::ObjectReferenceTemplate_Adapter_Impl (void)
    : tao_ort_template_ (0)
  {
  }

  ObjectReferenceTemplate_Adapter_Impl::~ObjectReferenceTemplate_Adapter_Impl (void)
  {
  }

  char *
  ObjectReferenceTemplate_Adapter_Impl::server_id (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    // @@Johnny, just document why duplicate isn't necessary.
    return this->ort_template_->server_id (ACE_ENV_SINGLE_ARG_PARAMETER);
  }

  char *
  ObjectReferenceTemplate_Adapter_Impl::orb_id (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    // @@Johnny, just document why duplicate isn't necessary.
    return this->ort_template_->orb_id (ACE_ENV_SINGLE_ARG_PARAMETER);
  }

  PortableInterceptor::AdapterName *
  ObjectReferenceTemplate_Adapter_Impl::adapter_name (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    // @@Johnny, comment for duplicate also applies here..
    return this->ort_template_->adapter_name (ACE_ENV_SINGLE_ARG_PARAMETER);
  }

  CORBA::Object_ptr
  ObjectReferenceTemplate_Adapter_Impl::make_object (const char *repo_id,
                                                     const TAO_PI::ObjectId &id
                                                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return this->ort_factory_->make_object (repo_id,
                                            id
                                            ACE_ENV_ARG_PARAMETER);
  }

  TAO_PI::ObjectReferenceTemplate *
  ObjectReferenceTemplate_Adapter_Impl::get_adapter_template (void)
  {
    CORBA::add_ref (ort_template_.in());

    return ort_template_;
  }

  TAO_PI::ObjectReferenceFactory *
  ObjectReferenceTemplate_Adapter_Impl::get_obj_ref_factory (void)
  {
    CORBA::add_ref (ort_factory_.in());

    return ort_factory_;
  }

  int
  ObjectReferenceTemplate_Adapter_Impl::set_obj_ref_factory (
    TAO_PI::ObjectReferenceFactory *cf
    ACE_ENV_ARG_DECL)
  {
    ort_factory_ = cf;

    CORBA::add_ref (ort_factory_.in ());

    return 0;
  }

  int
  ObjectReferenceTemplate_Adapter_Impl::activate (const char *server_id,
                                                  const char *orb_id,
                                                  TAO_PI::AdapterName *adapter_name,
                                                  TAO_POA *poa
                                                  ACE_ENV_ARG_DECL)
  {
    // @@Johnny, how do you protect against multiple activates. Just
    // for sanity.. Do we need a lock?
    // Create an ObjectReferenceTemplate for this POA.
    ACE_NEW_THROW_EX (this->tao_ort_template_,
                      ObjectReferenceTemplate (server_id,
                                               orb_id,
                                               adapter_name,
                                               poa),
                      CORBA::NO_MEMORY ());
    ACE_CHECK_RETURN (-1);

    this->ort_template_ = this->tao_ort_template_;

    // Must increase ref count since this->obj_ref_factory_ will
    // descrease it upon destruction.
    CORBA::add_ref (this->ort_template_.in ());
    this->ort_factory_ = this->ort_template_;

    return 0;
  }

  void
  ObjectReferenceTemplate_Adapter_Impl::poa (TAO_POA * poa)
  {
    // @@ Johnny, why isn't the POA  duplicated here?
    this->tao_ort_template_->poa (poa);
  }
}
