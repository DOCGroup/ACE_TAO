// =================================================================
/**
 * @file CodecFactory.cpp
 *
 * $Id$
 *
 * @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
// =================================================================

#include "tao/CodecFactory/CodecFactory.h"
#include "tao/CodecFactory/CodecFactory_impl.h"

#include "tao/ORB.h"
#include "tao/debug.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::Object_ptr
TAO_CodecFactory_Loader::create_object (CORBA::ORB_ptr orb,  int, ACE_TCHAR *[])
{
  CORBA::Object_ptr obj = CORBA::Object_ptr ();
  ACE_NEW_RETURN (obj,
                  TAO_CodecFactory (orb->orb_core ()),
                  CORBA::Object::_nil ());
  return obj;
}

int
TAO_CodecFactory_Loader::Initializer (void)
{
  return ACE_Service_Config::process_directive (ace_svc_desc_TAO_CodecFactory_Loader);
}

ACE_STATIC_SVC_DEFINE (TAO_CodecFactory_Loader,
                       ACE_TEXT ("CodecFactory_Loader"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_CodecFactory_Loader),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_CodecFactory, TAO_CodecFactory_Loader)

TAO_END_VERSIONED_NAMESPACE_DECL

