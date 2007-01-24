// -*- C++ -*-

//=============================================================================
/**
 *  @file    Servant_Impl_T.h
 *
 *  $Id$
 *
 *  This file contains the declaration of a mixin base class for
 *  the generated servant class.
 *
 *  @authors Boris Kolpackov <boris@dre.vanderbilt.edu>
 *           Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================


#ifndef CIAO_SERVANT_IMPL_T_H
#define CIAO_SERVANT_IMPL_T_H

#include /**/ "ace/pre.h"

#include "Servant_Impl_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace CIAO
{
  class Session_Container;

  /**
   * @class Servant_Impl
   *
   * @brief Mixin base class for generated servant.
   *
   * This class implements navigation and other operations
   * common to all generated servants.
   */
  template <typename BASE_SKEL,
            typename EXEC,
            typename CONTEXT>
  class Servant_Impl : public virtual BASE_SKEL,
                       public virtual Servant_Impl_Base
  {
  public:
    Servant_Impl (EXEC * exe,
                  Components::CCMHome_ptr home,
                  Home_Servant_Impl_Base *home_servant,
                  Session_Container * c);

    virtual ~Servant_Impl (void);

    // Operations for CCMObject interface.

    virtual CORBA::Boolean
    same_component (CORBA::Object_ptr object_ref
                    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual ::Components::CCMHome_ptr
    get_ccm_home (void)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual CORBA::Object_ptr
    _get_component (void)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual Components::SessionComponent_ptr
    get_executor (void)
      ACE_THROW_SPEC ((CORBA::SystemException));

   // CIAO-specific operations.

    void
    ciao_preactivate (void)
      ACE_THROW_SPEC ((CORBA::SystemException));

    void
    ciao_activate (void)
      ACE_THROW_SPEC ((CORBA::SystemException));

    void
    ciao_postactivate (void)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /// @@NOTE: The busted operation.
    void
    ciao_passivate (void)
      ACE_THROW_SPEC ((CORBA::SystemException));

    CORBA::Boolean
    is_activated (void)
      ACE_THROW_SPEC ((CORBA::SystemException));

    void
    activate_component (void)
      ACE_THROW_SPEC ((CORBA::SystemException));

  protected:

    CORBA::Boolean activated_;
    CORBA::Boolean pre_activated_;
    CORBA::Boolean post_activated_;

    /// Initialized in this constructor.
    typename EXEC::_var_type executor_;

    /// Initialized in the derived, generated servant constructor.
    CONTEXT * context_;
  };
}

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Servant_Impl_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Servant_Impl_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* CIAO_SERVANT_IMPL_T_H */
