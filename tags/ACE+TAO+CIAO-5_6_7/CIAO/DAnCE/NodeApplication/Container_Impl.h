// -*- C++ -*-

//=============================================================================
/**
 *  @file    Container_Impl.h
 *
 *  $Id$
 *
 *  This file contains implementation for the servant of
 *  Components::Deployment::Container interface.
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 *  @author Gan Deng <gan.deng@vanderbilt.edu>
 */
//=============================================================================


#ifndef CIAO_CONTAINER_IMPL_H
#define CIAO_CONTAINER_IMPL_H
#include /**/ "ace/pre.h"

#include "CIAO_NodeApplication_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "DAnCE/Deployment/Deployment_ContainerS.h"
#include "DAnCE/Deployment/Deployment_NodeApplicationC.h"
#include "DAnCE/Deployment//DeploymentC.h"
#include "ace/SString.h"
#include "ace/Auto_Ptr.h"
#include "ciao/Server_init.h"  // write_IOR function & NameUtility
#include "ciao/CIAO_common.h" // CIAO::debug_level
#include "ciao/Session_Container.h"
#include "NodeApp_Configurator.h"

namespace CIAO
{
  /**
   * @class Container_Impl
   *
   * @brief Servant implementation for the interface Deployment::Container
   *
   * This class implements the Deployment::Container
   * interface which is not defined by the CCM DnC specification.
   * As the interface implies, this is actually part of the deployment
   * interface and is used to manage the lifecycle of the installed
   * components and homes.
   */
  class NODEAPPLICATION_Export Container_Impl
    : public virtual POA_Deployment::Container
  {
  public:
    /// Constructor
    Container_Impl (CORBA::ORB_ptr o,
                    PortableServer::POA_ptr p,
                    ::Deployment::NodeApplication_ptr server,
                    NodeApp_Configurator &c,
                    const Static_Config_EntryPoints_Maps* static_entrypts_maps =0);

    /// Destructor
    virtual ~Container_Impl (void);

    /*-------------------------------------------------------------*/
    /*--------------------  IDL operations (idl) ------------------*/

    /// Initialize the container.
    virtual CORBA::Long init (const CORBA::PolicyList *policies);

    /// Install all homes and components
    Deployment::ComponentInfos *
      install (const ::Deployment::ContainerImplementationInfo & container_impl_info);

    /// Remove all homes and components
    virtual void remove ();

    /// Deployment::Container interface defined attributes/operations.
    virtual ::Deployment::Properties *properties ();

    virtual ::Deployment::NodeApplication_ptr get_node_application ();

    /*-------------------------------------------------------------*/
    /*-------------------  C++ help methods (c++) -----------------*/

    /// Get the containing POA.  This operation does *not*
    /// increase the reference count of the POA.
    virtual PortableServer::POA_ptr _default_POA (void);

    // Install the home of this particular component
    virtual ::Components::CCMHome_ptr
      install_home (const ::Deployment::ComponentImplementationInfo & impl_info);

    /**
     * @@Note: I don't know how to remove a home right now.
     *         I assume that user will only call remove instead.
     *         This is true at least for DnC run time.
     *
     * Right now, in this implementation I assumpe that there will be
     * same number of homes as the components even if the components
     * are of the same type. I don't think that we have the modeling
     * side support of this either. So bear me if you think I avoid
     * the real thinking for easiness.
     */
    // Remove the home of this particular component
    virtual void remove_home (const char * comp_ins_name);

    bool
    register_with_ns (const char * obj_name,
                      CORBA::ORB_ptr orb,
                      Components::CCMObject_ptr obj);

    bool
    unregister_with_ns (const char * obj_name, CORBA::ORB_ptr orb);

    // ------------------- CIAO Internal Operations ------------------------
    // These below two are helper methods to clean up components
    // should only be called when we are sure that there is no
    // active connection on this component.
    virtual void remove_components ();

    virtual void remove_component (const char * comp_ins_name);

    /// Set the cached object reference.
    void set_objref (Deployment::Container_ptr o);

    /// Get the cached object reference.  This operation will invoke
    /// _this if there's no cached reference available.  Notice that
    /// this method does *NOT* increase the reference count of the
    /// cached reference.
    Deployment::Container_ptr get_objref ();

protected:
    /// Keep a pointer to the managing ORB serving this servant.
    CORBA::ORB_var orb_;

    /// Keep a pointer to the managing POA.
    PortableServer::POA_var poa_;

    /// Internal container implementation.
    // @@ Gan/Jai, heard of auto_ptr <>?
    //CIAO::Container *container_;
    ACE_Auto_Ptr<CIAO::Container> container_;

    /// Cached ConfigValues.
    Deployment::Properties properties_;

    /// Cached Container reference (of ourselves.)
    Deployment::Container_var objref_;

    /// Cached NodeApplication.
    Deployment::NodeApplication_var nodeapp_;

    NodeApp_Configurator &configurator_;

    /// To store all created CCMHome object
    typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                    Components::CCMHome_ptr,
                                    ACE_Hash<ACE_CString>,
                                    ACE_Equal_To<ACE_CString>,
                                    ACE_Null_Mutex> CCMHome_Map;
    typedef CCMHome_Map::iterator Home_Iterator;
    CCMHome_Map home_map_;

    /// To store all created Component object.
    // @@Gan, see how this caching is duplicated..
    typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                    Components::CCMObject_var,
                                    ACE_Hash<ACE_CString>,
                                    ACE_Equal_To<ACE_CString>,
                                    ACE_Null_Mutex> CCMComponent_Map;
    typedef CCMComponent_Map::iterator Component_Iterator;
    CCMComponent_Map component_map_;

    typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                    ACE_CString,
                                    ACE_Hash<ACE_CString>,
                                    ACE_Equal_To<ACE_CString>,
                                    ACE_Null_Mutex> CCMNaming_Map;
    CCMNaming_Map naming_map_;

    const Static_Config_EntryPoints_Maps* static_entrypts_maps_;
  };
}

#if defined (__ACE_INLINE__)
# include "Container_Impl.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* CIAO_CONTAINER_IMPL_H */
