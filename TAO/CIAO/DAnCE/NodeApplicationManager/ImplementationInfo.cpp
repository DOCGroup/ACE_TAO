//$Id$
#ifndef CIAO_IMPLEMENTATION_INFO_C
#define CIAO_IMPLEMENTATION_INFO_C

#include "ImplementationInfo.h"
#include "ace/SString.h"

// Specialized operation that we will use for now.
namespace CIAO
{
  bool operator<< (Deployment::ImplementationInfos & info,
                   const Deployment::DeploymentPlan & plan)
  {
    const CORBA::ULong len = plan.instance.length ();
    info.length (len);

    for (CORBA::ULong i = 0; i < len; ++i)
      {
        const Deployment::InstanceDeploymentDescription & inst =
          plan.instance[i];

        // Get the component instance name.
        info[i].component_instance_name = inst.name.in ();

        const Deployment::MonolithicDeploymentDescription & impl =
          plan.implementation[inst.implementationRef];

        const CORBA::ULong artifact_num = impl.artifactRef.length ();

        // Copy Component instance related Properties if there is any.
        if (inst.configProperty.length () > 0)
          {
            info[i].component_config = inst.configProperty;
          }

        // For svnt artifact
        for (CORBA::ULong j = 0; j < artifact_num; ++j)
          {
            const Deployment::ArtifactDeploymentDescription & arti =
              plan.artifact[ impl.artifactRef[j] ];

            ACE_CString tmp = arti.name.in ();
            ssize_t pos;

            //@@ Note: I am not checking for redundancy here. Maybe
            //         the modeling tool should make sure of
            //         uniqueness.
            if ((pos  = tmp.find ("_svnt")) != ACE_CString::npos ||
                (pos  = tmp.find ("_Svnt")) != ACE_CString::npos)
              {
                if (arti.location.length() < 1 )
                  {
                    ACE_DEBUG ((LM_DEBUG, "Servant Artifact must have a location!\n"));
                    return  0;
                  }
                // Cpoy the servant dll/so name.
                // @@ Note: I ignore all the other locations except the first one.
                info[i].servant_dll = CORBA::string_dup (arti.location[0].in ());

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
                        info[i].servant_entrypt = CORBA::string_dup (entry);
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
            if ((pos  = tmp.find ("_exec")) != ACE_CString::npos ||
                (pos  = tmp.find ("_Exec")) != ACE_CString::npos)
              {
                if (arti.location.length() < 1 )
                  {
                    ACE_DEBUG ((LM_DEBUG, "Executor Artifact must have a location!\n"));
                    return 0;
                  }
                // Cpoy the servant dll/so name.
                // @@ Note: I ignore all the other locations except the first one.
                info[i].executor_dll = CORBA::string_dup (arti.location[0].in ());

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
                        info[i].executor_entrypt = CORBA::string_dup (entry);
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
      }
    return 1;
  }
}

#endif /* CIAO_IMPLEMENTATION_INFO_C */
