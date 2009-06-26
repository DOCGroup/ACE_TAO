#ifndef CIAO_EXEC_COMPONENT_EXEC_H
#define CIAO_EXEC_COMPONENT_EXEC_H

#include /**/ "ace/pre.h"

#include "Exec_Component_svnt.h"
#include "ace/Event_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Exec_Component_exec_export.h"
#include "tao/LocalObject.h"

namespace CIDL_Exec_Component
{
  class EXEC_COMPONENT_EXEC_Export Exec_Component_exec_i
    : public virtual Exec_Component_Exec,
      public virtual TAO_Local_RefCounted_Object,
      public virtual ACE_Event_Handler
  {
    public:
    Exec_Component_exec_i (void);
    virtual ~Exec_Component_exec_i (void);

    virtual int handle_timeout (const ACE_Time_Value &,
                                const void *ref);

    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
                      ::CORBA::SystemException,
                      ::Components::CCMException));

    virtual void
    ciao_preactivate (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
                      ::CORBA::SystemException,
                      ::Components::CCMException));

    virtual void
    configuration_complete (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
                      ::CORBA::SystemException,
                      ::Components::CCMException));

    virtual void
    ccm_activate (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
                      ::CORBA::SystemException,
                      ::Components::CCMException));

    virtual void
    ccm_passivate (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
                      ::CORBA::SystemException,
                      ::Components::CCMException));

    virtual void
    ccm_remove (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
                      ::CORBA::SystemException,
                      ::Components::CCMException));

    protected:
    Exec_Component_Context *context_;
  };

  class EXEC_COMPONENT_EXEC_Export Exec_Component_Factory_exec_i
    : public virtual Exec_Component_Factory_Exec,
      public virtual TAO_Local_RefCounted_Object
  {
    public:
    Exec_Component_Factory_exec_i (void);
    virtual ~Exec_Component_Factory_exec_i (void);

    // Supported or inherited operations.

    // Home operations.

    // Factory and finder operations.

    // Attribute operations.

    // Implicit operations.

    virtual ::Components::EnterpriseComponent_ptr
    create (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
                      ::CORBA::SystemException,
                      ::Components::CCMException));
  };

  extern "C" EXEC_COMPONENT_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_SPACE_Exec_Component_Factory_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* CIAO_EXEC_COMPONENT_EXEC_H */
