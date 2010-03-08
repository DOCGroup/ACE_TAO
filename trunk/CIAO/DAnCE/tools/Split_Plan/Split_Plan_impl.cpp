// $Id$

#include "Split_Plan_impl.h"
#include "DAnCE/Logger/Log_Macros.h"

namespace DAnCE
{
  Split_Plan::Split_Plan (const Deployment::DeploymentPlan &plan,
                          TNodePlans &sub_plans)
    : plan_ (plan),
      sub_plans_ (sub_plans)
  {
  }
  void
  Split_Plan::split_plan ()
  {
    DANCE_TRACE ( "Split_Plan::split_plan");
    
    const Deployment::DeploymentPlan &plan = this->plan_;
    TNodePlans &sub_plans = this->sub_plans_;

    DANCE_DEBUG (9, (LM_TRACE, ACE_TEXT("Split_Plan::split_plan - ")
                     ACE_TEXT("Creating sub-plans\n")));
    // Create empty sub-plans
    for (CORBA::ULong i = 0; i < plan.instance.length(); ++i)
      {
        ACE_CString node = plan.instance[i].node.in();
        if (0 == sub_plans.find (node)) continue;

        ::Deployment::DeploymentPlan tmp_plan;

        // Construct the UUID for the child plan.
        // Currently, this UUID consists of:
        // The global UUID for the parent plan + the name of the node the
        // child plan is to be installed on.
        ACE_CString child_uuid (plan.UUID.in ());
        //      child_uuid += "@"; CCM#61
        //      child_uuid += node;

        tmp_plan.UUID = CORBA::string_dup (child_uuid.c_str());
        tmp_plan.implementation.length (0);
        tmp_plan.instance.length (0);
        tmp_plan.connection.length (0);
        tmp_plan.externalProperty.length (0);
        tmp_plan.dependsOn.length (0);
        tmp_plan.artifact.length (0);

        // @@ There is an optimization point here, since we really don't have to
        // pass the entire CIAOServerResources into each individual child plan.
        tmp_plan.infoProperty = plan.infoProperty;
        
        CORBA::ULong num_localities (plan.localityConstraint.length ());
        Deployment::PlanLocalities default_loc (num_localities);
        default_loc.length (num_localities);
        
        for (CORBA::ULong i = 0; i < num_localities; ++i)
          {
            default_loc[i].constraint = Deployment::PlanNoConstraint;
          }
                            
        tmp_plan.localityConstraint = default_loc;

        sub_plans.bind (node, tmp_plan);

        DANCE_DEBUG (9, (LM_TRACE, DLINFO
                         ACE_TEXT("Split_Plan::split_plan - ")
                         ACE_TEXT("Created sub-plan for node %C with UUID %C\n"),
                         node.c_str (), child_uuid.c_str ()));
      }

    DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                     ACE_TEXT("Split_Plan::split_plan - ")
                     ACE_TEXT("First phase produced %u child plans, proceeding to second\n"),
                     sub_plans.current_size ()));

    // (1) Iterate over the <instance> field of the global DeploymentPlan
    //     structure.
    // (2) Retrieve the necessary information to contruct the node-level
    //     plans one by one.
    for (CORBA::ULong i = 0; i < plan.instance.length (); ++i)
      {
        DANCE_DEBUG (10, (LM_TRACE, DLINFO
                         ACE_TEXT("Split_Plan::split_plan - ")
                         ACE_TEXT("Processing instance: %C\n"), plan.instance[i].name.in()));
        // @@TODO Fill in the child deployment plan in the map.
        // If the component instance already exists in the child plan,
        // then we overwrite the existing instance, since the new instance
        // might have different resource usage requirements.

        // Get the instance deployment description
        const ::Deployment::InstanceDeploymentDescription & my_instance = plan.instance[i];

        // Get the child plan.
        ::Deployment::DeploymentPlan child_plan;

        if (0 != sub_plans.find (my_instance.node.in(), child_plan))
          {
            DANCE_ERROR (1, (LM_ERROR, DLINFO
                             ACE_TEXT("Split_Plan::split_plan - ")
                             ACE_TEXT("ERROR: Unable to find sub-plan for instance %C\n"),
                             my_instance.node.in ()));
          }

        // Fill in the contents of the child plan entry.

        // Append the "MonolithicDeploymentDescriptions implementation"
        // field with a new "implementation", which is specified by the
        // <implementationRef> field of <my_instance> entry.  NOTE: The
        // <artifactRef> field needs to be changed accordingly.
        ::Deployment::MonolithicDeploymentDescription const & my_implementation
            = plan.implementation[my_instance.implementationRef];

        CORBA::ULong index_imp = child_plan.implementation.length ();
        child_plan.implementation.length (++index_imp);
        child_plan.implementation[index_imp-1] = my_implementation;

        // Initialize with the correct sequence length.
        CORBA::ULongSeq ulong_seq;
        ulong_seq.length (my_implementation.artifactRef.length ());

        // Append the "ArtifactDeploymentDescriptions artifact" field
        // with some new "artifacts", which is specified by the
        // <artifactRef> sequence of <my_implementation> entry.
        CORBA::ULong const impl_length = my_implementation.artifactRef.length ();
        for (CORBA::ULong iter = 0;
             iter < impl_length;
             iter ++)
          {
            CORBA::ULong artifact_ref = my_implementation.artifactRef[iter];

            // Fill in the <artifact> field of the child plan
            CORBA::ULong index_art = child_plan.artifact.length ();
            child_plan.artifact.length (++index_art);
            child_plan.artifact[index_art-1] = plan.artifact[artifact_ref];

            // Fill in the artifactRef field of the MonolithicDeploymentDescription
            ulong_seq[iter] = index_art - 1;
          }

        // Change the <artifactRef> field of the "implementation".
        child_plan.implementation[index_imp-1].artifactRef = ulong_seq;

        // Append the "InstanceDeploymentDescription instance" field with
        // a new "instance", which is almost the same as the "instance" in
        // the global plan except the <implementationRef> field.
        // NOTE: The <implementationRef> field needs to be changed accordingly.
        CORBA::ULong index_ins = child_plan.instance.length ();
        child_plan.instance.length (++index_ins);
        child_plan.instance[index_ins-1] = my_instance;

        // Change the <implementationRef> field of the "instance".
        // @@ The implementationRef starts from 0.
        // @@TODO: Below logic needs to be changed once we want to avoid
        // duplicate <implementation> for the optimization.
        child_plan.instance[index_ins-1].implementationRef = index_imp - 1;

        DANCE_DEBUG (10, (LM_TRACE, DLINFO
                         ACE_TEXT("Split_Plan::split_plan - ")
                         ACE_TEXT("Processing connections.\n")));
        // Copy connections
        for (CORBA::ULong j = 0; j < plan.connection.length(); ++j)
          {
            DANCE_DEBUG (10, (LM_TRACE, DLINFO
                             ACE_TEXT("Split_Plan::split_plan - ")
                             ACE_TEXT("For connection: %C\n"), plan.connection[j].name.in()));
            for (CORBA::ULong k = 0; k < plan.connection[j].internalEndpoint.length(); ++k)
              {
                DANCE_DEBUG (10, (LM_TRACE, DLINFO
                                 ACE_TEXT("Split_Plan::split_plan - ")
                                 ACE_TEXT("For endpoint: %C(%C)\n"),
                                 plan.connection[j].internalEndpoint[k].portName.in(),
                                 plan.connection[j].internalEndpoint[k].provider ? "provider" : "client"));
                if (i == plan.connection[j].internalEndpoint[k].instanceRef)   // the instance (i) is referenced by the connection
                  {
                    Deployment::PlanConnectionDescription * connection_copied = 0;
                    for (CORBA::ULong m = 0; m < child_plan.connection.length(); ++m)
                      {
                        if (ACE_OS::strcmp (plan.connection[j].name.in (), child_plan.connection[m].name.in ()) == 0)
                          {
                            connection_copied = &child_plan.connection[m];
                            break;
                          }
                      }

                    if (!connection_copied)
                      {
                        // Copy the connection
                        CORBA::ULong const index_con = child_plan.connection.length();
                        child_plan.connection.length (index_con + 1);
                        child_plan.connection[index_con] = plan.connection[j];
                        connection_copied = &child_plan.connection[index_con];
                        connection_copied->internalEndpoint.length (0);
                      }

                    // Copy the endpoint
                    CORBA::ULong const index_ep = connection_copied->internalEndpoint.length();

                    DANCE_DEBUG (10, (LM_TRACE, DLINFO
                                     ACE_TEXT("Split_Plan::split_plan - ")
                                     ACE_TEXT ("Copying endpoint %u from connection into endpoint %u\n"),
                                     k, index_ep));

                    connection_copied->internalEndpoint.length (index_ep + 1);
                    connection_copied->internalEndpoint[index_ep] = plan.connection[j].internalEndpoint[k];
                    connection_copied->internalEndpoint[index_ep].instanceRef = index_ins - 1;
                  }
              }
          }
        
        for (CORBA::ULong j = 0; j < plan.localityConstraint.length (); ++j)
          {
            ::Deployment::PlanLocality loc = plan.localityConstraint[j];
            for (CORBA::ULong k = 0; k < loc.constrainedInstanceRef.length (); ++k)
              {
                if (loc.constrainedInstanceRef[k] == i) // we are the same instance...
                  {
                    // add our new instance ref to the child plan's corresponding contraint.
                    CORBA::ULong child_loc_len (child_plan.localityConstraint[j].constrainedInstanceRef.length ());

                    DANCE_DEBUG (10, (LM_TRACE, DLINFO
                                     ACE_TEXT("Split_Plan::split_plan - ")
                                     ACE_TEXT ("Found matching locality constraint %u:%u, adding to %u:%u as %u\n"),
                                     j, k, j, child_loc_len, index_ins - 1));
                    child_plan.localityConstraint[j].constraint = loc.constraint;
                    child_plan.localityConstraint[j].constrainedInstanceRef.length (child_loc_len + 1);
                    child_plan.localityConstraint[j].constrainedInstanceRef[child_loc_len] = index_ins - 1;
                  }
              }
          }
                    
                    
        sub_plans.rebind (my_instance.node.in(), child_plan);
      }

    //Debug
    DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                     ACE_TEXT("Split_Plan::split_plan - ")
                     ACE_TEXT("Original plan connection count: %u\n"), plan.connection.length()));
    CORBA::ULong cnt = 0;
    for (TNodePlans::iterator it = sub_plans.begin(); it != sub_plans.end(); ++it)
      {
        cnt += (*it).int_id_.connection.length();
      }
    DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("Split_Plan::split_plan - ")
                     ACE_TEXT("Child plans connection count: %u\n"), cnt)); 
  }
}
