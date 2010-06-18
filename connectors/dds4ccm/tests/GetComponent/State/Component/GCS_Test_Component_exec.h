// -*- C++ -*-
// $Id$

#ifndef CIAO_COMPONENT_EXEC_H_
#define CIAO_COMPONENT_EXEC_H_

#include "GCS_Test_ComponentEC.h"

#include /**/ "Component_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

namespace CIAO_GCS_Test_GetComponentStateTestComponent_Impl
{
  //============================================================
  // Component_exec_i
  //============================================================
  class COMPONENT_EXEC_Export Component_exec_i
    : public virtual GetComponentStateTestComponent_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Component_exec_i (void);
    virtual ~Component_exec_i (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_read_status (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_get_status (void);

    virtual ::GCSTestConnector::CCM_Listener_ptr
    get_info_listen_data_listener (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_listen_status (void);

    virtual ::GCSTestConnector::CCM_StateListener_ptr
    get_info_statelisten_data_listener (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_statelisten_status (void);

    ::CORBA::Boolean
    act_as_sender (void);

    void
    act_as_sender (::CORBA::Boolean act_as_sender);

    // Operations from Components::SessionComponent.
    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::GCS_Test::CCM_GetComponentStateTestComponent_Context_var context_;
    ::CORBA::Boolean act_as_sender_;
    void test_updater (void);
    void test_reader (void);
    void test_getter (void);
    void test_listener (void);
    void test_statelistener (void);
  };

  extern "C" COMPONENT_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_GetComponentStateTest_Component_Impl (void);
}

#endif /* ifndef */

