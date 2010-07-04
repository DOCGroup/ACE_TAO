// $Id$
#include "Connection_Handler.h"
#include "ciao/Logger/Log_Macros.h"
#include "Deployment/Deployment_InvalidConnectionC.h"
#include "CIAO_State.h"
namespace CIAO
{
  void 
  Connection_Handler::provide_endpoint_reference (const ::Deployment::DeploymentPlan & plan,
                                                  ::CORBA::ULong connectionRef,
                                                  ::CORBA::Any_out endpoint_reference)
  {
    CIAO_TRACE ("Component_Handler_i::provide_endpoint_reference");
    
    const ::Deployment::PlanConnectionDescription &conn = 
      plan.connection[connectionRef];
    
    try
      {
        CIAO_DEBUG (9, (LM_TRACE, CLINFO
                        "Connection_Handler::provide_endpoint_reference - "
                        "Called for connection <%C>\n",
                        conn.name.in ()));

        for (CORBA::ULong i = 0;
             i < conn.internalEndpoint.length ();
             ++i)
          {
            if (conn.internalEndpoint[i].provider)
              {
                CORBA::Any *out;
                CORBA::Object_var ref;
                CORBA::ULong instanceRef = conn.internalEndpoint[i].instanceRef;
                Components::CCMObject_var comp = 
                  DEPLOYMENT_STATE::instance ()->
                  fetch_component (plan.instance[instanceRef].name.in ());
                
                if (CORBA::is_nil (comp.in ()))
                  {
                    CIAO_ERROR (1, (LM_ERROR, CLINFO 
                                    "Connection_Handler::provide_endpoint_reference - "
                                    "Nil component reference from CIAO_State for <%C>\n",
                                    plan.instance[instanceRef].name.in ()));
                    throw ::Deployment::InvalidConnection (conn.name.in (),
                                                           "Component instance not deployed when "
                                                           "gathering connections.");
                  }
                
                ACE_NEW_THROW_EX (out,
                                  CORBA::Any (),
                                  CORBA::NO_MEMORY ());
                
                endpoint_reference = out;
                
                if (this->is_local_facet (conn))
                  (*out) <<= comp.in ();
                else
                  {
                    switch (conn.internalEndpoint[i].kind)
                      {
                      case ::Deployment::Facet:
                        ref = 
                          comp->provide_facet (conn.internalEndpoint[i].portName.in ());
                        (*out) <<= ref.in ();
                        break;
                      
                      case ::Deployment::EventConsumer:
                        ref = 
                          comp->get_consumer (conn.internalEndpoint[i].portName.in ());
                        (*out) <<= ref.in ();
                        break;
                      
                      default:
                        throw ::Deployment::InvalidConnection (conn.name.in (),
                                                               "Invalid provider port type.");
                      };
                  }
                return;
              }
          }
      }
    catch (const ::Deployment::InvalidConnection &)
      {
        // pass through
        throw;
      }
    catch (const CORBA::Exception &ex)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO 
                        "Connection_Handler::provide_endpoint_reference - "
                        "Caught CORBA Exception <%C> while fetching reference for "
                        "connection <%C>\n",
                        ex._info ().c_str (),
                        plan.connection[connectionRef].name.in ()));
        throw ::Deployment::InvalidConnection (plan.connection[connectionRef].name.in (),
                                               ex._info ().c_str ());
                        
      }
    catch (...)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO 
                        "Connection_Handler::provide_endpoint_reference - "
                        "Caught unknown C++ exception on instance %C",
                        plan.connection[connectionRef].name.in ()));
        throw ::Deployment::InvalidConnection (plan.connection[connectionRef].name.in (),
                                               "Unknown C++ exception whilst establishing "
                                               "connection");
      }
    
  }

  void
  Connection_Handler::connect_instance (const ::Deployment::DeploymentPlan & plan,
                                        ::CORBA::ULong c_id,
                                        const ::CORBA::Any & provided_reference)
  {
    CIAO_TRACE ("Connection_Handler::connect_instance");
    
    const ::Deployment::PlanConnectionDescription &conn = plan.connection[c_id];
    const char *name = conn.name.in ();

    if (conn.internalEndpoint.length () == 0)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Connection_Handler::connect_instance - "
                        "Connection <%C> lacks an internalEndpoint.\n",
                        name));
        throw ::Deployment::InvalidConnection (name,
                                       "No internal endpoint for connection\n");
      }
    
    CORBA::ULong endpoint (0);
    
    if (conn.internalEndpoint.length () > 1)
      {
        for (CORBA::ULong i = 0;
             i < conn.internalEndpoint.length ();
             ++i)
          {
            if (!conn.internalEndpoint[i].provider)
              endpoint = i;
          }
      }
    
    try
      {
        switch (conn.internalEndpoint[endpoint].kind)
          {
          case Deployment::Facet:
            this->connect_facet (plan, c_id, endpoint, provided_reference);
            break;

          case Deployment::SimplexReceptacle:
          case Deployment::MultiplexReceptacle:
            this->connect_receptacle (plan, c_id, endpoint, provided_reference);
            break;

          case Deployment::EventEmitter:
            this->connect_emitter (plan, c_id, endpoint, provided_reference);
            break;

          case Deployment::EventPublisher:
            this->connect_publisher (plan, c_id, endpoint, provided_reference);
            break;

          case Deployment::EventConsumer:
            this->connect_consumer (plan, c_id, endpoint, provided_reference);
            break;
        
          default:
            CIAO_ERROR (1, (LM_ERROR, CLINFO
                            "Connection_Handler::connect_instance - "
                            "Unsupported port type.\n"));
            throw ::Deployment::InvalidConnection (name,
                                                   "Unsupported port type");
        
          }
      }
    catch (const ::Deployment::InvalidConnection &)
      {
        // pass through
        throw;
      }
    catch (CORBA::Exception &ex)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Connection_Handler::connect_instance - "
                        "Caught CORBA exception whilst connecting <%C>: %C\n",
                        name,
                        ex._info ().c_str ()));
        throw ::Deployment::InvalidConnection (name,
                                               ex._info ().c_str ());
      }
    catch (...)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Connection_Handler::connect_instance - "
                        "Caught C++ exception whilst connecting <%C>\n",
                        name));
        throw ::Deployment::InvalidConnection (name,
                                               "Unknown C++ Exception");
      }
  }
  
  void
  Connection_Handler::disconnect_instance (const ::Deployment::DeploymentPlan &,
                                           ::CORBA::ULong)
  {
    CIAO_TRACE ("Connection_Handler::disconnect_instance");
    // no-op is current behavior, we'll do that for now.
  }
    
  void
  Connection_Handler::connect_facet (const ::Deployment::DeploymentPlan & plan,
                                     ::CORBA::ULong connectionRef,
                                     ::CORBA::ULong endpointRef,
                                     const ::CORBA::Any & provided_reference)
  {
    CIAO_TRACE ("Connection_Handler::connect_facet");
    
    const ::Deployment::PlanConnectionDescription &conn = 
      plan.connection[connectionRef];
    const ::Deployment::PlanSubcomponentPortEndpoint &endpoint = 
      conn.internalEndpoint[endpointRef];
    
    CIAO_DEBUG (6, (LM_DEBUG, CLINFO
                    "Connection_Handler::connect_facet - "
                    "Connecting connection <%C> on instance <%C>\n",
                    conn.name.in (),
                    plan.instance[endpoint.instanceRef].name.in ()));
    
    if (this->is_local_facet (conn))
      {
        CORBA::ULong other_endpointRef = (endpointRef + 1) % 2;
        if (conn.internalEndpoint.length () == 2 && 
            (conn.internalEndpoint[other_endpointRef].kind == ::Deployment::MultiplexReceptacle ||
             conn.internalEndpoint[other_endpointRef].kind == ::Deployment::SimplexReceptacle))
          {
            const ::Deployment::PlanSubcomponentPortEndpoint &other_endpoint = 
              conn.internalEndpoint[other_endpointRef];
            this->connect_local_port (plan.instance[endpoint.instanceRef].name.in (),
                                      endpoint.portName.in (),
                                      plan.instance[other_endpoint.instanceRef].name.in (),
                                      other_endpoint.portName.in ());
            return;
          }
        else
          {
            CIAO_ERROR (1, (LM_ERROR, CLINFO
                            "Connection_Handler::connect_facet - "
                            "Error: Wrong number of internal endpoints for local facet connection\n"));
            throw ::Deployment::InvalidConnection (conn.name.in (),
                                                   "Local facet connections require exactly 2 internalEndpoints");
          }
      }
    
    if (conn.externalReference.length () == 0)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Connection_Handler::connect_facet - "
                        "Error: Expected external reference endpoint for connection <%C>",
                        conn.name.in ()));
        throw ::Deployment::InvalidConnection (conn.name.in (),
                                               "Expected external reference connection.");
      }

    ::CORBA::Object_var obj;
    provided_reference >>= CORBA::Any::to_object (obj);
        
    ::Components::CCMObject_var provided = ::Components::CCMObject::_narrow (obj.in ());

    if (CORBA::is_nil (provided))
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Connection_Handler::connect_facet - "
                        "While connecting <%C>:"
                        "Provided reference for connection where primary is Facet must be "
                        "non-nil.\n",
                        plan.connection[connectionRef].name.in ()));
        throw ::Deployment::InvalidConnection (plan.connection[connectionRef].name.in (),
                                               "Provided reference for connection where primary is Facet must "
                                               "be non-nil.");
        
      }
    
    ::Components::CCMObject_var facet_provider = 
        DEPLOYMENT_STATE::instance ()->fetch_component (plan.instance[endpoint.instanceRef].name.in ());
    
    if (CORBA::is_nil (facet_provider))
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Connection_Handler::connect_facet - "
                        "While connecting <%C>:"
                        "Providing component not deployed.",
                        plan.connection[connectionRef].name.in ()));
        throw ::Deployment::InvalidConnection (plan.connection[connectionRef].name.in (),
                                               "Providing component not deployed.");
      }
    
    CORBA::Object_var facet = 
      facet_provider->provide_facet (endpoint.portName.in ());
    
    ::Components::Cookie_var cookie = provided->connect (conn.externalReference[0].portName.in (),
                                                         facet.in ());
    
    CIAO_DEBUG (5, (LM_INFO, CLINFO
                    "Connection_Handler::connect_facet - "
                    "Connection <%C> successfully established.\n",
                    conn.name.in ()));
    
    this->cookies_[conn.name.in ()] = CONNECTION_INFO (cookie,
                                                       ::Components::CCMObject::_duplicate (provided));
  }
  
  void  
  Connection_Handler::connect_receptacle (const ::Deployment::DeploymentPlan & plan,
                                          ::CORBA::ULong connectionRef,
                                          ::CORBA::ULong endpointRef,
                                          const ::CORBA::Any & provided_reference)
  {
    const ::Deployment::PlanConnectionDescription &conn = 
      plan.connection[connectionRef];
    const ::Deployment::PlanSubcomponentPortEndpoint &endpoint = 
      conn.internalEndpoint[endpointRef];
    
    CIAO_DEBUG (6, (LM_DEBUG, CLINFO
                    "Connection_Handler::connect_receptacle - "
                    "Connecting connection <%C> on instance <%C>\n",
                    conn.name.in (),
                    plan.instance[endpoint.instanceRef].name.in ()));
    
    if (this->is_local_facet (conn))
      {
        CORBA::ULong other_endpointRef = (endpointRef + 1) % 2;
        if (conn.internalEndpoint.length () == 2 && 
            (conn.internalEndpoint[other_endpointRef].kind == ::Deployment::Facet))
          {
            const ::Deployment::PlanSubcomponentPortEndpoint &other_endpoint = 
              conn.internalEndpoint[other_endpointRef];
            this->connect_local_port (plan.instance[other_endpoint.instanceRef].name.in (),
                                      other_endpoint.portName.in (),
                                      plan.instance[endpoint.instanceRef].name.in (),
                                      endpoint.portName.in ());
            return;
          }
        else
          {
            CIAO_ERROR (1, (LM_ERROR, CLINFO
                            "Connection_Handler::connect_receptacle - "
                            "Error: Wrong number of internal endpoints for local receptacle connection\n"));
            throw ::Deployment::InvalidConnection (conn.name.in (),
                                                   "Local receptacle connections require exactly 2 internalEndpoints");
          }
      }
    
    ::CORBA::Object_var provided;

    if (!(provided_reference >>= CORBA::Any::to_object  (provided)))
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Connection_Handler::connect_facet - "
                        "Unable to extract provided reference to CORBA::Object\n",
                        plan.connection[connectionRef].name.in ()));
        throw ::Deployment::InvalidConnection (plan.connection[connectionRef].name.in (),
                                               "Unable to extract provided reference to CORBA Object.");
      }
    
    if (conn.externalReference.length () != 0)
      {
        ::Components::CCMObject_var facet_provider = 
          ::Components::CCMObject::_narrow (provided.in ());
        
        if (CORBA::is_nil (facet_provider.in ()))
          {
            CIAO_ERROR (1, (LM_ERROR, CLINFO
                            "Connection_Handler::connect_facet - "
                            "Unable to narrow provided external reference "
                            "to CCMObject in connection <%C>\n",
                            conn.name.in ()));
            throw ::Deployment::InvalidConnection (conn.name.in (),
                                                   "Unable to narrow external reference to CCMObject\n");
          }
        
        provided = 
          facet_provider->provide_facet (conn.externalReference[0].portName.in ());
      }
    
    ::Components::CCMObject_var receptacle = 
        DEPLOYMENT_STATE::instance ()->fetch_component (plan.instance[endpoint.instanceRef].name.in ());
    
    if (CORBA::is_nil (receptacle))
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Connection_Handler::connect_receptacle - "
                        "While connecting <%C>:"
                        "Receptacle component <%C> not deployed.\n",
                        plan.connection[connectionRef].name.in (),
                        plan.instance[endpoint.instanceRef].name.in ()));
        throw ::Deployment::InvalidConnection (plan.connection[connectionRef].name.in (),
                                               "Receptacle component not deployed.");
      }
    
    ::Components::Cookie_var cookie = receptacle->connect (endpoint.portName.in (),
                                                           provided.in ());
    CIAO_DEBUG (5, (LM_INFO, CLINFO
                    "Connection_Handler::connect_receptacle - "
                    "Connection <%C> successfully established.\n",
                    conn.name.in ()));
    
    this->cookies_[conn.name.in ()] = CONNECTION_INFO (cookie,
                                                       ::Components::CCMObject::_duplicate (receptacle.in ()));
  }
    
  void  
  Connection_Handler::connect_publisher (const ::Deployment::DeploymentPlan & plan,
                                         ::CORBA::ULong connectionRef,
                                         ::CORBA::ULong endpointRef,
                                         const ::CORBA::Any & provided_reference)
  {
    CIAO_TRACE ("Connection_Handler::connect_publisher");
    
    const ::Deployment::PlanConnectionDescription &conn = 
      plan.connection[connectionRef];
    const ::Deployment::PlanSubcomponentPortEndpoint &endpoint = 
      conn.internalEndpoint[endpointRef];
    
    CIAO_DEBUG (6, (LM_DEBUG, CLINFO
                    "Connection_Handler::connect_publisher - "
                    "Connecting connection <%C> on instance <%C>\n",
                    conn.name.in (),
                    plan.instance[endpoint.instanceRef].name.in ()));
    
    ::CORBA::Object_var consumer;

    if (!(provided_reference >>= CORBA::Any::to_object  (consumer)) ||
        CORBA::is_nil (consumer.in ()))
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Connection_Handler::connect_publisher - "
                        "Unable to extract provided reference to CORBA::Object\n",
                        plan.connection[connectionRef].name.in ()));
        throw ::Deployment::InvalidConnection (plan.connection[connectionRef].name.in (),
                                               "Unable to extract provided reference to CORBA Object.");
      }

    Components::EventConsumerBase_var event = 
      Components::EventConsumerBase::_unchecked_narrow (consumer);
        
    ::Components::CCMObject_var publisher = 
        DEPLOYMENT_STATE::instance ()->fetch_component (plan.instance[endpoint.instanceRef].name.in ());
    
    if (CORBA::is_nil (publisher))
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Connection_Handler::connect_publisher - "
                        "While connecting <%C>:"
                        "Publishing component not deployed.",
                        plan.connection[connectionRef].name.in ()));
        throw ::Deployment::InvalidConnection (conn.name.in (),
                                               "Publisher component not deployed.");
      }
    
    Components::Cookie_var cookie = publisher->subscribe (endpoint.portName.in (),
                                                       event.in ());
    
    CIAO_DEBUG (5, (LM_INFO, CLINFO
                    "Connection_Handler::connect_publisher - "
                    "Connection <%C> successfully established.\n",
                    conn.name.in ()));
    
    this->cookies_[conn.name.in ()] = CONNECTION_INFO (cookie,
                                                       ::Components::CCMObject::_duplicate (publisher.in ()));
  }
  
  void  
  Connection_Handler::connect_consumer (const ::Deployment::DeploymentPlan & plan,
                                        ::CORBA::ULong connectionRef,
                                        ::CORBA::ULong endpointRef,
                                        const ::CORBA::Any &)
  {
    CIAO_TRACE ("Connection_Handler::connect_subscriber");
    
    const ::Deployment::PlanConnectionDescription &conn = 
      plan.connection[connectionRef];
    const ::Deployment::PlanSubcomponentPortEndpoint &endpoint = 
      conn.internalEndpoint[endpointRef];
    
    CIAO_DEBUG (6, (LM_DEBUG, CLINFO
                    "Connection_Handler::connect_consumer - "
                    "Connecting connection <%C> on instance <%C>\n",
                    conn.name.in (),
                    plan.instance[endpoint.instanceRef].name.in ()));
    
    throw ::Deployment::InvalidConnection (conn.name.in (),
                                           "ExternalReference connection of consumers "
                                           "currently unsupported.\n");
  }

  void 
  Connection_Handler::connect_emitter (const ::Deployment::DeploymentPlan & plan,
                                       ::CORBA::ULong connectionRef,
                                       ::CORBA::ULong endpointRef,
                                       const ::CORBA::Any & provided_reference)
  {
    CIAO_TRACE ("Connection_Handler::connect_emitter");
    
    const ::Deployment::PlanConnectionDescription &conn = 
      plan.connection[connectionRef];
    const ::Deployment::PlanSubcomponentPortEndpoint &endpoint = 
      conn.internalEndpoint[endpointRef];
    
    CIAO_DEBUG (6, (LM_DEBUG, CLINFO
                    "Connection_Handler::connect_emitter - "
                    "Connecting connection <%C> on instance <%C>\n",
                    conn.name.in (),
                    plan.instance[endpoint.instanceRef].name.in ()));
    
    ::CORBA::Object_var consumer;

    if (!(provided_reference >>= CORBA::Any::to_object  (consumer)) ||
        CORBA::is_nil (consumer.in ()))
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Connection_Handler::connect_emitter - "
                        "Unable to extract provided reference to CORBA::Object\n",
                        plan.connection[connectionRef].name.in ()));
        throw ::Deployment::InvalidConnection (plan.connection[connectionRef].name.in (),
                                               "Unable to extract provided reference to CORBA Object.");
      }

    Components::EventConsumerBase_var event = 
      Components::EventConsumerBase::_unchecked_narrow (consumer);
        
    ::Components::CCMObject_var emitter = 
        DEPLOYMENT_STATE::instance ()->fetch_component (plan.instance[endpoint.instanceRef].name.in ());
    
    if (CORBA::is_nil (emitter))
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Connection_Handler::connect_emitter - "
                        "While connecting <%C>:"
                        "Emitting component not deployed.",
                        plan.connection[connectionRef].name.in ()));
        throw ::Deployment::InvalidConnection (conn.name.in (),
                                               "Emitting component not deployed.");
      }
    
    emitter->connect_consumer (endpoint.portName.in (),
                               event.in ());
    
    CIAO_DEBUG (5, (LM_INFO, CLINFO
                    "Connection_Handler::connect_emitter - "
                    "Connection <%C> successfully established.\n",
                    conn.name.in ()));
    
    ::Components::Cookie_var nil_cookie;

    this->cookies_[conn.name.in ()] = CONNECTION_INFO (nil_cookie,
                                                       ::Components::CCMObject::_duplicate (emitter.in ()));    
  }
  
  void
  Connection_Handler::disconnect_facet (const ::Deployment::DeploymentPlan &,
                                        ::CORBA::ULong)
                                        
  {
    CIAO_TRACE ("Connection_Handler::disconnect_facet");
  }
  
  void  
  Connection_Handler::disconnect_receptacle (const ::Deployment::DeploymentPlan &,
                                             ::CORBA::ULong)
                                             
  {
    CIAO_TRACE ("Connection_Handler::disconnect_receptacle");
  }
    
  void  
  Connection_Handler::disconnect_publisher (const ::Deployment::DeploymentPlan &,
                                            ::CORBA::ULong)
                                            
  {
    CIAO_TRACE ("Connection_Handler::disconnect_publisher");
  }
  
  void  
  Connection_Handler::disconnect_subscriber (const ::Deployment::DeploymentPlan &,
                                             ::CORBA::ULong)
                                             
  {
    CIAO_TRACE ("Connection_Handler::disconnect_subscriber");
  }
  
  void
  Connection_Handler::connect_local_port (const char *facet_id,
                                          const char *facet_port,
                                          const char *receptacle_id,
                                          const char *receptacle_port)
  {
    CIAO_TRACE ("Connection_Handler::connect_local_port");
    
    const char *facet_cont = 
      DEPLOYMENT_STATE::instance ()->instance_to_container (facet_id);
    const char *recep_cont = 
      DEPLOYMENT_STATE::instance ()->instance_to_container (receptacle_id);
    
    if (facet_cont && recep_cont &&
        ACE_OS::strcmp (facet_cont, recep_cont) != 0)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Connection_Handler::connect_local_port - "
                        "Ports <%C> and <%C> participate in local facet/receptacle connection, "
                        "but are installed in differing containers <%C> and <%C>\n",
                        facet_id,
                        receptacle_id,
                        facet_cont,
                        recep_cont));
        throw ::Deployment::InvalidConnection (facet_id,
                                               "Component instance participates in a local connection with "
                                               "a non-local entity.");
      }
    
    CIAO::Container_var cont = 
      DEPLOYMENT_STATE::instance ()->fetch_container (facet_cont);
    
    Components::CCMObject_var 
      facet = DEPLOYMENT_STATE::instance ()->fetch_component (facet_id),
      receptacle = DEPLOYMENT_STATE::instance ()->fetch_component (receptacle_id);
    
    cont->connect_local_facet (facet,
                               facet_port,
                               receptacle,
                               receptacle_port);
    CIAO_DEBUG (5, (LM_INFO, CLINFO
                    "Connection_Handler::connect_local_port - "
                    "Connected local port <%C>:<%C> to <%C>:<%C>\n",
                    facet_id, facet_port,
                    receptacle_id, receptacle_port));
  }
  
  bool
  Connection_Handler::is_local_facet (const ::Deployment::PlanConnectionDescription &conn)
  {
    CIAO_TRACE ("Connection_Handler::is_local_facet");
    Deployment::Requirements const& deploy_req = conn.deployRequirement;
    for (CORBA::ULong i = 0; i < deploy_req.length (); i++)
      {
        if (ACE_OS::strcmp (deploy_req[i].name, "edu.dre.vanderbilt.DAnCE.ConnectionType") == 0 &&
            ACE_OS::strcmp (deploy_req[i].resourceType, "Local_Interface") == 0)
          {
            return true;
          }
      }
  return false;    
  }
}
