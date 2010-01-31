// $Id$

#include "Plan_Handler.h"
#include "DAnCE/Logger/Log_Macros.h"

namespace DAnCE
{
  void
  DAnCE_Utils::add_instance (
      ::Deployment::DeploymentPlan &deployment_plan,
      const char *instance_name,
      const char *node_name,
      const char *impl_name,
      const char *ns_name)
  {
    // Modifying the deployment plan in order to include the new instance ...
    ::Deployment::InstanceDeploymentDescriptions instance =
      deployment_plan.instance;

    instance.length (instance.length()+1);

    instance[instance.length()-1].name = CORBA::string_dup(instance_name); // Name of the instance
    instance[instance.length()-1].node = CORBA::string_dup(node_name);
    instance[instance.length()-1].source.length(0);

    // Looking for implementation block with name equals to type ...
    ::Deployment::MonolithicDeploymentDescriptions implementation =
      deployment_plan.implementation;

    CORBA::ULong i = 0;
    for (i = 0; i < implementation.length(); ++i)
      {
        if (ACE_OS::strcmp (implementation[i].name.in(), impl_name) == 0)
          {
            break;
          }
      }

    if (i < implementation.length())
      {
        instance[instance.length()-1].implementationRef = i;
      }
    else
      {
        throw ImplementationNotFound();
      }

    if (ns_name)
      {
        instance[instance.length()-1].configProperty.length(1);
        instance[instance.length()-1].configProperty[0].name =
          CORBA::string_dup ("RegisterNaming");
        instance[instance.length()-1].configProperty[0].value <<=
          CORBA::string_dup (ns_name);
      }

    // Re-assigning instances to deployment plan ...
    deployment_plan.instance = instance;
  }

  void
  DAnCE_Utils::print_instances (const ::Deployment::DeploymentPlan &deployment_plan)
  {
    const ::Deployment::InstanceDeploymentDescriptions instance =
      deployment_plan.instance;

    for (CORBA::ULong i = 0; i < instance.length(); ++i)
    {
      DANCE_DEBUG (6, (LM_DEBUG, "\nInstance no. %d\n", i));
      DANCE_DEBUG (6, (LM_DEBUG, "\tName: %C\n", instance[i].name.in()));
      DANCE_DEBUG (6, (LM_DEBUG, "\tNode: %C\n", instance[i].node.in()));

      DANCE_DEBUG (6, (LM_DEBUG, "\tImplementationRef: %d\n", instance[i].implementationRef));
      DANCE_DEBUG (6, (LM_DEBUG, "\tNumber of properties: %d\n", instance[i].configProperty.length()));

      for (CORBA::ULong k = 0; k < instance[i].configProperty.length(); k++)
        {
          DANCE_DEBUG (6, (LM_DEBUG, "\t\tName: %C\n", instance[i].configProperty[k].name.in()));
        }
    }
  }

