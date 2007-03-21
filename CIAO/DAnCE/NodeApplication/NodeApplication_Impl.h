//$Id$

/**========================================================
 *
 * @file   NodeApplication_Impl.h
 *
 * @Brief  This file contains the implementation of
 *         the NodeApplication interface.
 *
 * @author Tao Lu <lu@dre.vanderbilt.edu>
 * @author Gan Deng <dengg@dre.vanderbilt.edu>
 *========================================================*/

#ifndef NODEAPPLICATION_IMPL_H
#define NODEAPPLICATION_IMPL_H
#include /**/ "ace/pre.h"

#include "CIAO_NodeApplication_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Synch.h"
#include "ace/Synch_Traits.h"
#include "ace/SString.h"
#include "ace/Hash_Map_Manager_T.h"
#include "tao/ORB.h"
#include "DAnCE/Deployment/Deployment_NodeApplicationS.h"
#include "ciao/Server_init.h"
#include "ciao/CIAO_common.h"
#include "ciao/Object_Set_T.h"
#include "ciaosvcs/Events/CIAO_EventService_Factory_impl.h"
#include "ciaosvcs/Events/CIAO_Events_Base/CIAO_EventsS.h"

#include "NodeApp_Configurator.h"
#include "Session_Container.h"

using CIAO::Utility::write_IOR;

/**
 *
 * @class NodeApplication_Impl
 *
 * @brief This class implements the NodeApplication interface.
 * This interface is semantically very simillar to container
 * in the old DnC spec. However this class will also be used
 * as a Server for hosting home/component. This way we reduce the
 * complexity of the framework by omitting the componentserver layer.
 *
 * @@TODO add configuration capabilities. Threading is one of them.
 *
 * @@Assumptions:
 * 1. Now the implementation is not thread safe.
 * // @@Gan, the above assumption is _really_ bad. Could you please
 * use the lock in the imeplementation to do some simple
 * prootections.
 **/

