// $Id$
#include "Containers_Info_Map.h"
#include "ciao/CIAO_Config.h"
#include "ciao/CIAO_common.h"

namespace CIAO
{
  Containers_Info_Map::
  Containers_Info_Map (const Deployment::DeploymentPlan & plan)
    : map_ (CIAO_DEFAULT_MAP_SIZE),
      plan_ (plan)
  {
    this->initialize_map ();
    this->build_map ();
  }

  Deployment::ContainerImplementationInfos *
  Containers_Info_Map::containers_info (void)
  {
    // Fetch the information from the <containers_info_map_>
    // and return the corresponding sequence
    Deployment::ContainerImplementationInfos_var retv;

    ACE_NEW_RETURN (retv,
                    Deployment::ContainerImplementationInfos,
                    0);

    Iterator end = this->map_.end ();
    CORBA::ULong i = 0;

    for (Iterator b = this->map_.begin ();
         b != end;
         ++b)
      {
        retv->length (i + 1);
        (*retv)[i] = *((*b).int_id_);

        ++i;
      }
    return retv._retn ();
  }

  void
  Containers_Info_Map::
  initialize_map (void)
  {
    const CORBA::ULong instance_len = this->plan_.instance.length ();

    // Iterate over the instance list and look at the policy set id of each
    // component instance. For each policy set, we create a separate container
    // to host all the components with such policy set.
    // NOTE: all the component instances without policies are specified should
    // be hosted in the same container, and in our map the key is an empty string ""
    for (CORBA::ULong i = 0; i < instance_len; ++i)
      {
        char my_resource_id[256];
        char *my_policy_set_id;

        if (this->plan_.instance[i].deployedResource.length () != 0)
          {
            ACE_OS::strcpy (my_resource_id,
              this->plan_.instance[i].deployedResource[0].resourceName.in ());
              
            this->plan_.instance[i].deployedResource[0].resourceValue >>=
              my_policy_set_id;
          }
        else
          {
            my_policy_set_id = new char[256];
            ACE_OS::strcpy (my_resource_id, "");
            ACE_OS::strcpy (my_policy_set_id, "");
          }

        // If we find a different policy_set_id, then we bind it to the map.
        if (this->map_.find (my_policy_set_id) == 0)
          continue;
        else if (ACE_OS::strcmp (my_policy_set_id, "") == 0)
          {
            // empty policy_set_id
            Deployment::ContainerImplementationInfo * info;
            ACE_NEW (info, Deployment::ContainerImplementationInfo);
            this->map_.bind (my_policy_set_id, info);
            continue;
          }
        else
          {
            Deployment::ContainerImplementationInfo * info;
            ACE_NEW (info, Deployment::ContainerImplementationInfo);

            // Fetch the actual policy_set_def from the infoProperty
            // Ugly due to the IDL data structure definition! :(
            CORBA::ULong j;
            bool found = false;

            for (j = 0;
                  j < this->plan_.infoProperty.length ();
                  ++j)
              {
                CIAO::DAnCE::ServerResource *server_resource_def = 0;
                this->plan_.infoProperty[j].value >>= server_resource_def;
                if (ACE_OS::strcmp ((*server_resource_def).Id,
                                    my_resource_id) == 0)
                  {
                    // Iterate over the policy_sets
                    CORBA::ULong k;
                    for (k = 0;
                          k < (*server_resource_def).orb_config.policy_set.length ();
                          ++k)
                      {
                        ACE_DEBUG ((LM_DEBUG, "Looking for policy set id: %s\n", my_policy_set_id));
                        ACE_DEBUG ((LM_DEBUG, "Compare against policy set id: %s\n\n", 
                                    (*server_resource_def).orb_config.policy_set[k].Id.in ()));


                        if (ACE_OS::strcmp (my_policy_set_id,
                          (*server_resource_def).orb_config.policy_set[k].Id) == 0)
                          {
                            // Foud the target policy set def
                            info->container_config.length (1);
                            info->container_config[0].name =
                              CORBA::string_dup ("ContainerPolicySet");
                            info->container_config[0].value <<=
                              (*server_resource_def).orb_config.policy_set[k];

                            ACE_DEBUG ((LM_DEBUG, "Found matching rt policy set*****\n\n"));
                            found = true;
                            break;
                          }
                      }
                    if (k == (*server_resource_def).orb_config.policy_set.length ())
                      {
                        // No Server Resource Def found?
                        ACE_DEBUG ((LM_DEBUG,
                            "No matching policy set def found in resource def: %s!\n",
                            my_resource_id));
                      }
                  }

                // if we successfully found the policy_set_id
                if (found)
                  break;
              } // end of for loop for fetching policy_set_def

            if (j == this->plan_.infoProperty.length ())
              {
                // No Server Resource Def found?! Inconsistent descriptor files.
                ACE_DEBUG ((LM_ERROR, "(%P|%t) Descriptor error: "
                    "No matching server resource def found for component: %s!\n",
                    this->plan_.instance[i].name.in ()));
              }
            this->map_.bind (my_policy_set_id, info);
          }
      }
  }

