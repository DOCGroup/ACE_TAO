// -*- C++ -*-
// $Id$

#ifndef HELLO_SENDER_EXEC_H_
#define HELLO_SENDER_EXEC_H_


#include "Hello_SenderEC.h"

#include /**/ "Sender_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "ace/Task.h"
#include "ace/Reactor.h"

#include <map>

namespace CIAO_Hello_Sender_Impl
{
  class Sender_exec_i;
  
  class pulse_Generator : 
    public ACE_Event_Handler
  {
  public:
    pulse_Generator (Sender_exec_i &callback);

    ~pulse_Generator ();

    /// Handle the timeout.
    virtual int handle_timeout (const ACE_Time_Value &tv,
                                const void *arg);
  private:
    /// Maintains a handle that actually process the event
    Sender_exec_i &pulse_callback_;
  };
  
  class Sender_exec_i
    : public virtual Sender_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Sender_exec_i (void);
    virtual ~Sender_exec_i (void);
    
    // Supported operations and attributes.
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    void start (void);
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    void stop (void);
    
    void tick ();
    
    // Component attributes.
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual ::CORBA::ULong
    rate (void);
    
    virtual void
    rate (
      ::CORBA::ULong rate);

    // Port operations.
    
    // Operations from Components::SessionComponent.
    
    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);
    
    virtual void configuration_complete (void);
    
    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);
  
  private:
    ::Hello::CCM_Sender_Context_var context_;
    
    CCM_DDS::DDSHello_Writer_var writer_;
    pulse_Generator * ticker_;
    CORBA::ULong rate_;
    CORBA::Short iteration_;
  };
  
  extern "C" HELLO_SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Hello_Sender_Impl (void);
}

#endif /* ifndef */

