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

#include "ciao/Servants/Servant_Impl_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ccm/Session/CCM_SessionComponentC.h"
#include "ciao/Containers/Session/Session_Container.h"

namespace CIAO
{
  /**
   * @class Session_Servant_Impl_T
   *
   * @brief Mixin base class for generated servant.
   *
   * This class implements navigation and other operations
   * common to all generated servants.
   */
  template <typename BASE_SKEL,
            typename EXEC,
            typename CONTEXT>
  class Session_Servant_Impl_T
    : public virtual BASE_SKEL,
      public virtual CONTEXT::svnt_base_type
  {
  public:
    Session_Servant_Impl_T (EXEC * exe,
                            Components::CCMHome_ptr home,
                            const char * ins_name,
                            Home_Servant_Impl_Base *home_servant,
                            ::CIAO::Session_Container_ptr c);

    virtual ~Session_Servant_Impl_T (void);

    // Operations for CCMObject interface.

    virtual CORBA::Boolean same_component (CORBA::Object_ptr object_ref);

    virtual ::Components::CCMHome_ptr get_ccm_home ();

    virtual CORBA::Object_ptr _get_component ();

    virtual Components::EnterpriseComponent_ptr get_executor ();

    void configuration_complete ();

    virtual void activate_component ();

    virtual void passivate_component ();

  protected:
    virtual void ccm_remove (void);

    CORBA::Boolean activated_;
    CORBA::Boolean configuration_completed_;

    /// Initialized in this constructor.
    typename EXEC::_var_type executor_;

    /// Initialized in this constructor.
    CONTEXT* context_;

    const char * ins_name_;
  };
}

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ciao/Servants/Session/Servant_Impl_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("ciao/Servants/Session/Servant_Impl_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* CIAO_SERVANT_IMPL_T_H */
