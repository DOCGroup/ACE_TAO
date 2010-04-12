// -*- C++ -*-
// $Id$

#ifndef CIAO_RECEIVER_EXEC_H_
#define CIAO_RECEIVER_EXEC_H_

#include "Shapes_ReceiverEC.h"

#include /**/ "Receiver_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "ace/Task.h"
#include "ace/Reactor.h"

namespace CIAO_Shapes_Receiver_Impl
{
  class Receiver_exec_i;
  /**
  * @class reader activity generator
  *
  * @brief an active object used by Receiver to perform a periodical read action
  *
  */
  class read_action_Generator
    : public ACE_Event_Handler
  {
  public:
    read_action_Generator (Receiver_exec_i &callback);

    ~read_action_Generator ();

    virtual int handle_timeout (const ACE_Time_Value &tv,
                                const void *arg);

  private:
    Receiver_exec_i &pulse_callback_;

  };

  //============================================================
  // ShapeType_Listener_exec_i
  //============================================================
  class RECEIVER_EXEC_Export ShapeType_Listener_exec_i
    : public virtual ::Shapes::ShapeTypeConn::CCM_Listener,
      public virtual ::CORBA::LocalObject
  {
  public:
    ShapeType_Listener_exec_i (void);
    virtual ~ShapeType_Listener_exec_i (void);

    virtual void
    on_one_data (
      const ShapeType & an_instance,
      const ::CCM_DDS::ReadInfo & info);
    virtual void
    on_many_data (
      const ShapeTypeSeq & an_instance,
      const ::CCM_DDS::ReadInfoSeq & info);
  };

  class RECEIVER_EXEC_Export PortStatusListener_exec_i
    : public virtual ::CCM_DDS::CCM_PortStatusListener,
      public virtual ::CORBA::LocalObject
  {
  public:
    PortStatusListener_exec_i (void);
    virtual ~PortStatusListener_exec_i (void);

    virtual void
    on_requested_deadline_missed (
      ::DDS::DataReader_ptr the_reader,
      const ::DDS::RequestedDeadlineMissedStatus & status);

    virtual void
    on_sample_lost (
      ::DDS::DataReader_ptr the_reader,
      const ::DDS::SampleLostStatus & status);
  };

  class RECEIVER_EXEC_Export Receiver_exec_i
    : public virtual Receiver_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Receiver_exec_i (void);
    virtual ~Receiver_exec_i (void);

    // Supported operations and attributes.
    void read_one (void);
    void read_all (void);
    void get_one (void);
    void get_all (void);

    // Component attributes.
    virtual ::CORBA::ULong rate (void);

    virtual void rate (::CORBA::ULong rate);

    virtual ::CORBA::Boolean get_data (void);

    virtual void get_data (::CORBA::Boolean get_data);

    virtual ::CORBA::Boolean read_data (void);

    virtual void read_data (::CORBA::Boolean read_data);

    virtual ::CORBA::Boolean raw_listen (void);

    virtual void raw_listen (::CORBA::Boolean raw_listen);

    // Port operations.
    virtual ::Shapes::ShapeTypeConn::CCM_Listener_ptr
    get_info_out_data_listener (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_out_status (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_get_status (void);

    // Operations from Components::SessionComponent.
    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::Shapes::CCM_Receiver_Context_var context_;
    ::Shapes::ShapeTypeConn::Reader_var reader_;
    ::Shapes::ShapeTypeConn::Getter_var getter_;

    read_action_Generator * ticker_;
    CORBA::ULong rate_;
    ::CORBA::Boolean get_data_, read_data_, raw_listen_;
  };

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Shape_Receiver_Impl (void);
}

#endif /* ifndef */

