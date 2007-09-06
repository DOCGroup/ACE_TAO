#include "DomainDataManager.h"
#include "Handler_i.h"
#include "Config_Handlers/XML_File_Intf.h"
#include "Config_Handlers/DnC_Dump.h"

namespace CIAO
{
  namespace TM_Daemon
  {

    int
    run_main (int argc, char *argv[])
    {

      try
        {
          CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

          CORBA::Object_var poa_object =
            orb->resolve_initial_references ("RootPOA");

          PortableServer::POA_var poa =
            PortableServer::POA::_narrow (poa_object.in ());

          PortableServer::POAManager_var poa_manager =
            poa->the_POAManager ();

          poa_manager->activate ();

          // create the Domain data Manager
          DomainDataManager* manager = 
		  DomainDataManager::create (orb, NULL, argv[2]);

          sleep (10); // wait for all the monitors to upload their obj. refs

          manager->get_monitor_obj_ref ();

          ACE_DEBUG ((LM_DEBUG, "After get_monitor_obj_ref\n"));

          Monitor_Handler_I handler_i (manager);

//          PortableServer::ServantBase_var servant =
//              handler_i;

          Onl_Monitor::AMI_NM_MonitorHandler_var handlerV =
            handler_i._this ();

          ACE_DEBUG ((LM_DEBUG, "Making call to intf \n"));

          CIAO::Config_Handlers::XML_File_Intf intf (argv[1]);

          ::Deployment::DeploymentPlan_var plan = intf.get_plan ();

          ACE_DEBUG ((LM_DEBUG, "After call to get plan \n"));

          manager->start_monitor_qos (handlerV, plan);
	
	  //sleep (100);

          while (true)
          {
            sleep (5);
            ACE_DEBUG ((LM_DEBUG, "Calling get_all_data\n"));
            manager->get_all_data (handlerV);

            while (!manager->got_all_response ())
            {
              if (orb->work_pending ())
                orb->perform_work ();
            }

            // write the system snapshot
            manager->write_snapshot ();
          }
          // Finally destroy the ORB
          orb->run ();
          orb->destroy ();
        }
      catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("CIAO_TM_Daemon::main\n");
        return -1;
      }

      ACE_DEBUG ((LM_DEBUG,
            "CIAO_TM_Daemon has closed\n"));
      return 0;
    }
  }
}

int
main (int argc, char *argv[])
{
  return CIAO::TM_Daemon::run_main (argc, argv);
}