  void
  DAnCE_Utils::print_connections (const ::Deployment::DeploymentPlan &deployment_plan)
  {
    const ::Deployment::PlanConnectionDescriptions connection =
      deployment_plan.connection;

    for (CORBA::ULong i = 0; i < connection.length(); ++i)
      {
        DANCE_DEBUG (6, (LM_DEBUG, "\nConnection no. %d\n", i));
        DANCE_DEBUG (6, (LM_DEBUG, "\tName: %C\n", connection[i].name.in()));

        DANCE_DEBUG (6, (LM_DEBUG, "\tNo of deployRequirements: %d\n",
                    connection[i].deployRequirement.length()));

        for (CORBA::ULong j = 0; j < connection[i].deployRequirement.length(); ++j)
        {
           DANCE_DEBUG (6, (LM_DEBUG,
                       "\t\tDeploy Requirement %d:\n",
                       j+1));
           DANCE_DEBUG (6, (LM_DEBUG,
                       "\t\t\tResource Type: %C\n",
                       connection[i].deployRequirement[j].resourceType.in()));
           DANCE_DEBUG (6, (LM_DEBUG,
                       "\t\t\tName: %C\n",
                       connection[i].deployRequirement[j].name.in()));
           DANCE_DEBUG (6, (LM_DEBUG,
                       "\t\t\tNo of Properties: %d\n",
                       connection[i].deployRequirement[j].property.length()));
           for (CORBA::ULong k = 0; k < connection[i].deployRequirement[j].property.length(); ++k)
           {
              DANCE_DEBUG (6, (LM_DEBUG, "\t\t\tProperty %d:\n", k+1));
              DANCE_DEBUG (6, (LM_DEBUG,
                          "\t\t\t\tName: %C\n",
                          connection[i].deployRequirement[j].property[k].name.in()));
           }
        }

        DANCE_DEBUG (6, (LM_DEBUG,
                    "\tNo of externalEndpoints: %d\n",
                    connection[i].externalEndpoint.length()));
        for (CORBA::ULong j = 0; j < connection[i].externalEndpoint.length(); j++)
          {
            DANCE_DEBUG (6, (LM_DEBUG,
                       "\t\tPortname %d: %C\n", j+1,
                       connection[i].externalEndpoint[j].portName.in()));
          }

        DANCE_DEBUG (6, (LM_DEBUG,
                    "\tNo of internalEndpoints: %d\n",
                    connection[i].internalEndpoint.length()));
        for (CORBA::ULong j = 0; j < connection[i].internalEndpoint.length(); j++)
          {
            DANCE_DEBUG (6, (LM_DEBUG, "\t\tInternalEndpoint %d:\n", j+1));
            DANCE_DEBUG (6, (LM_DEBUG, "\t\t\tPortname: %C\n", connection[i].internalEndpoint[j].portName.in()));
            DANCE_DEBUG (6, (LM_DEBUG,  "\t\t\tProvider: %d\n", connection[i].internalEndpoint[j].provider));

              if (connection[i].internalEndpoint[j].kind == ::Deployment::Facet ||
                connection[i].internalEndpoint[j].kind == ::Deployment::SimplexReceptacle)
                DANCE_DEBUG (6, (LM_DEBUG,
                            "\t\t\tKind: %C\n",
                          (connection[i].internalEndpoint[j].kind == ::Deployment::Facet) ?
                          "Facet" : "SimplexReceptacle"));
            else
                DANCE_DEBUG (6, (LM_DEBUG,
                            "\t\t\tKind: %d\n",
                            connection[i].internalEndpoint[j].kind));

            DANCE_DEBUG (6, (LM_DEBUG,
                        "\t\t\tInstanceRef: %ld",
                        (long) connection[i].internalEndpoint[j].instanceRef));
            DANCE_DEBUG (6, (LM_DEBUG, "\tInstanceName: %C\n",
                        deployment_plan.instance[connection[i].internalEndpoint[j].instanceRef].name.in()));
          }

        DANCE_DEBUG (6, (LM_DEBUG,
                    "\tNo of externalReferenceEndpoints: %d\n",
                    connection[i].externalReference.length()));
        for (CORBA::ULong  j = 0; j < connection[i].externalReference.length(); ++j)
           DANCE_DEBUG (6, (LM_DEBUG,
                       "\t\tLocation %d: %C\n",
                       j+1,
                       connection[i].externalReference[j].location.in()));

        DANCE_DEBUG (6, (LM_DEBUG,
                    "\tNo of deployedResources: %d\n",
                    connection[i].deployedResource.length()));
        for (CORBA::ULong j = 0; j < connection[i].deployedResource.length(); j++)
          {
            DANCE_DEBUG (6, (LM_DEBUG,
                        "\t\tTargetName: %C\n",
                        connection[i].deployedResource[j].targetName.in()));
            DANCE_DEBUG (6, (LM_DEBUG, "\t\tRequirementName: %C\n",
                        connection[i].deployedResource[j].requirementName.in()));
            DANCE_DEBUG (6, (LM_DEBUG, "\t\tResourceName: %C\n",
                        connection[i].deployedResource[j].resourceName.in()));
          }
      }
  }
}

