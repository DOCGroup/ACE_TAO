/* -*- C++ -*- */

// $Id$
// =================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    PortableGroup_Loader.cpp
//
// = AUTHOR
//    Frank Hunleth <fhunleth@cs.wustl.edu>
//
// =================================================================

#include "PGA_Loader.h"
#include "PGA.h"

ACE_RCSID(IORManip, IORManip, "$Id$")

TAO_PGA_Loader::TAO_PGA_Loader (void)
{
}

CORBA::Object_ptr
TAO_PGA_Loader::create_object (CORBA::ORB_ptr orb,
                                    int,
                                    char *[],
                                    CORBA::Environment &)
  ACE_THROW_SPEC (())
{
  CORBA::Object_ptr obj;
  ACE_NEW_RETURN (obj,
                  TAO_PGA (orb->orb_core ()),
                  CORBA::Object::_nil ());
  return obj;
}

int
TAO_PGA_Loader::Initializer (void)
{
  ACE_Service_Config::static_svcs ()->
    insert (&ace_svc_desc_TAO_PGA_Loader);

  return 0;
}

ACE_STATIC_SVC_DEFINE (TAO_PGA_Loader,
                       ACE_TEXT ("PGA_Loader"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_PGA_Loader),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_PortableGroup, TAO_PGA_Loader)
