// $Id$
#include "Stub_Factory.h"
#include "debug.h"

ACE_RCSID(tao, TAO_Stub_Factory, "$Id$")

  TAO_Stub_Factory::~TAO_Stub_Factory (void)
{
  // No-Op.
}

TAO_Stub *
TAO_Stub_Factory::create_stub (const char *repository_id,
                               const TAO_MProfile &profiles,
                               TAO_ORB_Core *orb_core,
                               CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Stub *retval = 0;
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "don't want to be here\n"));

  ACE_NEW_THROW_EX (retval,
                    TAO_Stub (repository_id, profiles, orb_core),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_MAYBE));
  ACE_CHECK_RETURN (retval);

  return retval;
}
