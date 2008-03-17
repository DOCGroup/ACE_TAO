// -*- C++ -*-
// $Id$
//=============================================================================
/**
 *  @file    NodeApp_CB_Impl.h
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 *          Tao    Lu   <lu@dre.vanderbilt.edu>
 *
 //===========================================================================*/


#ifndef NODEAPPLICATION_CALLBACK_IMPL_H
#define NODEAPPLICATION_CALLBACK_IMPL_H
#include /**/ "ace/pre.h"
#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "CIAO_NodeApplication_CallBackS.h"
#include "ace/Synch.h"

namespace CIAO
{
  /**
   * @class NodeApplication_Callback_Impl
   *
   * @brief A call back interface for NodeApplication
   * @Note  Here is no need to be thread safe since for every nodeapplication
   *        we will have an unique callback object. --Tao
   */
  class Deployment_svnt_Export NodeApplication_Callback_Impl
    : public virtual POA_CIAO::NodeApplication_Callback
  {
  public:
    friend class NodeApplicationManager_Impl;

    /// Constructor.
    NodeApplication_Callback_Impl (CORBA::ORB_ptr o,
                                   PortableServer::POA_ptr p,
                                   Deployment::NodeApplicationManager_ptr s,
                                   const Deployment::Properties &properties,
                                   ACE_Condition<TAO_SYNCH_MUTEX> &wait,
                                   TAO_SYNCH_MUTEX& mutex);

    /// Get the containing POA.  This operation does *not* increase
    /// the reference count of the POA.
    virtual PortableServer::POA_ptr _default_POA (void);

    /// Record the NodeApplication reference returned by the newly
    /// spawned NodeApplication and give it back a reference to
    /// NodeApplicationManager
    Deployment::NodeApplicationManager_ptr
    register_node_application (Deployment::NodeApplication_ptr na,
                               Deployment::Properties_out properties);

    Deployment::NodeApplication_ptr get_nodeapp_ref (void);

    bool is_callback_completed (void);

  protected:
    /// Destructor.
    ~NodeApplication_Callback_Impl ();

    bool is_callback_completed_;

    CORBA::ORB_var orb_;

    PortableServer::POA_var poa_;

    Deployment::NodeApplicationManager_var nam_;

    Deployment::NodeApplication_var nodeapp_;

    Deployment::Properties_var properties_;

    ACE_Condition<TAO_SYNCH_MUTEX> &waitCond_;

    TAO_SYNCH_MUTEX &mutex_;
  };
}

#include /**/ "ace/post.h"
#endif /* NODEAPPLICATION_CALLBACK_IMPL_H */
