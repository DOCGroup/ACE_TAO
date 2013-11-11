// -*- C++ -*-
// $Id$

#ifndef CIAO_FOO_EXEC_H_
#define CIAO_FOO_EXEC_H_

#include "FooEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Foo_exec_export.h"
#include "tao/LocalObject.h"

namespace CIAO_Foo_Impl
{
  class FOO_EXEC_Export Foo_exec_i
    : public virtual Foo_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Foo_exec_i (void);
    virtual ~Foo_exec_i (void);

    //@{
    /** Supported operations and attributes. */

    virtual ::CORBA::Short
    supported_short (void);

    virtual void
    supported_short (
        ::CORBA::Short supported_short);
    //@}


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
    ::CCM_Foo_Context_var context_;

    bool supported_short_;
  };

  extern "C" FOO_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Foo_Impl (void);
}

#endif /* ifndef */

