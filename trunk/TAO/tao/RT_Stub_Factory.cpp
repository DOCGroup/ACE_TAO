// $Id$
#include "RT_Stub_Factory.h"
#include "RT_Stub.h"

#if (TAO_HAS_RT_CORBA == 1)

ACE_RCSID(tao, RT_TAO_Stub_Factory, "$Id$")

TAO_RT_Stub_Factory::~TAO_RT_Stub_Factory (void)
{
  // No-Op.
}

TAO_Stub *
TAO_RT_Stub_Factory::create_stub (const char *repository_id,
                                  const TAO_MProfile &profiles,
                                  TAO_ORB_Core *orb_core,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  // @@ TODO: As soon as the RT_STUB is done change this to
  // create an RT_STUB
  TAO_Stub *retval = 0;

  ACE_NEW_THROW_EX (retval,
                    TAO_RT_Stub (repository_id, profiles, orb_core),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_MAYBE));

  ACE_CHECK_RETURN (retval);

  return retval;
}

#endif /* TAO_HAS_RT_CORBA == 1 */
