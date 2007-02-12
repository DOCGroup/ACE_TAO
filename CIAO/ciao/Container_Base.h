// -*- C++ -*-

//=============================================================================
/**
 *  @file    Container_Base.h
 *
 *  $Id$
 *
 *  Header file for CIAO's container implementations
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 *  @author Gan Deng <dengg@dre.vanderbilt.edu>
 */
//=============================================================================


#ifndef CIAO_CONTAINER_BASE_H
#define CIAO_CONTAINER_BASE_H

#include /**/ "ace/pre.h"

#include "ciao/CCM_ContainerC.h"
#include "ciao/Deployment_CoreC.h"
#include "ciao/CIAO_Server_Export.h"

#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/Servant_Base.h"

#include "tao/ORB.h"

#include "ace/Hash_Map_Manager_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace CIAO
{
  class Servant_Activator;
  class Dynamic_Component_Servant_Base;
  class Container_Impl;

  /**
   * @class Container
   *
   * @brief Common container interface definition.
   *
   * Perhaps we can use local interface to define these interfaces as
   * we will also get reference counting automatically.
   */
  class CIAO_SERVER_Export Container
  {
  public:
    enum OA_Type
      {
        Component,
        Facet_Consumer
      };

    Container (CORBA::ORB_ptr o);
    Container (CORBA::ORB_ptr o, Container_Impl *container_impl);

    virtual ~Container (void) = 0;

    /// Get component's POA.
    /**
     * This operation does *NOT* increase the reference count of the
     * POA. Look at the const qualifier in the method.
     */
    PortableServer::POA_ptr the_POA (void) const;
    PortableServer::POA_ptr the_facet_cons_POA (void) const;

    /// Get a reference to the underlying ORB.
    CORBA::ORB_ptr the_ORB (void) const;

    /// Initialize the container with a name.
    virtual int init (const char *name = 0,
                      const CORBA::PolicyList *more_policies = 0)
      = 0;

    /// Install a new home
    virtual Components::CCMHome_ptr ciao_install_home (
        const char *exe_dll_name,
        const char *exe_entrypt,
        const char *sv_dll_name,
        const char *sv_entrypt,
        const char *ins_name
      )
      = 0;

    // Uninstall a servant for component or home.
    virtual void ciao_uninstall_home (Components::CCMHome_ptr homeref)
      = 0;

    // Uninstall a servant for component.
    virtual void uninstall_component (::Components::CCMObject_ptr objref,
                                      PortableServer::ObjectId_out oid)
      = 0;

    // @@Jai, please see the Session Container class for comments.
    // @@ Jai, do you really need the environment variable?
    virtual void add_servant_map (PortableServer::ObjectId &oid,
                                  Dynamic_Component_Servant_Base* servant) = 0;

    virtual void delete_servant_map (PortableServer::ObjectId &oid) = 0;

    virtual CORBA::Object_ptr get_home_objref (PortableServer::Servant p)
      = 0;

  protected:
    /// Reference to the ORB
    CORBA::ORB_var orb_;

    /// POA within which all the components in this container will be
    /// activated.
    PortableServer::POA_var component_poa_;

    /// POA within which all the facets and receptacles will be
    /// activated.
    /**
     * Having two POA's allows us to associate different policies that
     * are distinct from the component.
     */
    PortableServer::POA_var facet_cons_poa_;

    PortableServer::POA_var home_servant_poa_;
    Container_Impl *container_impl_;
  private:
    /// Not allowed to be used
    Container (void);
  };

  class Session_Container;

  typedef ::Components::HomeExecutorBase_ptr (*HomeFactory) (void);
  typedef ::PortableServer::Servant (*ServantFactory)
    (::Components::HomeExecutorBase_ptr p,
     ::CIAO::Session_Container *c,
     const char *ins_name
#if !defined (TAO_HAS_EXCEPTIONS) || defined (ACE_ENV_BKWD_COMPAT)
    , CORBA::Environment &
#endif
    );

  typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                  HomeFactory,
                                  ACE_Hash<ACE_CString>,
                                  ACE_Equal_To<ACE_CString>,
                                  ACE_Null_Mutex> HOMECREATOR_FUNCPTR_MAP;

  typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                  ServantFactory,
                                  ACE_Hash<ACE_CString>,
                                  ACE_Equal_To<ACE_CString>,
                                  ACE_Null_Mutex> HOMESERVANTCREATOR_FUNCPTR_MAP;

  struct Static_Config_EntryPoints_Maps
  {
    /// Map of home creator entry point name and func ptr
    HOMECREATOR_FUNCPTR_MAP* home_creator_funcptr_map_;

    /// Map of home servant creator entry point name and func ptr
    HOMESERVANTCREATOR_FUNCPTR_MAP* home_servant_creator_funcptr_map_;
  };

  class CIAO_SERVER_Export Session_Container : public Container
  {
  public:
    Session_Container (CORBA::ORB_ptr o,
                       Container_Impl *container_impl,
                       bool static_config_flag = false,
                       const Static_Config_EntryPoints_Maps* static_entrypts_maps =0);

    virtual ~Session_Container (void);

    /// Initialize the container with a name.
    virtual int init (const char *name = 0,
                      const CORBA::PolicyList *more_policies = 0)
;

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
    virtual Components::CCMHome_ptr ciao_install_home (
        const char *exe_dll_name,
        const char *exe_entrypt,
        const char *sv_dll_name,
        const char *sv_entrypt,
        const char *ins_name
      );

    /// Uninstall a servant for component or home.
    virtual void ciao_uninstall_home (Components::CCMHome_ptr homeref);

    /// Uninstall a servant for component.
    virtual void uninstall_component (::Components::CCMObject_ptr objref,
                                      PortableServer::ObjectId_out oid);

    /// Install a servant for component or home.
    virtual CORBA::Object_ptr install_servant (PortableServer::Servant p,
                                       Container::OA_Type t);

    /// Install a component servant.
    CORBA::Object_ptr install_component (PortableServer::Servant p,
                                         PortableServer::ObjectId_out oid);

    /// Get an object reference to a component or home from the servant.
    virtual CORBA::Object_ptr get_objref (PortableServer::Servant p);

    /// Uninstall a servant for component or home.
    void uninstall (CORBA::Object_ptr objref,
                    Container::OA_Type t);

    /// Uninstall a servant for component or home.
    void uninstall (PortableServer::Servant svt,
                    Container::OA_Type t);

    // @@Jai, please consider naming this method as
    // "add_servant_to_map ()" to be more descriptive.
    virtual void add_servant_map (PortableServer::ObjectId &oid,
                                  Dynamic_Component_Servant_Base* servant);

    // @@Jai, please consider naming this method as
    // "delete_servant_from_map ()" to be more descriptive.
    virtual void delete_servant_map (PortableServer::ObjectId &oid);

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
                                          Container::OA_Type t);

    /// Return the servant activator factory that activates the
    /// servants for facets and consumers.
    Servant_Activator *ports_servant_activator (void) const;

  private:

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
    unsigned long number_;

    /// Static variable to store the highest number we have given out until
    /// now
    static ACE_Atomic_Op <ACE_SYNCH_MUTEX, unsigned long> serial_number_;

    const bool static_config_flag_;
    const Static_Config_EntryPoints_Maps* static_entrypts_maps_;

    /// The servant activator factory used to activate facets and
    /// consumer servants.
    Servant_Activator *sa_;
  };
}

// Macro for registration of an OBV factory in the generated
// servant class. Similar to the macro for TAO in
// tao/ValueType/ValueFactory.h but here we take advantage of
// the fact that we have access to the current ORB indirectly
// through the context and container.
#define CIAO_REGISTER_OBV_FACTORY(FACTORY, VALUETYPE) \
  {  \
    CORBA::ValueFactory factory = new FACTORY; \
    CORBA::ORB_ptr orb = \
      this->context_->_ciao_the_Container ()->the_ORB (); \
    CORBA::ValueFactory prev_factory = \
      orb->register_value_factory ( \
               VALUETYPE::_tao_obv_static_repository_id (), \
               factory); \
    CORBA::remove_ref (prev_factory); \
    CORBA::add_ref (factory);  \
  }

#if defined (__ACE_INLINE__)
# include "Container_Base.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* CIAO_CONTAINER_BASE_H */
