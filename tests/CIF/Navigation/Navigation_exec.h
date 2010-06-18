// -*- C++ -*-
// $Id$

#ifndef CIAO_NAVIGATION_EXEC_H_
#define CIAO_NAVIGATION_EXEC_H_

#include /**/ "ace/pre.h"

#include "NavigationEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Navigation_exec_export.h"
#include "tao/LocalObject.h"


namespace CIAO_Navigation_Impl
{

  class NAVIGATION_EXEC_Export Navigation_exec_i
    : public virtual Navigation_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Navigation_exec_i (void);
    virtual ~Navigation_exec_i (void);

    virtual ::CCM_foo_ptr
    get_navigation_foo (void);

    virtual ::CCM_foo_inherited_ptr
    get_inherited_foo (void);

    //@{
    /** Operations from Components::SessionComponent. */

    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);
    //@}


  private:
    ::CCM_Navigation_Context_var context_;
  };

  extern "C" NAVIGATION_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Navigation_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */

