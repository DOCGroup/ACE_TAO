/*
 * $Id$
 */

#include "ace/Arg_Shifter.h"
#include "tao/PortableServer/POAManager.h"
#include "tao/PortableServer/PortableServer.h"

#include "PersistentPOA.h"
#include "DllORB.h"

const ACE_TCHAR *POAname_ = 0;
const ACE_TCHAR *ORBname_ = 0;

PersistentPoa::PersistentPoa ( )
{
} /* end of PersistentPoa::PersistentPoa ( ) */


PersistentPoa::~PersistentPoa ( )
  throw ()
{
} /* end of PersistentPoa::~PersistentPoa ( ) */


int PersistentPoa::init (int argc, ACE_TCHAR *argv[])
{
  int result = 0;

  ACE_Arg_Shifter as(argc, argv);
  POAname_ = as.get_current();
  as.ignore_arg();

  ORBname_ = as.get_current();
  as.ignore_arg();

  while(as.is_anything_left())
  {
    as.ignore_arg();
  }

  try
  {
    DllORB * p_orb = ACE_Dynamic_Service<DllORB>::instance (ORBname_);

    mv_orb = p_orb->orb();

    CORBA::Object_var v_poa = mv_orb->resolve_initial_references("RootPOA");

    mv_rootPOA = PortableServer::POA::_narrow(v_poa.in ());

    if(CORBA::is_nil(mv_rootPOA.in()))
    {
      ACE_DEBUG((
        LM_DEBUG,
        ACE_TEXT ("(%P|%t) init - nil root POA\n")
      ));
      return -1;
    }

    // Policies for the new POA
    CORBA::PolicyList policies(3);
    policies.length (3);

    // Threading policy
    policies[0] = mv_rootPOA->create_thread_policy(PortableServer::ORB_CTRL_MODEL);

    // Lifespan policy
    policies[1] = mv_rootPOA->create_lifespan_policy(PortableServer::PERSISTENT);

    // ID assignment policy
    policies[2] = mv_rootPOA->create_id_assignment_policy(PortableServer::USER_ID);

    mv_thisPOA = mv_rootPOA->create_POA(
      ACE_TEXT_ALWAYS_CHAR(POAname_),
      PortableServer::POAManager::_nil(),
      policies
    );
    if (CORBA::is_nil(mv_thisPOA.in()))
    {
      ACE_DEBUG((
        LM_DEBUG,
        ACE_TEXT ("(%P|%t) init - POA creation failed\n")
      ));
      return -1;
    }
    else
    {
      ACE_DEBUG((
        LM_DEBUG,
        ACE_TEXT ("(%P|%t) init - POA created\n")
      ));
    }

    mv_poaManager = mv_thisPOA->the_POAManager();

    mv_poaManager->activate();
    ACE_DEBUG((
      LM_DEBUG,
      ACE_TEXT ("(%P|%t) init - activated POA manager\n")
    ));
  }
  catch(...)
  {
    ACE_DEBUG((
      LM_DEBUG,
      ACE_TEXT ("(%P|%t) init - exception\n")
    ));
    result = -1;
    return result;
  }

  ACE_DEBUG((
    LM_DEBUG,
    ACE_TEXT ("(%P|%t) init - POA activated\n")
  ));

  return result;
} /* end of PersistentPoa::init ( ) */


int PersistentPoa::fini (void)
{
  int result = 0;

  try
  {
    mv_poaManager->deactivate(1, 1);
    mv_thisPOA->destroy(1, 1);
    mv_thisPOA = PortableServer::POA::_nil();
  }
  catch(...)
  {
    ACE_DEBUG((
      LM_DEBUG,
      ACE_TEXT ("(%P|%t) init - exception\n")
    ));
    result = -1;
    return result;
  }
  ACE_DEBUG((
    LM_DEBUG,
    ACE_TEXT ("(%P|%t) fini - POA deactivated\n")
  ));

  return result;
} /* end of PersistentPoa::fini ( ) */


ACE_FACTORY_DEFINE (bug2936, PersistentPoa)
