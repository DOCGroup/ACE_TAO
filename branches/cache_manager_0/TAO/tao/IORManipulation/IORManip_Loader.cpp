/* -*- C++ -*- */

// $Id$
// =================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    IORManip_Loader.cpp
//
// = AUTHOR
//    Bala Natarajan <bala@cs.wustl.edu>
//
// =================================================================

#include "IORManip_Loader.h"
#include "IORManipulation.h"

ACE_RCSID(IORManip, IORManip, "$Id$")

TAO_IORManip_Loader::TAO_IORManip_Loader (void)
{
}

CORBA::Object_ptr
TAO_IORManip_Loader::create_object (CORBA::ORB_ptr,
                                    int,
                                    char *[]
                                    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
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
