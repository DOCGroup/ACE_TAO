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

#include "ciao/CIAO_Server_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ciao/Container_Base.h"

#include "tao/PortableServer/Servant_Base.h"

namespace CIAO
{
  class Session_Container;
  class Servant_Activator;

  typedef ::Components::HomeExecutorBase_ptr (*HomeFactory) (void);
  typedef ::PortableServer::Servant (*ServantFactory) (
    ::Components::HomeExecutorBase_ptr p,
    ::CIAO::Session_Container *c,
    const char *ins_name);

  typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                  HomeFactory,
                                  ACE_Hash<ACE_CString>,
                                  ACE_Equal_To<ACE_CString>,
                                  ACE_Null_Mutex>
    HOMECREATOR_FUNCPTR_MAP;

  typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                  ServantFactory,
                                  ACE_Hash<ACE_CString>,
                                  ACE_Equal_To<ACE_CString>,
                                  ACE_Null_Mutex>
    HOMESERVANTCREATOR_FUNCPTR_MAP;

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
                      const CORBA::PolicyList *more_policies = 0);

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
      const char *ins_name);

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
    void uninstall (CORBA::Object_ptr objref, Container::OA_Type t);

    /// Uninstall a servant for component or home.
    void uninstall (PortableServer::Servant svt, Container::OA_Type t);

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
    static ACE_Atomic_Op <TAO_SYNCH_MUTEX, unsigned long> serial_number_;

    const bool static_config_flag_;
    const Static_Config_EntryPoints_Maps* static_entrypts_maps_;

    /// The servant activator factory used to activate facets and
    /// consumer servants.
    Servant_Activator *sa_;
  };
}

#if defined (__ACE_INLINE__)
# include "Session_Container.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* CIAO_SESSION_CONTAINER_H */
