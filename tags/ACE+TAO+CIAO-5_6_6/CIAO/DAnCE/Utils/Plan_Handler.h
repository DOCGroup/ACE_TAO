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

namespace CIAO
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
        const char *ns_name = 0);

    /// Add a certain number of instances
    /// The name of the instances will be "instance_name_X" where
    /// <X> is a positive integer number starting from 1 to <num>.
    static void add_instances (
        int num,
        ::Deployment::DeploymentPlan &deployment_plan, 
        const char *instance_name, 
        const char *node_name, 
        const char *impl_name, 
        const char *ns_name = 0);

    static void add_connection (
        ::Deployment::DeploymentPlan &deployment_plan, 
        const char *connection_name, 
        const char *port_name, 
        const char *facet_instance, 
        const char *receptacle_instance);

    static void remove_instance (
        ::Deployment::DeploymentPlan &deployment_plan, 
        const char *instance_name);

    static void remove_connection (
        ::Deployment::DeploymentPlan &deployment_plan, 
        const char *connection_name);

    static void print_instances (
      const ::Deployment::DeploymentPlan &deployment_plan);

    static void print_connections (
      const ::Deployment::DeploymentPlan &deployment_plan);
  };
}

#endif /* CIAO_DANCE_UTILS_H */

