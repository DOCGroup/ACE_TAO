/* -*- C++ -*- */
// $Id$

// =================================================================
//
// = LIBRARY
//    TAO/orbsvcs/TypeCodeFactory_DLL
//
// = FILENAME
//    TCF_Loader.cpp
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// =================================================================

#include "TCF_Loader.h"
#include "TypeCodeFactory_i.h"

ACE_RCSID(IFR_Service, TCF_Loader, "$Id$")

TCF_Loader::TCF_Loader (void)
{
}

CORBA::Object_ptr
TCF_Loader::create_object (CORBA::ORB_ptr orb,
                           int,
                           char * [],
                           CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC (())
{
  CORBA::Object_ptr obj;
  ACE_NEW_RETURN (obj,
                  TAO_TypeCodeFactory_i,
                  CORBA::Object::_nil ());
  return obj;
}

int
TCF_Loader::Initializer (void)
{
  ACE_Service_Config::static_svcs ()->
    insert (&ace_svc_desc_TCF_Loader);

  return 0;
}

ACE_STATIC_SVC_DEFINE (TCF_Loader,
                       ACE_TEXT ("TCF_Loader"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TCF_Loader),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TCF, TCF_Loader)
