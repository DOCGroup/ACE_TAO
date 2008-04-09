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
#include "CIAO_SwapExecC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/Key_Adapters.h"
#include "ace/Hash_Map_Manager_T.h"

namespace CIAO
{
  template <typename COMP_SVNT>
  class Dynamic_Component_Servant
    : public virtual Dynamic_Component_Servant_Base
  {
  public:
    // @@Jai, please add documentation for these methods.
    Dynamic_Component_Servant (Components::EnterpriseComponent_ptr ec,
                               Components::CCMHome_ptr home,
                               const char* ins_name,
                               Home_Servant_Impl_Base *home_servant,
                               Session_Container *c);

    virtual ~Dynamic_Component_Servant (void);

    virtual PortableServer::Servant create (PortableServer::ObjectId &oid);

    virtual void destroy (PortableServer::ObjectId &oid);

    virtual void update_destroy_count (void);

  protected:
    Components::EnterpriseComponent_var executor_;

    CORBA::String_var ins_name_;

    Home_Servant_Impl_Base *home_servant_;

    Components::CCMHome_var home_;

    // @@ Jai, please document why this is there.
    ACE_Hash_Map_Manager_Ex<PortableServer::ObjectId,
                            COMP_SVNT *,
                            TAO_ObjectId_Hash,
                            ACE_Equal_To<PortableServer::ObjectId>,
                            TAO_SYNCH_MUTEX>
      servant_map_;

    // @@ Jai, please explain what this flag means?
    bool component_removed_;
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
