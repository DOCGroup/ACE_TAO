// $Id$

#include "tao/ORB.h"
#include "tao/Object.h"
#include "tao/Utils/ORB_Destroyer.h"
#include "tao/Utils/PolicyList_Destroyer.h"
#include "DAnCE/Logger/Log_Macros.h"
#include "DAnCE/Logger/Logger_Service.h"
#include "DAnCE/DAnCE_Utility.h"

#include "PL_Daemon_Impl.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  DANCE_DISABLE_TRACE ();

  int retval = 0;

  try
    {
      DAnCE::Logger_Service * dlf =
        ACE_Dynamic_Service<DAnCE::Logger_Service>::instance ("DAnCE_Logger");

      if (dlf)
        {
          dlf->init (argc, argv);
        }

      DANCE_DEBUG (6, (LM_TRACE, DLINFO
                    ACE_TEXT("PL_Daemon - initializing ORB\n")));

      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      DANCE_DEBUG (6, (LM_TRACE, DLINFO
                       ACE_TEXT("PL_Daemon - initializing module instance\n")));


      TAO::Utils::ORB_Destroyer safe_orb (orb);

      CORBA::Object_var poa_obj
        = orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var poa
        = PortableServer::POA::_narrow (poa_obj.in ());

      PortableServer::POAManager_var mgr = poa->the_POAManager ();
      PortableServer::POA_var persistent_poa;
      TAO::Utils::PolicyList_Destroyer policies (2);
      policies.length (2);
      try
        {
          DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("PL_Daemon - ")
                           ACE_TEXT("before creating the \"Managers\" POA.\n")));

          policies[0] = poa->create_id_assignment_policy (PortableServer::USER_ID);
          policies[1] = poa->create_lifespan_policy (PortableServer::PERSISTENT);
          persistent_poa = poa->create_POA ("Managers",
                                            mgr.in(),
                                            policies);
        }
      catch (const PortableServer::POA::AdapterAlreadyExists &)
        {
          persistent_poa = poa->find_POA ("Managers", 0);
        }

      DAnCE::Plan_Launcher_Daemon_i *pl_daemon (0);
      
      ACE_NEW_RETURN (pl_daemon,
                      DAnCE::Plan_Launcher_Daemon_i (orb.in ()),
                      0);
      
      PortableServer::ServantBase_var safe_servant (pl_daemon);
      
      PortableServer::ObjectId_var oid =
        PortableServer::string_to_ObjectId ("Plan_Launcher_Daemon");
      persistent_poa->activate_object_with_id (oid, pl_daemon);
      
      CORBA::Object_var pl_obj = persistent_poa->id_to_reference (oid.in ());
      CORBA::String_var pl_ior = orb->object_to_string (pl_obj.in ());
      
      DAnCE::Utility::write_IOR (ACE_TEXT ("PL_Daemon.ior"),
                                 pl_ior.in ());
      
      orb->run ();
      
      DANCE_DEBUG (6, (LM_TRACE, DLINFO
                       ACE_TEXT("PL_Daemon - destroying ORB\n")));
      
      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("PL_Daemon");
      retval = -1;
    }
  catch (...)
    {
      DANCE_ERROR (1, (LM_ERROR, "PL_Daemon - Error: Unknown exception.\n"));
      retval = -1;
    }

  return retval;
}

