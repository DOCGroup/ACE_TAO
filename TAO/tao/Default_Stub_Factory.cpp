// $Id$
#include "Default_Stub_Factory.h"
#include "Stub.h"

ACE_RCSID(tao, TAO_Default_Stub_Factory, "$Id$")

TAO_Default_Stub_Factory::~TAO_Default_Stub_Factory (void)
{
  // No-Op.
}

TAO_Stub *
TAO_Default_Stub_Factory::create_stub (const char *repository_id,
                               const TAO_MProfile &profiles,
                               TAO_ORB_Core *orb_core
                               TAO_ENV_ARG_DECL)
{
  TAO_Stub *retval = 0;

  ACE_NEW_THROW_EX (retval,
                    TAO_Stub (repository_id, profiles, orb_core),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_MAYBE));
  ACE_CHECK_RETURN (retval);

  return retval;
}

// ****************************************************************

ACE_STATIC_SVC_DEFINE (TAO_Default_Stub_Factory,
                       ACE_TEXT ("Default_Stub_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Default_Stub_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO, TAO_Default_Stub_Factory)
