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

#include "CCM_ComponentS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/PortableServer.h"
#include "CIAO_Server_Export.h"

namespace CIAO
{
  class CIAO_SERVER_Export Dynamic_Component_Servant_Base
  {
  public:
    explicit Dynamic_Component_Servant_Base (void);

    virtual ~Dynamic_Component_Servant_Base (void);

    virtual PortableServer::Servant create (void) = 0;

  };

  template<typename COMP_SERVANT>
  class Dynamic_Component_Servant : public Dynamic_Component_Servant_Base
  {
  public:
    PortableServer::Servant create (void)
    {
      return new COMP_SERVANT;
    }
}

#include /**/ "ace/post.h"

#endif /* CIAO_DYNAMIC_COMPONENT_SERVANT_BASE_H */
