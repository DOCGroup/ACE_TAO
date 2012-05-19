/* -*- C++ -*- */

// =================================================================
/**
 * @file PICurrent_Loader.cpp
 *
 * $Id$
 *
 * @author Johnny Willemsen  <jwillemsen@remedy.nl>
 *
 */
// =================================================================

#include "tao/PI/PICurrent_Loader.h"

#if TAO_HAS_INTERCEPTORS == 1

#include "tao/PI/PICurrent.h"

#include "tao/ORB.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::Object_ptr
TAO_PICurrent_Loader::create_object (CORBA::ORB_ptr orb,  int, ACE_TCHAR *[])
{
  CORBA::Object_ptr obj = CORBA::Object::_nil ();
  ACE_NEW_RETURN (obj,
                  TAO::PICurrent (*orb->orb_core ()),
                  CORBA::Object::_nil ());
  return obj;
}


ACE_STATIC_SVC_DEFINE (TAO_PICurrent_Loader,
                       ACE_TEXT ("PICurrent_Loader"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_PICurrent_Loader),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_PI, TAO_PICurrent_Loader)

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_HAS_INTERCEPTORS == 1 */
