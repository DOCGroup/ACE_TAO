// -*- C++ -*-

//=============================================================================
/**
 *  @file    Simple_Server_i.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================


#ifndef CIAO_SIMPLE_SERVER_I_H
#define CIAO_SIMPLE_SERVER_I_H
#include "ace/pre.h"

#include "Simple_ServerS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace CIAO
{
  class Simple_Server_i
    : public virtual POA_CIAO::Simple_Server,
      public virtual PortableServer::RefCountServantBase
  {
  public:
    /// Constructor
    Simple_Server_i (CORBA::ORB_ptr o,
                     PortableServer::POA_ptr p,
                     Components::CCMHome_ptr h);

    /// Destructor
    virtual ~Simple_Server_i (void);

    /// Get the containing POA.  This operation does *not*
    /// increase the reference count of the POA.
    virtual PortableServer::POA_ptr _default_POA (void);

    virtual Components::CCMHome_ptr
    get_home (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual Components::CCMObject_ptr
    get_component (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

  protected:
    /// Keep a pointer to the managing ORB serving this servant.
    CORBA::ORB_var orb_;

    /// Keep a pointer to the managing POA.
    PortableServer::POA_var poa_;

    /// Cached CCMHome reference.
    Components::CCMHome_var home_;

    /// Cached CCMObject reference.
    Components::CCMObject_var component_;
  };
}

#include "ace/post.h"
#endif /* CIAO_SIMPLE_SERVER_H */
