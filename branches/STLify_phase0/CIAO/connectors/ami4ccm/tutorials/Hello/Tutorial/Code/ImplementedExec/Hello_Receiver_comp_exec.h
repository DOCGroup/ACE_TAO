// -*- C++ -*-
// $Id$

#ifndef CIAO_HELLO_RECEIVER_EXEC_H_
#define CIAO_HELLO_RECEIVER_EXEC_H_

#include "Hello_Receiver_compEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "Hello_Receiver_comp_exec_export.h"

namespace CIAO_Hello_Receiver_comp_Impl
{
  class MyFoo_obj_exec_i
    : public virtual ::Hello::CCM_MyFoo_obj,
      public virtual ::CORBA::LocalObject
  {
  public:
    MyFoo_obj_exec_i (void);
    virtual ~MyFoo_obj_exec_i (void);

    virtual ::CORBA::Long foo (const char * in_str, ::CORBA::String_out answer);

    virtual void hello (::CORBA::Long_out answer);

    virtual ::CORBA::Short rw_attrib ();

    virtual void rw_attrib (::CORBA::Short new_value);

    virtual CORBA::Short ro_attrib ();
  };

  class  Receiver_comp_exec_i
    : public virtual Receiver_comp_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Receiver_comp_exec_i (void);
    virtual ~Receiver_comp_exec_i (void);

    virtual ::Hello::CCM_MyFoo_obj_ptr get_do_my_foo (void);

    virtual void set_session_context (::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::Hello::CCM_Receiver_comp_Context_var context_;
  };

  extern "C" HELLO_RECEIVER_COMP_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Hello_AMI_Receiver_comp_Impl (void);
}
#endif /* ifndef */
