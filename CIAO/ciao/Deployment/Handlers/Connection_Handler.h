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
#include "ciao/Containers/Container_BaseC.h"
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
                           ::CORBA::ULong c_id,
                           const ::CORBA::Any & provided_reference);

    void disconnect_instance (const ::Deployment::DeploymentPlan & plan,
                              ::CORBA::ULong c_id);

  private:

#if !defined (CCM_NOEVENT)
    void connect_publisher (const ::Deployment::DeploymentPlan & plan,
                            const ::Deployment::PlanConnectionDescription &conn,
                            const ::Deployment::PlanSubcomponentPortEndpoint &endpoint,
                            const ::CORBA::Any & provided_reference);
#endif

#if !defined (CCM_NOEVENT)
  void connect_consumer (const ::Deployment::DeploymentPlan & plan,
                         const ::Deployment::PlanConnectionDescription &conn,
                         const ::Deployment::PlanSubcomponentPortEndpoint &endpoint,
                         const ::CORBA::Any & provided_reference);
#endif

#if !defined (CCM_NOEVENT)
    void connect_emitter (const ::Deployment::DeploymentPlan & plan,
                          const ::Deployment::PlanConnectionDescription &conn,
                          const ::Deployment::PlanSubcomponentPortEndpoint &endpoint,
                          const ::CORBA::Any & provided_reference);
#endif

#if !defined (CCM_NOEVENT)
    void disconnect_publisher (const ::Deployment::PlanConnectionDescription &conn,
                               const ::Deployment::PlanSubcomponentPortEndpoint &endpoint);
#endif

#if !defined (CCM_NOEVENT)
    void disconnect_consumer (const ::Deployment::PlanConnectionDescription &conn,
                              const ::Deployment::PlanSubcomponentPortEndpoint &endpoint);
#endif

#if !defined (CCM_NOEVENT)
    void disconnect_emitter (const ::Deployment::PlanConnectionDescription &conn,
                             const ::Deployment::PlanSubcomponentPortEndpoint &endpoint);
#endif

    void connect_local_port (const ::Deployment::DeploymentPlan & plan,
                             const ::Deployment::PlanConnectionDescription &conn,
                             ::CORBA::ULong endpointRef,
                             const ::Deployment::PlanSubcomponentPortEndpoint &receptacle_endpoint);

    void disconnect_local_port (const ::Deployment::DeploymentPlan & plan,
                                const ::Deployment::PlanConnectionDescription &conn,
                                ::CORBA::ULong endpointRef,
                                const ::Deployment::PlanSubcomponentPortEndpoint &receptacle_endpoint);

    void connect_non_local_receptacle (const ::Deployment::DeploymentPlan & plan,
                                       const ::Deployment::PlanConnectionDescription &conn,
                                       const ::Deployment::PlanSubcomponentPortEndpoint &endpoint,
                                       const ::CORBA::Any & provided_reference);

    void connect_non_local_facet (const ::Deployment::DeploymentPlan & plan,
                                  const ::Deployment::PlanConnectionDescription &conn,
                                  const ::Deployment::PlanSubcomponentPortEndpoint &endpoint,
                                  const ::CORBA::Any & provided_reference);

    void disconnect_non_local (const ::Deployment::PlanConnectionDescription &conn,
                               const char * port_name);

    bool is_local_connection (const ::Deployment::PlanConnectionDescription &conn);

    ::CORBA::ULong retrieve_endpoint (const ::Deployment::PlanConnectionDescription &conn);

    typedef std::pair < ::Components::Cookie_var,
                        ::Components::CCMObject_var> CONNECTION_INFO;
    typedef std::map <std::string,
                      CONNECTION_INFO> COOKIES;

    void insert_cookie (const char * connection_name,
                        const CONNECTION_INFO conn_info);

    ::Components::Cookie * get_cookie (const char * connection_name);

    void remove_cookie (const char * connection_name);

    ::Components::CCMObject_ptr get_ccm_object (const char * connection_name);

    COOKIES cookies_;
  };
}
#endif
