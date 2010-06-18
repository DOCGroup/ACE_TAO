// -*- C++ -*-
// $Id$

#ifndef CIAO_COMPONENT_EXEC_H_
#define CIAO_COMPONENT_EXEC_H_

#include "NonChangeable_ComponentEC.h"

#include /**/ "Component_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

#include "dds4ccm/impl/dds/DataWriter.h"

namespace CIAO_NonChangeable_NonChangeableComponent_Impl
{
  //============================================================
  // Component_exec_i
  //============================================================
  class COMPONENT_EXEC_Export Component_exec_i
    : public virtual NonChangeableComponent_Exec,
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
    ::NonChangeable::CCM_NonChangeableComponent_Context_var context_;

    bool
    test_key_fields (
      ::NonChangeable::NonChangeableTestConnector::CCM_DDS_Event_ptr conn);

    bool
    test_attributes (
      ::NonChangeable::NonChangeableTestConnector::CCM_DDS_Event_ptr conn);

    bool
    test_qos_profile (
      ::NonChangeable::NonChangeableTestConnector::CCM_DDS_Event_ptr conn);

    bool
    test_topic_name (
      ::NonChangeable::NonChangeableTestConnector::CCM_DDS_Event_ptr conn);

    bool
    test_domain_id (
      ::NonChangeable::NonChangeableTestConnector::CCM_DDS_Event_ptr conn);

  };

  extern "C" COMPONENT_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_NonChangeable_Component_Impl (void);
}

#endif /* ifndef */

