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

#include "orbsvcs/Log_Macros.h"
#include "tao/PortableServer/Servant_Base.h"

ImR_Adapter::ImR_Adapter (void)
  : default_servant_ (0)
{
}

void
ImR_Adapter::init (TAO_ServantBase * servant)
{
  ORBSVCS_DEBUG ((LM_DEBUG, "ImR_Adapter::init with default servant\n"));
  default_servant_ = servant;
}

CORBA::Boolean
ImR_Adapter::unknown_adapter (PortableServer::POA_ptr parent,
                              const char *name)
{
  ACE_ASSERT (! CORBA::is_nil(parent));
  ACE_ASSERT (name != 0);
  CORBA::PolicyList policies (3);

  const char *exception_message = "Null Message";
  policies.length (3);
  try
    {
      // Servant Retention Policy
      exception_message = "While PortableServer::POA::create_servant_retention_policy";
      policies[0] =
        parent->create_servant_retention_policy (PortableServer::NON_RETAIN);

      // Request Processing Policy
      exception_message = "While PortableServer::POA::create_request_processing_policy";

      policies[1] =
        parent->create_request_processing_policy (PortableServer::USE_DEFAULT_SERVANT);
      policies[2] =
        parent->create_id_uniqueness_policy (PortableServer::MULTIPLE_ID);

      PortableServer::POAManager_var poa_manager =
        parent->the_POAManager ();

      exception_message = "While create_POA";
      PortableServer::POA_var child =
        parent->create_POA (name,
                            poa_manager.in (),
                            policies);

      exception_message = "While policy->destroy";
      for (CORBA::ULong i = 0; i < policies.length (); ++i)
        {
          CORBA::Policy_ptr policy = policies[i];
          policy->destroy ();
        }

      exception_message = "While child->the_activator";
      child->the_activator (this);

      exception_message = "While set_servant";
      child->set_servant (this->default_servant_);
    }
  catch (const CORBA::Exception& ex)
    {
      ORBSVCS_ERROR ((LM_ERROR,
                  "IMR_Adapter_Activator::unknown_adapter - %s\n",
                  exception_message));
      ex._tao_print_exception ("System Exception");
      return 0;
    }

  // Finally, now everything is fine
  return 1;
}

