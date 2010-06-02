// -*- C++ -*-
// $Id$

#ifndef CIAO_COMPONENT_EXEC_H_
#define CIAO_COMPONENT_EXEC_H_

#include "UCC_Test_ComponentEC.h"

#include /**/ "Component_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

#include <map>

namespace CIAO_UCC_Test_UCCTestComponent_Impl
{
  class Component_exec_i;

  //============================================================
  // WriteVariableHandler
  //============================================================
  class WriteVariableHandler :
    public ACE_Event_Handler
  {
  public:
    WriteVariableHandler (Component_exec_i &callback);
    int handle_timeout (const ACE_Time_Value &, const void *);
  private:
    /// Maintains a handle that actually process the event
    Component_exec_i &callback_;
  };

  //============================================================
  // WriteFixedHandler
  //============================================================
  class WriteFixedHandler :
    public ACE_Event_Handler
  {
  public:
    WriteFixedHandler (Component_exec_i &callback);
    int handle_timeout (const ACE_Time_Value &, const void *);
  private:
    /// Maintains a handle that actually process the event
    Component_exec_i &callback_;
  };

  //============================================================
  // Component_exec_i
  //============================================================
  class COMPONENT_EXEC_Export Component_exec_i
    : public virtual UCCTestComponent_Exec,
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

    void write_variable (void);

    void write_fixed (void);

  private:
    ::UCC_Test::CCM_UCCTestComponent_Context_var context_;

    //Variable sized struct
    ::UCCVariableSizedStruct_Test::UCCVariableSizedStructTestConnector::Writer_var var_writer_;
    void create_variable_sized_samples (void);
    WriteVariableHandler *var_rh_;

    TAO_SYNCH_MUTEX var_mutex_;
    typedef std::map< ACE_CString, UCCVariableSizedStructTest_var > UCCVarSized_Table;
    UCCVarSized_Table var_samples_;

    // Fixed sized struct
    ::UCCFixedSizeStruct_Test::UCCFixedSizedStructTestConnector::Writer_var fixed_writer_;
    void create_fixed_sized_samples (void);
    WriteFixedHandler *fixed_rh_;

    TAO_SYNCH_MUTEX fixed_mutex_;
    typedef std::map< ::CORBA::UShort, UCCFixedSizedStructTest_var > UCCFixedSized_Table;
    UCCFixedSized_Table fixed_samples_;
  };

  extern "C" COMPONENT_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_UCCTest_Component_Impl (void);
}

#endif /* ifndef */

