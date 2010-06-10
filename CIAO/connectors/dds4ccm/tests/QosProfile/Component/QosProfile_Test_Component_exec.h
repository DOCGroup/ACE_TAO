// -*- C++ -*-
// $Id$

#ifndef CIAO_COMPONENT_EXEC_H_
#define CIAO_COMPONENT_EXEC_H_

#include "QosProfile_Test_ComponentEC.h"

#include /**/ "Component_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

namespace CIAO_QosProfile_Test_QosProfileComponent_Impl
{
  //============================================================
  // Component_exec_i
  //============================================================
  class COMPONENT_EXEC_Export Component_exec_i
    : public virtual QosProfileComponent_Exec,
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

  private:
    ::QosProfile_Test::CCM_QosProfileComponent_Context_var context_;
    ::QosProfile_Test::QosProfileTestConnector::Writer_var writer_;
  };

  extern "C" COMPONENT_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_QosProfile_Test_Component_Impl (void);
}

#endif /* ifndef */

