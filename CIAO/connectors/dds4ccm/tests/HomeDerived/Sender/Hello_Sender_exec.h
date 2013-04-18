// -*- C++ -*-
// $Id$

#ifndef CIAO_HELLO_SENDER_EXEC_YPB9XX_H_
#define CIAO_HELLO_SENDER_EXEC_YPB9XX_H_

#include /**/ "ace/pre.h"

#include "Hello_SenderEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Sender_exec_export.h"
#include "tao/LocalObject.h"
#include <map>

namespace CIAO_Hello_Base_comp_Impl
{

  class HELLO_SENDER_EXEC_Export fooSub_data_listener_exec_i
    : public virtual ::Foo_conn::CCM_Listener,
      public virtual ::CORBA::LocalObject
  {
  public:
    fooSub_data_listener_exec_i (
      ::Hello::CCM_Base_comp_Context_ptr ctx);

    virtual ~fooSub_data_listener_exec_i (void);

    // Operations and attributes from Hello::Listener

    virtual
    void on_one_data (
      const ::DDSHello & datum,
      const ::CCM_DDS::ReadInfo & info);

    virtual
    void on_many_data (
      const ::DDSHelloSeq & data,
      const ::CCM_DDS::ReadInfoSeq & infos);

  private:
    ::Hello::CCM_Base_comp_Context_var ciao_context_;
  };

  class HELLO_SENDER_EXEC_Export fooSub_status_exec_i
      : public virtual ::CCM_DDS::CCM_PortStatusListener,
        public virtual ::CORBA::LocalObject
  {
  public:
    fooSub_status_exec_i (
      ::Hello::CCM_Base_comp_Context_ptr ctx);
    virtual ~fooSub_status_exec_i (void);

    // Operations and attributes from CCM_DDS::PortStatusListener

    virtual
    void on_requested_deadline_missed (
      ::DDS::DataReader_ptr the_reader,
      const ::DDS::RequestedDeadlineMissedStatus & status);

    virtual
    void on_sample_lost (
      ::DDS::DataReader_ptr the_reader,
      const ::DDS::SampleLostStatus & status);

  private:
    ::Hello::CCM_Base_comp_Context_var ciao_context_;
  };


  class Base_comp_exec_i;

  class HELLO_SENDER_EXEC_Export Base_comp_exec_i
    : public virtual Base_comp_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Base_comp_exec_i (void);
    virtual ~Base_comp_exec_i (void);

    //@{
    /** Supported operations and attributes. */
    //@}

    //@{
    /** Component attributes and port operations. */
    virtual ::CORBA::Short
    baseAttr (void);

    virtual void
    baseAttr (::CORBA::Short baseAttr);

    virtual ::Foo_conn::CCM_Listener_ptr
    get_fooSub_data_listener (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_fooSub_status (void);

    virtual ::Hello::CCM_Base_obj_ptr
    get_baseFacet (void);

    //@}

    //@{
    /** Operations from Components::SessionComponent. */

    virtual void set_session_context (::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);
    //@}

  private:
    ::Hello::CCM_Base_comp_Context_var ciao_context_;
    ::CCM_DDS::CCM_ConnectorStatusListener_var ciao_connector_status_;
    ::Foo_conn::CCM_Listener_var ciao_fooSub_data_listener_;
    ::CCM_DDS::CCM_PortStatusListener_var ciao_fooSub_status_;
    CORBA::Short baseAttr_;
 };

  extern "C" HELLO_SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Hello_Base_comp_Impl (void);
}
namespace CIAO_Hello_Base_comp_Impl
{
  class HELLO_SENDER_EXEC_Export Base_compHome_exec_i
    : public virtual Base_compHome_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Base_compHome_exec_i (void);

    virtual ~Base_compHome_exec_i (void);

    // All operations and attributes.

    // Factory operations.

    // Finder operations.

    // Implicit operations.

    virtual ::Components::EnterpriseComponent_ptr
    create (void);
  private:

  };

  extern "C" HELLO_SENDER_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_Hello_Base_compHome_Impl (void);
}

namespace CIAO_Hello_Derived_comp_Impl
{

  class HELLO_SENDER_EXEC_Export barSub_data_listener_exec_i
      : public virtual ::Bar_conn::CCM_Listener,
        public virtual ::CORBA::LocalObject
  {
  public:
    barSub_data_listener_exec_i (
      ::Hello::CCM_Derived_comp_Context_ptr ctx);

    virtual ~barSub_data_listener_exec_i (void);

    // Operations and attributes from Hello::Listener

    virtual
    void on_one_data (
      const ::DDSHello & datum,
      const ::CCM_DDS::ReadInfo & info);

    virtual
    void on_many_data (
      const ::DDSHelloSeq & data,
      const ::CCM_DDS::ReadInfoSeq & infos);

  private:
    ::Hello::CCM_Derived_comp_Context_var ciao_context_;
  };

