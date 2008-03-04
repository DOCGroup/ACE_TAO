// $Id$

#include "Plan_Handler.h"

#include "ace/OS.h"

namespace DAnCE
{
  void 
  DAnCE_Utils::add_instance (
      ::Deployment::DeploymentPlan &deployment_plan, 
      const char *instance_name, 
      const char *node_name, 
      const char *impl_name, 
      const char *ns_name)
    throw (ImplementationNotFound)
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

    CORBA::ULong i;
    for (i = 0; i < implementation.length(); ++i)
      if (ACE_OS::strcmp (implementation[i].name.in(), impl_name) == 0)
        break;

    if (i < implementation.length())
      instance[instance.length()-1].implementationRef = i;
    else
      throw ImplementationNotFound();
    
    if (ns_name != 0)
    {
      instance[instance.length()-1].configProperty.length(1);
      instance[instance.length()-1].configProperty[0].name = CORBA::string_dup ("RegisterNaming");
      instance[instance.length()-1].configProperty[0].value <<= CORBA::string_dup (ns_name);
    }

    // Re-assigning instances to deployment plan ...
    deployment_plan.instance = instance;
  }

  /*
  void
  DAnCE_Utils::add_connection (::Deployment::DeploymentPlan_var &deployment_plan, const char *connection_name, const char *port_name, const char *facet_instance, const char *receptacle_instance)
    throw (InstanceNotFound)
  {
    // Modifying the deployment plan in order to include the new connection ...

    ::Deployment::PlanConnectionDescriptions connection = deployment_plan->connection;
    connection.length(connection.length()+1);

    connection[connection.length()-1].name = CORBA::string_dup(connection_name);
    connection[connection.length()-1].source.length(0);
    connection[connection.length()-1].deployRequirement.length(0);
    connection[connection.length()-1].externalEndpoint.length(0);
    connection[connection.length()-1].internalEndpoint.length(2);
    connection[connection.length()-1].internalEndpoint[0].portName = CORBA::string_dup(port_name);
    connection[connection.length()-1].internalEndpoint[0].provider = 0L;
    connection[connection.length()-1].internalEndpoint[0].kind = ::Deployment::Facet;

    unsigned int i;

    // Looking for instance block with name equals to facet_instance ...

    ::Deployment::InstanceDeploymentDescriptions instance = deployment_plan->instance;

    for (i = 0; i < instance.length(); i++)
      if (!strcmp(instance[i].name.in(), facet_instance))
        break;

    if (i < instance.length())
      connection[connection.length()-1].internalEndpoint[0].instanceRef = i;
    else
      throw InstanceNotFound();

    connection[connection.length()-1].internalEndpoint[1].portName = CORBA::string_dup(port_name);
    connection[connection.length()-1].internalEndpoint[1].provider = 0L;
    connection[connection.length()-1].internalEndpoint[1].kind = ::Deployment::SimplexReceptacle;

    // Looking for instance block with name equals to receptacle_instance ...

    for (i = 0; i < instance.length(); i++)
      if (!strcmp(instance[i].name.in(), receptacle_instance))
        break;

    if (i < instance.length())
      connection[connection.length()-1].internalEndpoint[1].instanceRef = i;
    else
      throw InstanceNotFound();

    // Re-assigning connection to deployment plan

    deployment_plan->connection = connection;
  }

  void
  DAnCE_Utils::remove_instance (::Deployment::DeploymentPlan_var &deployment_plan, const char *instance_name)
    throw (InstanceNotFound)
  {
    // Modifying the deployment plan in order to remove the instance ...

    ::Deployment::InstanceDeploymentDescriptions instance = deployment_plan->instance;
    ::Deployment::PlanConnectionDescriptions connection = deployment_plan->connection;

    // Looking for the instance position in sequence of instances ...

    unsigned int instance_ref = 0, i;

    for (i = 0; i < instance.length(); i++)
      if (!strcmp(instance[i].name.in(), instance_name))
      {
        instance_ref = i;
        break;
      }

    if (i == instance.length())
      throw InstanceNotFound();

    // First of all, we need to remove all connections of which this instance participates ...

    for (i = 0; i < connection.length(); i++)
      for (unsigned int j = 0; j < connection[i].internalEndpoint.length(); j++)
        if (connection[i].internalEndpoint[j].instanceRef == instance_ref)
    {
      remove_connection(deployment_plan, connection[i].name.in());
      // Updating i in order to verify the shifted connection ...
      i--;
          // Re-acquiring instances from the deployment plan ...
      connection = deployment_plan->connection;
      break;
    }

    // And then, removing the instance itself ...

    for (unsigned int j = instance_ref; j < instance.length() - 1; j++)
    {
      instance[j].name = instance[j+1].name;
      instance[j].node = instance[j+1].node;
      instance[j].source.length(instance[j+1].source.length());
      for (unsigned int k = 0; k < instance[j].source.length(); k++)
        instance[j].source[k] = instance[j+1].source[k];
      instance[j].implementationRef = instance[j].implementationRef;;
      instance[j].configProperty.length(instance[j+1].configProperty.length());
      for (unsigned int k = 0; k < instance[j].configProperty.length(); k++)
      {
        instance[j].configProperty[k].name  = instance[j+1].configProperty[k].name;
        instance[j].configProperty[k].value = instance[j+1].configProperty[k].value;
      }
    }
    instance.length(instance.length()-1);

    // Re-assigning instances to the deployment plan ...
    deployment_plan->instance = instance;

    // Updating connections for the shifted instances ...
    connection = deployment_plan->connection;
    for (i = 0; i < connection.length(); i++)
      for (unsigned int j = 0; j < connection[i].internalEndpoint.length(); j++)
        if (connection[i].internalEndpoint[j].instanceRef > instance_ref)
          connection[i].internalEndpoint[j].instanceRef--;
    deployment_plan->connection = connection;

    return;

  }

  void
  DAnCE_Utils::remove_connection (
      ::Deployment::DeploymentPlan_var &deployment_plan, 
      const char *connection_name)
    throw (ConnectionNotFound)
  {
    ::Deployment::PlanConnectionDescriptions connection = deployment_plan->connection;

    for (unsigned int i = 0; i < connection.length(); i++)
      if (!strcmp(connection[i].name.in(), connection_name))
      {
        for (unsigned int k = i; k < connection.length() - 1; k++)
        {
          unsigned int l;
          connection[k].name = connection[k+1].name;
          connection[k].source.length(connection[k+1].source.length());
          for (l = 0; l < connection[k].source.length(); l++)
            connection[k].source[l] = connection[k+1].source[l];
          connection[k].deployRequirement.length(connection[k+1].deployRequirement.length());
          for (l = 0; l < connection[k].deployRequirement.length(); l++)
          {
            connection[k].deployRequirement[l].resourceType = connection[k+1].deployRequirement[l].resourceType;
            connection[k].deployRequirement[l].name = connection[k+1].deployRequirement[l].name;
            connection[k].deployRequirement[l].property.length(connection[k+1].deployRequirement[l].property.length());
            for (unsigned int m = 0; m < connection[k].deployRequirement[l].property.length(); m++)
            {
              connection[k].deployRequirement[l].property[m].name = connection[k+1].deployRequirement[l].property[m].name;
              connection[k].deployRequirement[l].property[m].value = connection[k+1].deployRequirement[l].property[m].value;
            }
          }
          connection[k].externalEndpoint.length(connection[k+1].externalEndpoint.length());
          for (l = 0; l < connection[k].externalEndpoint.length(); l++)
            connection[k].externalEndpoint[l].portName = connection[k+1].externalEndpoint[l].portName;
          connection[k].internalEndpoint.length(connection[k+1].internalEndpoint.length());
          for (l = 0; l < connection[k].internalEndpoint.length(); l++)
          {
            connection[k].internalEndpoint[l].portName = connection[k+1].internalEndpoint[l].portName;
            connection[k].internalEndpoint[l].provider = connection[k+1].internalEndpoint[l].provider;
            connection[k].internalEndpoint[l].kind = connection[k+1].internalEndpoint[l].kind;
            connection[k].internalEndpoint[l].instanceRef = connection[k+1].internalEndpoint[l].instanceRef;
          }
          connection[k].deployedResource.length(connection[k+1].deployedResource.length());
          for (l = 0; l < connection[k].deployedResource.length(); l++)
          {
            connection[k].deployedResource[l].targetName = connection[k+1].deployedResource[l].targetName;
            connection[k].deployedResource[l].requirementName = connection[k+1].deployedResource[l].requirementName;
            connection[k].deployedResource[l].resourceName = connection[k+1].deployedResource[l].resourceName;
            connection[k].deployedResource[l].resourceValue = connection[k+1].deployedResource[l].resourceValue;
          }
        }
        connection.length(connection.length()-1);
    // Re-assigning connection to the deployment plan ...
    deployment_plan->connection = connection;
        return;
      }
    // Throw exception if connection name not found ...
    throw ConnectionNotFound();
  }
*/
  void
  DAnCE_Utils::print_instances (const ::Deployment::DeploymentPlan &deployment_plan)
  {
    const ::Deployment::InstanceDeploymentDescriptions instance = 
      deployment_plan.instance;

    for (CORBA::ULong i = 0; i < instance.length(); ++i)
    {
      ACE_DEBUG((LM_DEBUG, "[%M] \nInstance no. %d\n", i));
      ACE_DEBUG((LM_DEBUG, "[%M] \tName: %s\n", instance[i].name.in()));
      ACE_DEBUG((LM_DEBUG, "[%M] \tNode: %s\n", instance[i].node.in()));

      ACE_DEBUG((LM_DEBUG, "[%M] \tImplementationRef: %d\n", instance[i].implementationRef));
      ACE_DEBUG((LM_DEBUG, "[%M] \tNumber of properties: %d\n", instance[i].configProperty.length()));

      for (CORBA::ULong k = 0; k < instance[i].configProperty.length(); k++)
          ACE_DEBUG((LM_DEBUG, "[%M] \t\tName: %s\n", instance[i].configProperty[k].name.in()));
    }
  }

  void
  DAnCE_Utils::print_connections (const ::Deployment::DeploymentPlan &deployment_plan)
  {
    const ::Deployment::PlanConnectionDescriptions connection = 
      deployment_plan.connection;

    for (CORBA::ULong i = 0; i < connection.length(); ++i)
      {
        ACE_DEBUG((LM_DEBUG, "[%M] \nConnection no. %d\n", i));
        ACE_DEBUG((LM_DEBUG, "[%M] \tName: %s\n", connection[i].name.in()));

        ACE_DEBUG((LM_DEBUG, "[%M] \tNo of deployRequirements: %d\n", 
                    connection[i].deployRequirement.length()));

        for (CORBA::ULong j = 0; j < connection[i].deployRequirement.length(); ++j)
        {
           ACE_DEBUG((LM_DEBUG, 
                       "\t\tDeploy Requirement %d:\n", 
                       j+1));
           ACE_DEBUG((LM_DEBUG, 
                       "\t\t\tResource Type: %s\n", 
                       connection[i].deployRequirement[j].resourceType.in()));
           ACE_DEBUG((LM_DEBUG, 
                       "\t\t\tName: %s\n",
                       connection[i].deployRequirement[j].name.in()));
           ACE_DEBUG((LM_DEBUG, 
                       "\t\t\tNo of Properties: %d\n",
                       connection[i].deployRequirement[j].property.length()));
           for (CORBA::ULong k = 0; k < connection[i].deployRequirement[j].property.length(); ++k)
           {
              ACE_DEBUG((LM_DEBUG, "[%M] \t\t\tProperty %d:\n", k+1));
              ACE_DEBUG((LM_DEBUG, 
                          "\t\t\t\tName: %s\n", 
                          connection[i].deployRequirement[j].property[k].name.in()));
           }
        }

        ACE_DEBUG((LM_DEBUG, 
                    "\tNo of externalEndpoints: %d\n",
                    connection[i].externalEndpoint.length()));
        for (CORBA::ULong j = 0; j < connection[i].externalEndpoint.length(); j++)
           ACE_DEBUG((LM_DEBUG, 
                       "\t\tPortname %d: %s\n", j+1,
                       connection[i].externalEndpoint[j].portName.in()));

        ACE_DEBUG((LM_DEBUG, 
                    "\tNo of internalEndpoints: %d\n",
                    connection[i].internalEndpoint.length()));
        for (CORBA::ULong j = 0; j < connection[i].internalEndpoint.length(); j++)
          {
            ACE_DEBUG((LM_DEBUG, "[%M] \t\tInternalEndpoint %d:\n", j+1));
            ACE_DEBUG((LM_DEBUG, "[%M] \t\t\tPortname: %s\n", connection[i].internalEndpoint[j].portName.in()));
            ACE_DEBUG((LM_DEBUG,  "\t\t\tProvider: %d\n", connection[i].internalEndpoint[j].provider));

              if (connection[i].internalEndpoint[j].kind == ::Deployment::Facet || 
                connection[i].internalEndpoint[j].kind == ::Deployment::SimplexReceptacle)
                ACE_DEBUG((LM_DEBUG, 
                            "\t\t\tKind: %s\n", 
                          (connection[i].internalEndpoint[j].kind == ::Deployment::Facet) ? 
                          "Facet" : "SimplexReceptacle"));
            else
                ACE_DEBUG((LM_DEBUG, 
                            "\t\t\tKind: %d\n",
                            connection[i].internalEndpoint[j].kind));

            ACE_DEBUG((LM_DEBUG, 
                        "\t\t\tInstanceRef: %ld",
                        (long) connection[i].internalEndpoint[j].instanceRef));
            ACE_DEBUG((LM_DEBUG, "[%M] \tInstanceName: %s\n", 
                        deployment_plan.instance[connection[i].internalEndpoint[j].instanceRef].name.in()));
          }

        ACE_DEBUG((LM_DEBUG, 
                    "\tNo of externalReferenceEndpoints: %d\n",
                    connection[i].externalReference.length()));
        for (CORBA::ULong  j = 0; j < connection[i].externalReference.length(); ++j)
           ACE_DEBUG((LM_DEBUG, 
                       "\t\tLocation %d: %s\n", 
                       j+1,
                       connection[i].externalReference[j].location.in()));

        ACE_DEBUG((LM_DEBUG, 
                    "\tNo of deployedResources: %d\n",
                    connection[i].deployedResource.length()));
        for (CORBA::ULong j = 0; j < connection[i].deployedResource.length(); j++)
          {
            ACE_DEBUG((LM_DEBUG, 
                        "\t\tTargetName: %s\n",
                        connection[i].deployedResource[j].targetName.in()));
            ACE_DEBUG((LM_DEBUG, "[%M] \t\tRequirementName: %s\n",
                        connection[i].deployedResource[j].requirementName.in()));
            ACE_DEBUG((LM_DEBUG, "[%M] \t\tResourceName: %s\n",
                        connection[i].deployedResource[j].resourceName.in()));
          }
      }
  }
}

