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
  template <typename COMP_SERVANT, typename COMP_EXEC, 
            typename COMP_EXEC_VAR, typename EXEC>
  class Dynamic_Component_Servant
    : public virtual Dynamic_Component_Servant_Base
  {
  public:
    Dynamic_Component_Servant (EXEC *exe, Components::CCMHome_ptr home);

    virtual ~Dynamic_Component_Servant (void);

    virtual PortableServer::Servant create (void);

  protected:
    EXEC_VAR executor_;
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
