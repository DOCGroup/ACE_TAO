// -*- C++ -*-

//=============================================================================
/**
 *  @file    Dynamic_Component_Servant_T.h
 *
 *  $Id$
 *
 *  This file contains the declaration of a mixin base class for
 *  the generated home servant class.
 *
 */
//=============================================================================


#ifndef CIAO_DYNAMIC_COMPONENT_SERVANT_T_H
#define CIAO_DYNAMIC_COMPONENT_SERVANT_T_H

#include /**/ "ace/pre.h"

#include "Dynamic_Component_Servant_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/Key_Adapters.h"
#include "ace/Hash_Map_Manager_T.h"

namespace CIAO
{
  // @@ Jai, isn't this like a factory base instaed of a servant base?
  // Why are inheriting from RefCountServantBase? Makes less sense to
  // me.
  template <typename COMP_SERVANT>
  class Dynamic_Component_Servant
    : public virtual Dynamic_Component_Servant_Base,
      public virtual PortableServer::RefCountServantBase
  {
  public:
    virtual PortableServer::Servant create (void);
  };
}

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Dynamic_Component_Servant_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Dynamic_Component_Servant_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* CIAO_DYNAMIC_COMPONENT_SERVANT_T_H */
