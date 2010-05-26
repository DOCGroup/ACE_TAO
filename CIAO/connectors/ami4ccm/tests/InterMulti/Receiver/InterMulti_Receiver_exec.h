// -*- C++ -*-
// $Id$

#ifndef CIAO_INTERMULTI_RECEIVER_EXEC_H_
#define CIAO_INTERMULTI_RECEIVER_EXEC_H_

#include "InterMulti_ReceiverEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "InterMulti_Receiver_exec_export.h"

namespace CIAO_InterMulti_Receiver_Impl
{
  class One_exec_i
    : public virtual ::InterMulti::CCM_One,
      public virtual ::CORBA::LocalObject
  {
  public:
    One_exec_i (void);
    virtual ~One_exec_i (void);

    virtual ::CORBA::Long foo (const char * in_str, ::CORBA::Long cmd,
                               ::CORBA::String_out answer);
    virtual ::CORBA::Long sec (const char * in_str, ::CORBA::Long cmd,
                               ::CORBA::String_out answer);

  };
   class Two_exec_i
    : public virtual ::InterMulti::CCM_Two,
      public virtual ::CORBA::LocalObject
  {
  public:
    Two_exec_i (void);
    virtual ~Two_exec_i (void);

    virtual void bar (::CORBA::Long cmd,
                      ::CORBA::String_out answer);
  };
    class Three_exec_i
    : public virtual ::InterMulti::CCM_Three,
      public virtual ::CORBA::LocalObject
  {
  public:
    Three_exec_i (void);
    virtual ~Three_exec_i (void);

    virtual void plus (::CORBA::Long cmd,
                       ::CORBA::String_out answer);
    virtual ::CORBA::Long foo (const char * in_str,
                               ::CORBA::Long cmd,
                               ::CORBA::String_out answer);
    virtual ::CORBA::Long sec (const char * in_str,
                               ::CORBA::Long cmd,
                               ::CORBA::String_out answer);
    virtual void bar (::CORBA::Long cmd,
                      ::CORBA::String_out answer);
  };

  class  Receiver_exec_i
    : public virtual Receiver_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Receiver_exec_i (void);
    virtual ~Receiver_exec_i (void);

    virtual ::InterMulti::CCM_One_ptr get_do_my_one (void);
    virtual ::InterMulti::CCM_Two_ptr get_do_my_two (void);
    virtual ::InterMulti::CCM_Three_ptr get_do_my_three (void);

    virtual void set_session_context (::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::InterMulti::CCM_Receiver_Context_var context_;
  };

  extern "C" INTERMULTI_RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_InterMulti_AMI_Receiver_Impl (void);
}

#endif /* ifndef */
