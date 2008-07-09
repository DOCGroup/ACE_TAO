// $Id$

#include "ace/Arg_Shifter.h"
#include "tao/PortableServer/POAManager.h"
#include "tao/PortableServer/PortableServer.h"

#include "PersistentPoa.h"

PersistentPoa::PersistentPoa ( )
{
} /* end of PersistentPoa::PersistentPoa ( ) */

PersistentPoa::~PersistentPoa ( )
  throw ()
{
} /* end of PersistentPoa::~PersistentPoa ( ) */

int PersistentPoa::init (int argc, ACE_TCHAR *argv[])
{
  ACE_Arg_Shifter as (argc, argv);
  m_poaName = std::string (ACE_TEXT_ALWAYS_CHAR (as.get_current ()));
  as.ignore_arg ();

  std::string orbName (ACE_TEXT_ALWAYS_CHAR (as.get_current()));
  as.ignore_arg ();

  while (as.is_anything_left ())
  {
    as.ignore_arg();
  }

  try
  {
    // left out all safety checks
    DllOrb * p_orb = ACE_Dynamic_Service<DllOrb>::instance (orbName.c_str ());
    mv_orb = p_orb->orb ();
    CORBA::Object_var v_poa = mv_orb->resolve_initial_references ("RootPOA");
    mv_rootPOA = PortableServer::POA::_narrow (v_poa.in ());

    // Policies for the new POA
    CORBA::PolicyList policies (3);
    policies.length (3);
    policies[0] = mv_rootPOA->create_thread_policy (PortableServer::ORB_CTRL_MODEL);
    policies[1] = mv_rootPOA->create_lifespan_policy (PortableServer::PERSISTENT);
    policies[2] = mv_rootPOA->create_id_assignment_policy (PortableServer::USER_ID);

    mv_thisPOA = mv_rootPOA->create_POA (
      m_poaName.c_str (),
      PortableServer::POAManager::_nil (),
      policies
    );

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
} /* end of PersistentPoa::init ( ) */


int PersistentPoa::fini (void)
{
  try
  {
    mv_poaManager->deactivate (1, 1);
    mv_poaManager = PortableServer::POAManager::_nil ();

    mv_thisPOA->destroy (1, 1);
    mv_thisPOA = PortableServer::POA::_nil ();
  }
  catch(...)
  {
    ACE_DEBUG ((LM_ERROR, ACE_TEXT ("deactivate failed\n")));
    return -1;
  }

  ACE_DEBUG ((LM_ERROR, ACE_TEXT ("POA deactivated\n")));

  return 0;
} /* end of PersistentPoa::fini ( ) */


ACE_FACTORY_DEFINE (bug_3251, PersistentPoa)
