/**
 * @file Connection_Handler.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * $Id$
 *
 * Common connection handling logic for components.
 */

#ifndef CONNECTION_HANDLER_H
#define CONNECTION_HANDLER_H

#include "dance/Deployment/Deployment_DeploymentPlanC.h"
#include "ccm/CCM_ObjectC.h"

#include <map>
#include <string>

namespace CIAO
{
  class Connection_Handler
  {
  public:
    void
    provide_endpoint_reference (const ::Deployment::DeploymentPlan & plan,
                                ::CORBA::ULong connectionRef,
                                ::CORBA::Any_out endpoint_reference);

    void connect_instance (const ::Deployment::DeploymentPlan & plan,
                           ::CORBA::ULong connectionRef,
                           const ::CORBA::Any & provided_reference);

    void disconnect_instance (const ::Deployment::DeploymentPlan & plan,
                              ::CORBA::ULong connectionRef);

  private:
    void connect_facet (const ::Deployment::DeploymentPlan & plan,
                        ::CORBA::ULong connectionRef,
                        ::CORBA::ULong endpoint,
                        const ::CORBA::Any & provided_reference);

    void connect_receptacle (const ::Deployment::DeploymentPlan & plan,
                             ::CORBA::ULong connectionRef,
                             ::CORBA::ULong endpoint,
                             const ::CORBA::Any & provided_reference);

    void connect_publisher (const ::Deployment::DeploymentPlan & plan,
                            ::CORBA::ULong connectionRef,
                            ::CORBA::ULong endpoint,
                            const ::CORBA::Any & provided_reference);

    void connect_emitter (const ::Deployment::DeploymentPlan & plan,
                          ::CORBA::ULong connectionRef,
                          ::CORBA::ULong endpoint,
                          const ::CORBA::Any & provided_reference);

    void connect_consumer (const ::Deployment::DeploymentPlan & plan,
                             ::CORBA::ULong connectionRef,
                             ::CORBA::ULong endpoint,
                             const ::CORBA::Any & provided_reference);

    void disconnect_facet (const ::Deployment::DeploymentPlan & plan,
                           ::CORBA::ULong connectionRef);


    void disconnect_receptacle (const ::Deployment::DeploymentPlan & plan,
                             ::CORBA::ULong connectionRef);

    void disconnect_publisher (const ::Deployment::DeploymentPlan & plan,
                            ::CORBA::ULong connectionRef);

    void disconnect_subscriber (const ::Deployment::DeploymentPlan & plan,
                             ::CORBA::ULong connectionRef);

    void connect_local_port (const char *facet_id,
                             const char *facet_port,
                             const char *receptacle_id,
                             const char *receptacle_port);

    bool is_local_facet (const ::Deployment::PlanConnectionDescription &conn);

    typedef std::pair < ::Components::Cookie_var,
                        ::Components::CCMObject_var> CONNECTION_INFO;
    typedef std::map <std::string,
                      CONNECTION_INFO> COOKIES;

    COOKIES cookies_;
  };
}
#endif
