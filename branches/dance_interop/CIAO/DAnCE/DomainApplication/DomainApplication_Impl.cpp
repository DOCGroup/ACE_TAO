// -*- C++ -*-
//
// $Id$


#include "DomainApplicationI.h"

// Implementation skeleton constructor
DomainApplication_i::DomainApplication_i (Deployment::NodeApplications_var &nas,
                                          Deployment::Conenctions_var &conns)
  : nas_ (Deployment::NodeApplications::_duplicate (nas)),
    conns_ (Deployment::Connections::_duplicate (conns)
{
}

// Implementation skeleton destructor
DomainApplication_i::~DomainApplication_i (void)
{
}

void DomainApplication_i::finishLaunch (const ::Deployment::Connections & providedReference,
                                        ::CORBA::Boolean start)
  throw (::CORBA::SystemException,
         ::Deployment::StartError,
         ::Deployment::InvalidConnection)

{
  CIAO_TRACE("CIAO::DomainApplicationManager_Impl::finishLaunch");
  try
    {
      // Invoke finishLaunch() operation on each cached NodeApplication object.
      // This will establish bindings for only those internal components, but
      // NOT for those external/shared components, which requires special
      // handling, since these components are outside the control of this
      // DomainApplicationManager.
      for (CORBA::ULong i = 0; i < this->num_child_plans_; ++i)
        {

          // Get the Connections variable, if ReDaC is true, then we get
          // those new connections only. NOTE: get_outgoing_connections
          // by default will get *all* connections.
          Deployment::Connections * my_connections =
            this->get_outgoing_connections (
              (entry->int_id_).child_plan_.in (),
              !is_ReDaC,
              true,  // we search *new* plan
              DomainApplicationManager_Impl::Internal_Connections);

          if (my_connections == 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "DAnCE (%P|%t) DomainApplicationManager_Impl.cpp -"
                          "CIAO::DomainApplicationManager_Impl::finishLaunch -"
                          "ERROR while getting the outgoing connections "
                          "for the node [%s] \n",
                          this->node_manager_names_[i].c_str ()));
              
              throw Deployment::StartError
                ("DomainApplicationManager_Impl::finish_launch",
                   "There was some error establishing connections."));
            }

          // Dump the connections for debug purpose.
          if (CIAO::debug_level () > 1)
            {
              ACE_DEBUG ((LM_DEBUG,
                  "==============================================\n"));
              ACE_DEBUG ((LM_DEBUG,
                  "dump outgoing connections for child plan:%s\n",
                 (entry->int_id_).child_plan_->UUID.in ()));
              dump_connections (*my_connections);
              ACE_DEBUG ((LM_DEBUG,
                  "==============================================\n"));
            }

          // Invoke finishLaunch() operation on NodeApplication.
          if (my_connections->length () != 0)
            {
              entry->int_id_.node_application_->finishLaunch
                 (*my_connections,
                  start,
                  true);  // "true" => establish new connections only);
            }
        }
    }
  catch (Deployment::StartError &ex)
    {
      ACE_ERROR ((LM_ERROR, "DAM_Impl::finishLaunch - StartError: %s, %s\n",
      ex.name.in (),
      ex.reason.in ()));
      throw;
   }
  catch (CORBA::SystemException &ex)
    {
      CIAO_PRINT_EXCEPTION (1, ex, "DomainApplication_Impl::FinishLaunch\n");
      
      // Invoke destroyManager() operation on each cached
      // NodeManager object.
      for (CORBA::ULong i = 0; i < this->num_child_plans_; ++i)
        {
          // Get the NodeManager and NodeApplicationManager object references.
          ACE_Hash_Map_Entry <ACE_CString, Chained_Artifacts> * entry = 0;
          this->artifact_map_.find (this->node_manager_names_[i],entry);

          ::Deployment::NodeApplicationManager_ptr
              my_node_application_manager =
                  (entry->int_id_).node_application_manager_.in ();

          // Invoke destoryApplication() operation on the NodeApplicationManger.
          // Since we have the first arg is not used by NAM anyway.
          my_node_application_manager->destroyApplication (0);
        }
      throw;
    }
}

void DomainApplication_i::start (void)
  throw (::CORBA::SystemException,
         ::Deployment::StartError)
{
  // Add your implementation here
}