  class HELLO_SENDER_EXEC_Export barSub_status_exec_i
       : public virtual ::CCM_DDS::CCM_PortStatusListener,
         public virtual ::CORBA::LocalObject
  {
  public:
    barSub_status_exec_i (
       ::Hello::CCM_Base_comp_Context_ptr ctx);
    virtual ~barSub_status_exec_i (void);

    // Operations and attributes from CCM_DDS::PortStatusListener

    virtual
    void on_requested_deadline_missed (
      ::DDS::DataReader_ptr the_reader,
      const ::DDS::RequestedDeadlineMissedStatus & status);

    virtual
    void on_sample_lost (
      ::DDS::DataReader_ptr the_reader,
      const ::DDS::SampleLostStatus & status);

  private:
    ::Hello::CCM_Base_comp_Context_var ciao_context_;
  };

  class HELLO_SENDER_EXEC_Export Derived_comp_exec_i
    : public virtual Derived_comp_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Derived_comp_exec_i (void);
    virtual ~Derived_comp_exec_i (void);

    //@{
    /** Component attributes and port operations. */
    virtual ::CORBA::LongLong
    derivedAttr (void);

    virtual void
    derivedAttr (::CORBA::LongLong);

    virtual ::CORBA::Short
    baseAttr (void);

    virtual void
    baseAttr (::CORBA::Short baseAttr);

    virtual ::Foo_conn::CCM_Listener_ptr
    get_fooSub_data_listener (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_fooSub_status (void);

    virtual ::Bar_conn::CCM_Listener_ptr
    get_barSub_data_listener (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_barSub_status (void);

    virtual ::Hello::CCM_Base_obj_ptr
    get_baseFacet (void);

    virtual ::Hello::CCM_Derived_obj_ptr
    get_derivedFacet (void);

    //@}

    //@{
    /** Operations from Components::SessionComponent. */

    virtual void set_session_context (::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);
    //@}

  private:
    ::Hello::CCM_Derived_comp_Context_var ciao_context_;
    ::Foo_conn::CCM_Listener_var ciao_fooSub_data_listener_;
    ::CCM_DDS::CCM_PortStatusListener_var ciao_fooSub_status_;
    ::Bar_conn::CCM_Listener_var ciao_barSub_data_listener_;
    ::CCM_DDS::CCM_PortStatusListener_var ciao_barSub_status_;

    CORBA::Short baseAttr_;
    CORBA::ULongLong derivedAttr_;
 //   Atomic_Boolean ready_to_start_;
  };

  extern "C" HELLO_SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Hello_Derived_comp_Impl (void);
}


namespace  CIAO_Hello_Derived_comp_Impl
{
  class HELLO_SENDER_EXEC_Export Derived_compHome_exec_i
    : public virtual Derived_compHome_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Derived_compHome_exec_i (void);

    virtual ~Derived_compHome_exec_i (void);

    // All operations and attributes.

    // Factory operations.
    // Finder operations.

    // Implicit operations.

    virtual ::Components::EnterpriseComponent_ptr
    create (void);
  private:

  };

  extern "C" HELLO_SENDER_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_Hello_Derived_compHome_Impl (void);
}

namespace CIAO_Hello_DerivedEmpty_comp_Impl
{
 class HELLO_SENDER_EXEC_Export DerivedEmpty_comp_exec_i
    : public virtual DerivedEmpty_comp_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
   DerivedEmpty_comp_exec_i (void);
    virtual ~DerivedEmpty_comp_exec_i (void);

    //@{
    /** Component attributes and port operations. */
    virtual ::CORBA::Short
    baseAttr (void);

    virtual void
    baseAttr (::CORBA::Short baseAttr);

    virtual ::Foo_conn::CCM_Listener_ptr
    get_fooSub_data_listener (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_fooSub_status (void);

    virtual ::Hello::CCM_Base_obj_ptr
    get_baseFacet (void);

    //@}

    //@{
    /** Operations from Components::SessionComponent. */

    virtual void set_session_context (::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);
    //@}

  private:
    ::Hello::CCM_DerivedEmpty_comp_Context_var ciao_context_;
    ::Foo_conn::CCM_Listener_var ciao_fooSub_data_listener_;
    ::CCM_DDS::CCM_PortStatusListener_var ciao_fooSub_status_;
    CORBA::Short baseAttr_;
    CORBA::ULongLong derivedAttr_;
  };

  extern "C" HELLO_SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Hello_DerivedEmpty_comp_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */
