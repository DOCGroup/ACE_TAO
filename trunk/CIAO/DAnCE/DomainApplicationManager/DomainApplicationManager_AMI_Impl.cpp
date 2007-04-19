// $Id$

#include "DomainApplicationManager_AMI_Impl.h"
#include "ace/OS_NS_unistd.h"

CIAO::DomainApplicationManager_AMI_Impl::
DomainApplicationManager_AMI_Impl (
  CORBA::ORB_ptr orb,
  PortableServer::POA_ptr poa,
  Deployment::TargetManager_ptr manager,
  Execution_Manager::Execution_Manager_Impl *em,
  const Deployment::DeploymentPlan &plan,
  const char * deployment_file)
  : DomainApplicationManager_Impl (orb, poa, manager, em, plan, deployment_file),
  start_launch_reply_count_ (0)
{
}

void
CIAO::DomainApplicationManager_AMI_Impl::
decrease_start_launch_reply_count ()
{
  -- this->start_launch_reply_count_;
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

      this->start_launch_reply_count_ = this->num_child_plans_;

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
          AMI_NAM_Handler ami_nam_handler;

          ACE::Value_Ptr <Deployment_AMI_NodeApplicationManagerHandler_i> 
            handler_ptr (new Deployment_AMI_NodeApplicationManagerHandler_i (this));

          PortableServer::ObjectId_var oid = 
            this->poa_->activate_object (&(*handler_ptr));

          CORBA::Object_var handler_obj = poa_->id_to_reference (oid.in ());
          
          ami_nam_handler.obj_ref_ =
            Deployment::AMI_NodeApplicationManagerHandler::_narrow (handler_obj.in ());

          this->ami_nam_handler_table_.bind (this->node_manager_names_[i],
                                             ami_nam_handler);

          // Invoke operation asynchronously
          my_nam->sendc_startLaunch (ami_nam_handler.obj_ref_.in (), 
                                     configProperty, 0);
        }

      ACE_DEBUG ((LM_ERROR, "******************Step 1\n"));
      //while (start_launch_reply_count_ > 0)
      while (true)
        {
         //ACE_DEBUG ((LM_ERROR, "**********************Step 1.1\n"));

         //ACE_DEBUG ((LM_ERROR, "**********************Count = [%d]\n", 
         //            this->start_launch_reply_count_));

          // Check whether the reply has been returned
          if (this->orb_->work_pending ())
            {
              this->orb_->perform_work ();

              if (this->start_launch_reply_count_ == 0)
                break;
            }

         //ACE_DEBUG ((LM_ERROR, "**********************Step 1.2\n"));
          //  ACE_OS::sleep (5);
        }

      ACE_DEBUG ((LM_ERROR, "******************Step 2\n"));
      for (AMI_NAM_Handler_Table_Iterator iter (this->ami_nam_handler_table_.begin ());
           iter != this->ami_nam_handler_table_.end ();
           ++iter)
      {
         ACE_DEBUG ((LM_ERROR, "**********************Map size [%d]\n", 
                     this->ami_nam_handler_table_.current_size ()));    

         ACE_DEBUG ((LM_ERROR, "**********************Step 2.1\n"));

        // Cache the returned set of connections into the list.
        this->add_connections (iter->int_id_.servant_->get_connections ());

        // Cache the returned NodeApplication object reference into
        // the chained artifact table.
        ACE_Hash_Map_Entry
          <ACE_CString,
          Chained_Artifacts> *entry = 0;

         ACE_DEBUG ((LM_ERROR, "**********************Step 2.2\n"));
        this->artifact_map_.find (iter->ext_id_, // node name
                                  entry);

         ACE_DEBUG ((LM_ERROR, "**********************Step 2.3\n"));
        (entry->int_id_).node_application_ = 
          Deployment::NodeApplication::_duplicate (
            iter->int_id_.servant_->get_node_app ());

         ACE_DEBUG ((LM_ERROR, "**********************Step 2.4\n"));
      }

      ACE_DEBUG ((LM_ERROR, "******************Step 3\n"));
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
