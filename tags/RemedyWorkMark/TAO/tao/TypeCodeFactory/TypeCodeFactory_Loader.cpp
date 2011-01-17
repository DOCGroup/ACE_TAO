// -*- C++ -*-

//=============================================================================
/**
 *  @file    TypeCodeFactory_Loader.cpp
 *
 *  $Id$
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================

#include "tao/TypeCodeFactory/TypeCodeFactory_Loader.h"
#include "tao/TypeCodeFactory/TypeCodeFactory_i.h"
#include "ace/Log_Msg.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_TypeCodeFactory_Loader::TAO_TypeCodeFactory_Loader (void)
{
}

CORBA::Object_ptr
TAO_TypeCodeFactory_Loader::create_object (CORBA::ORB_ptr, int, ACE_TCHAR * [])
{
  CORBA::Object_ptr obj;
  ACE_NEW_RETURN (obj,
                  TAO_TypeCodeFactory_i,
                  CORBA::Object::_nil ());
  return obj;
}

int
TAO_TypeCodeFactory_Loader::Initializer (void)
{
  return ACE_Service_Config::process_directive (ace_svc_desc_TAO_TypeCodeFactory_Loader);
}

TAO_END_VERSIONED_NAMESPACE_DECL


ACE_STATIC_SVC_DEFINE (
    TAO_TypeCodeFactory_Loader,
    ACE_TEXT ("TypeCodeFactory_Loader"),
    ACE_SVC_OBJ_T,
    &ACE_SVC_NAME (TAO_TypeCodeFactory_Loader),
    ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
    0
  )

ACE_FACTORY_DEFINE (TAO_TypeCodeFactory, TAO_TypeCodeFactory_Loader)
