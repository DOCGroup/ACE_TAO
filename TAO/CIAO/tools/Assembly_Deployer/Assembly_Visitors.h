// -*- C++ -*-

//=============================================================================
/**
 *  @file    Assembly_Visitors.h
 *
 *  $Id$
 *
 *  This file contains implementations for various visitors that
 *  traverse and process CIAO::Assembly_Spec.
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================


#ifndef CIAO_ASSEMBLY_VISITOR_H
#define CIAO_ASSEMBLY_VISITOR_H
#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "CCM_DeploymentS.h"
#include "ace/Hash_Map_Manager_T.h"
#include "../XML_Helpers/Assembly_Spec.h"
#include "Deployment_Configuration.h"

namespace CIAO
{
  /**
   * @typedef Assembly_Context
   *
   * This is a structure for collecting information of a running Assembly.
   */
  typedef struct _assembly_context
  {
    typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                    Components::CCMHome_var,
                                    ACE_Hash<ACE_CString>,
                                    ACE_Equal_To<ACE_CString>,
                                    ACE_Null_Mutex> HOME_MAP;
    HOME_MAP installed_homes_;

    typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                    Components::CCMObject_var,
                                    ACE_Hash<ACE_CString>,
                                    ACE_Equal_To<ACE_CString>,
                                    ACE_Null_Mutex> COMP_MAP;
    COMP_MAP instantiated_components_;

    typedef ACE_Unbounded_Queue<Components::Deployment::ComponentServer_var> SERVER_QUEUE;
    SERVER_QUEUE component_servers_;
  } Assembly_Context;

  /**
   * @class Assembly_Builder_Visitor
   *
   * @brief A visitor implementation that "build" up the application
   * ACE_Hash_Map_Manager_Exby visiting nodes in an Assembly_Spec
   */
  class Assembly_Builder_Visitor : public CIAO::Assembly_Placement::Visitor
  {
  public:
    /// Constructor
    Assembly_Builder_Visitor (CORBA::ORB_ptr o,
                              Assembly_Context &context,
                              CIAO::ID_IMPL_MAP &idmap,
                              Deployment_Configuration &config);

    /// Destructor
    virtual ~Assembly_Builder_Visitor (void);

    virtual int visit_Container (Assembly_Placement::Container *c
                                 ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    virtual int visit_hostcollocation (Assembly_Placement::hostcollocation *hc
                                       ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    virtual int visit_processcollocation (Assembly_Placement::processcollocation *pc
                                          ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    virtual int visit_homeplacement (Assembly_Placement::homeplacement *hp
                                     ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    virtual int visit_componentinstantiation (Assembly_Placement::componentinstantiation *ci
                                              ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    Components::Deployment::Container_ptr
    get_current_container (void);

  protected:
    /// Registering a component using the info specified in @c i .
    void register_component (Assembly_Placement::componentinstantiation::Register_Info *i,
                             Components::CCMObject_ptr c
                             ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    /// Current Component Server.
    Components::Deployment::ComponentServer_var compserv_;

    /// Current Container.  (This shouldn't be necessary because the
    /// component server supposedly should be able to figure out if a
    /// component home can be installed in the same container based on
    /// its configvalue.  However, our component server don't do that
    /// yet, so we just install all home in a process into one single
    /// container.)
    Components::Deployment::Container_var container_;

    /// Current Component Home.  We only support keyless home
    /// operations for now.
    Components::KeylessCCMHome_var home_;

    /// Keep a pointer to the managing ORB serving this servant.
    CORBA::ORB_var orb_;

    /// Context to build on.
    Assembly_Context &context_;

    /// Context to build on.
    CIAO::ID_IMPL_MAP &impl_idref_map_;

    /// Deployment Configuration Info.
    Deployment_Configuration &deployment_config_;
  };
}

#if defined (__ACE_INLINE__)
# include "Assembly_Visitors.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* CIAO_ASSEMBLY_VISITOR_H */
