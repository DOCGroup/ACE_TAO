/**
 * @file AMI_ClientRequestInfo_i.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 *
 */

#include "AMI_ClientRequestInfo_i.h"

#if (TAO_HAS_INTERCEPTORS == 1)

#include "Twoway_Asynch_Invocation.h"


ACE_RCSID (Messaging,
           AMI_ClientRequestInfo_i,
           "$Id$")


TAO_AMI_ClientRequestInfo_i::
TAO_AMI_ClientRequestInfo_i (TAO_GIOP_Twoway_Asynch_Invocation * invocation,
                             CORBA::Object_ptr target)
  : TAO_ClientRequestInfo_i (invocation, target)
{
}

Dynamic::ParameterList *
TAO_AMI_ClientRequestInfo_i::arguments (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_RESOURCES (CORBA::OMGVMCID | 1,
                                         CORBA::COMPLETED_NO),
                    0);

}

Dynamic::ExceptionList *
TAO_AMI_ClientRequestInfo_i::exceptions (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_RESOURCES (CORBA::OMGVMCID | 1,
                                         CORBA::COMPLETED_NO),
                    0);

}

CORBA::Any *
TAO_AMI_ClientRequestInfo_i::result (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_RESOURCES (CORBA::OMGVMCID | 1,
                                         CORBA::COMPLETED_NO),
                    0);

}

#endif /* TAO_HAS_INTERCEPTORS == 1 */
