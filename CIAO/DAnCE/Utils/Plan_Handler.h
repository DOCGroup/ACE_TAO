//$Id$
/*========================================================
 *
 * @file   Plan_Handler.h
 *
 * @Brief  This file contains the implementation of
 *         the Plan_Handler class.
 *
 * @author Gan Deng <gan.deng@vanderbilt.edu>
 *========================================================*/

#ifndef CIAO_DANCE_UTILS_H
#define CIAO_DANCE_UTILS_H

#include "DAnCE_Utils_Export.h"
#include "Deployment/DeploymentC.h"

namespace DAnCE
{
  class ImplementationNotFound {};
  class InstanceNotFound {};
  class ConnectionNotFound {};

  /**
   * @class Plan_Handler
   *
   * This class takes an existing deployment plan as input, and
   * can modify the deployment plan internals.
   *
   * The main methods on this class are:
   *  
   *  - Add a instance to the deployment plan;
   *  - Add a connection to the deployment plan;
   *  - Remove a connection from the deployment plan;
   *  - Remove a instance (and, consequently all its connection) from the
   *    deployment plan;
   *  - Show all instances in the deployment plan;
   *  - Show all connections in the deployment plan;
   */

  class DAnCE_Utils_Export DAnCE_Utils
  {
  public:
    static void add_instance (
        ::Deployment::DeploymentPlan &deployment_plan, 
        const char *instance_name, 
        const char *node_name, 
        const char *impl_name, 
        const char *ns_name = 0)
      throw (ImplementationNotFound);

    static void add_connection (
        ::Deployment::DeploymentPlan &deployment_plan, 
        const char *connection_name, 
        const char *port_name, 
        const char *facet_instance, 
        const char *receptacle_instance)
      throw (InstanceNotFound);

    static void remove_instance (
        ::Deployment::DeploymentPlan &deployment_plan, 
        const char *instance_name)
      throw (InstanceNotFound);

    static void remove_connection (
        ::Deployment::DeploymentPlan &deployment_plan, 
        const char *connection_name)
      throw (ConnectionNotFound);

    static void print_instances (
      const ::Deployment::DeploymentPlan &deployment_plan);

    static void print_connections (
      const ::Deployment::DeploymentPlan &deployment_plan);
  };
}

#endif /* CIAO_DANCE_UTILS_H */

