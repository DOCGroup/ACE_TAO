// $Id$

#ifndef CIAO_NULL_COMPONENT_EXEC_H
#define CIAO_NULL_COMPONENT_EXEC_H

#include /**/ "ace/pre.h"

#include "Null_ComponentEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Null_Component_exec_export.h"
#include "tao/LocalObject.h"

namespace CIAO_Null_Component_Impl
{
  class NULL_COMPONENT_EXEC_Export Null_Interface_exec_i
    : public virtual ::CCM_Null_Interface,
      public virtual ::CORBA::LocalObject
  {
    public:
    Null_Interface_exec_i (void);
    virtual ~Null_Interface_exec_i (void);

    // Operations from ::Null_Interface

    virtual void
    null_operation ();
  };

  class NULL_COMPONENT_EXEC_Export Null_Component_exec_i
    : public virtual Null_Component_Exec,
      public virtual ::CORBA::LocalObject
  {
    public:
    Null_Component_exec_i (void);
    virtual ~Null_Component_exec_i (void);

    // Supported or inherited operations.

    // Attribute operations.

    // Port operations.

    virtual ::CCM_Null_Interface_ptr
    get_null_facet ();

    // Operations from Components::SessionComponent

    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete ();

    virtual void ccm_activate ();

    virtual void ccm_passivate ();

    virtual void ccm_remove ();

    private:
    ::CCM_Null_Component_Context_var context_;
  };

  extern "C" NULL_COMPONENT_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Null_Component_Impl (void);

  class NULL_COMPONENT_EXEC_Export Null_ComponentHome_exec_i
    : public virtual Null_ComponentHome_Exec,
      public virtual ::CORBA::LocalObject
  {
    public:
    Null_ComponentHome_exec_i (void);
    virtual ~Null_ComponentHome_exec_i (void);

    // Supported or inherited operations.

    // Home operations.

    // Factory and finder operations.

    // Attribute operations.

    // Implicit operations.

    virtual ::Components::EnterpriseComponent_ptr
    create ();
  };

  extern "C" NULL_COMPONENT_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_Null_ComponentHome_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* CIAO_NULL_COMPONENT_EXEC_H */

