// -*- C++ -*-

#include "PICurrent_ORBInitializer.h"


#if TAO_HAS_INTERCEPTORS == 1

#include "ORB_Core.h"

#include "PICurrent.h"
#include "ORBInitInfo.h"

ACE_RCSID (TAO,
           PICurrent_ORBInitializer,
           "$Id$")

void
TAO_PICurrent_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr info
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_ORBInitInfo_var tao_info =
    TAO_ORBInitInfo::_narrow (info TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  TAO_ORB_Core *orb_core = tao_info->orb_core ();

  TAO_PICurrent *tao_pi_current = 0;
  ACE_NEW_THROW_EX (tao_pi_current,
                    TAO_PICurrent (orb_core),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  PortableInterceptor::Current_var current = tao_pi_current;

  // The PICurrent is stateless and reentrant, so share a single
  // instance between all ORBs.
  info->register_initial_reference ("PICurrent",
                                    current.in ()
                                     TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Cache the downcasted pointer in the ORB Core.
  orb_core->pi_current (tao_pi_current);
}

void
TAO_PICurrent_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}


#endif  /* TAO_HAS_INTERCEPTORS == 1 */
