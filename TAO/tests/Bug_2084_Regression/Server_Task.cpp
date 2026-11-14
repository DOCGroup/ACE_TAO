#include "Server_Task.h"
#include "TestS.h"
#include "EventNode.h"

#include "ace/Manual_Event.h"
#include "tao/ORB_Core.h"
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
      CORBA::Object_var const poa_object =
        this->sorb_->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();
      PortableServer::ServantBase_var const event_node_impl (
        new EventNode (this->sorb_.in (), ACE_Thread::self ()));
      PortableServer::ObjectId_var const id =
        root_poa->activate_object (event_node_impl.in ());
      CORBA::Object_var const object = root_poa->id_to_reference (id.in ());
      Test::EventNode_var const event_node = Test::EventNode::_narrow (object.in ());

      ACE_DEBUG ((LM_DEBUG,
                  "Server (%t) optimize_collocation_objects=%d use_global_collocation=%d\n",
                  this->sorb_->orb_core ()->optimize_collocation_objects (),
                  this->sorb_->orb_core ()->use_global_collocation ()));

      CORBA::String_var const ior =
        this->sorb_->object_to_string (event_node.in ());

      // Write and close the IOR file before waking the main thread.
      {
        std::unique_ptr<FILE, int (*) (FILE *)> const output_file (
          ACE_OS::fopen (this->output_, "w"), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot open output file for writing IOR: %s",
                             this->output_),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      poa_manager->activate ();

      // Signal the main thread before calling ORB::run.
      this->me_.signal ();
      this->sorb_->run ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));
    }
  catch (CORBA::BAD_INV_ORDER const &)
    {
      // Periodically we get a bad inv order on fast machines. This false negative is safe to ignore.
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
