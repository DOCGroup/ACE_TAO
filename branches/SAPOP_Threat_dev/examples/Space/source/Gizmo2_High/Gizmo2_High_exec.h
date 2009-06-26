#ifndef CIAO_GIZMO2_HIGH_EXEC_H
#define CIAO_GIZMO2_HIGH_EXEC_H

#include /**/ "ace/pre.h"

#include "Gizmo2_High_svnt.h"
#include "ace/Event_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Gizmo2_High_exec_export.h"
#include "tao/LocalObject.h"

namespace CIDL_Gizmo2_High
{
  class GIZMO2_HIGH_EXEC_Export Gizmo2_High_exec_i
    : public virtual Gizmo2_High_Exec,
      public virtual TAO_Local_RefCounted_Object,
      public virtual ACE_Event_Handler
  {
    public:
    Gizmo2_High_exec_i (void);
    virtual ~Gizmo2_High_exec_i (void);

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
    Gizmo2_High_Context *context_;
  };

  class GIZMO2_HIGH_EXEC_Export Gizmo2_High_Factory_exec_i
    : public virtual Gizmo2_High_Factory_Exec,
      public virtual TAO_Local_RefCounted_Object
  {
    public:
    Gizmo2_High_Factory_exec_i (void);
    virtual ~Gizmo2_High_Factory_exec_i (void);

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

  extern "C" GIZMO2_HIGH_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_SPACE_Gizmo2_High_Factory_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* CIAO_GIZMO2_HIGH_EXEC_H */
