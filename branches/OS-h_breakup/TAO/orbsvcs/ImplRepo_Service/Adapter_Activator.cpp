// -*- C++ -*-
//=============================================================================
/**
 *  @file   Adapter_Activator.cpp
 *
 *  $Id$
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================

#include "Adapter_Activator.h"

ImR_Adapter_Activator::ImR_Adapter_Activator (
  PortableServer::ServantLocator_ptr servant
)
  : servant_locator_ (servant)
{
  // Nothing
}

CORBA::Boolean
ImR_Adapter_Activator::unknown_adapter (
  PortableServer::POA_ptr parent,
  const char *name
  ACE_ENV_ARG_DECL
)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::PolicyList policies (2);
  policies.length (2);

  const char *exception_message = "Null Message";

  ACE_TRY
    {
      // Servant Retention Policy
      exception_message = "While PortableServer::POA::create_servant_retention_policy";
      policies[0] =
        parent->create_servant_retention_policy
                  (PortableServer::NON_RETAIN ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Request Processing Policy
      exception_message = "While PortableServer::POA::create_request_processing_policy";
      policies[1] =
        parent->create_request_processing_policy
                  (PortableServer::USE_SERVANT_MANAGER ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        parent->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      exception_message = "While create_POA";
      PortableServer::POA_var child = parent->create_POA (name,
                                                          poa_manager.in (),
                                                          policies
                                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      exception_message = "While unknown_adapter::policy->destroy";
      for (CORBA::ULong i = 0; i < policies.length (); ++i)
        {
          CORBA::Policy_ptr policy = policies[i];
          policy->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      exception_message = "While child->the_activator";
      child->the_activator (this ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      exception_message = "While unknown_adapter, set_servant_manager";
      child->set_servant_manager(this->servant_locator_ ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_ERROR ((LM_ERROR,
                  "IMR_Adapter_Activator::unknown_adapter - %s\n",
                  exception_message));
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "System Exception");
      return 0;
    }
  ACE_ENDTRY;

  // Finally, now everything is fine
  return 1;
}

