#include "TM_Daemon.h"
#include "Config_Handlers/XML_File_Intf.h"
#include "Client_Task.h"
#include "tools/Config_Handlers/DnC_Dump.h"
namespace CIAO
{
  namespace TM_Daemon
  {


    Daemon_impl::Daemon_impl (CORBA::ORB_ptr orb,
			      vector<string> deployment_plans,
                              const char *mapping,
                              const char *domain,
                              ::PortableServer::POA_ptr poa,
                              size_t threads)
      : orb_ (CORBA::ORB::_duplicate (orb)),
        thread_count_ (threads)

    {

      ACE_DEBUG ((LM_DEBUG, "Making call to intf \n"));
      ACE_DEBUG ((LM_DEBUG, "After call to get plan \n"));

      // Now create the manager.
      this->manager_ = new DomainDataManager (orb, mapping, domain, deployment_plans);

      // Create the AMI handler providing it with the object reference of
      // the manager.
      Monitor_Handler_I * handler_i = new  Monitor_Handler_I (this->manager_);
      PortableServer::ObjectId_var id = poa->activate_object (handler_i);
      CORBA::Object_var object = poa->id_to_reference (id.in ());
      this->handler_  = Onl_Monitor::AMI_NM_MonitorHandler::_narrow (object.in ());

      //      this->manager_->start_monitor_qos (this->handler_);
      //      ACE_DEBUG ((LM_DEBUG, "Created the Daemon!\n"));

    }

    Daemon_impl::~Daemon_impl ()
    {
      //delete this->manager_;
    }

    int
    Daemon_impl::run ()
    {
      // Now active the threads that run the orb.
      Client_Task client_task (this->orb_.in ());
      // Spawn multiple threads to run the orb event loop.
      if (client_task.activate (THR_NEW_LWP | THR_JOINABLE,
                                this->thread_count_) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot activate ORB threads\n"), 1);
        }
      ACE_DEBUG ((LM_DEBUG, "ORB threads have been activated, now waiting "
                  "for incoming requests.\n"));
      // Wait for all the threads to complete execution.
      client_task.thr_mgr ()->wait ();
      return 0;
    }

    ::Deployment::Domain*
    Daemon_impl::getInitialDomain ()
    {
      ACE_DEBUG ((LM_DEBUG, "In getInitialDomain!\n"));
      ::Deployment::Domain_var domain =
          new ::Deployment::Domain (this->manager_->initial_domain ());
      return domain._retn ();
    }


    ::Deployment::Domain*
    Daemon_impl::getSnapshot ()
    {
      ACE_DEBUG ((LM_DEBUG, "In getSnapshot!\n"));
//       ::Deployment::Domain_var domain = new
//           ::Deployment::Domain ();
      ::Deployment::Domain_var domain = new
          ::Deployment::Domain (this->manager_->get_all_data (this->handler_));

//       std::map<std::string, ::Deployment::Node> temp =
//         this->manager_->get_all_data (this->handler_);

//       domain->node.length (temp.size ());
//       std::map<std::string, ::Deployment::Node>::iterator itr = temp.begin ();
//       CORBA::ULong i = 0;
//       for (;itr != temp.end (); ++itr, ++i)
//         {
//           domain->node [i] = (*itr).second;
//         }
      return domain._retn ();
    }

    void
    Daemon_impl::shutdown ()
    {
      this->orb_->shutdown (0);
    }
  }
}
