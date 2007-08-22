// $Id$

#include "test_i.h"
#include "tao/IORTable/IORTable.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Thread_Manager.h"
#include "ace/Task.h"

class ORB_Run_Task : public ACE_Task_Base
{
private:
  CORBA::ORB_var orb_;

public:
  ORB_Run_Task (CORBA::ORB_ptr orb)
    : orb_(CORBA::ORB::_duplicate(orb))
  {
  }

  int svc (void)
  {
    try
      {
        this->orb_->run();
      }
    catch (CORBA::Exception &ex)
      {
        ACE_DEBUG ((LM_DEBUG,
                    "(%P|%t) server orb run thread caught %s\n",ex._name()));
      }
    return 0;
  }

};

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      int nthreads = 10;
      const char *ior_file = "test.ior";
      bool endpoint_found = false;
      for (int i = 0; i < argc; i++)
        {
          if ((ACE_OS::strcasecmp (argv[i],"-ORBEndpoint") == 0) ||
              (ACE_OS::strcasecmp (argv[i],"-ORBListenEndpoints") == 0))
            endpoint_found = 1;
          else if (ACE_OS::strcasecmp (argv[i],"-threads") == 0)
            {
              ++i;
              nthreads = ACE_OS::atoi (argv[i]);
            }
          else if (ACE_OS::strcasecmp (argv[i],"-o") == 0)
            {
              ++i;
              ior_file = argv[i];
            }
        }

      if (!endpoint_found)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "ERROR: %s requires an explicit endpoint!\n",
                           argv[0]), 1);
      ACE_DEBUG ((LM_DEBUG,"(%P|%t) server started\n"));
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var obj =
        orb->resolve_initial_references("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      obj = orb->resolve_initial_references("IORTable");

      IORTable::Table_var iortable =
        IORTable::Table::_narrow(obj.in());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil IORTable\n"),
                          1);

      ORB_Killer killer (orb.in());
      test_i *servant;
      ACE_NEW_RETURN (servant,
                      test_i (&killer),
                      1);
      PortableServer::ServantBase_var owner_transfer(servant);

      PortableServer::ObjectId_var id =
        root_poa->activate_object (servant);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      Test::IORTable_Shutdown_Race_var target =
        Test::IORTable_Shutdown_Race::_narrow (object.in ());

      CORBA::String_var ior =
        orb->object_to_string (target.in ());

      iortable->bind ("Racer",ior.in());

      PortableServer::POAManager_var poa_manager = root_poa->the_POAManager ();

      poa_manager->activate ();

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) server - activating ORB threads\n"));

      ORB_Run_Task ort (orb.in());
      ort.activate (THR_NEW_LWP | THR_JOINABLE, 3);
      ACE_DEBUG ((LM_DEBUG,"(%P|%t) server - ORB running\n"));


      // Output the IOR to the <ior_file> - to signal readiness.
      FILE *output_file= ACE_OS::fopen (ior_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s\n",
                           ior_file),
                           1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      ACE_Thread_Manager *tm = ort.thr_mgr();
      tm->wait();
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

      orb->destroy ();
    }
  catch (CORBA::Exception &ex)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server main - caught %s\n",ex._name()));
      return 1;
    }

  return 0;
}
