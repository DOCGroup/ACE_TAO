// -*- C++ -*-

//=============================================================================
/**
 *  @file    Swapping_Servant_Home_Impl_Base.h
 *
 *  $Id$
 *
 */
//=============================================================================


#ifndef CIAO_SWAPPING_SERVANT_HOME_IMPL_BASE_H
#define CIAO_SWAPPING_SERVANT_HOME_IMPL_BASE_H

#include /**/ "ace/pre.h"

#include "CCM_ComponentS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "CIAO_Server_Export.h"

namespace CIAO
{
  class Swapping_Container;

  /**
   * @class Swapping_Servant_Home_Impl_Base
   *
   */
  // @@ Jai, why are you inheriting from "home" here? What is the
  // point?
  // @@ JAI, probably my class naming convention was the problem.
  // This is a home implementation and that is why inheriting from
  // "home".
  //
  class CIAO_SERVER_Export Swapping_Servant_Home_Impl_Base
    : public virtual POA_Components::CCMHome
  {
  public:
    explicit Swapping_Servant_Home_Impl_Base (void);

    Swapping_Servant_Home_Impl_Base (Swapping_Container * c);

    virtual ~Swapping_Servant_Home_Impl_Base (void);

    // Operations for CCMHome interface.
    virtual ::CORBA::IRObject_ptr
    get_component_def (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual ::CORBA::IRObject_ptr
    get_home_def (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

  protected:
    Swapping_Container *container_;
  };
}

#include /**/ "ace/post.h"

#endif /* CIAO_SWAPPING_SERVANT_HOME_IMPL_BASE_H */
