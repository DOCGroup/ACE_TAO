/* -*- C++ -*- */

// $Id$
// =================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    DynamicAny.cpp
//
// = AUTHOR
//    Carlos O'Ryan <coryan@uci.edu>
//
// =================================================================

#include "DynamicAny.h"
#include "DynAnyFactory.h"

ACE_RCSID(DynamicAny, DynamicAny, "$Id$")

TAO_DynamicAny_Loader::TAO_DynamicAny_Loader (void)
{
}

CORBA::Object_ptr
TAO_DynamicAny_Loader::create_object (CORBA::ORB_ptr,
                                      int,
                                      ACE_TCHAR *[]
                                      ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Object_ptr obj;
  ACE_NEW_RETURN (obj,
                  TAO_DynAnyFactory,
                  CORBA::Object::_nil ());
  return obj;
}

int
TAO_DynamicAny_Loader::Initializer (void)
{
  return ACE_Service_Config::process_directive (ace_svc_desc_TAO_DynamicAny_Loader);
}

ACE_STATIC_SVC_DEFINE (TAO_DynamicAny_Loader,
                       ACE_TEXT ("DynamicAny_Loader"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_DynamicAny_Loader),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_DynamicAny, TAO_DynamicAny_Loader)
