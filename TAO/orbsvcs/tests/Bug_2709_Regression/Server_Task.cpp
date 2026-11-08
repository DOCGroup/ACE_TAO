#include "ace/OS_NS_unistd.h"
#include "TestImpl.h"
#include "Server_Task.h"

#include <memory>

Server_Task::Server_Task (ACE_TCHAR const *output,
                          CORBA::ORB_ptr sorb,
                          ACE_Manual_Event &me,
                          ACE_Thread_Manager *thr_mgr)
  : ACE_Task_Base (thr_mgr)
  , output_ (output)
  , me_ (me)
  , sorb_ (CORBA::ORB::_duplicate (sorb))
{
}

int
Server_Task::svc ()
{
  try
    {
      CORBA::Object_var poa_object (
        sorb_->resolve_initial_references ("RootPOA"));

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, " (%P|%t) Unable to initialize the POA.\n"), 1);

      PortableServer::POA_var root_poa (
        PortableServer::POA::_narrow (poa_object.in ()));

      PortableServer::POAManager_var poa_manager (root_poa->the_POAManager ());

      poa_manager->activate ();

      TestImpl *server_impl {};
      ACE_NEW_RETURN (server_impl, TestImpl (sorb_.in ()), 1);
      PortableServer::ServantBase_var servant (server_impl);

      PortableServer::ObjectId_var id (root_poa->activate_object (servant.in ()));

      CORBA::Object_var test_obj (root_poa->id_to_reference (id.in ()));
      CORBA::String_var ior (sorb_->object_to_string (test_obj.in ()));

      // Output the IOR to the <ior_output_file>
      {
        std::unique_ptr<FILE, decltype (&ACE_OS::fclose)> output_file (
          ACE_OS::fopen (output_, "w"), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR, "Cannot open IOR output file.\n"), 1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
        ACE_DEBUG ((LM_DEBUG, "(%P): Server's IOR was written to file: %s.\n", output_));
      }

      // Signal the main thread before we call orb->run ();
      this->me_.signal ();

      this->sorb_->run ();

      ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));

      root_poa->destroy (true, true);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught in server task:");
      return 1;
    }

  return 0;
}