  bool
  Containers_Info_Map::
  build_map (void)
  {
    const CORBA::ULong instance_len = this->plan_.instance.length ();

    for (CORBA::ULong i = 0; i < instance_len; ++i)
      {
        const Deployment::InstanceDeploymentDescription & instance =
          this->plan_.instance[i];

        if (! this->insert_instance_into_map (instance))
          return false;
      }

    return true;
  }

  bool
  Containers_Info_Map::insert_instance_into_map (
    const Deployment::InstanceDeploymentDescription & instance)
  {
    Deployment::ContainerImplementationInfo container_info;

    const char * policy_set_id = "";
    if (instance.deployedResource.length () != 0)
      {
        instance.deployedResource[0].resourceValue >>= policy_set_id;
      }

    // Find the ContainerImplementationInfo entry from the map
    MAP::ENTRY *entry = 0;
    if (this->map_.find (policy_set_id, entry) != 0)
      return false; //should never happen
    else
      {
        this->insert_instance_into_container (
                instance,
                entry->int_id_->impl_infos);
      }

    return true;
  }

  bool
  Containers_Info_Map::insert_instance_into_container (
      const Deployment::InstanceDeploymentDescription & instance,
      Deployment::ComponentImplementationInfos & impl_infos)
  {
    // Increase the length of the ComponentImplementationInfos by one
    CORBA::ULong i = impl_infos.length ();
    impl_infos.length (i + 1);

    // Fill in the information about this component instance
    // Get the component instance name.
    impl_infos[i].component_instance_name = instance.name.in ();

    const Deployment::MonolithicDeploymentDescription & impl =
      this->plan_.implementation[instance.implementationRef];

    const CORBA::ULong artifact_num = impl.artifactRef.length ();

    // Copy Component instance related Properties if there is any.
    if (instance.configProperty.length () > 0)
      {
        impl_infos[i].component_config = instance.configProperty;
      }

    bool svnt_found = false;
    bool exec_found = false;

    // For svnt artifact
    for (CORBA::ULong j = 0; j < artifact_num; ++j)
      {
        const Deployment::ArtifactDeploymentDescription & arti =
          this->plan_.artifact[ impl.artifactRef[j] ];

        ACE_CString tmp = arti.name.in ();
        ssize_t pos;

        //@@ Note: I am not checking for redundancy here. Maybe
        //         the modeling tool should make sure of
        //         uniqueness, i.e., one component implementation
        //         should have only 1 _svnt and 1 _exec libs.
        if (!svnt_found &&
                ((pos  = tmp.find ("_svnt")) != ACE_CString::npos ||
                (pos  = tmp.find ("_Svnt")) != ACE_CString::npos))
          {
            if (arti.location.length() < 1 )
              {
                ACE_DEBUG ((LM_DEBUG, "Servant Artifact must have a location!\n"));
                return  0;
              }

            svnt_found = true;
            // Copy the servant dll/so name.
            // @@ Note: I ignore all the other locations except the first one.
            impl_infos[i].servant_dll =
              CORBA::string_dup (arti.location[0].in ());

            // Get the entry point.
		        const CORBA::ULong prop_length = arti.execParameter.length ();

            for (CORBA::ULong prop_num = 0;
                prop_num < prop_length;
                ++prop_num)
              {
                ACE_CString name (arti.execParameter[prop_num].name.in ());
                if (name == ACE_CString ("entryPoint"))
                  {
                    const char * entry;
                    (arti.execParameter[prop_num].value) >>= entry;
                    impl_infos[i].servant_entrypt = CORBA::string_dup (entry);
                  }
                else
                  {
                    ACE_DEBUG ((LM_DEBUG, "Found unknown property in the artifact!\n"));
                    ACE_DEBUG ((LM_DEBUG, "We only support entrypoint at this point in CIAO.\n"));
                  }
              }
          }

        // As one can see, code is duplicated here. I will come back for this later.
        // For exec artifact
        if (!exec_found &&
                ((pos  = tmp.find ("_exec")) != ACE_CString::npos ||
                (pos  = tmp.find ("_Exec")) != ACE_CString::npos))
          {
            if (arti.location.length() < 1 )
              {
                ACE_DEBUG ((LM_DEBUG, "Executor Artifact must have a location!\n"));
                return 0;
              }

            exec_found = true;
            // Cpoy the servant dll/so name.
            // @@ Note: I ignore all the other locations except the first one.
            exec_found = true;
            impl_infos[i].executor_dll =
              CORBA::string_dup (arti.location[0].in ());

            // Get the entry point.
            const CORBA::ULong prop_length = arti.execParameter.length ();
            for (CORBA::ULong prop_num = 0;
                prop_num < prop_length;
                ++prop_num)
              {
                ACE_CString name (arti.execParameter[prop_num].name.in ());
                if (name == ACE_CString ("entryPoint"))
                  {
                    const char * entry;
                    (arti.execParameter[prop_num].value) >>= entry;
                    impl_infos[i].executor_entrypt = CORBA::string_dup (entry);
                  }
                else
                  {
                    ACE_DEBUG ((LM_DEBUG, "Found unknown property in the artifact!\n"));
                    ACE_DEBUG ((LM_DEBUG, "We only support entrypoint at this point in CIAO.\n"));
                  }
              }
          }
        else
          // We see artifact other than servant/executor and we ignore them.
          continue;
      }
    return true;
  }
}
