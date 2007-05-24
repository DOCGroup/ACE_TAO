// $Id$

#include "DomainApplicationManager_ActiveObject_Impl.h"
#include "ace/OS_NS_unistd.h"

CIAO::DomainApplicationManager_ActiveObject_Impl::
DomainApplicationManager_ActiveObject_Impl (
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
CIAO::DomainApplicationManager_ActiveObject_Impl::
startLaunch (const ::Deployment::Properties & configProperty,
             ::CORBA::Boolean start)
{
  CIAO_TRACE("CIAO::DomainApplicationManager_ActiveObject_Impl::startLaunch");
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

          // Create a startLaunch task 
          Task_StartLaunch * task;
          
          ACE_NEW (task, 
                   Task_StartLaunch (this->node_manager_names_[i].c_str (),
                                     my_nam,
                                     configProperty));

          this->start_launch_task_table_.bind (this->node_manager_names_[i],
                                               task);

          // Activate as an active object
          task->activate (THR_NEW_LWP | THR_JOINABLE);
        }

      // Wait for the completion of the task
      start_launch_task_table_.begin ()->int_id_->thr_mgr()->wait ();

      // Collect the results of each startLaunch task
      for (StartLaunch_Task_Table_Iterator iter (this->start_launch_task_table_.begin ());
           iter != this->start_launch_task_table_.end ();
           ++iter)
      {
         ACE_DEBUG ((LM_ERROR, "**********************Table size [%d]\n", 
                     this->start_launch_task_table_.current_size ()));    

         ACE_DEBUG ((LM_ERROR, "**********************Step 2.1\n"));

        // Cache the returned set of connections into the list.
        this->add_connections (iter->int_id_->get_connections ());

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
            iter->int_id_->get_node_app ());

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
        "DomainApplicationManager_ActiveObject_Impl::startLaunch\t\n");
      throw;
    }

}
