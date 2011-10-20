// $Id$

#include "tao/Compression/Compression.h"
#include "tao/Compression/Compression_Manager.h"
#include "tao/debug.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::Object_ptr
TAO_Compression_Loader::create_object (CORBA::ORB_ptr, int, ACE_TCHAR *[])
{
  CORBA::Object_ptr obj = CORBA::Object::_nil ();
  ACE_NEW_RETURN (obj,
                      TAO::CompressionManager (),
                      CORBA::Object::_nil ());
  return obj;
}


int
TAO_Compression_Loader::Initializer (void)
{
  return ACE_Service_Config::process_directive (
    ace_svc_desc_TAO_Compression_Loader);
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DEFINE (TAO_Compression_Loader,
                       ACE_TEXT ("Compression_Loader"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Compression_Loader),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO_Compression, TAO_Compression_Loader)
