// $Id$

#include "RT_ORB_Loader.h"
#include "RT_ORB.h"
#include "RT_Current.h"

#include "ORB_Core.h"

ACE_RCSID (TAO, RT_ORB_Loader, "$Id$")

TAO_RT_ORB_Loader::TAO_RT_ORB_Loader (void)
{
}

CORBA::Object_ptr
TAO_RT_ORB_Loader::create_object (CORBA::ORB_ptr orb,
                                  int,
                                  char *[],
                                  CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  /// Return RT_ORB
  CORBA::Object_ptr rt_orb;
  ACE_NEW_THROW_EX (rt_orb,
                    TAO_RT_ORB,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  CORBA::Object_var safe_rt_orb = rt_orb;

  /// Sets the RT_Current object in the ORB.
  this->set_objects (orb->orb_core (), ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  return safe_rt_orb._retn ();
}

void
TAO_RT_ORB_Loader::set_objects (TAO_ORB_Core *orb_core,
                                CORBA::Environment &ACE_TRY_ENV)
{
  // Setup the RT_Current object in the ORB

  CORBA::Object_ptr current;
  ACE_NEW_THROW_EX (current,
                    TAO_RT_Current (orb_core),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  CORBA::Object_var safe_current = current;
  orb_core->rt_current (safe_current.in ());
}

ACE_FACTORY_DEFINE (TAO, TAO_RT_ORB_Loader)
ACE_STATIC_SVC_DEFINE (TAO_RT_ORB_Loader,
                       ACE_TEXT ("RT_ORB_Loader"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_RT_ORB_Loader),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)
