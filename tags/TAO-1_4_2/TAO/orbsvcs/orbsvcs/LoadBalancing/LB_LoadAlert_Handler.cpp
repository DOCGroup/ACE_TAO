#include "LB_LoadAlert_Handler.h"

#include "tao/debug.h"


ACE_RCSID (LoadBalancing,
           LB_LoadAlert_Handler,
           "$Id$")


TAO_LB_LoadAlert_Handler::~TAO_LB_LoadAlert_Handler (void)
{
}

void
TAO_LB_LoadAlert_Handler::enable_alert (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
TAO_LB_LoadAlert_Handler::enable_alert_excep (
    CosLoadBalancing::AMI_LoadAlertExceptionHolder *
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (TAO_debug_level > 0)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Exception caught when invoking ")
                ACE_TEXT ("LoadAlert::enable_alert()\n")));
}

void
TAO_LB_LoadAlert_Handler::disable_alert (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
TAO_LB_LoadAlert_Handler::disable_alert_excep (
    CosLoadBalancing::AMI_LoadAlertExceptionHolder *
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (TAO_debug_level > 0)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Exception caught when invoking ")
                ACE_TEXT ("LoadAlert::disable_alert()\n")));
}
