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

#include "ciao/Containers/Container_Base_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include <ace/Hash_Map_Manager_T.h>
#include <tao/LocalObject.h>
#include <tao/PortableServer/PortableServer.h>
#include <ccm/CCM_ContainerC.h>

#include "ciao/Containers/Container_BaseC.h"

namespace CIAO
{
  class Dynamic_Component_Servant_Base;
  
  namespace Deployment
  {
    class CIAO_Container_i;
  }

  typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                  CORBA::PolicyList,
                                  ACE_Hash<ACE_CString>,
                                  ACE_Equal_To<ACE_CString>,
                                  ACE_Null_Mutex> REC_POL_MAP;
  typedef ACE_Hash_Map_Iterator<ACE_CString,
                                CORBA::PolicyList,
                                ACE_Null_Mutex> REC_POL_MAP_ITERATOR;

  /**
   * @class Container
   *
   * @brief Common container interface definition.
   *
   * Perhaps we can use local interface to define these interfaces as
   * we will also get reference counting automatically.
   */
  class Container_Base_Export Container_i : 
    public virtual Container,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    Container_i (CORBA::ORB_ptr o);
    Container_i (CORBA::ORB_ptr o, Deployment::CIAO_Container_i *container_impl);

    virtual ~Container_i (void) = 0;

    /// Initialize the container with a name.
    virtual void init (const char *name = 0,
		       const CORBA::PolicyList *more_policies = 0) = 0;

    /// Get component's POA.
    /**
     * This operation does *NOT* increase the reference count of the
     * POA. Look at the const qualifier in the method.
     */
    virtual PortableServer::POA_ptr the_POA (void) const;
    virtual PortableServer::POA_ptr the_facet_cons_POA (void) const;

    /// get the receptacle policy given the receptacle name
    CORBA::PolicyList * get_receptacle_policy (const char *name);

    /// Install a new home
    virtual Components::CCMHome_ptr install_home (const char *primary_artifact,
                                                  const char *entry_point,
                                                  const char *name) = 0;

    // Uninstall a servant for component or home.
    virtual void uninstall_home (Components::CCMHome_ptr homeref) = 0;
    
    virtual Components::CCMObject_ptr install_component (const char *primary_artifact,
                                                     const char *entry_point,
                                                     const char *name) = 0;

    virtual void uninstall_component (Components::CCMObject_ptr compref) = 0;

    virtual CORBA::Object_ptr get_home_objref (PortableServer::Servant p) = 0;

    // Uninstall a servant for component.
    virtual void uninstall_component_servant (::Components::CCMObject_ptr objref,
                                              PortableServer::ObjectId_out oid) = 0;

    virtual void add_servant_to_map (PortableServer::ObjectId &oid,
                                     Dynamic_Component_Servant_Base* servant) = 0;

    virtual void delete_servant_from_map (PortableServer::ObjectId &oid) = 0;

    /// Get a reference to the underlying ORB.
    CORBA::ORB_ptr the_ORB (void) const;

    /// Set the policy map for all the receptacles hosted in this container.
    void set_receptacle_policy_map (::CIAO::REC_POL_MAP &rec_pol_map);

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

    Deployment::CIAO_Container_i *container_impl_;

    ::CIAO::REC_POL_MAP rec_pol_map_;
    
  private:
    /// Not allowed to be used
    Container_i (void);
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
