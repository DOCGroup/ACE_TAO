/* -*- C++ -*- */
// $Id$

// =================================================================
//
// = LIBRARY
//    TAO/tao/TypeCodeFactory
//
// = FILENAME
//    TypeCodeFactory_Loader.cpp
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// =================================================================

#include "TypeCodeFactory_Loader.h"
#include "TypeCodeFactory_i.h"

ACE_RCSID(TypeCodeFactory, TypeCodeFactory_Loader, "$Id$")

TAO_TypeCodeFactory_Loader::TAO_TypeCodeFactory_Loader (void)
{
}

CORBA::Object_ptr
TAO_TypeCodeFactory_Loader::create_object (CORBA::ORB_ptr,
                                           int,
                                           char * []
                                           ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
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

ACE_STATIC_SVC_DEFINE (
    TAO_TypeCodeFactory_Loader,
    ACE_TEXT ("TypeCodeFactory_Loader"),
    ACE_SVC_OBJ_T,
    &ACE_SVC_NAME (TAO_TypeCodeFactory_Loader),
    ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
    0
  )

ACE_FACTORY_DEFINE (TAO_TypeCodeFactory, TAO_TypeCodeFactory_Loader)
