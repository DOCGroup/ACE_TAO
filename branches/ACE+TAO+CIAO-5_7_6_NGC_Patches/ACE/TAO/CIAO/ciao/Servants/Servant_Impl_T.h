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
                  const char * ins_name,
                  Home_Servant_Impl_Base *home_servant,
                  Container_ptr c);

    virtual ~Servant_Impl (void);

    // Operations for CCMObject interface.

    virtual CORBA::Boolean same_component (CORBA::Object_ptr object_ref);

    virtual ::Components::CCMHome_ptr get_ccm_home ();

    virtual CORBA::Object_ptr _get_component ();

    virtual Components::SessionComponent_ptr get_executor ();

    void configuration_complete ();

    virtual void activate_component ();

    virtual void passivate_component ();

  protected:
    CORBA::Boolean activated_;
    CORBA::Boolean configuration_completed_;

    /// Initialized in this constructor.
    typename EXEC::_var_type executor_;

    /// Initialized in the derived, generated servant constructor.
    CONTEXT * context_;

    const char * ins_name_;
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
