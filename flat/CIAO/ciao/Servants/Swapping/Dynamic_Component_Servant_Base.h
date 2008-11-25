// -*- C++ -*-

//=============================================================================
/**
 *  @file    Dynamic_Component_Servant_Base.h
 *
 *  $Id$
 *
 */
//=============================================================================

#ifndef CIAO_DYNAMIC_COMPONENT_SERVANT_BASE__H
#define CIAO_DYNAMIC_COMPONENT_SERVANT_BASE__H

#include /**/ "ace/pre.h"

#include "ciao/Servants/Swapping/CIAO_Swapping_Servant_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/PortableServer.h"

namespace CIAO
{
  class Session_Container;

  class CIAO_Swapping_Servant_Export Dynamic_Component_Servant_Base
  {
  public:
    /// @todo Not to be used, no idea why this should be public, have to check this
    Dynamic_Component_Servant_Base (void);

    explicit Dynamic_Component_Servant_Base (Session_Container *c);

    virtual ~Dynamic_Component_Servant_Base (void);

    virtual PortableServer::Servant
      create (PortableServer::ObjectId &oid) = 0;

    virtual void destroy (PortableServer::ObjectId &oid) = 0;

    virtual void update_destroy_count () = 0;

  protected:
    Session_Container *container_;
  };

}

#include /**/ "ace/post.h"

#endif /* CIAO_DYNAMIC_COMPONENT_SERVANT_BASE__H */
