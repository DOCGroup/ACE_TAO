// -*- C++ -*-

//=============================================================================
/**
 *  @file    Swapping_Servant_Impl_T.h
 *
 *  $Id$
 *
 *  This file contains the declaration of a mixin base class for
 *  the generated home servant class.
 *
 */
//=============================================================================


#ifndef CIAO_SWAPPING_SERVANT_IMPL_T_H
#define CIAO_SWAPPING_SERVANT_IMPL_T_H

#include /**/ "ace/pre.h"

#include "Swapping_Servant_Impl_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/Key_Adapters.h"
#include "ace/Hash_Map_Manager_T.h"

namespace CIAO
{
  class Swapping_Container;

  /**
   * @class Swapping_Servant_Impl
   *
   * @brief Mixin base class for generated home servant.
   *
   * This class implements operations
   * common to all generated home servants.
   */
  template <typename BASE_SKEL, 
            typename EXEC, 
            typename EXEC_VAR,
            typename COMP,
            typename COMP_VAR,
            typename COMP_EXEC,
            typename COMP_EXEC_VAR,
            typename COMP_SVNT>
  class Swapping_Servant_Impl
    : public virtual BASE_SKEL,
      public virtual Swapping_Servant_Impl_Base,
      public virtual PortableServer::RefCountServantBase
  {
  public:
    Swapping_Servant_Impl (EXEC * exe,
                       Swapping_Container * c);
                       
    virtual ~Swapping_Servant_Impl (void);
    
    // Operations for CCMHome interface.

    virtual void
    remove_component (Components::CCMObject_ptr comp
                      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::RemoveFailure));
                       
    // Operations for keyless home interface.

    virtual ::Components::CCMObject_ptr
    create_component (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CreateFailure));

    // Operations for implicit home interface.

    virtual COMP *
    create (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CreateFailure));

  protected:
    // CIAO-specific operations.

    COMP *
    _ciao_activate_component (COMP_EXEC *exe
                              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));
  
    void
    _ciao_passivate_component (COMP *comp
                               ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

  protected:
    EXEC_VAR executor_;

    ACE_Hash_Map_Manager_Ex<PortableServer::ObjectId,
                            COMP_SVNT *,
                            TAO_ObjectId_Hash,
                            ACE_Equal_To<PortableServer::ObjectId>,
                            ACE_SYNCH_MUTEX>
      component_map_;
  };
}

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Swapping_Servant_Impl_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Swapping_Servant_Impl_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* CIAO_SWAPPING_SERVANT_IMPL_T_H */
   
