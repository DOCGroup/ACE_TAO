#include "Server_Task.h"
#include "TestS.h"
#include "Hello.h"

#include "ace/Manual_Event.h"
#include <memory>

Server_Task::Server_Task (std::basic_string<ACE_TCHAR> const &output,
                          CORBA::ORB_ptr sorb,
                          ACE_Manual_Event &me,
                          ACE_Thread_Manager *const thr_mgr)
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
      CORBA::Object_var const poa_object {
        this->sorb_->resolve_initial_references ("RootPOA")};

      PortableServer::POA_var const root_poa {PortableServer::POA::_narrow (poa_object.in ())};

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var const poa_manager {root_poa->the_POAManager ()};

      HelloAgain *hello_impl {};
      ACE_NEW_RETURN (hello_impl,
                      HelloAgain (this->sorb_.in (), ACE_Thread::self ()),
                      1);
      // The servant var owns the implementation while the POA uses it.
      PortableServer::ServantBase_var owner_transfer (hello_impl);

      PortableServer::ObjectId_var const id {root_poa->activate_object (hello_impl)};

      CORBA::Object_var const object {root_poa->id_to_reference (id.in ())};

      Test::HelloAgain_var const hello {Test::HelloAgain::_narrow (object.in ())};

      CORBA::String_var const ior {this->sorb_->object_to_string (hello.in ())};

      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (this->output_.c_str (), ACE_TEXT ("w")), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                             this->output_.c_str ()),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      poa_manager->activate ();

      // Signal the main thread before we call orb->run ();
      this->me_.signal ();

      this->sorb_->run ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
