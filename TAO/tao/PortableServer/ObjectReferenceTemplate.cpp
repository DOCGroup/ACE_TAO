#include "tao/PortableServer/ObjectReferenceTemplate.h"
#include "tao/PortableServer/POA.h"

#include "tao/ORB_Core.h"

ACE_RCSID (PortableServer,
           ObjectReferenceTemplate,
           "$Id$")

TAO_ObjectReferenceTemplate::
TAO_ObjectReferenceTemplate (const char *server_id,
                             const char *orb_id,
                             PortableInterceptor::AdapterName *adapter_name,
                             TAO_POA *poa)
  : server_id_ (server_id),
    orb_id_ (orb_id),
    adapter_name_ (adapter_name),
    poa_ (poa)
{
  /// Constructor
}

TAO_ObjectReferenceTemplate::~TAO_ObjectReferenceTemplate (void)
{
  /// Destructor
}

char *
TAO_ObjectReferenceTemplate::server_id (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ Priyanka, you're not conforming to the C++ mapping.  Do a
  //    "return CORBA::string_dup()".  The caller owns the
  //    storage/memory!
  return ACE_const_cast (char *, this->server_id_);
}

char *
TAO_ObjectReferenceTemplate::orb_id (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ Priyanka, you're not conforming to the C++ mapping.  Do a
  //    "return CORBA::string_dup()".  The caller owns the
  //    storage/memory!
  return ACE_const_cast (char *, this->orb_id_);
}

PortableInterceptor::AdapterName *
TAO_ObjectReferenceTemplate::adapter_name (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ Priyanka, you're not conforming to the C++ mapping.  You must
  //    instantiate a new PortableInterceptor::AdapterName instance,
  //    and initialize it with this->adapter_name_ (e.g. via the
  //    constructor).  Specifically, you must return a copy (in this
  //    case).  The caller owns the storage/memory!
  return this->adapter_name_;
}

// @@ Priyanka, technically this should be CORBA::Object_ptr!
CORBA::Object *
TAO_ObjectReferenceTemplate::make_object (
  const char * intf,
  const PortableInterceptor::ObjectId & id
  TAO_ENV_ARG_DECL)
  // @@ Priyanka, is this method supposed to have an exception
  //    specification?
{
  CORBA::OctetSeq object_id = id;

  // @@ Priyanka, reinterpret_cast<>s are generally really bad.  Do
  // you really have to use it?
  PortableServer::ObjectId *user_id =
    ACE_reinterpret_cast (PortableServer::ObjectId *,
                          &object_id);

  // Create a reference
  CORBA::Object_var object =
    this->poa_->invoke_key_to_object (intf,
                                      *user_id
                                      TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  // @@ Priyanka, what purpose does this object_to_string() call
  //    serve?  Can't you remove it?
  CORBA::String_var ior =
    this->poa_->orb_core ().orb ()->object_to_string (object.in ()
                                                      TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  return object._retn ();
}

void
TAO_ObjectReferenceTemplate::destroy (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  // @@ Priyanka, this is a reference counted object.  You cannot
  //    directly call delete() on the instance.  Use the reference
  //    counting mechanism.  For valuetypes, this is
  //    CORBA::remove_ref().
  delete this;
}
