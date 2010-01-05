// $Id$

#ifndef CIAO_SIMPLEATTRIBUTE_EXEC_H
#define CIAO_SIMPLEATTRIBUTE_EXEC_H

#include /**/ "ace/pre.h"

#include "SimpleAttributeEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "SimpleAttribute_exec_export.h"
#include "tao/LocalObject.h"

namespace CIAO_Simple_SimpleAttribute_Impl
{
  class SIMPLEATTRIBUTE_EXEC_Export SimpleAttribute_exec_i
    : public virtual SimpleAttribute_Exec,
      public virtual ::CORBA::LocalObject
  {
    public:
    SimpleAttribute_exec_i (void);
    virtual ~SimpleAttribute_exec_i (void);

    // Supported or inherited operations.

    // Attribute operations.

    virtual char *
    str_attr ();

    virtual void
    str_attr (const char * str_attr);

    virtual ::CORBA::Long
    long_attr ();

    virtual void
    long_attr (::CORBA::Long long_attr);

    virtual ::CORBA::Short
    short_attr ();

    virtual void
    short_attr (::CORBA::Short short_attr);

    // Port operations.

    // Operations from Components::SessionComponent

    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete ();

    virtual void ccm_activate ();

    virtual void ccm_passivate ();

    virtual void ccm_remove ();

    private:
    ::Simple::CCM_SimpleAttribute_Context_var context_;
    ACE_CString str_;
    ::CORBA::Long long_;
    ::CORBA::Short short_;
  };

  class SIMPLEATTRIBUTE_EXEC_Export SimpleAttributeHome_exec_i
    : public virtual SimpleAttributeHome_Exec,
      public virtual ::CORBA::LocalObject
  {
    public:
    SimpleAttributeHome_exec_i (void);
    virtual ~SimpleAttributeHome_exec_i (void);

    // Supported or inherited operations.

    // Home operations.

    // Factory and finder operations.

    // Attribute operations.

    virtual char *
    str_attr ();

    virtual void
    str_attr (const char * str_attr);

    virtual ::CORBA::Long
    long_attr ();

    virtual void
    long_attr (::CORBA::Long long_attr);

    virtual ::CORBA::Short
    short_attr ();

    virtual void
    short_attr (::CORBA::Short short_attr);

    // Implicit operations.

    virtual ::Components::EnterpriseComponent_ptr
    create ();
  private:
    ACE_CString str_;
    ::CORBA::Long long_;
    ::CORBA::Short short_;
  };

  extern "C" SIMPLEATTRIBUTE_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_Simple_SimpleAttributeHome_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* CIAO_SIMPLEATTRIBUTE_EXEC_H */