namespace CIAO
{
  // @@ Gan, as we discussed before can you please wrap this
  // implementation in a namespace Node_Application or whatever to
  // signify that it belongs to another software piece of CIAO?
  class NODEAPPLICATION_Export NodeApplication_Impl
    : public virtual POA_Deployment::NodeApplication
  {
  public:
    enum Component_State
    {
      NEW_BORN, PRE_ACTIVE, ACTIVE, POST_ACTIVE, PASSIVE, DEACTIVATED
    };

    typedef struct _component_state_info
    {
      Components::CCMObject_var objref_;
      Component_State state_;
    } Component_State_Info;

    NodeApplication_Impl (CORBA::ORB_ptr o,
                          PortableServer::POA_ptr p,
                          NodeApp_Configurator &c,
                          const Static_Config_EntryPoints_Maps* static_entrypts_maps =0);

    /// Default destructor.
    virtual ~NodeApplication_Impl (void);

    /**
     * @brief This operation dose 2 things.
     *        1. Get the external connction (facet and Event source)
     *           and connect them to the local receptacle/event sink.
     *        2. If the start augment is true, start the Components.
     * @Note:
     * The connection containes the object ref of the provided object
     * reference (facet/event consumer) of components from other NodeApplications.
     * However the name field stores the name of the port on the local component.
     */
    virtual void
    finishLaunch (const Deployment::Connections & connections,
                  CORBA::Boolean start,
                  CORBA::Boolean add_connection);

    virtual void start ();

    /*-------------  CIAO specific IDL operations (idl)----------
     *
     *-----------------------------------------------------------*/

    virtual void ciao_preactivate ();

    virtual void ciao_postactivate ();

    virtual void ciao_passivate ();

    /// Initialize the NodeApplication
    virtual CORBA::Long init ();

    /// Start install homes and components.
    virtual ::Deployment::ComponentInfos *
      install (const ::Deployment::NodeImplementationInfo & node_impl_info);

    /// Install a number of CIAO_Event_Service objects within the NA
    virtual ::CIAO::CIAO_Event_Service *
      install_es (const ::CIAO::DAnCE::EventServiceDeploymentDescription & es_info);

    /// Get the object reference of the NodeApplicationManager.
    /// This might come in handy later.
    virtual ::CORBA::Object_ptr get_node_application_manager ();

    /// Access the readonly attribute.
    virtual ::Deployment::Properties * properties ();

    /// Remove a component instance from the NodeApplication
    virtual void remove_component (const char * inst_name);

    virtual void activate_component (const char * name);

    virtual void passivate_component (const char * name);

    /// Remove everything inside including all components and homes.
    virtual void remove ();

    /// Create a container interface, which will be hosted in this NodeApplication.
    virtual ::Deployment::Container_ptr
      create_container (const ::Deployment::Properties &properties);

    /// Remove a container interface.
    virtual void remove_container (::Deployment::Container_ptr cref);

    /// Get all container object refs
    virtual ::Deployment::Containers * get_containers ();

    /*-------------  CIAO specific helper functions (C++)---------
     *
     *-----------------------------------------------------------*/

    /// Get the containing POA.  This operation does *not*
    /// increase the reference count of the POA.
    virtual PortableServer::POA_ptr _default_POA (void);

    /// Return the cached object reference of this NodeApplication object.
    /// This operation does *NOT* increase the reference count.
    ::Deployment::NodeApplication_ptr
    get_objref ();

    /*------- CIAO helper functions for pub/sub service -------
     *
     *--------------------------------------------------------*/

    /// Set up a connection using the CIAO_Event_Service, which
    /// is available as a field in the <Deployment::Connection>
    /// struct type.
    /// If <add_or_remove> input parameter is true, then we will
    /// add the event connection, otherwise we will remove the
    /// event connection.
    void build_event_connection (
        const Deployment::Connection & connection,
        bool add_or_remove);

  protected:
    /// If <add_connection> is "false", then we shall "remove"
    /// the connections, otherwise we will add these connections.
    virtual void
    finishLaunch_i (const Deployment::Connections & connections,
                    CORBA::Boolean start,
                    CORBA::Boolean add_connection);
    virtual void
    handle_facet_receptable_connection (
        Components::CCMObject_ptr comp,
        const Deployment::Connection & connection,
        CORBA::Boolean add_connection);

    virtual void
    handle_emitter_consumer_connection (
        Components::CCMObject_ptr comp,
        const Deployment::Connection & connection,
        CORBA::Boolean add_connection);

    virtual void
    handle_publisher_consumer_connection (
        Components::CCMObject_ptr comp,
        const Deployment::Connection & connection,
        CORBA::Boolean add_connection);

    virtual bool
    _is_es_consumer_conn (Deployment::Connection conn);

    virtual bool
    _is_publisher_es_conn (Deployment::Connection conn);

    /// Register the publisher to the CIAO event service
    /// The only fields of <connection> struct used in this method
    /// are: <type>, <event_service>, <instanceName>, <portName>.
    virtual void
    handle_publisher_es_connection (
        Components::CCMObject_ptr comp,
        const Deployment::Connection & connection,
        CORBA::Boolean add_connection);

    /// Register the consumer to the CIAO event service
    virtual void
    handle_es_consumer_connection (
        const Deployment::Connection & connection,
        CORBA::Boolean add_connection);

    /// Create and initialize all the containers
    virtual CORBA::Long create_all_containers (
        const ::Deployment::ContainerImplementationInfos & container_infos);

    /// Create a "key" for the connection
    virtual ACE_CString *
    create_connection_key (const Deployment::Connection & connection);

    /// To build a map between a component instance and  its container
    typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                    Deployment::Container_var,
                                    ACE_Hash<ACE_CString>,
                                    ACE_Equal_To<ACE_CString>,
                                    ACE_Null_Mutex> Component_Container_Map;
    typedef Component_Container_Map::iterator Component_Container_Iterator;
    Component_Container_Map component_container_map_;


    /// To store all created Component objects as well as their lifecycle
    /// states..
    typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                    Component_State_Info,
                                    ACE_Hash<ACE_CString>,
                                    ACE_Equal_To<ACE_CString>,
                                    ACE_Null_Mutex> CCMComponent_Map;
    typedef CCMComponent_Map::iterator Component_Iterator;
    CCMComponent_Map component_state_map_;

    /// A Map which stores all the connection cookies
    typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                    ::Components::Cookie_var,
                                    ACE_Hash<ACE_CString>,
                                    ACE_Equal_To<ACE_CString>,
                                    ACE_Null_Mutex> Cookie_Map;
    typedef Cookie_Map::iterator Cookie_Map_Iterator;
    Cookie_Map cookie_map_;

    /// Synchronize access to the object set.
    TAO_SYNCH_MUTEX lock_;

    /// Keep a list of managed Container objects.
    Object_Set<Deployment::Container, Deployment::Container_var> container_set_;

    /// Keep a pointer to the managing ORB serving this servant.
    CORBA::ORB_var orb_;

    /// Keep a pointer to the managing POA.
    // @@Gan/Jai, which POA is this? Same as the component POA or a
    // different one. My sense is that its different. Could you please
    //document it?
    PortableServer::POA_var poa_;

    // Configurator for allocating NodeApp resources and policies
    NodeApp_Configurator &configurator_;

    /// Cached properties
    Deployment::Properties properties_;

    /// And a reference to the NodeApplicationManager that created us.
    ::CORBA::Object_var node_app_manager_;

    /// Cache the object reference (of ourselves).
    ::Deployment::NodeApplication_var objref_;

    /// A factory to create CIAO event services
    EventService_Factory_impl es_factory_;

    /// Cache the (NA specific) installation info of all the
    /// CIAO_Event_Services
    typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                    DAnCE::EventServiceDeploymentDescriptions_var,
                                    ACE_Hash<ACE_CString>,
                                    ACE_Equal_To<ACE_CString>,
                                    ACE_Null_Mutex> ES_Installation_Map;
    typedef ES_Installation_Map::iterator ES_Installation_Map_Iterator;
    ES_Installation_Map es_info_map_;

    const Static_Config_EntryPoints_Maps* static_entrypts_maps_;
  private:
    /// Default constructor, noop
    NodeApplication_Impl(void);
  };
}

#if defined (__ACE_INLINE__)
# include "NodeApplication_Impl.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* NODEAPPLICATION_IMPL_H */
