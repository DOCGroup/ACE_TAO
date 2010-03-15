// -*- C++ -*-

//=============================================================================
/**
 *  @file    Session_Container.h
 *
 *  $Id$
 *
 *  Header file for CIAO's container implementations
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 *  @author Gan Deng <dengg@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef CIAO_SESSION_CONTAINER_H
#define CIAO_SESSION_CONTAINER_H

#include /**/ "ace/pre.h"

#include "ciao/Containers/Session/Session_Container_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/Servant_Base.h"
#include "ciao/Containers/Container_Base.h"

namespace CIAO
{
  class Session_Container;
  class Servant_Activator;

  namespace Deployment
  {
    class CIAO_Container_i;
  }

  typedef ::Components::HomeExecutorBase_ptr (*HomeFactory) (void);
  typedef ::PortableServer::Servant (*HomeServantFactory) (::Components::HomeExecutorBase_ptr p,
                                                           ::CIAO::Container_ptr c,
                                                           const char *ins_name);
  typedef ::Components::EnterpriseComponent_ptr (*ComponentFactory) (void);
  typedef ::PortableServer::Servant (*ComponentServantFactory) (::Components::EnterpriseComponent_ptr,
                                                                ::CIAO::Container_ptr,
                                                                const char *);

  typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                  HomeFactory,
                                  ACE_Hash<ACE_CString>,
                                  ACE_Equal_To<ACE_CString>,
                                  ACE_Null_Mutex>
    HOMECREATOR_FUNCPTR_MAP;

  typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                  HomeServantFactory,
                                  ACE_Hash<ACE_CString>,
                                  ACE_Equal_To<ACE_CString>,
                                  ACE_Null_Mutex>
  HOMESERVANTCREATOR_FUNCPTR_MAP;

  typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                  ComponentFactory,
                                  ACE_Hash<ACE_CString>,
                                  ACE_Equal_To<ACE_CString>,
                                  ACE_Null_Mutex>
    COMPONENTCREATOR_FUNCPTR_MAP;

  typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                  ComponentServantFactory,
                                  ACE_Hash<ACE_CString>,
                                  ACE_Equal_To<ACE_CString>,
                                  ACE_Null_Mutex>
  COMPONENTSERVANTCREATOR_FUNCPTR_MAP;


  struct SESSION_CONTAINER_Export Static_Config_EntryPoints_Maps
  {
    /// Map of home creator entry point name and func ptr
    HOMECREATOR_FUNCPTR_MAP* home_creator_funcptr_map_;

    /// Map of home servant creator entry point name and func ptr
    HOMESERVANTCREATOR_FUNCPTR_MAP* home_servant_creator_funcptr_map_;

    /// Map of home creator entry point name and func ptr
    COMPONENTCREATOR_FUNCPTR_MAP* component_creator_funcptr_map_;

    /// Map of home servant creator entry point name and func ptr
    COMPONENTSERVANTCREATOR_FUNCPTR_MAP* component_servant_creator_funcptr_map_;
  };

  class SESSION_CONTAINER_Export Session_Container : public Container_i
  {
  public:
    Session_Container (CORBA::ORB_ptr o,
                       PortableServer::POA_ptr poa,
                       Deployment::CIAO_Container_i *container_impl,
                       bool static_config_flag = false,
                       const Static_Config_EntryPoints_Maps* static_entrypts_maps =0,
                       const char *name = 0,
                       const CORBA::PolicyList *more_policies = 0);

    virtual ~Session_Container (void);

    /**
     * @brief Simply installing a home executor into the component.
     *
     * This operation install a home executor into the component.  It
     * requires the name of the DLLs to executor and the servant glue
     * code, and the entry points to the respective DLLs.  Currently,
     * we don't try to manage the lifetime of DLL objects, but we
     * should at some later point.
     *
     * @retval Home objref of the installed home.
     */
    /// Install a new home
    virtual Components::CCMHome_ptr install_home (const char *primary_artifact,
                                                  const char *entry_point,
                                                  const char *servant_artifact,
                                                  const char *servant_entrypoint,
                                                  const char *name);

