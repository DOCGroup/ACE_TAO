#include "LB_LoadAlert.h"


ACE_RCSID (LoadBalancer,
           LB_LoadAlert,
           "$Id$")

#if !defined (__ACE_INLINE__)
# include "LB_LoadAlert.inl"
#endif /* __ACE_INLINE__ */

TAO_LB_LoadAlert::TAO_LB_LoadAlert (void)
  : alerted_ (0),
    forward_ ()
{
}

TAO_LB_LoadAlert::~TAO_LB_LoadAlert (void)
{
}

void
TAO_LB_LoadAlert::enable_alert (CORBA::Object /* object_group */
                                ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->alerted_ = 1;
}

void
TAO_LB_LoadAlert::disable_alert (ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->alerted_ = 0;
}
