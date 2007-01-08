// -*- C++ -*-

//=============================================================================
/**
 *  @file    Dynamic_Component_Servant_Base.h
 *
 *  $Id$
 *
 */
//=============================================================================


#ifndef CIAO_DYNAMIC_COMPONENT_SERVANT_BASE_H
#define CIAO_DYNAMIC_COMPONENT_SERVANT_BASE_H

#include /**/ "ace/pre.h"

// @@ Jai, why is this includion necessary. Looks heavy weight to me.
#include "CCM_ComponentS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/PortableServer.h"
#include "CIAO_Server_Export.h"

namespace CIAO
{
  class Session_Container;
  class CIAO_SERVER_Export Dynamic_Component_Servant_Base
  {
  public:
    /// @todo Not to be used, no idea why this should be public, have to check this
    Dynamic_Component_Servant_Base (void);

    explicit Dynamic_Component_Servant_Base (Session_Container *c);

    virtual ~Dynamic_Component_Servant_Base (void);

    virtual PortableServer::Servant
      create (PortableServer::ObjectId &oid
              ACE_ENV_ARG_DECL_WITH_DEFAULTS) = 0;

    virtual void destroy (PortableServer::ObjectId &oid) = 0;

    virtual void update_destroy_count () = 0;

  protected:
    Session_Container *container_;
  };

}

#include /**/ "ace/post.h"

#endif /* CIAO_DYNAMIC_COMPONENT_SERVANT_BASE_H */
