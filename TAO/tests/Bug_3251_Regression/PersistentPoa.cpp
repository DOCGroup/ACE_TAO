#include <string>

#include "ace/Arg_Shifter.h"
#include "tao/PortableServer/POAManager.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/Utils/PolicyList_Destroyer.h"

#include "PersistentPoa.h"

PersistentPoa::~PersistentPoa () noexcept = default;

int PersistentPoa::init (int argc, ACE_TCHAR *argv[])
{
  ACE_Arg_Shifter as (argc, argv);
  std::string const poa_name (ACE_TEXT_ALWAYS_CHAR (as.get_current ()));
  as.ignore_arg ();

  std::string const orb_name (ACE_TEXT_ALWAYS_CHAR (as.get_current ()));
  as.ignore_arg ();

  while (as.is_anything_left ())
  {
    as.ignore_arg ();
  }

  try
  {
    // left out all safety checks
    // The service configuration owns this dynamically registered DllOrb.
    DllOrb *const orb_service = ACE_Dynamic_Service<DllOrb>::instance (orb_name.c_str ());
    mv_orb = orb_service->orb ();
    CORBA::Object_var const poa_object = mv_orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var const root_poa = PortableServer::POA::_narrow (poa_object.in ());

    // Policies for the new POA
    TAO::Utils::PolicyList_Destroyer policies (3);
    policies.length (3);
    policies[0] = root_poa->create_thread_policy (PortableServer::ORB_CTRL_MODEL);
    policies[1] = root_poa->create_lifespan_policy (PortableServer::PERSISTENT);
    policies[2] = root_poa->create_id_assignment_policy (PortableServer::USER_ID);

    mv_thisPOA = root_poa->create_POA (
      poa_name.c_str (),
      PortableServer::POAManager::_nil (),
      policies);

    mv_poaManager = mv_thisPOA->the_POAManager ();
    mv_poaManager->activate ();
  }
  catch (...)
  {
    ACE_DEBUG ((LM_ERROR, ACE_TEXT ("activate failed\n")));
    return -1;
  }

  ACE_DEBUG ((LM_ERROR, ACE_TEXT ("POA activated\n")));

  return 0;
}


int PersistentPoa::fini ()
{
  try
  {
    mv_poaManager->deactivate (1, 1);
    mv_poaManager = PortableServer::POAManager::_nil ();

    mv_thisPOA->destroy (1, 1);
    mv_thisPOA = PortableServer::POA::_nil ();
  }
  catch (...)
  {
    ACE_DEBUG ((LM_ERROR, ACE_TEXT ("deactivate failed\n")));
    return -1;
  }

  ACE_DEBUG ((LM_ERROR, ACE_TEXT ("POA deactivated\n")));

  return 0;
}


ACE_FACTORY_DEFINE (bug_3251, PersistentPoa)
