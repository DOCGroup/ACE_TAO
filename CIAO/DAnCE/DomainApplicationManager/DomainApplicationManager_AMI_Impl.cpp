// $Id$

#include "DomainApplicationManager_AMI_Impl.h"
#include "DAnCE/Deployment/Deployment_NodeApplicationManagerC.h"
#include "DAnCE/Deployment/Deployment_ApplicationC.h"
#include "Reply_Handler_i.h"

CIAO::DomainApplicationManager_AMI_Impl::
DomainApplicationManager_AMI_Impl (
  CORBA::ORB_ptr orb,
  PortableServer::POA_ptr poa,
  Deployment::TargetManager_ptr manager,
  Execution_Manager::Execution_Manager_Impl *em,
  const Deployment::DeploymentPlan &plan,
  const char * deployment_file)
  : DomainApplicationManager_Impl (orb, poa, manager, em, plan, deployment_file)
{
}

void
CIAO::DomainApplicationManager_AMI_Impl::
startLaunch (const ::Deployment::Properties & configProperty,
             ::CORBA::Boolean start)
{
  CIAO_TRACE("CIAO::DomainApplicationManager_AMI_Impl::startLaunch");
  ACE_UNUSED_ARG (start);

  try
    {
      // Clean up all cached connections first
      this->all_connections_->length (0);

      // Invoke startLaunch() operations on each cached NodeApplicationManager
      for (CORBA::ULong i = 0; i < this->num_child_plans_; ++i)
        {
          // Get the NodeApplicationManager object reference.
          ACE_Hash_Map_Entry
            <ACE_CString,
            Chained_Artifacts> *entry = 0;

          if (this->artifact_map_.find (this->node_manager_names_[i],
                                        entry) != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "DAnCE (%P|%t) DomainApplicationManager_Impl.cpp -"
                          "CIAO::DomainApplicationManager_Impl::startLaunch -"
                          "ERROR while finding the node specific plan "
                          "for the node [%s] \n",
                          this->node_manager_names_[i].c_str ()));

              ACE_CString error
                 ("Unable to resolve a reference to node manager: ");
              error += this->node_manager_names_[i];

              throw
                 (Deployment::StartError
                    ("DomainApplicationManager_Impl:startLaunch",
                     error.c_str ()));
            }

          ::Deployment::NodeApplicationManager_ptr my_nam =
            (entry->int_id_).node_application_manager_.in ();

          if (CORBA::is_nil (my_nam))
            {
              ACE_ERROR ((LM_ERROR,
                          "DAnCE (%P|%t) DomainApplicationManager_Impl.cpp -"
                          "CIAO::DomainApplicationManager_Impl::startLaunch -"
                          "ERROR while narrowing the NAM "
                          "for the node [%s] \n",
                          this->node_manager_names_[i].c_str ()));

              ACE_CString error
                 ("While starting launch, the DomainApplicationManager\
                  has a nil reference for NodeApplicationManager\n");
              ACE_DEBUG ((LM_DEBUG, error.c_str ()));

              throw
                (Deployment::StartError
                   ("DomainApplicationManager_Impl::startLaunch",
                     error.c_str ()));
            }

          // Create a reply-handler servant
          Deployment_AMI_NodeApplicationManagerHandler_i *ami_handler_servant = 0;

          ACE_NEW_THROW_EX (
            ami_handler_servant,
            Deployment_AMI_NodeApplicationManagerHandler_i (),
            CORBA::NO_MEMORY ());

          PortableServer::ObjectId_var oid = 
            this->poa_->activate_object (ami_handler_servant);

          CORBA::Object_var handler_obj = poa_->id_to_reference (oid.in ());
          
          Deployment::AMI_NodeApplicationManagerHandler_var replyHandler=
            Deployment::AMI_NodeApplicationManagerHandler::_narrow (handler_obj.in ());

          // Invoke operation asynchronously
          my_nam->sendc_startLaunch (replyHandler.in (), configProperty, 0);

          while (1)
          {
            // Check whether the reply has been returned
            if (this->orb_->work_pending ())
              {
                this->orb_->perform_work ();

                if (! CORBA::is_nil (ami_handler_servant->get_node_app ()))
                  break;
              }
          }

          // Cache the returned set of connections into the list.
          this->add_connections (ami_handler_servant->get_connections ());

          // Cache the returned NodeApplication object reference into
          // the hash table.
          (entry->int_id_).node_application_ = 
            Deployment::NodeApplication::_duplicate (ami_handler_servant->get_node_app ());
        }
    }
  catch (const Deployment::StartError& ex)
    {
      ACE_DEBUG ((LM_DEBUG, "DAM_Impl:StartError: %s, %s\n",
      ex.name.in (),
      ex.reason.in ()));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "DomainApplicationManager_Impl::startLaunch\t\n");
      throw;
    }

}
