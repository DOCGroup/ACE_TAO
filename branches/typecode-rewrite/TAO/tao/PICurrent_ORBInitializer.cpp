#include "PICurrent_ORBInitializer.h"

#if TAO_HAS_INTERCEPTORS == 1

#include "ORB_Core.h"
#include "ORBInitInfo.h"
#include "SystemException.h"

ACE_RCSID (tao,
           PICurrent_ORBInitializer,
           "$Id$")

void
TAO_PICurrent_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_ORBInitInfo_var tao_info =
    TAO_ORBInitInfo::_narrow (info ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  TAO_ORB_Core * orb_core = tao_info->orb_core ();

  TAO::PICurrent * tao_pi_current = 0;
  ACE_NEW_THROW_EX (tao_pi_current,
                    TAO::PICurrent (orb_core),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        0,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  PortableInterceptor::Current_var current = tao_pi_current;

  info->register_initial_reference ("PICurrent",
                                    current.in ()
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Cache the downcasted pointer in the ORB Core.
  orb_core->pi_current (tao_pi_current);
}

void
TAO_PICurrent_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}


#endif  /* TAO_HAS_INTERCEPTORS == 1 */