    virtual void uninstall_home (Components::CCMHome_ptr homeref);

    virtual Components::CCMObject_ptr install_component (const char *primary_artifact,
                                                         const char *entry_point,
                                                         const char *servant_artifact,
                                                         const char *servant_entrypoint,
                                                         const char *name);
    
    virtual void connect_local_facet (::Components::CCMObject_ptr provider,
                                      const char * provider_port,
                                      ::Components::CCMObject_ptr user,
                                      const char * user_port);
    
    virtual void disconnect_local_facet (::Components::CCMObject_ptr provider,
                                         const char * provider_port,
                                         ::Components::CCMObject_ptr user,
                                         const char * user_port);

    /// Activate component
    virtual void activate_component (Components::CCMObject_ptr compref);

    virtual void passivate_component (Components::CCMObject_ptr compref);

    virtual void uninstall_component (Components::CCMObject_ptr compref);

    /// Uninstall a servant
    virtual void uninstall_servant (PortableServer::Servant objref,
                                    Container_Types::OA_Type type,
                                    PortableServer::ObjectId_out oid);

    /// Install a servant for component or home.
    virtual CORBA::Object_ptr install_servant (PortableServer::Servant p,
                                               Container_Types::OA_Type type,
                                               PortableServer::ObjectId_out oid);

    /// Get an object reference to a component or home from the servant.
    virtual CORBA::Object_ptr get_objref (PortableServer::Servant p);

    /// Uninstall a servant for component or home.
    void uninstall (CORBA::Object_ptr objref, Container_Types::OA_Type t);

    /// Uninstall a servant for component or home.
    void uninstall (PortableServer::Servant svt, Container_Types::OA_Type t);

    virtual void add_servant_to_map (PortableServer::ObjectId &oid,
                                     Dynamic_Component_Servant_Base* servant);

    virtual void delete_servant_from_map (PortableServer::ObjectId &oid);

    // @@Jai, could yo please add documentation?
    /*
     * @@Jai, you may want to consider moving these away from the
     * container interface. I know what you are going to say
     * :-). Consider using dynamic_cast <> to access
     * add_servant_to_map, delete_servant_from_map and
     * deactivate_facet from the Swapping_Conatiner's interface. It
     * would make the base container interface much cleaner.
     */
    virtual void deactivate_facet (const PortableServer::ObjectId &oid);

    virtual CORBA::Object_ptr get_home_objref (PortableServer::Servant p);

    /// Analog of the POA method that creates an object reference from
    /// an object id string.
    CORBA::Object_ptr generate_reference (const char *obj_id,
                                          const char *repo_id,
                                          Container_Types::OA_Type t);

    /// Return the servant activator factory that activates the
    /// servants for facets and consumers.
    ::CIAO::Servant_Activator_ptr ports_servant_activator (void);

  private:

    /// Initialize the container with a name.
    virtual void init (const char *name = 0,
                       const CORBA::PolicyList *more_policies = 0);

    /// Create POA for the component.
    /**
     * This is the POA that is returned to the component applications
     * if they need one.
     */
    void create_component_POA (const char *name,
                               const CORBA::PolicyList *p,
                               PortableServer::POA_ptr root);

    /// Create POA for the facets and consumers alone.
    void create_facet_consumer_POA (const char *name,
                                    const CORBA::PolicyList *p,
                                    PortableServer::POA_ptr root);

    /// Not allowed to be
    Session_Container (void);

  protected:
    /// Static variable to store the highest number we have given out until
    /// now
    static ACE_Atomic_Op <TAO_SYNCH_MUTEX, unsigned long> serial_number_;

    bool const static_config_flag_;
    const Static_Config_EntryPoints_Maps* static_entrypts_maps_;

    /// The servant activator factory used to activate facets and
    /// consumer servants.
    Servant_Activator_var sa_;
  };
}

#if defined (__ACE_INLINE__)
# include "Session_Container.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* CIAO_SESSION_CONTAINER_H */
