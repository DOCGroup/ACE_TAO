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
    ACE_Hash_Map_Manager_Ex<ACE_CString,
                            Components::CCMHome_var,
                            ACE_Hash<ACE_CString>,
                            ACE_Equal_To<ACE_CString>,
                            ACE_Null_Mutex> installed_homes_;

    ACE_Hash_Map_Manager_Ex<ACE_CString,
                            Components::CCMObject_var,
                            ACE_Hash<ACE_CString>,
                            ACE_Equal_To<ACE_CString>,
                            ACE_Null_Mutex> instantiated_components_;
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
                              Deployment_Configuration &config);

    /// Destructor
    virtual ~Assembly_Builder_Visitor (void);

    virtual int visit_Container (Assembly_Placement::Container *c);

    virtual int visit_hostcollocation (Assembly_Placement::hostcollocation *hc);

    virtual int visit_processcollocation (Assembly_Placement::processcollocation *pc);

    virtual int visit_homeplacement (Assembly_Placement::homeplacement *hp);

    virtual int visit_componentinstantiation (Assembly_Placement::componentinstantiation *ci);

  protected:
    /// Keep a pointer to the managing ORB serving this servant.
    CORBA::ORB_var orb_;

    /// Context to build on.
    Assembly_Context &context_;

    /// Deployment Configuration Info.
    Deployment_Configuration &deployment_config_;
  };
}

#if defined (__ACE_INLINE__)
# include "Assembly_Visitor.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* CIAO_ASSEMBLY_VISITOR_H */
