// -*- C++ -*-
// $Id$

#ifndef CIAO_COMPONENT_EXEC_H_
#define CIAO_COMPONENT_EXEC_H_

#include "SetConnectorAttribute_ComponentEC.h"

#include /**/ "Component_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

#include "dds4ccm/impl/dds/DataWriter_T.h"

namespace CIAO_SetConnectorAttribute_SetConnectorAttributeComponent_Impl
{
  //============================================================
  // Component_exec_i
  //============================================================
  class COMPONENT_EXEC_Export Component_exec_i
    : public virtual SetConnectorAttributeComponent_Exec,
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
    ::SetConnectorAttribute::CCM_SetConnectorAttributeComponent_Context_var context_;

    void check_attributes (DDSDataWriter * dw);

    void check_profile (DDSPublisher * pub);

    void check_topic_name (DDSTopic * tp);

    void check_domain_id (DDSPublisher * pub);

  };

  extern "C" COMPONENT_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_SetConnectorAttribute_Component_Impl (void);
}

#endif /* ifndef */

