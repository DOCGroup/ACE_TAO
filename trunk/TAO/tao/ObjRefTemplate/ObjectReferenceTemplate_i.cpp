// $Id$

#include "tao/ObjRefTemplate/ObjectReferenceTemplate_i.h"
#include "tao/PortableServer/Root_POA.h"

#include "tao/CORBA_String.h"
#include "tao/ORB_Constants.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  ObjectReferenceTemplate::ObjectReferenceTemplate (
  const char *server_id,
  const char *orb_id,
  PortableInterceptor::AdapterName *adapter_name,
  PortableServer::POA_ptr poa)
  : server_id_ (server_id),
    orb_id_ (orb_id),
    adapter_name_ (adapter_name),
    poa_ (PortableServer::POA::_duplicate (poa))
  {
  }

  ObjectReferenceTemplate::~ObjectReferenceTemplate (void)
  {
  }

  ::CORBA::ValueBase *
  ObjectReferenceTemplate::_copy_value (void)
  {
    ::CORBA::ValueBase *ret_val= 0;
    ACE_NEW_THROW_EX (
      ret_val,
      ObjectReferenceTemplate (
        server_id_,
        orb_id_,
        adapter_name_,
        poa_.in ()
      ),
      ::CORBA::NO_MEMORY ()
    );
    return ret_val;
  }

  char *
  ObjectReferenceTemplate::server_id (void)
  {
    return CORBA::string_dup (this->server_id_);
  }

  char *
  ObjectReferenceTemplate::orb_id (void)
  {
    return CORBA::string_dup (this->orb_id_);
  }

  PortableInterceptor::AdapterName *
  ObjectReferenceTemplate::adapter_name (void)
  {
    PortableInterceptor::AdapterName *adapter_name = 0;

    ACE_NEW_THROW_EX (adapter_name,
                      PortableInterceptor::AdapterName (
                                                        *(this->adapter_name_)),
                      CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                              TAO::VMCID,
                              ENOMEM),
                          CORBA::COMPLETED_NO));

    return adapter_name;
  }

  CORBA::Object_ptr
  ObjectReferenceTemplate::make_object (
      const char *,
      const PortableInterceptor::ObjectId &)
  {
    if (CORBA::is_nil (this->poa_.in ()))
      throw ::CORBA::BAD_INV_ORDER ();

    TAO_Root_POA* const tao_poa =
      dynamic_cast<TAO_Root_POA*> (this->poa_.in());

    if (!tao_poa)
      throw ::CORBA::INTERNAL ();

    return tao_poa->invoke_key_to_object ();
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
