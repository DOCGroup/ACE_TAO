// $Id$

#include "tao/IORManipulation/IORManip_Loader.h"
#include "tao/IORManipulation/IORManipulation.h"

#include "ace/Log_Msg.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_IORManip_Loader::TAO_IORManip_Loader (void)
{
}

CORBA::Object_ptr
TAO_IORManip_Loader::create_object (CORBA::ORB_ptr, int, ACE_TCHAR *[])
{
  CORBA::Object_ptr obj;
  ACE_NEW_RETURN (obj,
                  TAO_IOR_Manipulation_impl,
                  CORBA::Object::_nil ());
  return obj;
}

int
TAO_IORManip_Loader::Initializer (void)
{
  return ACE_Service_Config::process_directive (ace_svc_desc_TAO_IORManip_Loader);
}


ACE_STATIC_SVC_DEFINE (TAO_IORManip_Loader,
                       ACE_TEXT ("IORManip_Loader"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_IORManip_Loader),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_IORManip, TAO_IORManip_Loader)

TAO_END_VERSIONED_NAMESPACE_DECL
