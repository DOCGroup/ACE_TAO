#ifndef CIAO_PRIMARY_GIZMO_EXEC_H
#define CIAO_PRIMARY_GIZMO_EXEC_H

#include /**/ "ace/pre.h"

#include "Primary_Gizmo_svnt.h"
#include "ace/Event_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Primary_Gizmo_exec_export.h"
#include "tao/LocalObject.h"

namespace CIDL_Primary_Gizmo
{
  class PRIMARY_GIZMO_EXEC_Export Primary_Gizmo_exec_i
    : public virtual Primary_Gizmo_Exec,
      public virtual TAO_Local_RefCounted_Object,
      public virtual ACE_Event_Handler
  {
    public:
    Primary_Gizmo_exec_i (void);

    virtual ~Primary_Gizmo_exec_i (void);

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
    Primary_Gizmo_Context *context_;
  };

  class PRIMARY_GIZMO_EXEC_Export Primary_Gizmo_Factory_exec_i
    : public virtual Primary_Gizmo_Factory_Exec,
      public virtual TAO_Local_RefCounted_Object
  {
    public:
    Primary_Gizmo_Factory_exec_i (void);
    virtual ~Primary_Gizmo_Factory_exec_i (void);

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

  extern "C" PRIMARY_GIZMO_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_SPACE_Primary_Gizmo_Factory_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* CIAO_PRIMARY_GIZMO_EXEC_H */
