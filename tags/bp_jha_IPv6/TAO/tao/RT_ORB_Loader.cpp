// $Id$

#include "RT_ORB_Loader.h"
#include "RT_ORB.h"
#include "RT_Current.h"

#include "tao/ORB_Core.h"
#include "tao/Priority_Mapping_Manager.h"

ACE_RCSID(TAO, RT_ORB_Loader, "$Id: ")

TAO_RT_ORB_Loader::TAO_RT_ORB_Loader (void)
{
}

CORBA::Object_ptr
TAO_RT_ORB_Loader::create_object (CORBA::ORB_ptr orb,
                                  int,
                                  char *[],
                                  CORBA::Environment &)
  ACE_THROW_SPEC (())
{
  /// Return RT_ORB
  CORBA::Object_ptr obj;
  ACE_NEW_RETURN (obj,
                  TAO_RT_ORB,
                  CORBA::Object::_nil ());

  /// Sets the RT_Current object and Priority_Mapping_Manager
  /// in the ORB.
  this->set_objects (orb->orb_core ());

  return obj;
}

void
TAO_RT_ORB_Loader::set_objects (TAO_ORB_Core *orb_core)
{
  /// Setup the RT_Current object in the ORB
  CORBA::Object_var current = new TAO_RT_Current (orb_core);
  orb_core->rt_current (current.in ());
}

ACE_FACTORY_DEFINE (TAO, TAO_RT_ORB_Loader)
ACE_STATIC_SVC_DEFINE (TAO_RT_ORB_Loader,
                       ACE_TEXT ("RT_ORB_Loader"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_RT_ORB_Loader),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
