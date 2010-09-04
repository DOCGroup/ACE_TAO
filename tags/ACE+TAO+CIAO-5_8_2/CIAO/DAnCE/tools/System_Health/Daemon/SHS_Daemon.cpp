// $Id$

#include "tao/ORB.h"
#include "tao/Object.h"
#include "tao/Utils/ORB_Destroyer.h"
#include "tao/Utils/PolicyList_Destroyer.h"
#include "orbsvcs/CosNamingC.h"
#include "DAnCE/Logger/Log_Macros.h"
#include "DAnCE/Logger/Logger_Service.h"
#include "DAnCE/DAnCE_Utility.h"

#include "SHS_Daemon_Impl.h"

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
                    ACE_TEXT("SHS_Deamon - initializing ORB\n")));

      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      DANCE_DEBUG (6, (LM_TRACE, DLINFO
                       ACE_TEXT("SHS_Deamon - initializing module instance\n")));


      TAO::Utils::ORB_Destroyer safe_orb (orb);
      
      CORBA::Object_var tmp = orb->resolve_initial_references ("NameService");
      CosNaming::NamingContext_var domain_nc = CosNaming::NamingContext::_narrow (tmp);
      
      if (CORBA::is_nil (domain_nc.in ()))
        {
          ACE_ERROR ((LM_EMERGENCY, "SHS_Daemon - Unable to register with the CORBA Naming Service\n"));
          return -1;
        }
      
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
          DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("SHS_Deamon - ")
                           ACE_TEXT("before creating the \"SHS\" POA.\n")));

          policies[0] = poa->create_id_assignment_policy (PortableServer::USER_ID);
          policies[1] = poa->create_lifespan_policy (PortableServer::PERSISTENT);
          persistent_poa = poa->create_POA ("SHS",
                                            mgr.in(),
                                            policies);
        }
      catch (const PortableServer::POA::AdapterAlreadyExists &)
        {
          persistent_poa = poa->find_POA ("Managers", 0);
        }

      DAnCE::SHS_Daemon_i *shs_daemon (0);
      
      ACE_NEW_RETURN (shs_daemon,
                      DAnCE::SHS_Daemon_i (orb.in ()),
                      0);
      
      PortableServer::ServantBase_var safe_servant (shs_daemon);
      PortableServer::ObjectId_var oid =
        PortableServer::string_to_ObjectId ("SHS_Daemon");
      persistent_poa->activate_object_with_id (oid, shs_daemon);

      CORBA::Object_var shs_obj = persistent_poa->id_to_reference (oid.in ());
      CORBA::String_var shs_ior = orb->object_to_string (shs_obj.in ());
      
      DAnCE::Utility::write_IOR (ACE_TEXT ("SHS_Daemon.ior"),
                                 shs_ior.in ());
      
      
      CosNaming::Name name (1);
      name.length (1);
      name[0].id = "DAnCE.SystemHealthDaemon";
      
      domain_nc->rebind (name, shs_obj.in ());
      
      mgr->activate ();
      orb->run ();
      
      DANCE_DEBUG (6, (LM_TRACE, DLINFO
                       ACE_TEXT("SHS_Daemon - destroying ORB\n")));
      
      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("SHS_Daemon");
      retval = -1;
    }
  catch (...)
    {
      DANCE_ERROR (1, (LM_ERROR, "SHS_Daemon - Error: Unknown exception.\n"));
      retval = -1;
    }

  return retval;
}

