// -*- C++ -*-
// $Id$

#ifndef CIAO_COMPONENT_EXEC_H_
#define CIAO_COMPONENT_EXEC_H_

#include "ParseError_Test_ComponentEC.h"

#include /**/ "Component_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

namespace CIAO_ParseError_Test_ParseErrorTestComponent_Impl
{
  //============================================================
  // Component_exec_i
  //============================================================
  class COMPONENT_EXEC_Export Component_exec_i
    : public virtual ParseErrorTestComponent_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Component_exec_i (void);
    virtual ~Component_exec_i (void);

    // Operations from Components::SessionComponent.
    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

    virtual CCM_ParseErrorTest_ptr
    get_to_do();

    virtual CORBA::ULong
    test();

    virtual void
    test(CORBA::ULong);

  private:
    ::ParseError_Test::CCM_ParseErrorTestComponent_Context_var context_;
  };

  extern "C" COMPONENT_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_ParseErrorTest_Component_Impl (void);
}

#endif /* ifndef */

